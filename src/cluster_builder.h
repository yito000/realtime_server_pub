#ifndef CLUSTER_BUILDER_H
#define CLUSTER_BUILDER_H

#include "setting.h"
#include "thread/bidirectional_communicator.h"

#include "cluster/cluster.h"

BEGIN_NS

class ClusterBuilder
{
public:
    static Cluster::ptr build(Setting::const_ptr setting,
        BidirectionalCommunicator::ptr t_comm);
};

END_NS

#endif
