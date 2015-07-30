#ifndef WEBSOCKET_ASYNC_H
#define WEBSOCKET_ASYNC_H

#include "smart_ptr.hpp"

#include <string>
#include <vector>
#include <list>

#include <functional>
#include <boost/asio.hpp>

#include "frame_data.h"

#include "../packet.h"
#include "../socket_buffer.h"

#include "handshake.h"
#include "websocket_delegate.h"

namespace client {

typedef std::function<void(boost::system::error_code ec)> SendCallback;

class WebsocketAsync : public SmartPtr<WebsocketAsync>
{
public:
    typedef boost::intrusive_ptr<WebsocketAsync> ptr;

    WebsocketAsync(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned short _port,
        int _timeout_millis, int _retry);
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

private:
    void connectInternal(boost::system::error_code err,
        boost::asio::ip::tcp::resolver::iterator endpoint_it,
        HandShakeRequest::ptr handshake_req);
    void handShake(HandShakeRequest::ptr handshake_req);
    void receiveHandShake(ByteBuffer* buf, 
        HandShakeRequest::ptr handshake_req);

    bool validateHandshakeResponse(ByteBuffer* buf, 
        HandShakeRequest::ptr handshake_req);

    void readAsync();
    void writeAsync(PacketData::ptr packet_data,
        const std::string mask_key, SendCallback send_callback);

    void receivePacket();
    int createWebsocketData(ByteBuffer* buf, std::list<PacketData::ptr>& pd_list);

    //
    void serializeFramingData(bool end, PacketType packet_type,
        const std::string& mask, const std::vector<char>& body, 
        std::vector<char>& out_data);
    bool deserializeFramingData(std::vector<char>& data,
        SocketFrame& socket_frame, int start_index);

    void checkDeadline(boost::asio::deadline_timer* timer);

    std::string host;
    unsigned short port;
    int timeout_millis;
    int retry;

    boost::asio::io_service& ios;
    boost::asio::io_service::strand ios_st;
    boost::asio::ip::tcp::socket socket;

    boost::asio::deadline_timer read_timer;
    boost::asio::deadline_timer write_timer;

    enum { MAX_LENGTH = 8192 };
    char data[MAX_LENGTH];

    WebsocketDelegate* ws_delegate;

    // todo enum state flag
    bool first_process;
    bool connected;
    bool handshake_ok;
    
    ByteBuffer tmp_buffer;
};

};

#endif
