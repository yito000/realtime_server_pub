#include "ws_packet_data_helper.h"

#include "random/random.h"

#include <chrono>

namespace {
    Random s_rand(std::chrono::system_clock::now().
        time_since_epoch().count() / 1000 / 1000);
};

PacketData::ptr WsPacketDataHelper::buildPacket(int op_code, 
    const unsigned char* ser_data, int size)
{
    auto packet = new PacketData;

    unsigned char s_op_code[4] = {0};
    unsigned char size_buf[4] = {0};

    s_op_code[0] = (op_code >> 24) & 255;
    s_op_code[1] = (op_code >> 16) & 255;
    s_op_code[2] = (op_code >> 8) & 255;
    s_op_code[3] = op_code & 255;

    size_buf[0] = (size >> 24) & 255;
    size_buf[1] = (size >> 16) & 255;
    size_buf[2] = (size >> 8) & 255;
    size_buf[3] = size & 255;

    //
    auto& p_buf = packet->data;
    p_buf.reserve(4 + 4 + size);
    for (int i = 0; i < 4; i++) {
        p_buf.push_back(s_op_code[i]);
    }

    for (int i = 0; i < 4; i++) {
        p_buf.push_back(size_buf[i]);
    }

    for (int i = 0; i < size; i++) {
        p_buf.push_back(ser_data[i]);
    }

    return packet;
}

PacketData::ptr WsPacketDataHelper::buildPacket(int op_code, 
    std::vector<unsigned char>& v)
{
    auto packet = new PacketData;
    auto size = v.size();

    unsigned char s_op_code[4] = {0};
    unsigned char size_buf[4] = {0};

    s_op_code[0] = (op_code >> 24) & 255;
    s_op_code[1] = (op_code >> 16) & 255;
    s_op_code[2] = (op_code >> 8) & 255;
    s_op_code[3] = op_code & 255;

    size_buf[0] = (size >> 24) & 255;
    size_buf[1] = (size >> 16) & 255;
    size_buf[2] = (size >> 8) & 255;
    size_buf[3] = size & 255;

    //
    auto& p_buf = packet->data;
    p_buf.reserve(4 + 4 + size);
    for (int i = 0; i < 4; i++) {
        p_buf.push_back(s_op_code[i]);
    }

    for (int i = 0; i < 4; i++) {
        p_buf.push_back(size_buf[i]);
    }

    p_buf.insert(p_buf.end(), v.begin(), v.end());

    return packet;
}

std::string WsPacketDataHelper::buildMaskKey()
{
    char mask_key[4] = {0};
    for (int i = 0; i < 4; i++) {
        mask_key[i] = s_rand.getInt(0x0, 0xff);
    }
    
    std::string ret(mask_key, 4);
    return ret;
}
