#ifndef FB_EVENT_UIID_H
#define FB_EVENT_UIID_H

#include "actor/ws_actor.h"

class FbEvUiid
{
public:
    static void addUser(long actor_key, 
        const char* data, int size);
};

#endif
