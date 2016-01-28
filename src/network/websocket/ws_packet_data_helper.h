#ifndef WS_PACKET_DATA_HELPER_H
#define WS_PACKET_DATA_HELPER_H

#include "fw_env.h"

#include <vector>
#include "network/websocket/packet.h"

BEGIN_NS

class WsPacketDataHelper
{
public:
    static PacketData::ptr buildPacket(int op_code, const unsigned char* data, int size);
    static PacketData::ptr buildPacket(int op_code, std::vector<unsigned char>& v);
    
    static std::string buildMaskKey();
};

END_NS

#endif
