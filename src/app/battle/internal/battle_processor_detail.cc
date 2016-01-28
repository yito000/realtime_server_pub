#include "battle_processor_detail.h"

#include "app/flatbuffers/demo_battle.h"
#include "app/flatbuffers/demo_battle_udp.h"
#include "log/logger.h"

//USING_NS

BattlePrcoessorDetail::ptr BattlePrcoessorDetail::create()
{
    BattlePrcoessorDetail::ptr inst = new BattlePrcoessorDetail;
    if (!inst->init()) {
        return nullptr;
    }
    
    return inst;
}

void BattlePrcoessorDetail::playerAction(BattleInfo::ptr battle_info, 
    PlayerInputPacket* packet)
{
    auto player1 = battle_info->getPlayer1();
    auto player2 = battle_info->getPlayer2();
    
    BattlePlayer::ptr self_player;
    BattlePlayer::ptr target_player;
    
    if (player1->getActorKey() == packet->actor_key) {
        self_player = player1;
        target_player = player2;
    } else if (player2->getActorKey() == packet->actor_key) {
        self_player = player2;
        target_player = player1;
    }
    
    if (!self_player || !target_player) {
        return;
    }
    
    // TODO: targeting
    BattleCharacter::ptr target_mobile;
    for (auto t_ch: target_player->getCharacters()) {
        if (!t_ch->isDead()) {
            target_mobile = t_ch;
            break;
        }
    }
    
    if (!target_mobile) {
        return;
    }
    
    auto valid_input = battle_info->validSeqId(packet->seq_id, self_player);
    if (!valid_input) {
        Logger::log("packet duprecated! ... seq_id=%d", packet->seq_id);
        return;
    }
    
    battle_info->setSeqId(packet->seq_id, self_player);
    engage(battle_info, self_player, target_player, packet->input_list);
}

void BattlePrcoessorDetail::playerActionUdp(BattleInfo::ptr battle_info, PlayerInputUdpPacket* packet)
{
    auto player1 = battle_info->getPlayer1();
    auto player2 = battle_info->getPlayer2();
    
    BattlePlayer::ptr self_player;
    BattlePlayer::ptr target_player;
    
    if (player1->enableUdp() && player1->getUdpEndpoint() == packet->ep) {
        self_player = player1;
        target_player = player2;
    } else if (player2->enableUdp() && player2->getUdpEndpoint() == packet->ep) {
        self_player = player2;
        target_player = player1;
    }
    
    if (!self_player || !target_player) {
        return;
    }
    
    auto valid_input = battle_info->validSeqId(packet->seq_id, self_player);
    if (!valid_input) {
        Logger::log("packet duprecated! ... seq_id=%d", packet->seq_id);
        return;
    }
    
    battle_info->setSeqId(packet->seq_id, self_player);
    engage(battle_info, self_player, target_player, packet->input_list);
}

BattleTurnResult::ptr BattlePrcoessorDetail::update(BattleInfo::ptr battle_info, 
    float delta_time)
{
    BattleTurnResult::ptr result = new BattleTurnResult;
    result->status_code = (int)BattleStatus::CONTINUE;
    
    // TODO: check dead status
    auto player1 = battle_info->getPlayer1();
    auto player2 = battle_info->getPlayer2();
    
    bool player1_is_dead = true;
    for (auto ch: player1->getCharacters()) {
        if (!ch->isDead()) {
            player1_is_dead = false;
            break;
        }
    }
    
    bool player2_is_dead = true;
    for (auto ch: player2->getCharacters()) {
        if (!ch->isDead()) {
            player2_is_dead = false;
            break;
        }
    }
    
    if (player1_is_dead && player2_is_dead) {
        result->status_code = (int)BattleStatus::FINISH;
    } else if (player1_is_dead) {
        result->status_code = (int)BattleStatus::FINISH;
        result->win_player_id = player2->getPlayerId();
    } else if (player2_is_dead) {
        result->status_code = (int)BattleStatus::FINISH;
        result->win_player_id = player1->getPlayerId();
    }
    
    return result;
}

// private member function
BattlePrcoessorDetail::BattlePrcoessorDetail()
{
    //
}

bool BattlePrcoessorDetail::init()
{
    return true;
}

void BattlePrcoessorDetail::engage(BattleInfo::ptr battle_info, BattlePlayer::ptr self_player,
    BattlePlayer::ptr target_player, const std::list<CharacterInputPacket::ptr>& input_list)
{
    // TODO: targeting
    BattleCharacter::ptr target_mobile;
    for (auto t_ch: target_player->getCharacters()) {
        if (!t_ch->isDead()) {
            target_mobile = t_ch;
            break;
        }
    }
    
    if (!target_mobile) {
        return;
    }
    
    std::vector<DemoBattle::AttackDetail> attack_detail_list;
    
    // for test
    for (auto ch_packet: input_list) {
        BattleCharacter::ptr self_mobile = self_player->getCharacterFromId(ch_packet->character_id);
        
        // TODO: dispatch command id
        
        const int DAMAGE = 50;
        target_mobile->damageHp(DAMAGE);
        
        DemoBattle::AttackDetail command(ch_packet->character_id, 
            target_player->getPlayerId(), target_mobile->getCharacterId(), 
            ch_packet->command_id, DAMAGE); 
        attack_detail_list.push_back(command);
    }
    
    if (attack_detail_list.size() <= 0) {
        return;
    }
    
    // for test
    flatbuffers::FlatBufferBuilder fbb;
    fbb.CreateVectorOfStructs(attack_detail_list);
    auto buffer_p = 
        reinterpret_cast<const flatbuffers::Vector<
            const DemoBattle::AttackDetail *>*>(fbb.GetBufferPointer());
    
    if (self_player->enableUdp()) {
        DemoBattle::notify_attack_action_udp(battle_info->getBattleSeqId(),
            self_player->getPlayerId(), buffer_p, self_player->getUdpEndpoint());
    }
    
    if (target_player->enableUdp()) {
        DemoBattle::notify_attack_action_udp(battle_info->getBattleSeqId(),
            self_player->getPlayerId(), buffer_p, target_player->getUdpEndpoint());
    }
    
    DemoBattle::notify_attack_action(battle_info->getBattleSeqId(),
        self_player->getPlayerId(), buffer_p, self_player->getActorKey());
    DemoBattle::notify_attack_action(battle_info->getBattleSeqId(),
        self_player->getPlayerId(), buffer_p, target_player->getActorKey());
    
    battle_info->incBattleSeqId();
}
