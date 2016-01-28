#include "websocket_session.h"

#include "../secret/secret_key.h"

#include "network/io/async_socket.h"
#include "network/io/async_ssl_socket.h"

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "network/websocket/websocket_exception.hpp"
#include "log/logger.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

BEGIN_NS

namespace {
    // TODO
    auto uuid_gen = boost::uuids::random_generator();
};

namespace server {

using boost::asio::ip::tcp;

WebsocketSession* WebsocketSession::create(boost::asio::io_service& _ios, 
    int _timeout_millis)
{
    auto inst = new WebsocketSession(_ios, _timeout_millis);
    if (!inst->init(_ios)) {
        delete inst;
        return nullptr;
    }
    
    return inst;
}

WebsocketSession* WebsocketSession::createSSL(boost::asio::io_service& _ios, 
    boost::asio::ssl::context& _ssl_context, int _timeout_millis)
{
    auto inst = new WebsocketSession(_ios, _timeout_millis);
    if (!inst->initWithSSL(_ios, _ssl_context)) {
        delete inst;
        return nullptr;
    }
    
    return inst;
}

WebsocketSession::~WebsocketSession()
{
    //
}

tcp::socket& WebsocketSession::getSocket()
{
    return socket->getDetail();
}

void WebsocketSession::start()
{
    if (start_flag) {
        return;
    }

    start_flag = true;

    //
    if (ssl_mode) {
        receiveSSLHandshake();
    } else {
        ByteBuffer* tmp_buf = new ByteBuffer;
        receiveWsHandshake(tmp_buf);
    }
}

bool WebsocketSession::isOpen()
{
    return socket->isOpen();
}

void WebsocketSession::read()
{
    readAsync();
}

void WebsocketSession::write(PacketData::ptr packet_data,
    SendCallback send_callback)
{
    writeAsync(packet_data, send_callback);
}

void WebsocketSession::close()
{
    ios.dispatch([this]() {
        try {
            socket->close();

            Logger::debug("close websocket session");
        } catch (BaseException& e) {
            e.printAll();
        }
    });
}

void WebsocketSession::destroyAsync()
{
    ios.dispatch([this]() {
        try {
            socket->close();
        } catch (BaseException& e) {
            e.printAll();
        }

        delete this;
    });
}

int64_t WebsocketSession::getKey() const
{
    return std::hash<int64_t>()(reinterpret_cast<int64_t>(this)) ^
        std::hash<std::string>()(uuid);
}

// private member function
WebsocketSession::WebsocketSession(boost::asio::io_service& _ios,
    int _timeout_millis) : 
    ios(_ios),
    ssl_mode(false),
    process_write(false)
{
    start_flag = false;
    handshake_ok = false;

    timeout_millis = _timeout_millis;

    session_delegate = NULL;
    
    uuid = boost::lexical_cast<std::string>(boost::uuids::uuid(uuid_gen()));
}

bool WebsocketSession::init(boost::asio::io_service& _ios)
{
    socket = new AsyncSocket(_ios);
    socket->setConnectTimeoutCallback([this](AsyncSocketInf&) {
        // TODO: callback
        return false;
    });
    socket->setReadTimeoutCallback([this](AsyncSocketInf&) {
        // TODO: callback
        return false;
    });
    socket->setWriteTimeoutCallback([this](AsyncSocketInf&) {
        // TODO: callback
        return false;
    });
    
    return true;
}

bool WebsocketSession::initWithSSL(boost::asio::io_service& _ios, 
    boost::asio::ssl::context& _ssl_context)
{
    auto p_socket = new AsyncSSLSocket(_ios, _ssl_context);
    p_socket->setConnectTimeoutCallback([this](AsyncSocketInf&) {
        // TODO: callback
        return false;
    });
    p_socket->setReadTimeoutCallback([this](AsyncSocketInf&) {
        // TODO: callback
        return false;
    });
    p_socket->setWriteTimeoutCallback([this](AsyncSocketInf&) {
        // TODO: callback
        return false;
    });
    
    socket = p_socket;
    ssl_mode = true;
    
    return true;
}

void WebsocketSession::receiveSSLHandshake()
{
    auto s = boost::dynamic_pointer_cast<AsyncSSLSocket>(socket);
    if (s) {
        auto du = boost::posix_time::milliseconds(timeout_millis);
        
        s->handshake(du, 
            [this](boost::system::error_code ec) {
                if (!ec) {
                    ByteBuffer* tmp_buf = new ByteBuffer;
                    receiveWsHandshake(tmp_buf);
                } else {
                    Logger::debug("ssl handshake error: %s", ec.message().c_str());
                    session_delegate->onError(this, 
                        server::SessionDelegate::Operation::CONNECT, ec);
                }
            });
    } else {
        session_delegate->onError(this, 
            server::SessionDelegate::Operation::CONNECT, 
            boost::asio::error::host_not_found);
    }
}

void WebsocketSession::receiveWsHandshake(ByteBuffer* buf)
{
    auto du = boost::posix_time::milliseconds(timeout_millis);
    
    socket->read(du, 
        [this, buf](boost::system::error_code ec, char* data, std::size_t s) {
            if (!ec) {
                for (int i = 0; i < s; i++) {
                    buf->push_back(data[i]);
                }

                try {
                    wsHandshake(buf);
                } catch (BaseException& e) {
                    e.printAll();

                    delete buf;
                    session_delegate->onError(this, 
                        server::SessionDelegate::Operation::CONNECT, ec);
                }
            } else {
                Logger::debug("ws handshake error: %s", ec.message().c_str());
                delete buf;
                
                session_delegate->onError(this, 
                    server::SessionDelegate::Operation::CONNECT, ec);
            }
        });
}

void WebsocketSession::wsHandshake(ByteBuffer* buf)
{
    auto res = validateWsHandshakeRequest(buf);

    if (res) {
        delete buf;
        handshake_ok = true;

        sendWsHandshakeOK(res);
    } else {
        receiveWsHandshake(buf);
    }
}

void WebsocketSession::sendWsHandshakeOK(HandShakeResponse::ptr h_res)
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
    
