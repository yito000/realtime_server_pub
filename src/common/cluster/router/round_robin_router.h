#ifndef ROUND_ROBIN_ROUTER_H
#define ROUND_ROBIN_ROUTER_H

#include "node_router.h"

class RoundRobinRouter : public NodeRouter
{
public:
    RoundRobinRouter();

    virtual long getNode(PacketData::ptr pd);

private:
    int last_seq;
};

#endif

