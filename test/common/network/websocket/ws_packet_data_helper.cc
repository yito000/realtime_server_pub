#include "ws_packet_data_helper.h"

PacketData::ptr WsPacketDataHelper::buildPacket(int op_code, 
    const char* ser_data, int size)
{
    auto packet = new PacketData;

    char s_op_code[4] = {0};
    char size_buf[4] = {0};

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
    std::vector<char>& v)
{
    auto packet = new PacketData;
    auto size = v.size();

    char s_op_code[4] = {0};
    char size_buf[4] = {0};

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
    for (int i = 0; i < 4; i++) {
        p_buf.push_back(s_op_code[i]);
    }

    for (int i = 0; i < 4; i++) {
        p_buf.push_back(size_buf[i]);
    }

    p_buf.insert(p_buf.end(), v.begin(), v.end());

    return packet;
}

