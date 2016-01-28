#ifndef BATTLE_PROCESSOR_DETAIL_H
#define BATTLE_PROCESSOR_DETAIL_H

#include "fw_env.h"

#include "battle/battle_info.h"
#include "battle/packet/player_input_packet.h"
#include "battle/packet/player_input_udp_packet.h"
#include "battle/internal/battle_turn_result.h"

class BattlePrcoessorDetail : public SmartPtr<BattlePrcoessorDetail>
{
public:
    typedef boost::intrusive_ptr<BattlePrcoessorDetail> ptr;
    
    static BattlePrcoessorDetail::ptr create();
    
    void playerAction(BattleInfo::ptr battle_info, PlayerInputPacket* packet);
    void playerActionUdp(BattleInfo::ptr battle_info, PlayerInputUdpPacket* packet);
    BattleTurnResult::ptr update(BattleInfo::ptr battle_info, float delta_time);
    
private:
    BattlePrcoessorDetail();
    bool init();
    
    void engage(BattleInfo::ptr battle_info, BattlePlayer::ptr self_player,
        BattlePlayer::ptr target_player, const std::list<CharacterInputPacket::ptr>& input_list);
};

#endif
