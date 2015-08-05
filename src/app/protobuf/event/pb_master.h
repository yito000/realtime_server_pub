#ifndef PB_MASTER_H
#define PB_MASTER_H

#include "actor/ws_actor.h"

class PbMaster
{
public:
    static void sendWorker(long actor_key,
        const char* data, int size);
};

#endif
