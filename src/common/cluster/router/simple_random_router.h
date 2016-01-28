#ifndef SIMPLE_RANDOM_ROUTER_H
#define SIMPLE_RANDOM_ROUTER_H

#include "node_router.h"

BEGIN_NS

class SimpleRandomRouter : public NodeRouter
{
public:
    virtual long getNode(PacketData::ptr pd);
};

END_NS

#endif
