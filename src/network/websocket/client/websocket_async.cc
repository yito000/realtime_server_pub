#include "websocket_async.h"

#include "network/io/async_socket.h"
#include "network/io/async_ssl_socket.h"

#include "atomic/atomic_operator.hpp"

#include <sstream>
#include <regex>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/thread.hpp>

#include "handshake_helper.h"
#include "network/websocket/websocket_exception.hpp"
#include "log/logger.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace {
    const int DEFAULT_TIMEOUT = 60 * 1000;

    std::regex upgrade("Upgrade", std::regex_constants::icase);
    std::regex connection("Connection", std::regex_constants::icase);
    std::regex sec_accept("Sec-WebSocket-Accept", 
        std::regex_constants::icase);
    std::regex sec_protocol("Sec-WebSocket-Protocol", 
        std::regex_constants::icase);
    
    // TODO
    auto uuid_gen = boost::uuids::random_generator();
};

namespace client {

using boost::asio::ip::tcp;

WebsocketAsync* WebsocketAsync::create(boost::asio::io_service& _ios, 
    const std::string& _host, unsigned short _port,
    int _timeout_millis)
{
    auto inst = new WebsocketAsync(_ios, _timeout_millis);
    if (!inst->init(_ios, _host, _port)) {
        delete inst;
        return nullptr;
    }
    
    return inst;
}

WebsocketAsync* WebsocketAsync::createSSL(boost::asio::io_service& _ios, 
    boost::asio::ssl::context& _ssl_context, 
    const std::string& _host, unsigned short _port,
    int _timeout_millis)
{
    auto inst = new WebsocketAsync(_ios, _timeout_millis);
    if (!inst->initWithSSL(_ios, _ssl_context, _host, _port)) {
        delete inst;
        return nullptr;
    }
    
    return inst;
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
    
    auto du = boost::posix_time::milliseconds(timeout_millis);
    socket->connect(du, [this, handshake_req](boost::system::error_code ec) {
        if (!ec) {
            if (ssl_mode) {
                receiveSSLHandshake(handshake_req);
            } else {
                first_process = false;
                connected = true;
                wsHandShake(handshake_req);
            }
        } else {
            // TODO: custom error defined
            ws_delegate->onError(this, 
                client::WebsocketDelegate::Operation::CONNECT, ec);
        }
    });
}

void WebsocketAsync::receiveSSLHandshake(HandShakeRequest::ptr handshake_req)
{
    auto s = boost::dynamic_pointer_cast<AsyncSSLSocket>(socket);
    if (!s) {
        // TODO: assert
        Logger::debug("WebsocketAsync: invalid status");
        return;
    }
    
    auto du = boost::posix_time::milliseconds(timeout_millis);
    s->handshake(du, [this, handshake_req](boost::system::error_code ec) {
        if (!ec) {
            first_process = false;
            connected = true;
            
            wsHandShake(handshake_req);
        } else {
            Logger::debug("ssl handshake error: %s", ec.message().c_str());
            // TODO: custom error defined
            ws_delegate->onError(this, 
                client::WebsocketDelegate::Operation::CONNECT, ec);
        }
    });
}

bool WebsocketAsync::isOpen()
{
    return socket->isOpen();
}

void WebsocketAsync::read()
{
    if (!handshake_ok) {
        return;
    }

    readAsync();
}

void WebsocketAsync::write(PacketData::ptr packet_data, const std::string& mask_key,
    SendCallback send_callback)
{
    if (!handshake_ok) {
        return;
    }
    
    writeAsync(packet_data, mask_key, send_callback);
}

void WebsocketAsync::close()
{
    ios.dispatch([this]() {
        try {
            socket->close();

            Logger::debug("close websocket client");
            connected = false;
        } catch (BaseException& e) {
            e.printAll();
        }
    });
}

void WebsocketAsync::destroyAsync()
{
    ios.dispatch([this]() {
        try {
            socket->close();
        } catch (BaseException& e) {
            e.printAll();
        }
        
        Logger::log("websocket async: destroy");
        delete this;
    });
}

long WebsocketAsync::getKey() const
{
    return std::hash<long>()(reinterpret_cast<long>(this)) ^
        std::hash<std::string>()(uuid);
}

// private member function
WebsocketAsync::WebsocketAsync(boost::asio::io_service& _ios, 
    int _timeout_millis) :
    ios(_ios),
    ssl_mode(false),
    process_write(false)
{
    first_process = true;
    connected = false;
    handshake_ok = false;

    timeout_millis = _timeout_millis;
    ws_delegate = NULL;
    
    uuid = boost::lexical_cast<std::string>(boost::uuids::uuid(uuid_gen()));
}

bool WebsocketAsync::init(boost::asio::io_service& _ios, 
    const std::string& _host, unsigned short _port)
{
    socket = new AsyncSocket(_ios, _host, _port);
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

bool WebsocketAsync::initWithSSL(boost::asio::io_service& _ios, 
    boost::asio::ssl::context& _ssl_context, 
    const std::string& _host, unsigned short _port)
{
    auto p_socket = new AsyncSSLSocket(_ios, _ssl_context, _host, _port);
    
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

void WebsocketAsync::wsHandShake(HandShakeRequest::ptr handshake_req)
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
    req_str += "Host: " + socket->getHost() + "\r\n";
    req_str += "Upgrade: websocket\r\n";
    req_str += "Connection: Upgrade\r\n";
    req_str += "Origin: game://\r\n";
    req_str += "Sec-WebSocket-Key: " + key + "\r\n";
    req_str += "Sec-WebSocket-Protocol: " + protocol + "\r\n";
    req_str += "Sec-WebSocket-Version: " + version + "\r\n";
    req_str += "\r\n";
    
    Logger::debug("%s", req_str.c_str());

    ByteBuffer* req = new ByteBuffer;
    req->insert(req->end(), req_str.begin(), req_str.end());
    
    //
    auto du = boost::posix_time::milliseconds(timeout_millis);
    
    socket->write(*req, du, 
        [this, handshake_req, req](boost::system::error_code ec, 
            std::size_t s) {

            if (!ec) {
                ByteBuffer* tmp_buf = new ByteBuffer;

                receiveWsHandShake(tmp_buf, handshake_req);
            } else {
                // TODO: custom error defined
                ws_delegate->onError(this, 
                    client::WebsocketDelegate::Operation::CONNECT, ec);
                
                Logger::debug("WebsocketAsync: ws handshake error, %s", ec.message().c_str());
            }

            delete req;
        });
}

void WebsocketAsync::receiveWsHandShake(ByteBuffer* buf, 
    HandShakeRequest::ptr handshake_req)
{
    auto du = boost::posix_time::milliseconds(timeout_millis);
    
    socket->read(du, 
        [this, buf, handshake_req]
            (boost::system::error_code ec, char* data, std::size_t s) {
            
            if (!ec) {
                for (int i = 0; i < s; i++) {
                    buf->push_back(data[i]);
                }

                bool response_ok = validateWsHandshakeResponse(
                    buf, handshake_req);
                delete buf;
                
                if (response_ok && ws_delegate) {
                    handshake_ok = true;

                    ws_delegate->onStart(this);
                } else {
                    // TODO: custom error defined
                    ws_delegate->onError(this, 
                        client::WebsocketDelegate::Operation::CONNECT, ec);
                }
            } else {
                Logger::debug("WebsocketAsync: receive handshake error, %s", ec.message().c_str());
                delete buf;
                
                ws_delegate->onError(this, 
                    client::WebsocketDelegate::Operation::CONNECT, ec);
            }
        });
}

bool WebsocketAsync::validateWsHandshakeResponse(ByteBuffer* buf,
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
    ios.dispatch([this, packet_data, mask_key, send_callback]() {
        if (process_write) {
            PacketInfo p_info;
            p_info.packet = packet_data;
            p_info.mask_key = mask_key;
            p_info.callback = send_callback;
            
            packet_queue.push_back(p_info);
        } else {
            writeAsyncInternal(packet_data, mask_key, send_callback);
        }
    });
}

void WebsocketAsync::writeAsyncInternal(PacketData::ptr packet_data,
    const std::string mask_key, SendCallback send_callback)
{
    AtomicOperator<bool>::lock_test_and_set(&process_write, true);
    
    auto du = boost::posix_time::milliseconds(timeout_millis);
    
    ByteBuffer* ser_packet_data = new ByteBuffer;
    serializeFramingData(packet_data->fin, packet_data->packet_type,
        mask_key, packet_data->data, *ser_packet_data);
    
    socket->write(*ser_packet_data, du, 
        [this, send_callback, ser_packet_data](
            boost::system::error_code ec, std::size_t s) {
            
            if (!ec) {
                if (send_callback) {
                    send_callback(ec);
                }
            } else {
                if (ws_delegate) {
                    ws_delegate->onError(this, 
                        client::WebsocketDelegate::Operation::WRITE, ec);
                }

                if (send_callback) {
                    send_callback(ec);
                }

                close();
            }
            
            if (ws_delegate) {
                ws_delegate->onSendFinish(this, ec);
            }

            delete ser_packet_data;
            
            if (isOpen() && !packet_queue.empty()) {
                auto p_info = packet_queue.front();
                packet_queue.pop_front();
                
                writeAsyncInternal(p_info.packet, p_info.mask_key, p_info.callback);
            }
            
            AtomicOperator<bool>::lock_test_and_set(&process_write, false);
        });
}

void WebsocketAsync::receivePacket()
{
    socket->read(boost::posix_time::pos_infin, 
        [this](boost::system::error_code ec, char* data, std::size_t s) {
            if (!ec) {
                for (int i = 0; i < s; i++) {
                    tmp_buffer.push_back(data[i]);
                }
                
                std::list<PacketData::ptr> pd_list;
                createWebsocketData(&tmp_buffer, pd_list);

                for (auto pd: pd_list) {
                    if (ws_delegate) {
                        ws_delegate->onReceive(this, pd);
                    }
                }
            } else {
                if (ws_delegate) {
                    ws_delegate->onError(this, 
                        client::WebsocketDelegate::Operation::READ, ec);
                }

                close();
            }
            
            if (ws_delegate) {
                ws_delegate->onReceiveFinish(this, ec);
            }
        });
}

void WebsocketAsync::createWebsocketData(ByteBuffer* buf, 
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

void WebsocketAsync::serializeFramingData(bool end, PacketType packet_type,
    const std::string& mask, const std::vector<unsigned char>& body, 
    std::vector<unsigned char>& out_data)
{
    FrameData::serialize(end, packet_type, mask, body, out_data);
}

bool WebsocketAsync::deserializeFramingData(std::vector<unsigned char>& data,
    SocketFrame& socket_frame, int start_index)
{
    std::vector<unsigned char> tmp_data;
    for (int i = start_index; i < data.size(); i++) {
        tmp_data.push_back(data[i]);
    }

    return FrameData::deserialize(tmp_data, socket_frame);
}

};
