#ifndef WS_PACKET_DATA_HELPER_H
#define WS_PACKET_DATA_HELPER_H

#include <vector>
#include "network/websocket/packet.h"

class WsPacketDataHelper
{
public:
    static PacketData::ptr buildPacket(int op_code, const unsigned char* data, int size);
    static PacketData::ptr buildPacket(int op_code, std::vector<unsigned char>& v);
    
    static std::string buildMaskKey();
};

#endif
