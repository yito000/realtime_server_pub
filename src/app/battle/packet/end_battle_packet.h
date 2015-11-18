#ifndef END_BATTLE_PACKET_H
#define END_BATTLE_PACKET_H

#include "battle/packet/battle_packet.h"

#include <string>

struct EndBattlePacket : public BattlePacket
{
    std::string battle_key;
    
    EndBattlePacket()
    {
        packet_type = (int)BattlePacketType::END_BATTLE;
    }
};

#endif
