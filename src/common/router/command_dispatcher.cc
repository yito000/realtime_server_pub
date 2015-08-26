#include "command_dispatcher.h"

#include "common_object.h"
#include "actor/ws_actor.h"

namespace {
    const int OP_CODE_SIZE = 4;
    const int SIZE_DATA_SIZE = 4;

    const int HEADER_SIZE = OP_CODE_SIZE + SIZE_DATA_SIZE;
};

int charToInt(const char* s)
{
    int ret = 0;
    ret = s[0] << 24;
    ret |= s[1] << 16;
    ret |= s[2] << 8;
    ret |= s[3];

    return ret;
}

void CommandDispatcher::bulkDispatch(long actor_key, 
    const std::vector<char>& data)
{
    if (data.size() <= HEADER_SIZE) {
        return;
    }

    int cur_pos = 0;

    while (cur_pos < data.size()) {
        char s_op_code[4] = {
            data[cur_pos], data[cur_pos + 1], 
            data[cur_pos + 2], data[cur_pos + 3]
        };
        char s_data_size[4] = {
            data[cur_pos + 4], data[cur_pos + 5], 
            data[cur_pos + 6], data[cur_pos + 7]
        };

        int op_code = charToInt(s_op_code);
        int data_size = charToInt(s_data_size);

        cur_pos += HEADER_SIZE;

        if (data_size <= data.size() - cur_pos) {
            int start_index = cur_pos;
            dispatchData(actor_key, op_code, 
                start_index, data_size, data);

            cur_pos += data_size;
        } else {
            break;
        }
    }
}

// global function
static bool DispatchCommand(
    Router::ptr router, long actor_key,
    int op_code, int start_index, int data_size,
    const std::vector<char>& data)
{
    auto callback = router->getRoute(op_code);

    if (callback) {
        char* buf = new char[data_size];
        int pos = 0;
        int end_index = start_index + data_size;
        for (int i = start_index; i < end_index; i++) {
            buf[pos] = data[i];
            pos++;
        }

        callback(actor_key, buf, data_size);

        delete[] buf;

        return true;
    } else {
        return false;
    }
}

// private member function
void CommandDispatcher::dispatchData(long actor_key, 
    int op_code, int start_index, int data_size, 
    const std::vector<char>& data)
{
    auto sys_router = 
        CommonObject::getInstance()->getSystemRouter();

    if (!DispatchCommand(sys_router, actor_key, 
        op_code, start_index, data_size, data)) {

        auto app_router =
            CommonObject::getInstance()->getUserRouter();

        DispatchCommand(app_router, actor_key, 
            op_code, start_index, data_size, data);
    }
}