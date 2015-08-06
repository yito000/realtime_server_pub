#ifndef WEBSOCKET_ASYNC_H
#define WEBSOCKET_ASYNC_H

#include "smart_ptr.hpp"
#include "allocator/custom_allocator.hpp"

#include <string>
#include <vector>
#include <list>

#include <functional>
#include <boost/asio.hpp>

#include "client_frame_data.h"

#include "../packet.h"
#include "../socket_buffer.h"

#include "network/io/async_socket_inf.h"
#include "handshake.h"
#include "websocket_delegate.h"

#include <boost/asio/ssl.hpp>

namespace client {

enum class SSLVerifyMode {
    VERIFY_NODE = boost::asio::ssl::context::verify_none,
    VERIFY_PEER = boost::asio::ssl::context::verify_peer,
    VERIFY_CLIENT_ONCE = boost::asio::ssl::context::verify_client_once,
    VERIFY_PEER_FAIL_IF_NO_PEER_CERT = boost::asio::ssl::context::verify_fail_if_no_peer_cert
};

typedef std::function<void(boost::system::error_code ec)> SendCallback;

class WebsocketAsync
{
public:
    static WebsocketAsync* create(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned short _port,
        int _timeout_millis);
    static WebsocketAsync* createSSL(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned short _port,
        int _timeout_millis, int verify_mode, const ByteBuffer& verify_cert);
    
    ~WebsocketAsync();

    void connect(HandShakeRequest::ptr handshake_req);

    bool isOpen();
    void read();
    void write(PacketData::ptr packet_data, const char* mask_key,
        SendCallback send_callback);
    void close();

    void destroyAsync();

    WebsocketDelegate* getDelegate()
    {
        return ws_delegate;
    }

    void setDelegate(WebsocketDelegate* wd)
    {
        ws_delegate = wd;
    }
    
    long getKey() const;

private:
    WebsocketAsync(boost::asio::io_service& _ios, int _timeout_millis);
    bool init(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned short _port);
    bool initWithSSL(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned short _port,
        int verify_mode, const ByteBuffer& verify_cert);
    
    void receiveSSLHandshake(HandShakeRequest::ptr handshake_req);
    
    void wsHandShake(HandShakeRequest::ptr handshake_req);
    void receiveWsHandShake(ByteBuffer* buf, 
        HandShakeRequest::ptr handshake_req);

    bool validateWsHandshakeResponse(ByteBuffer* buf, 
        HandShakeRequest::ptr handshake_req);

    void readAsync();
    void writeAsync(PacketData::ptr packet_data,
        const std::string mask_key, SendCallback send_callback);

    void receivePacket();
    void createWebsocketData(ByteBuffer* buf, std::list<PacketData::ptr>& pd_list);

    //
    void serializeFramingData(bool end, PacketType packet_type,
        const std::string& mask, const std::vector<char>& body, 
        std::vector<char>& out_data);
    bool deserializeFramingData(std::vector<char>& data,
        SocketFrame& socket_frame, int start_index);

    AsyncSocketInf::ptr socket;
    boost::asio::io_service& ios;
    
    int timeout_millis;
    WebsocketDelegate* ws_delegate;

    // todo enum state flag
    bool first_process;
    bool connected;
    bool handshake_ok;
    
    ByteBuffer tmp_buffer;
    std::string uuid;
    bool ssl_mode;
};

};

#endif
