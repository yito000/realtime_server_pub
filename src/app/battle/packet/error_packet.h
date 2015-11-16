#ifndef ERROR_PACKET_H
#define ERROR_PACKET_H

#include "battle/packet/battle_packet.h"

#include <string>

struct ErrorPacket : public BattlePacket
{
    std::string battle_key;
    int player_id;
    long actor_key;
    int error_code;
    
    ErrorPacket()
    {
        packet_type = (int)BattlePacketType::ERROR;
        
        player_id = -1;
        actor_key = -1;
        error_code = 0;
    }
};

#endif
