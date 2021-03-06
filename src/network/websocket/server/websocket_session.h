#ifndef WEBSOCKET_SESSION_H
#define WEBSOCKET_SESSION_H

#include "fw_env.h"
#include "allocator/custom_allocator.hpp"

#include <string>
#include <vector>
#include <list>
#include <deque>

#include <functional>
#include <boost/asio.hpp>
#include <atomic>

#include "handshake.h"
#include "server_frame_data.h"

#include "network/websocket/packet.h"
#include "network/io/socket_buffer.h"

#include "session_delegate.h"
#include "network/io/async_socket_inf.h"

#include <boost/asio/ssl.hpp>

BEGIN_NS

namespace server {

typedef std::function<void(boost::system::error_code ec)> SendCallback;

class WebsocketContext
{
public:
    virtual ~WebsocketContext() {}

    virtual bool isOpen() = 0;
    virtual void close() = 0;
    virtual void destroyAsync() = 0;

    virtual void read() = 0;
    virtual void write(PacketData::ptr packet_data,
        SendCallback send_callback) = 0;
    
    virtual int64_t getKey() const = 0;
};

class WebsocketSession : public WebsocketContext
{
public:
    static WebsocketSession* create(boost::asio::io_service& _ios, 
        int _timeout_millis);
    static WebsocketSession* createSSL(boost::asio::io_service& _ios, 
        boost::asio::ssl::context& _ssl_context, int _timeout_millis);
    
    virtual ~WebsocketSession();

    boost::asio::ip::tcp::socket& getSocket();
    void start();

    virtual bool isOpen() override;
    virtual void read() override;
    virtual void write(PacketData::ptr packet_data,
        SendCallback send_callback) override;
    virtual void close() override;

    virtual void destroyAsync() override;

    SessionDelegate* getDelegate()
    {
        return session_delegate;
    }

    void setDelegate(SessionDelegate* sd)
    {
        session_delegate = sd;
    }

    std::string getValidProtocol()
    {
        return valid_protocol;
    }

    void setValidProtocol(const std::string& p)
    {
        valid_protocol = p;
    }
    
    virtual int64_t getKey() const override;

private:
    struct PacketInfo
    {
        PacketData::ptr packet;
        SendCallback callback;
        
        PacketInfo()
        {
            //
        }
    };
    
    WebsocketSession(boost::asio::io_service& _ios, 
        int _timeout_millis);
    bool init(boost::asio::io_service& _ios);
    bool initWithSSL(boost::asio::io_service& _ios, 
        boost::asio::ssl::context& _ssl_context);
    
    void receiveSSLHandshake();
    void receiveWsHandshake(ByteBuffer* buf);
    void wsHandshake(ByteBuffer* buf);
    void sendWsHandshakeOK(HandShakeResponse::ptr h_res);
    HandShakeResponse::ptr validateWsHandshakeRequest(ByteBuffer* buf);

    void readAsync();
    void writeAsync(PacketData::ptr packet_data,
        SendCallback send_callback);
    void writeAsyncInternal(PacketData::ptr packet_data,
        SendCallback send_callback);

    void receivePacket();
    void createWebsocketData(ByteBuffer* buf, 
        std::list<PacketData::ptr>& pd_list);

    //
    void serializeFramingData(bool end, PacketType packet_type,
        const std::vector<unsigned char>& body, std::vector<unsigned char>& out_data);
    bool deserializeFramingData(std::vector<unsigned char>& data,
        SocketFrame& socket_frame, int start_index);

    boost::asio::io_service& ios;
    AsyncSocketInf::ptr socket;
    
    SessionDelegate* session_delegate;
    std::string valid_protocol;
    std::deque<PacketInfo> packet_queue;
    std::atomic<bool> process_write;

    bool start_flag;
    bool handshake_ok;
    int timeout_millis;
    
    ByteBuffer tmp_buffer;
    std::string uuid;
    bool ssl_mode;
};

};

END_NS

#endif
