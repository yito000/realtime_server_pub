#ifndef WEBSOCKET_SESSION_H
#define WEBSOCKET_SESSION_H

#include <functional>
#include <boost/asio.hpp>

#include <vector>
#include <list>

#include "handshake.h"
#include "server_frame_data.h"

#include "../packet.h"
#include "../socket_buffer.h"

#include "session_delegate.h"
#include "network/io/async_socket_inf.h"

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
};

class WebsocketSession : public WebsocketContext
{
public:
    static WebsocketSession* create(boost::asio::io_service& _ios, 
        int _timeout_millis);
    
    virtual ~WebsocketSession();

    boost::asio::ip::tcp::socket& getSocket();
    void start();

    bool isOpen();
    void read();
    void write(PacketData::ptr packet_data,
        SendCallback send_callback);
    void close();

    void destroyAsync();

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

private:
    WebsocketSession(boost::asio::io_service& _ios, 
        int _timeout_millis);
    bool init(boost::asio::io_service& _ios);
    
    void receiveHandShake(ByteBuffer* buf);
    void handShake(ByteBuffer* buf);
    void sendHandShakeOK(HandShakeResponse::ptr h_res);
    HandShakeResponse::ptr validateHandShakeRequest(ByteBuffer* buf);

    void readAsync();
    void writeAsync(PacketData::ptr packet_data,
        SendCallback send_callback);

    void receivePacket();
    void createWebsocketData(ByteBuffer* buf, 
        std::list<PacketData::ptr>& pd_list);

    //
    void serializeFramingData(bool end, PacketType packet_type,
        const std::vector<char>& body, std::vector<char>& out_data);
    bool deserializeFramingData(std::vector<char>& data,
        SocketFrame& socket_frame, int start_index);

    boost::asio::io_service& ios;
    AsyncSocketInf::ptr socket;
    
    SessionDelegate* session_delegate;
    std::string valid_protocol;

    bool start_flag;
    bool handshake_ok;
    int timeout_millis;
    
    ByteBuffer tmp_buffer;
};

};

#endif
