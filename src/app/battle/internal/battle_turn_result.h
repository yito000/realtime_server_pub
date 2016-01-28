#ifndef BATTLE_TURN_RESULT_H
#define BATTLE_TURN_RESULT_H

#include "fw_env.h"

#include "battle/internal/battle_status_code.h"

struct BattleTurnResult : public SmartPtr<BattleTurnResult>
{
    typedef boost::intrusive_ptr<BattleTurnResult> ptr;
    
    int status_code;
    int win_player_id;
    
    BattleTurnResult()
    {
        status_code = (int)BattleStatus::CONTINUE;
        win_player_id = -1;
    }
};

#endif
