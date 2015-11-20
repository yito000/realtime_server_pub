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
    
    CharacterInputPacket() :
        character_id(-1),
        command_id(-1)
    {
        //
    }
    
    CharacterInputPacket(int _character_id, int _command_id) :
        character_id(_character_id),
        command_id(_command_id)
    {
        //
    }
};

struct PlayerInputPacket : public BattlePacket
{
    std::string battle_key;
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
