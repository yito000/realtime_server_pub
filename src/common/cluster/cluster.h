#ifndef CLUSTER_H
#define CLUSTER_H

#include "fw_env.h"
#include <list>

#include "cluster/cluster_node_info.h"
#include "router/node_router.h"

#include "network/websocket/packet.h"
#include "thread/bidirectional_communicator.h"

BEGIN_NS

class ClusterBuilder;

typedef std::function<void(long, 
    boost::system::error_code ec)> NodeSendCallback;
typedef std::function<void(std::list<ClusterNodeInfo::ptr>&)> NodeListCallback;

class Cluster : public SmartPtr<Cluster>
{
    friend class ClusterBuilder;
    friend class ClusterNodeDelegate;

public:
    typedef boost::intrusive_ptr<Cluster> ptr;

    void addNode(long node_id, 
        const std::string& host, unsigned short port,
        const std::string& protocol);
    void removeNode(long node_id);

    void setNodeRouter(NodeRouter::ptr router);

    void broadcast(PacketData::ptr pd, NodeSendCallback callback);
    void sendOnce(long node_id, PacketData::ptr pd,
        NodeSendCallback callback);
    void sendRouter(PacketData::ptr pd, NodeSendCallback callback);

    bool isWatchMode();
    void setWatchMode(bool flag);

    void run();

private:
    Cluster(BidirectionalCommunicator::ptr t_comm);

    void addNewNodeInfo(long node_id,
        const std::string& host, unsigned short port,
        const std::string& protocol);
    void addActiveNode(long node_id,
        client::WebsocketAsync* websocket);
    void removeActiveNode(long node_id);
    
    BidirectionalCommunicator::ptr task_comm;
    NodeRouter::ptr node_router;
    std::list<ClusterNodeInfo::ptr> node_list;
    std::list<ActiveClusterNodeInfo::ptr> active_node_list;

    bool watch_mode;
};

END_NS

#endif
