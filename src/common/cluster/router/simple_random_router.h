#ifndef SIMPLE_RANDOM_ROUTER_H
#define SIMPLE_RANDOM_ROUTER_H

#include "node_router.h"

class SimpleRandomRouter : public NodeRouter
{
public:
    virtual long getNode(PacketData::ptr pd);
};

#endif

