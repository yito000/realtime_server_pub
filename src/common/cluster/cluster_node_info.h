#ifndef CLUSTER_NODE_INFO_H
#define CLUSTER_NODE_INFO_H

#include "smart_ptr.hpp"

struct ClusterNodeInfo : public SmartPtr<ClusterNodeInfo>
{
    typedef boost::intrusive_ptr<ClusterNodeInfo> ptr;

    long node_id;
    std::string host;
    unsigned short port;

    ClusterNodeInfo()
    {
        node_id = -1;
        port = 0;
    }
};

#endif

