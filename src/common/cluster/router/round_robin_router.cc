#include "round_robin_router.h"

#include "network/websocket/client/websocket_async.h"

RoundRobinRouter::RoundRobinRouter() : 
    last_seq(-1)
{
    //
}

long RoundRobinRouter::getNode(PacketData::ptr pd)
{
    if (!ref_node_list || ref_node_list->size() <= 0) {
        return -1;
    }

    int seq = 0;
    auto& node_list = *ref_node_list;

    for (auto node_info: node_list) {
        if (node_info->websocket->isOpen() && seq == last_seq + 1) {
            last_seq++;
            return node_info->node_id;
        }

        seq++;
    }

    last_seq = 0;
    auto node_info = *node_list.begin();

    return node_info->node_id;
}
