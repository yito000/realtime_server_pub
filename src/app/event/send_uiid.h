#ifndef SEND_UIID_H
#define SEND_UIID_H

#include <string>
#include "actor/ws_actor.h"

class SendUiid
{
public:
    static void exec(const WsActor* user_client, 
        const std::string uiid);
};

#endif
