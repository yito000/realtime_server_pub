#ifndef PLAYER_INPUT_PACKET_H
#define PLAYER_INPUT_PACKET_H

#include "battle/packet/battle_packet.h"

#include "smart_ptr.hpp"

#include <string>
#include <list>

struct CharacterInputPacket : public SmartPtr<CharacterInputPacket>
{
    typedef boost::intrusive_ptr<CharacterInputPacket> ptr;
    
    int character_id;
    int command_id;
    
    CharacterInputPacket()
    {
        character_id = -1;
        command_id = -1;
    }
};

struct PlayerInputPacket : public BattlePacket
{
    int player_id;
    long actor_key;
    std::list<CharacterInputPacket::ptr> input_list;
    
    PlayerInputPacket()
    {
        packet_type = (int)BattlePacketType::PLAYER_INPUT;
        
        player_id = -1;
        actor_key = -1;
    }
};

#endif
