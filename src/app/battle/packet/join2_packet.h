#ifndef JOIN2_PACKET_H
#define JOIN2_PACKET_H

#include "battle/packet/battle_packet.h"

#include <string>

struct Join2Packet : public BattlePacket
{
    std::string battle_key;
    int player_id;
    long actor_key;
    std::string access_token;
    std::string base64_redis_value;
    
    Join2Packet()
    {
        packet_type = (int)BattlePacketType::JOIN2;
        
        player_id = -1;
        actor_key = -1;
    }
};

#endif
