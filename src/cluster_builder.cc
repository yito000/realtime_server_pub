#include "cluster_builder.h"

#include "cluster/router/round_robin_router.h"
#include "cluster/router/simple_random_router.h"

BEGIN_NS

Cluster::ptr ClusterBuilder::build(Setting::const_ptr setting,
    BidirectionalCommunicator::ptr t_comm)
{
    auto cluster = new Cluster(t_comm);
    
    // TODO: setting
//    cluster->setNodeRouter(new RoundRobinRouter);
    cluster->setNodeRouter(new SimpleRandomRouter);

    //
    auto& cluster_nodes = setting->cluster_nodes;
    for (auto& node: cluster_nodes) {
        cluster->addNode(node->node_id, node->host, 
            node->port, node->protocol);
    }

    return cluster;
}

END_NS
