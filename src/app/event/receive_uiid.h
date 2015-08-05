#ifndef RECEIVE_UIID_H
#define RECEIVE_UIID_H

#include <string>
#include "actor/ws_actor.h"

class ReceiveUiid
{
public:
    static void exec(long actor_key,
        const std::string uiid);
};

#endif
