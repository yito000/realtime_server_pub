#ifndef CLUSTER_NODE_INFO_H
#define CLUSTER_NODE_INFO_H

#include "fw_env.h"

#include <string>

BEGIN_NS

struct ClusterNodeInfo : public SmartPtr<ClusterNodeInfo>
{
    typedef boost::intrusive_ptr<ClusterNodeInfo> ptr;

    long node_id;
    std::string host;
    unsigned short port;
    std::string protocol;

    ClusterNodeInfo()
    {
        node_id = -1;
        port = 0;
    }
};

namespace client {
    class WebsocketAsync;
};

struct ActiveClusterNodeInfo : public SmartPtr<ActiveClusterNodeInfo>
{
    typedef boost::intrusive_ptr<ActiveClusterNodeInfo> ptr;

    long node_id;
    client::WebsocketAsync* websocket;

    ActiveClusterNodeInfo()
    {
        node_id = -1;
        websocket = nullptr;
    }
};

END_NS

#endif

