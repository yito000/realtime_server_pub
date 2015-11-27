#ifndef CHARACTER_INPUT_PACKET_INFO_H
#define CHARACTER_INPUT_PACKET_INFO_H

#include "smart_ptr.hpp"

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

#endif
