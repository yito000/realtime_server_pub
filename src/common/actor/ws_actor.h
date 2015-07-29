#ifndef WS_ACTOR_H
#define WS_ACTOR_H

#include "smart_ptr.hpp"
#include "network/websocket/packet.h"

#include <functional>
#include <boost/system/error_code.hpp>

typedef std::function<void(boost::system::error_code)> WsUserSendCallback;

class WsActor : public SmartPtr<WsActor>
{
public:
    typedef boost::intrusive_ptr<WsActor> ptr;
    typedef boost::intrusive_ptr<WsActor const> const_ptr;

    WsActor();
    virtual ~WsActor();

    virtual void update() const = 0;
    virtual void write(PacketData::ptr pd, 
        WsUserSendCallback send_callback) const = 0;

    virtual void onStart() const = 0;
    virtual void onReceive(PacketData::ptr pd) const = 0;
    virtual void onReceiveFinish() const = 0;
    virtual void onSendFinish() const = 0;
    virtual void onError(boost::system::error_code ec) const = 0;

    virtual bool isOpen() const = 0;
    virtual long getKey() const = 0;
};

#endif

