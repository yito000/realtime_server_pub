#ifndef SESSION_DELEGATE_H
#define SESSION_DELEGATE_H

#include "fw_env.h"

#include "../packet.h"
#include <boost/system/error_code.hpp>

#include <functional>

BEGIN_NS

namespace server {

class WebsocketSession;

class SessionDelegate
{
public:
    enum class Operation {
        CONNECT,
        WRITE,
        READ
    };
    
    virtual ~SessionDelegate() {}

    virtual void onStart(WebsocketSession* session) = 0;

    virtual void onReceive(WebsocketSession* session,
        PacketData::ptr) = 0;
    virtual void onReceiveFinish(WebsocketSession* session,
        boost::system::error_code ec) = 0;
    virtual void onSendFinish(WebsocketSession* session,
        boost::system::error_code ec) = 0;

    virtual void onError(WebsocketSession* session, 
        Operation operation, boost::system::error_code ec) = 0;
};

};

END_NS

#endif
