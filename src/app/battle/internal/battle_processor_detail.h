#ifndef BATTLE_PROCESSOR_DETAIL_H
#define BATTLE_PROCESSOR_DETAIL_H

#include "smart_ptr.hpp"

#include "battle/battle_info.h"
#include "battle/packet/player_input_packet.h"
#include "battle/internal/battle_turn_result.h"

class BattlePrcoessorDetail : public SmartPtr<BattlePrcoessorDetail>
{
public:
    typedef boost::intrusive_ptr<BattlePrcoessorDetail> ptr;
    
    static BattlePrcoessorDetail::ptr create();
    
    void playerAction(BattleInfo::ptr battle_info, PlayerInputPacket* packet);
    BattleTurnResult::ptr update(BattleInfo::ptr battle_info, float delta_time);
    
private:
    BattlePrcoessorDetail();
    bool init();
};

#endif
