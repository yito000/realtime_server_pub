#ifndef FB_MASTER_H
#define FB_MASTER_H

#include "actor/ws_actor.h"

class FbMaster
{
public:
    static void sendWorker(long actor_key,
        const char* data, int size);
};

#endif
