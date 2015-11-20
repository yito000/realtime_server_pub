#ifndef ATTACK_ACTION_H
#define ATTACK_ACTION_H

#include "battle_action.h"

class AttackAction : public BattleAction
{
public:
    typedef boost::intrusive_ptr<AttackAction> ptr;
    
private:
    AttackAction();
};

#endif
