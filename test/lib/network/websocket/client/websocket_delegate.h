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
    virtual ~WebsocketDelegate() {}

    virtual void onStart(WebsocketAsync* ws) = 0;

    virtual void onReceive(WebsocketAsync* ws,
        PacketData::ptr) = 0;
    virtual void onReceiveFinish(WebsocketAsync* ws) = 0;
    virtual void onSendFinish(WebsocketAsync* ws) = 0;

    virtual void onError(WebsocketAsync* ws,
        boost::system::error_code ec) = 0;
};

};

#endif

