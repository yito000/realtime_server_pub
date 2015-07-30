#ifndef USER_CLIENT_H
#define USER_CLIENT_H

#include "actor/ws_actor.h"
#include "network/websocket/server/websocket_session.h"

class UserClient : public WsActor
{
public:
    UserClient(server::WebsocketContext* s);
    virtual ~UserClient();

    virtual void update() const;
    virtual void write(PacketData::ptr pd, 
        server::SendCallback send_callback) const;

    virtual void onStart() const;
    virtual void onReceive(PacketData::ptr r_pd) const;
    virtual void onReceiveFinish() const;
    virtual void onSendFinish() const;
    virtual void onError(boost::system::error_code ec) const;

    virtual bool isOpen() const
    {
        return session->isOpen();
    }

    virtual long getKey() const;

private:
    server::WebsocketContext* session;
};

#endif
