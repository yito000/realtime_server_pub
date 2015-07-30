#ifndef PB_EV_RES_UIID_H
#define PB_EV_RES_UIID_H

#include "actor/ws_actor.h"

class PbEvResponseUiid
{
public:
    static void response(const WsActor* user_client,
        const char* data, int size);
};

#endif
