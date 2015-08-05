#ifndef FB_EV_RES_UIID_H
#define FB_EV_RES_UIID_H

#include "actor/ws_actor.h"

class FbEvResponseUiid
{
public:
    static void response(long actor_key,
        const char* data, int size);
};

#endif
