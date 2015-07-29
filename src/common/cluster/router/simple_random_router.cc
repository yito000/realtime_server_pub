#include "simple_random_router.h"

#include "common_object.h"

long SimpleRandomRouter::getNode(PacketData::ptr pd)
{
    if (!ref_node_list || ref_node_list->size() <= 0) {
        return -1;
    }

    auto& node_list = *ref_node_list;
    auto node_size = node_list.size();

    int max_val = node_size * 1000000;
    int rand_int = 
        CommonObject::getInstance()->getRandomInt(0, max_val);

    int req_seq = rand_int % node_size;
    int seq = 0;

    for (auto node_info: node_list) {
        if (seq == req_seq) {
            return node_info->node_id;
        }

        seq++;
    }

    return -1;
}

