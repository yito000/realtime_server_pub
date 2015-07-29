#ifndef PB_EV_UIID_H
#define PB_EV_UIID_H

#include "actor/ws_actor.h"

class PbEvUiid
{
public:
    static void addUser(const WsActor* user_client, 
        const char* data, int size);
};

#endif

