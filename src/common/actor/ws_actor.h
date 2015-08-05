#ifndef WS_ACTOR_H
#define WS_ACTOR_H

#include "smart_ptr.hpp"
#include "allocator/custom_allocator.hpp"
#include "network/websocket/packet.h"

#include <functional>
#include <boost/system/error_code.hpp>

typedef std::function<void(boost::system::error_code)> WsUserSendCallback;

class WsActor : public SmartPtr<WsActor>, public CustomAllocator<>
{
public:
    typedef boost::intrusive_ptr<WsActor> ptr;
    typedef boost::intrusive_ptr<WsActor const> const_ptr;
    
    enum class Operation {
        WRITE,
        READ
    };

    WsActor();
    virtual ~WsActor();

    virtual void update() const = 0;
    virtual void write(PacketData::ptr pd, 
        WsUserSendCallback send_callback) const = 0;
    virtual void close() const = 0;
    
    virtual void onStart() const = 0;
    virtual void onReceive(PacketData::ptr pd) const = 0;
    virtual void onReceiveFinish(boost::system::error_code ec) const = 0;
    virtual void onSendFinish(boost::system::error_code ec) const = 0;
    virtual void onError(Operation operation, boost::system::error_code ec) const = 0;
    
    // TODO:
    virtual bool isActive() const = 0;

    virtual bool isOpen() const = 0;
    virtual long getKey() const = 0;
};

#endif
