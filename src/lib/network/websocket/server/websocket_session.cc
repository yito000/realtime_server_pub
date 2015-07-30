#include "websocket_session.h"

#include "../secret/secret_key.h"

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "exception/websocket_exception.hpp"
#include "log/logger.h"

namespace server {

using boost::asio::ip::tcp;

WebsocketSession::WebsocketSession(boost::asio::io_service& _ios,
    int _timeout_millis, int _retry) : ios(_ios), ios_st(ios), socket(ios), 
    read_timer(ios), write_timer(ios)
{
    start_flag = false;
    handshake_ok = false;

    timeout_millis = _timeout_millis;
    retry = _retry;

    session_delegate = NULL;
}

WebsocketSession::~WebsocketSession()
{
    //
}

tcp::socket& WebsocketSession::getSocket()
{
    return socket;
}

void WebsocketSession::start()
{
    if (start_flag) {
        return;
    }

    read_timer.async_wait(
        ios_st.wrap(std::bind(&WebsocketSession::checkDeadline, 
            this, &read_timer)));

    write_timer.async_wait(
        ios_st.wrap(std::bind(&WebsocketSession::checkDeadline, 
            this, &write_timer)));

    start_flag = true;

    //
    ByteBuffer* tmp_buf = new ByteBuffer;

    receiveHandShake(tmp_buf);
}

bool WebsocketSession::isOpen()
{
    return socket.is_open();
}

void WebsocketSession::read()
{
    ios.dispatch([this]() {
        readAsync();
    });
}

void WebsocketSession::write(PacketData::ptr packet_data,
    SendCallback send_callback)
{
    ios.dispatch([this, packet_data, send_callback]() {
        writeAsync(packet_data, send_callback);
    });
}

void WebsocketSession::close()
{
    ios.dispatch([this]() {
        try {
            if (socket.is_open()) {
                socket.close();
            }

            Logger::debug("close websocket session");

            read_timer.cancel();
            write_timer.cancel();
        } catch (BaseException& e) {
            e.printAll();
        }
    });
}

void WebsocketSession::destroyAsync()
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
void WebsocketSession::receiveHandShake(ByteBuffer* buf)
{
    auto du = boost::posix_time::milliseconds(timeout_millis);
    read_timer.expires_from_now(du);

    socket.async_read_some(boost::asio::buffer(data, MAX_LENGTH),
        [this, buf](boost::system::error_code ec, std::size_t s) {
            if (!ec) {
                for (int i = 0; i < s; i++) {
                    buf->push_back(data[i]);
                }

                try {
                    handShake(buf);
                } catch (BaseException& e) {
                    // todo 400 error

                    e.printAll();

                    delete buf;
                    close();
                }
            } else {
                delete buf;
                close();
            }

            read_timer.expires_at(boost::posix_time::pos_infin);
        });
}

void WebsocketSession::handShake(ByteBuffer* buf)
{
    auto res = validateHandShakeRequest(buf);

    if (res) {
        delete buf;
        handshake_ok = true;

        sendHandShakeOK(res);
    } else {
        receiveHandShake(buf);
    }
}

void WebsocketSession::sendHandShakeOK(HandShakeResponse::ptr h_res)
{
    std::string protocol = h_res->protocol;
    std::string sec_key = h_res->secret_accept;

    std::string http_version = "HTTP/" + h_res->http_version;
    std::string status_code = 
        boost::lexical_cast<std::string>(h_res->status_code);
    std::string res_message = "Switching Protocols";

    std::string res_str;
    res_str += http_version + " " + status_code + " " + res_message + "\r\n";
    res_str += "Upgrade: websocket\r\n";
    res_str += "Connection: Upgrade\r\n";
    res_str += "Sec-WebSocket-Accept: " + sec_key + "\r\n";
    res_str += "Sec-WebSocket-Protocol: " + protocol + "\r\n";
    res_str += "\r\n";

    ByteBuffer* res = new ByteBuffer;
    res->insert(res->end(), res_str.begin(), res_str.end());

    //
    auto du = boost::posix_time::milliseconds(timeout_millis);
    write_timer.expires_from_now(du);

    socket.async_write_some(boost::asio::buffer(*res),
        [this, res](boost::system::error_code ec, std::size_t s) {
            // todo timeout
            if (!ec) {
                if (session_delegate) {
                    session_delegate->onStart(this);
                }
            } else {
                close();
            }

            delete res;
            write_timer.expires_at(boost::posix_time::pos_infin);
        });
}

HandShakeResponse::ptr WebsocketSession::validateHandShakeRequest(
    ByteBuffer* buf)
{
    HandShakeRequest::ptr req = HandShake::parseReqHeader(buf);
    if (!req) {
        return nullptr;
    }

    const int WS_VERSION = 13;
    HandShakeResponse::ptr res = new HandShakeResponse;

    // todo error response
    res->status_code = 400;

    //
    if (req->method == "GET") {
        //
    } else {
        THROW_WEBSOCKET_EXCEPTION("invalid http method");
    }

    if (req->http_version == "1.1") {
        res->http_version = req->http_version;
    } else {
        THROW_WEBSOCKET_EXCEPTION("invalid http version");
    }

    //
    auto u_name = req->upgrade_name;
    std::transform(req->upgrade_name.begin(), 
        req->upgrade_name.end(), u_name.begin(), tolower);

    if (u_name == "websocket") {
        res->upgrade = req->upgrade_name;
    } else {
        THROW_WEBSOCKET_EXCEPTION("invalid upgrade");
    }

    //
    auto c_name = req->connection_name;
    std::transform(req->connection_name.begin(),
        req->connection_name.end(), c_name.begin(), tolower);

    if (c_name == "upgrade") {
        res->connection = req->connection_name;
    } else {
        THROW_WEBSOCKET_EXCEPTION("invalid connection name");
    }

    if (req->secret_key != "") {
        res->secret_accept = SecretKey::calcResponse(req->secret_key);
    } else {
        THROW_WEBSOCKET_EXCEPTION("auto error: secret key");
    }

    auto p_it = req->protocols.begin();
    for (; p_it != req->protocols.end(); ++p_it) {
        auto protocol = *p_it;

        if (protocol == valid_protocol) {
            res->protocol = valid_protocol;
            break;
        }
    }

    if (res->protocol == "") {
        THROW_WEBSOCKET_EXCEPTION("invalid websocket protocol");
    }

    if (req->ws_version != WS_VERSION) {
        THROW_WEBSOCKET_EXCEPTION("websocket version is not 13");
    }

    res->status_code = 101;

    return res;
}

void WebsocketSession::readAsync()
{
    receivePacket();
}

void WebsocketSession::writeAsync(PacketData::ptr packet_data,
    SendCallback send_callback)
{
    auto du = boost::posix_time::milliseconds(timeout_millis);
    write_timer.expires_from_now(du);

    ByteBuffer* ser_packet_data = new ByteBuffer;
    serializeFramingData(packet_data->fin, packet_data->packet_type,
        packet_data->data, *ser_packet_data);

    socket.async_write_some(boost::asio::buffer(*ser_packet_data),
        [this, send_callback, ser_packet_data](
            boost::system::error_code ec, std::size_t s) {

            if (!ec) {
                if (session_delegate) {
                    session_delegate->onSendFinish(this);
                }

                if (send_callback) {
                    send_callback(ec);
                }
            } else {
                // todo retry (boost::asio::error::timed_out)
                if (session_delegate) {
                    session_delegate->onError(this, ec);
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

void WebsocketSession::receivePacket()
{
    socket.async_read_some(boost::asio::buffer(data, MAX_LENGTH),
        [this](boost::system::error_code ec, std::size_t s) {
            if (!ec) {
                Logger::debug("packet size = %ld", s);

                for (int i = 0; i < s; i++) {
                    tmp_buffer.push_back(data[i]);
                }

                std::list<PacketData::ptr> pd_list;
                int ret = createWebsocketData(&tmp_buffer, pd_list);

                Logger::debug("status = %d, packet count = %ld", 
                    ret, pd_list.size());

                for (auto pd: pd_list) {
                    if (session_delegate) {
                        session_delegate->onReceive(this, pd);
                    }
                }

                if (session_delegate) {
                    session_delegate->onReceiveFinish(this);
                }
            } else {
                // todo retry (boost::asio::error::timed_out)
                if (session_delegate) {
                    session_delegate->onError(this, ec);
                }

                close();
            }

            read_timer.expires_at(boost::posix_time::pos_infin);
        });
}

int WebsocketSession::createWebsocketData(ByteBuffer* buf, 
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
                    int packet_size = 6 + socket_frame.payload_length;
                    start_index += packet_size;
                } else if (socket_frame.payload_length <= 65535) {
                    int packet_size = 8 + socket_frame.payload_length;
                    start_index += packet_size;
                } else {
                    int packet_size = 14 + socket_frame.payload_length;
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

void WebsocketSession::serializeFramingData(bool end, PacketType packet_type,
    const std::vector<char>& body, std::vector<char>& out_data)
{
    FrameData::serialize(end, packet_type, body, out_data);
}

bool WebsocketSession::deserializeFramingData(std::vector<char>& data,
    SocketFrame& socket_frame, int start_index)
{
    std::vector<char> tmp_data;
    for (int i = start_index; i < data.size(); i++) {
        tmp_data.push_back(data[i]);
    }

    return FrameData::deserialize(tmp_data, socket_frame);
}

void WebsocketSession::checkDeadline(boost::asio::deadline_timer* timer)
{
    if (timer->expires_at() <= 
        boost::asio::deadline_timer::traits_type::now()) {
        close();

        timer->expires_at(boost::posix_time::pos_infin);
    }

    if (socket.is_open()) {
        timer->async_wait(
            ios_st.wrap(std::bind(&WebsocketSession::checkDeadline, 
                this, timer)));
    }
}

};
