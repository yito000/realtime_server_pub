#ifndef WEBSOCKET_ASYNC_H
#define WEBSOCKET_ASYNC_H

#include "fw_env.h"
#include "allocator/custom_allocator.hpp"

#include <string>
#include <vector>
#include <list>
#include <deque>

#include <atomic>
#include <functional>
#include <boost/asio.hpp>

#include "client_frame_data.h"

#include "network/websocket/packet.h"
#include "network/io/socket_buffer.h"

#include "network/io/async_socket_inf.h"
#include "handshake.h"
#include "websocket_delegate.h"

#include <boost/asio/ssl.hpp>

BEGIN_NS

namespace client {

typedef std::function<void(boost::system::error_code ec)> SendCallback;

class WebsocketAsync
{
public:
    static WebsocketAsync* create(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned short _port,
        int _timeout_millis);
    static WebsocketAsync* createSSL(boost::asio::io_service& _ios, 
        boost::asio::ssl::context& _ssl_context,
        const std::string& _host, unsigned short _port,
        int _timeout_millis);
    
    ~WebsocketAsync();

    void connect(HandShakeRequest::ptr handshake_req);

    bool isOpen();
    void read();
    void write(PacketData::ptr packet_data, const std::string& mask_key,
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
    
    int64_t getKey() const;

private:
    struct PacketInfo
    {
        PacketData::ptr packet;
        std::string mask_key;
        SendCallback callback;
        
        PacketInfo()
        {
            //
        }
    };
    
    WebsocketAsync(boost::asio::io_service& _ios, int _timeout_millis);
    bool init(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned short _port);
    bool initWithSSL(boost::asio::io_service& _ios, 
        boost::asio::ssl::context& _ssl_context, 
        const std::string& _host, unsigned short _port);
    
    void receiveSSLHandshake(HandShakeRequest::ptr handshake_req);
    
    void wsHandShake(HandShakeRequest::ptr handshake_req);
    void receiveWsHandShake(ByteBuffer* buf, 
        HandShakeRequest::ptr handshake_req);

    bool validateWsHandshakeResponse(ByteBuffer* buf, 
        HandShakeRequest::ptr handshake_req);

    void readAsync();
    void writeAsync(PacketData::ptr packet_data,
        const std::string mask_key, SendCallback send_callback);
    void writeAsyncInternal(PacketData::ptr packet_data,
        const std::string mask_key, SendCallback send_callback);
    
    void receivePacket();
    void createWebsocketData(ByteBuffer* buf, std::list<PacketData::ptr>& pd_list);

    //
    void serializeFramingData(bool end, PacketType packet_type,
        const std::string& mask, const std::vector<unsigned char>& body, 
        std::vector<unsigned char>& out_data);
    bool deserializeFramingData(std::vector<unsigned char>& data,
        SocketFrame& socket_frame, int start_index);

    AsyncSocketInf::ptr socket;
    boost::asio::io_service& ios;
    
    int timeout_millis;
    WebsocketDelegate* ws_delegate;
    std::deque<PacketInfo> packet_queue;
    std::atomic<bool> process_write;

    // todo enum state flag
    bool first_process;
    bool connected;
    bool handshake_ok;
    
    ByteBuffer tmp_buffer;
    std::string uuid;
    bool ssl_mode;
};

};

END_NS

#endif
