#ifndef ROUND_ROBIN_ROUTER_H
#define ROUND_ROBIN_ROUTER_H

#include "node_router.h"

BEGIN_NS

class RoundRobinRouter : public NodeRouter
{
public:
    RoundRobinRouter();

    virtual long getNode(PacketData::ptr pd);

private:
    int last_seq;
};

END_NS

#endif
