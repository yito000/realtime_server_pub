#ifndef CLUSTER_ACTOR_H
#define CLUSTER_ACTOR_H

#include "actor/ws_actor.h"
#include "network/websocket/client/websocket_async.h"

class ClusterActor : public WsActor
{
public:
    ClusterActor(int _node_id, client::WebsocketAsync* ws);
    virtual ~ClusterActor();

    virtual void update() const;
    virtual void write(PacketData::ptr pd,
        client::SendCallback send_callback) const;

    virtual void onStart() const;
    virtual void onReceive(PacketData::ptr r_pd) const;
    virtual void onReceiveFinish() const;
    virtual void onSendFinish() const;
    virtual void onError(boost::system::error_code ec) const;

    virtual bool isOpen() const
    {
        return websocket->isOpen();
    }

    virtual long getKey() const;

private:
    int node_id;
    client::WebsocketAsync* websocket;
};

#endif
