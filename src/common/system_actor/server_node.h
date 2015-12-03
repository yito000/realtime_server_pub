#ifndef SERVER_NODE_H
#define SERVER_NODE_H

#include "actor/ws_actor.h"
#include "network/websocket/server/websocket_session.h"

#include <atomic>

class ServerNode : public WsActor
{
public:
    ServerNode(server::WebsocketContext* s);
    virtual ~ServerNode();

    virtual void update() const;
    virtual void write(PacketData::ptr pd, 
        server::SendCallback send_callback) const;
    virtual void close() const;
    
    virtual void onStart() const;
    virtual void onReceive(PacketData::ptr r_pd) const;
    virtual void onReceiveFinish(boost::system::error_code ec) const;
    virtual void onSendFinish(boost::system::error_code ec) const;
    virtual void onError(Operation operation, boost::system::error_code ec) const;
    
    virtual bool isActive() const override;

    virtual bool isOpen() const
    {
        return session->isOpen();
    }

    virtual long getKey() const;

private:
    server::WebsocketContext* session;
    
    mutable std::atomic<bool> first_process;
    mutable std::atomic<size_t> read_cnt;
    mutable std::atomic<size_t> write_cnt;
};

#endif
