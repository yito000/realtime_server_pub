#include "simple_random_router.h"
#include "common/common_object.h"

#include "common/global_rand.h"
#include "network/websocket/client/websocket_async.h"

#include "log/logger.h"

long SimpleRandomRouter::getNode(PacketData::ptr pd)
{
    if (!ref_node_list || ref_node_list->size() <= 0) {
        return -1;
    }

    auto& node_list = *ref_node_list;
    auto node_size = node_list.size();

    int max_val = node_size * 1000000;
    int rand_int = 
        GlobalRand::getInstance()->getInt(0, max_val);

    int req_seq = rand_int % node_size;
    int seq = 0;

    for (auto node_info: node_list) {
        if (node_info->websocket->isOpen() && seq == req_seq) {
            return node_info->node_id;
        }

        seq++;
    }

    return -1;
}
