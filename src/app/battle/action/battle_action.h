#ifndef BATTLE_ACTION_H
#define BATTLE_ACTION_H

#include "fw_env.h"

class BattleAction : public SmartPtr<BattleAction>
{
public:
    typedef boost::intrusive_ptr<BattleAction> ptr;
    
    virtual ~BattleAction() {}
    
    //
};

#endif
