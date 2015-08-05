#ifndef CLUSTER_NODE_DELEGATE_H
#define CLUSTER_NODE_DELEGATE_H

#include "common/network/websocket/client/websocket_async.h"
#include "thread/bidirectional_communicator.h"

class Cluster;
typedef boost::intrusive_ptr<Cluster> ClusterPtr;

class ClusterNodeDelegate : public client::WebsocketDelegate
{
public:
    ClusterNodeDelegate(int _node_id, BidirectionalCommunicator::ptr t_comm,
        ClusterPtr _cluster);

    virtual void onStart(client::WebsocketAsync* ws);
    virtual void onReceive(client::WebsocketAsync* ws,
        PacketData::ptr pd);
    virtual void onReceiveFinish(client::WebsocketAsync* ws,
        boost::system::error_code ec);
    virtual void onSendFinish(client::WebsocketAsync* ws,
        boost::system::error_code ec);
    virtual void onError(client::WebsocketAsync* ws,
        Operation operation, boost::system::error_code ec);

private:
    int node_id;
    BidirectionalCommunicator::ptr task_comm;
    
    ClusterPtr cluster;
};

#endif
