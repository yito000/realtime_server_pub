#ifndef BATTLE_INPUT_INFO_H
#define BATTLE_INPUT_INFO_H

#include "smart_ptr.hpp"

#include <string>
#include <vector>

struct BattleInputCommandDetail
{
    int character_id;
    int command_id;
    
    BattleInputCommandDetail() :
        character_id(-1),
        command_id(-1)
    {
        //
    }
    
    BattleInputCommandDetail(int _character_id, int _command_id) :
        character_id(_character_id),
        command_id(_command_id)
    {
        //
    }
};

struct BattleInputInfo : public SmartPtr<BattleInputInfo>
{
    typedef boost::intrusive_ptr<BattleInputInfo> ptr;
    
    std::string battle_key;
    int player_id;
    std::vector<BattleInputCommandDetail> list;
    long actor_key;
    
    BattleInputInfo()
    {
        player_id = -1;
        actor_key = -1;
    }
};

#endif
