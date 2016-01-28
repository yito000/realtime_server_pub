#ifndef PLAYER_INPUT_PACKET_H
#define PLAYER_INPUT_PACKET_H

#include "battle/packet/battle_packet.h"
#include "battle/packet/character_input_packet_info.h"

#include "fw_env.h"

#include <string>
#include <list>

struct PlayerInputPacket : public BattlePacket
{
    int seq_id;
    std::string battle_key;
    int player_id;
    long actor_key;
    std::list<CharacterInputPacket::ptr> input_list;
    
    PlayerInputPacket()
    {
        packet_type = (int)BattlePacketType::PLAYER_INPUT;
        
        seq_id = 0;
        player_id = -1;
        actor_key = -1;
    }
};

#endif
