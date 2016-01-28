#ifndef NODE_ROUTER_H
#define NODE_ROUTER_H

#include "fw_env.h"
#include <list>

#include "cluster/cluster_node_info.h"
#include "network/websocket/packet.h"

BEGIN_NS

class NodeRouter : public SmartPtr<NodeRouter>
{
public:
    typedef boost::intrusive_ptr<NodeRouter> ptr;

    NodeRouter() : ref_node_list(nullptr) {}
    virtual ~NodeRouter() {}

    virtual long getNode(PacketData::ptr pd) = 0;

    void setNodeList(std::list<ActiveClusterNodeInfo::ptr>* ref_list)
    {
        ref_node_list = ref_list;
    }

protected:
    std::list<ActiveClusterNodeInfo::ptr>* ref_node_list;
};

END_NS

#endif