    socket->write(*res, du, 
        [this, res](boost::system::error_code ec, std::size_t s) {
            if (!ec) {
                if (session_delegate) {
                    session_delegate->onStart(this);
                } else {
                    session_delegate->onError(this, 
                        server::SessionDelegate::Operation::CONNECT, ec);
                }
            } else {
                Logger::debug("sendWsHandshakeOK error: %s", ec.message().c_str());
                session_delegate->onError(this, 
                    server::SessionDelegate::Operation::CONNECT, ec);
            }

            delete res;
        });
}

HandShakeResponse::ptr WebsocketSession::validateWsHandshakeRequest(
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
        throw WebsocketException("invalid http method");
    }

    if (req->http_version == "1.1") {
        res->http_version = req->http_version;
    } else {
        throw WebsocketException("invalid http version");
    }

    //
    auto u_name = req->upgrade_name;
    std::transform(req->upgrade_name.begin(), 
        req->upgrade_name.end(), u_name.begin(), tolower);

    if (u_name == "websocket") {
        res->upgrade = req->upgrade_name;
    } else {
        throw WebsocketException("invalid upgrade");
    }

    //
    auto c_name = req->connection_name;
    std::transform(req->connection_name.begin(),
        req->connection_name.end(), c_name.begin(), tolower);

    if (c_name == "upgrade") {
        res->connection = req->connection_name;
    } else {
        throw WebsocketException("invalid connection name");
    }

    if (req->secret_key != "") {
        res->secret_accept = SecretKey::calcResponse(req->secret_key);
    } else {
        throw WebsocketException("auto error: secret key");
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
        throw WebsocketException("invalid websocket protocol");
    }

    if (req->ws_version != WS_VERSION) {
        throw WebsocketException("websocket version is not 13");
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
    ios.dispatch([this, packet_data, send_callback]() {
        if (process_write.load()) {
            PacketInfo p_info;
            p_info.packet = packet_data;
            p_info.callback = send_callback;
            
            packet_queue.push_back(p_info);
        } else {
            writeAsyncInternal(packet_data, send_callback);
        }
    });
}

void WebsocketSession::writeAsyncInternal(PacketData::ptr packet_data,
    SendCallback send_callback)
{
    process_write.store(true);
    
    auto du = boost::posix_time::milliseconds(timeout_millis);
    
    ByteBuffer* ser_packet_data = new ByteBuffer;
    serializeFramingData(packet_data->fin, packet_data->packet_type,
        packet_data->data, *ser_packet_data);
    
    socket->write(*ser_packet_data, du,
        [this, send_callback, ser_packet_data](
            boost::system::error_code ec, std::size_t s) {

            if (!ec) {
                if (send_callback) {
                    send_callback(ec);
                }
            } else {
                if (session_delegate) {
                    session_delegate->onError(this, 
                        server::SessionDelegate::Operation::WRITE, ec);
                }

                if (send_callback) {
                    send_callback(ec);
                }

                close();
            }
            
            if (session_delegate) {
                session_delegate->onSendFinish(this, ec);
            }

            delete ser_packet_data;
            
            if (isOpen() && !packet_queue.empty()) {
                auto p_info = packet_queue.front();
                packet_queue.pop_front();
                
                writeAsyncInternal(p_info.packet, p_info.callback);
            }
            
            process_write.store(false);
        });
}

void WebsocketSession::receivePacket()
{
    socket->read(boost::posix_time::pos_infin, 
        [this](boost::system::error_code ec, char* data, std::size_t s) {
            if (!ec) {
                Logger::debug("packet size = %ld", s);

                for (int i = 0; i < s; i++) {
                    tmp_buffer.push_back(data[i]);
                }

                std::list<PacketData::ptr> pd_list;
                createWebsocketData(&tmp_buffer, pd_list);

                Logger::debug("packet count = %ld", 
                    pd_list.size());

                for (auto pd: pd_list) {
                    if (session_delegate) {
                        session_delegate->onReceive(this, pd);
                    }
                }
            } else {
                if (session_delegate) {
                    session_delegate->onError(this, 
                        server::SessionDelegate::Operation::READ, ec);
                }

                close();
            }
            
            if (session_delegate) {
                session_delegate->onReceiveFinish(this, ec);
            }
        });
}

void WebsocketSession::createWebsocketData(ByteBuffer* buf, 
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
            return;
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

    return;
}

void WebsocketSession::serializeFramingData(bool end, PacketType packet_type,
    const std::vector<unsigned char>& body, std::vector<unsigned char>& out_data)
{
    FrameData::serialize(end, packet_type, body, out_data);
}

bool WebsocketSession::deserializeFramingData(std::vector<unsigned char>& data,
    SocketFrame& socket_frame, int start_index)
{
    std::vector<unsigned char> tmp_data;
    for (int i = start_index; i < data.size(); i++) {
        tmp_data.push_back(data[i]);
    }

    return FrameData::deserialize(tmp_data, socket_frame);
}

};

END_NS
