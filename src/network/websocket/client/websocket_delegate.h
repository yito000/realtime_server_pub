#ifndef WEBSOCKET_DELEGATE_H
#define WEBSOCKET_DELEGATE_H

#include "../packet.h"
#include <boost/system/error_code.hpp>

#include <functional>

namespace client {

class WebsocketAsync;

class WebsocketDelegate
{
public:
    enum class Operation {
        WRITE,
        READ
    };
    
    virtual ~WebsocketDelegate() {}

    virtual void onStart(WebsocketAsync* ws) = 0;

    virtual void onReceive(WebsocketAsync* ws,
        PacketData::ptr) = 0;
    virtual void onReceiveFinish(WebsocketAsync* ws,
        boost::system::error_code ec) = 0;
    virtual void onSendFinish(WebsocketAsync* ws,
        boost::system::error_code ec) = 0;

    virtual void onError(WebsocketAsync* ws,
        Operation operation, boost::system::error_code ec) = 0;
};

};

#endif
