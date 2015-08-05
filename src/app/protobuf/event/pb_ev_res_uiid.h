#ifndef PB_EV_RES_UIID_H
#define PB_EV_RES_UIID_H

#include "actor/ws_actor.h"

class PbEvResponseUiid
{
public:
    static void response(long actor_key,
        const char* data, int size);
};

#endif
