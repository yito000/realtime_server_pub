#include "battle_info.h"

BattleInfo::ptr BattleInfo::create(const std::string& _battle_key,
    BattlePlayer::ptr p1, BattlePlayer::ptr p2)
{
    BattleInfo::ptr inst = new BattleInfo;
    if (!inst->init(_battle_key, p1, p2)) {
        return nullptr;
    }
    
    return inst;
}

const std::string& BattleInfo::getBattleKey() const
{
    return battle_key;
}

BattlePlayer::ptr BattleInfo::getPlayer1() const
{
    return player1;
}

BattlePlayer::ptr BattleInfo::getPlayer2() const
{
    return player2;
}

BattlePlayer::ptr BattleInfo::getPlayerFromId(int player_id)
{
    BattlePlayer::ptr ret;
    if (player1->getPlayerId() == player_id) {
        ret = player1;
    } else if (player2->getPlayerId() == player_id) {
        ret = player2;
    }
    
    return ret;
}

void BattleInfo::updateTimer(const AppTime::Point& tp)
{
    last_update = tp;
}

int BattleInfo::diffMillis(const AppTime::Point& other) const
{
    return (int)((float)(other - last_update).count() / 1000.0);
}

int BattleInfo::getBattleSeqId() const
{
    return battle_seq_id;
}

void BattleInfo::incBattleSeqId()
{
    battle_seq_id++;
}

bool BattleInfo::validSeqId(int seq_id, BattlePlayer::ptr player)
{
    if (player == player1) {
        auto it = player1_seq_ids.find(seq_id);
        auto ret = it == player1_seq_ids.end();
        
        return ret;
    } else if (player == player2) {
        auto it = player2_seq_ids.find(seq_id);
        auto ret = it == player2_seq_ids.end();
        
        return ret;
    }
    
    return false;
}

void BattleInfo::setSeqId(int seq_id, BattlePlayer::ptr player)
{
    if (player == player1) {
        player1_seq_ids.insert(seq_id);
    } else if (player == player2) {
        player2_seq_ids.insert(seq_id);
    }
}

// private member function
BattleInfo::BattleInfo() :
    battle_seq_id(1)
{
    //
}

bool BattleInfo::init(const std::string& _battle_key,
    BattlePlayer::ptr p1, BattlePlayer::ptr p2)
{
    if (!p1 || !p2) {
        return false;
    }
    
    battle_key = _battle_key;
    player1 = p1;
    player2 = p2;
    
    updateTimer(AppTime::now());
    
    return true;
}
