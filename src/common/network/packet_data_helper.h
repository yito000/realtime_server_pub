#ifndef PACKET_DATA_HELPER_H
#define PACKET_DATA_HELPER_H

#include <vector>
#include "network/websocket/packet.h"

class PacketDataHelper
{
public:
    static PacketData::ptr buildPacket(int op_code, const char* data, int size);
    static PacketData::ptr buildPacket(int op_code, std::vector<char>& v);
};

#endif

