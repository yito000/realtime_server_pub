#include "websocket_async.h"

#include <sstream>
#include <regex>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/thread.hpp>

#include "handshake_helper.h"
#include "exception/websocket_exception.hpp"
#include "log/logger.h"

namespace {
    const int DEFAULT_TIMEOUT = 60 * 1000;

    std::regex upgrade("Upgrade", std::regex_constants::icase);
    std::regex connection("Connection", std::regex_constants::icase);
    std::regex sec_accept("Sec-WebSocket-Accept", 
        std::regex_constants::icase);
    std::regex sec_protocol("Sec-WebSocket-Protocol", 
        std::regex_constants::icase);
};

namespace client {

using boost::asio::ip::tcp;

WebsocketAsync::WebsocketAsync(boost::asio::io_service& _ios,
    const std::string& _host, unsigned short _port, int _timeout_millis) : 
    host(_host), 
    port(_port),
    ios(_ios), 
    ios_st(ios), 
    socket(ios), 
    read_timer(ios), 
    write_timer(ios)
{
    first_process = true;
    connected = false;
    handshake_ok = false;

    timeout_millis = _timeout_millis;
    ws_delegate = NULL;

    //
    read_timer.async_wait(
        ios_st.wrap(std::bind(&WebsocketAsync::checkDeadline,
            this, &read_timer)));

    write_timer.async_wait(
        ios_st.wrap(std::bind(&WebsocketAsync::checkDeadline,
            this, &write_timer)));
}

WebsocketAsync::~WebsocketAsync()
{
    //
}

void WebsocketAsync::connect(HandShakeRequest::ptr handshake_req)
{
    if (connected) {
        return;
    }

    boost::asio::ip::tcp::resolver resolver(ios);
    boost::asio::ip::tcp::resolver::query query(host, std::to_string(port));
    auto endpoint_it = resolver.resolve(query);

    //
    auto du = boost::posix_time::milliseconds(timeout_millis);
    read_timer.expires_from_now(du);

    auto err = boost::asio::error::host_not_found;
    connectInternal(err, endpoint_it, handshake_req);
}

bool WebsocketAsync::isOpen()
{
    return socket.is_open();
}

void WebsocketAsync::read()
{
    if (!handshake_ok) {
        return;
    }

    ios.dispatch([this]() {
        readAsync();
    });
}

void WebsocketAsync::write(PacketData::ptr packet_data, 
    const char* mask_key, SendCallback send_callback)
{
    if (!handshake_ok) {
        return;
    }

    std::string mk(mask_key, 0, 4);

    ios.dispatch([this, packet_data, mk, send_callback]() {
        writeAsync(packet_data, mk, send_callback);
    });
}

void WebsocketAsync::close()
{
    ios.dispatch([this]() {
        try {
            if (socket.is_open()) {
                socket.close();
            }

            Logger::debug("close websocket client");
            connected = false;

            read_timer.cancel();
            write_timer.cancel();
        } catch (BaseException& e) {
            e.printAll();
        }
    });
}

void WebsocketAsync::destroyAsync()
{
    ios.dispatch([this]() {
        try {
            if (socket.is_open()) {
                socket.close();
            }

            read_timer.cancel();
            write_timer.cancel();
        } catch (BaseException& e) {
            e.printAll();
        }

        delete this;
    });
}

// private member function
void WebsocketAsync::connectInternal(boost::system::error_code err,
    boost::asio::ip::tcp::resolver::iterator endpoint_it,
    HandShakeRequest::ptr handshake_req)
{
    boost::asio::ip::tcp::resolver::iterator end;

    if (err && endpoint_it != end) {
        socket.async_connect(*endpoint_it++, 
            [this, endpoint_it, handshake_req](boost::system::error_code ec) {
                if (ec) {
                    connectInternal(ec, endpoint_it, handshake_req);
                } else {
                    first_process = false;
                    connected = true;

                    handShake(handshake_req);
                }
            });
    } else {
        first_process = false;
    }
}

void WebsocketAsync::handShake(HandShakeRequest::ptr handshake_req)
{
    if (!connected || handshake_req->path == "" ||
        handshake_req->secret_key == "" ||
        handshake_req->protocols.size() <= 0) {

        // todo exception
        return;
    }

    int t = timeout_millis < 1 ? DEFAULT_TIMEOUT : timeout_millis;

    std::string path = handshake_req->path;;
    std::string key = handshake_req->secret_key;
    std::string version =
        boost::lexical_cast<std::string>(handshake_req->version);
    std::string protocol;

    bool p_first = true;
    std::set<std::string>::iterator p_it = handshake_req->protocols.begin();
    for (; p_it != handshake_req->protocols.end(); ++p_it) {
        if (p_first) {
            protocol = *p_it;
        } else {
            protocol += ", " + *p_it;
        }
    }

    boost::posix_time::time_duration timeout =
        boost::posix_time::milliseconds(t);

    //
    std::string req_str;
    req_str += "GET " + path + " HTTP/1.1\r\n";
    req_str += "Host: " + host + "\r\n";
    req_str += "Upgrade: websocket\r\n";
    req_str += "Connection: Upgrade\r\n";
    req_str += "Sec-WebSocket-Key: " + key + "\r\n";
    req_str += "Sec-WebSocket-Protocol: " + protocol + "\r\n";
    req_str += "Sec-WebSocket-Version: " + version + "\r\n";
    req_str += "\r\n";

    ByteBuffer* req = new ByteBuffer;
    req->insert(req->end(), req_str.begin(), req_str.end());

    //
    auto du = boost::posix_time::milliseconds(timeout_millis);
    write_timer.expires_from_now(du);

    socket.async_write_some(boost::asio::buffer(*req),
        [this, handshake_req, req](boost::system::error_code ec, 
            std::size_t s) {

            if (!ec) {
                ByteBuffer* tmp_buf = new ByteBuffer;

                receiveHandShake(tmp_buf, handshake_req);
            } else {
                close();
            }

            delete req;
            write_timer.expires_at(boost::posix_time::pos_infin);
        });
}

void WebsocketAsync::receiveHandShake(ByteBuffer* buf, 
    HandShakeRequest::ptr handshake_req)
{
    auto du = boost::posix_time::milliseconds(timeout_millis);
    read_timer.expires_from_now(du);

    socket.async_read_some(boost::asio::buffer(data, MAX_LENGTH),
        [this, buf, handshake_req]
            (boost::system::error_code ec, std::size_t s) {

            if (!ec) {
                for (int i = 0; i < s; i++) {
                    buf->push_back(data[i]);
                }

                bool response_ok = validateHandshakeResponse(
                    buf, handshake_req);
                delete buf;

                if (response_ok && ws_delegate) {
                    handshake_ok = true;

                    ws_delegate->onStart(this);
                } else {
                    close();
                }
            } else {
                delete buf;
                close();
            }

            read_timer.expires_at(boost::posix_time::pos_infin);
        });
}

bool WebsocketAsync::validateHandshakeResponse(ByteBuffer* buf,
    HandShakeRequest::ptr handshake_req)
{
    return HandshakeHelper::validate(buf, handshake_req);
}

void WebsocketAsync::readAsync()
{
    receivePacket();
}

void WebsocketAsync::writeAsync(PacketData::ptr packet_data,
    const std::string mask_key, SendCallback send_callback)
{
    auto du = boost::posix_time::milliseconds(timeout_millis);
    write_timer.expires_from_now(du);

    ByteBuffer* ser_packet_data = new ByteBuffer;
    serializeFramingData(packet_data->fin, packet_data->packet_type,
        mask_key, packet_data->data, *ser_packet_data);

    socket.async_write_some(boost::asio::buffer(*ser_packet_data),
        [this, send_callback, ser_packet_data](
            boost::system::error_code ec, std::size_t s) {

            if (!ec) {
                if (ws_delegate) {
                    ws_delegate->onSendFinish(this);
                }

                if (send_callback) {
                    send_callback(ec);
                }
            } else {
                if (ws_delegate) {
                    ws_delegate->onError(this, ec);
                }

                if (send_callback) {
                    send_callback(ec);
                }

                close();
            }

            delete ser_packet_data;
            write_timer.expires_at(boost::posix_time::pos_infin);
        });
}

void WebsocketAsync::receivePacket()
{
    socket.async_read_some(boost::asio::buffer(data, MAX_LENGTH),
        [this](boost::system::error_code ec, std::size_t s) {
            if (!ec) {
                for (int i = 0; i < s; i++) {
                    tmp_buffer.push_back(data[i]);
                }

                std::list<PacketData::ptr> pd_list;
                int ret = createWebsocketData(&tmp_buffer, pd_list);

                for (auto pd: pd_list) {
                    if (ws_delegate) {
                        ws_delegate->onReceive(this, pd);
                    }
                }

                if (ws_delegate) {
                    ws_delegate->onReceiveFinish(this);
                }
            } else {
                if (ws_delegate) {
                    ws_delegate->onError(this, ec);
                }

                close();
            }

            read_timer.expires_at(boost::posix_time::pos_infin);
        });
}

int WebsocketAsync::createWebsocketData(ByteBuffer* buf, 
    std::list<PacketData::ptr>& pd_list)
{
    ByteBuffer& data = *buf;
    int start_index = 0;

    while (1) {
        SocketFrame socket_frame;

        try {
            PacketData::ptr pd = new PacketData;
            bool parse_complete = deserializeFramingData(data, 
                socket_frame, start_index);

            if (parse_complete) {
                if (socket_frame.payload_length <= 125) {
                    int packet_size = 2 + socket_frame.payload_length;
                    start_index += packet_size;
                } else if (socket_frame.payload_length <= 65535) {
                    int packet_size = 4 + socket_frame.payload_length;
                    start_index += packet_size;
                } else {
                    int packet_size = 10 + socket_frame.payload_length;
                    start_index += packet_size;
                }

                pd->packet_type = (PacketType)socket_frame.opcode;
                pd->data.insert(pd->data.end(), socket_frame.body.begin(),
                    socket_frame.body.end());

                pd_list.push_back(pd);

                if (start_index < data.size()) {
                    //
                } else {
                    break;
                }
            } else {
                break;
            }
        } catch (std::exception& e) {
            Logger::debug("error start index: %d", start_index);
            return 2;
        }
    }
    
    // delete value
    auto start_it = buf->begin();
    auto it = buf->begin();
    for (int i = 0; i < start_index; i++) {
        ++it;
    }

    if (start_index > 0) {
        buf->erase(start_it, it);
    }

    return 0;
}

void WebsocketAsync::serializeFramingData(bool end, PacketType packet_type,
    const std::string& mask, const std::vector<char>& body, 
    std::vector<char>& out_data)
{
    FrameData::serialize(end, packet_type, mask, body, out_data);
}

bool WebsocketAsync::deserializeFramingData(std::vector<char>& data,
    SocketFrame& socket_frame, int start_index)
{
    std::vector<char> tmp_data;
    for (int i = start_index; i < data.size(); i++) {
        tmp_data.push_back(data[i]);
    }

    return FrameData::deserialize(tmp_data, socket_frame);
}

void WebsocketAsync::checkDeadline(boost::asio::deadline_timer* timer)
{
    if (timer->expires_at() <=
        boost::asio::deadline_timer::traits_type::now()) {
        close();

        timer->expires_at(boost::posix_time::pos_infin);
    }

    if (socket.is_open()) {
        timer->async_wait(
            ios_st.wrap(std::bind(&WebsocketAsync::checkDeadline,
                this, timer)));
    }
}

};
