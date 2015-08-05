#ifndef PB_COMMAND_DISPATCHER_H
#define PB_COMMAND_DISPATCHER_H

#include <vector>
#include "protobuf/protobuf_router.h"

class WsActor;
typedef boost::intrusive_ptr<WsActor> WsActorPtr;

class PbCommandDispatcher
{
public:
    static void bulkDispatch(long actor_key, 
        const std::vector<char>& data);

private:
    static void dispatchData(long actor_key, 
        int op_code, int start_index, int data_size, 
        const std::vector<char>& data);
};

#endif
