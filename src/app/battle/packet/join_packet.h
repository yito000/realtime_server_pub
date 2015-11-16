#ifndef JOIN_PACKET_H
#define JOIN_PACKET_H

#include "battle/packet/battle_packet.h"

#include <string>

struct JoinPacket : public BattlePacket
{
    std::string battle_key;
    int player_id;
    long actor_key;
    
    JoinPacket()
    {
        packet_type = (int)BattlePacketType::JOIN;
        
        player_id = -1;
        actor_key = -1;
    }
};

#endif
