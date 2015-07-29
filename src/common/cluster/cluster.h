#ifndef CLUSTER_H
#define CLUSTER_H

#include "smart_ptr.hpp"
#include <list>

#include "cluster_node_info.h"
#include "router/node_router.h"

#include "network/websocket/packet.h"
#include "thread/bidirectional_communicator.h"

class ClusterBuilder;

typedef std::function<void(long, 
    boost::system::error_code ec)> NodeSendCallback;
typedef std::function<void(std::list<ClusterNodeInfo::ptr>&)> NodeListCallback;

class Cluster : public SmartPtr<Cluster>
{
    friend class ClusterBuilder;

public:
    typedef boost::intrusive_ptr<Cluster> ptr;

    void addNode(long node_id, 
        const std::string& host, unsigned short port);
    void removeNode(long node_id);
    void cloneNodeList(NodeListCallback callback);

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
        const std::string& host, unsigned short port);

    BidirectionalCommunicator::ptr task_comm;
    NodeRouter::ptr node_router;
    std::list<ClusterNodeInfo::ptr> node_list;

    bool watch_mode;
};

#endif

