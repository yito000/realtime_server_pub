#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include "smart_ptr.hpp"
#include <vector>

class WsActor;
typedef boost::intrusive_ptr<WsActor> WsActorPtr;

class CommandDispatcher
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
