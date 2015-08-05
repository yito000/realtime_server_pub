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
    virtual void close() const;
    
    virtual void onStart() const;
    virtual void onReceive(PacketData::ptr r_pd) const;
    virtual void onReceiveFinish(boost::system::error_code ec) const;
    virtual void onSendFinish(boost::system::error_code ec) const;
    virtual void onError(Operation operation, boost::system::error_code ec) const;
    
    virtual bool isActive() const override;

    virtual bool isOpen() const
    {
        return websocket->isOpen();
    }

    virtual long getKey() const;

private:
    int node_id;
    client::WebsocketAsync* websocket;
    
    mutable bool first_process;
    mutable size_t read_cnt;
    mutable size_t write_cnt;
    std::string uuid;
};

#endif
