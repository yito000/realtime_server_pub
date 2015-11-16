#ifndef LEAVE_PACKET_H
#define LEAVE_PACKET_H

#include "battle/packet/battle_packet.h"

#include <string>

struct LeavePacket : public BattlePacket
{
    std::string battle_key;
    int player_id;
    long actor_key;
    
    LeavePacket()
    {
        packet_type = (int)BattlePacketType::LEAVE;
        
        player_id = -1;
        actor_key = -1;
    }
};

#endif
