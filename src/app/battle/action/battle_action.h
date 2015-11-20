#ifndef BATTLE_ACTION_H
#define BATTLE_ACTION_H

#include "smart_ptr.hpp"

class BattleAction : public SmartPtr<BattleAction>
{
public:
    typedef boost::intrusive_ptr<BattleAction> ptr;
    
    virtual ~BattleAction() {}
    
    //
};

#endif
