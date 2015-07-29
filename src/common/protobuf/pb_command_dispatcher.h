#ifndef PB_COMMAND_DISPATCHER_H
#define PB_COMMAND_DISPATCHER_H

#include <vector>
#include "protobuf/protobuf_router.h"

class WsActor;

class PbCommandDispatcher
{
public:
    static void bulkDispatch(const WsActor* user_client, 
        const std::vector<char>& data);

private:
    static void dispatchData(const WsActor* user_client, 
        int op_code, int start_index, int data_size, 
        const std::vector<char>& data);
};

#endif

