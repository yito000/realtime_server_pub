#ifndef BATTLE_INFO_H
#define BATTLE_INFO_H

#include "fw_env.h"
#include <string>
#include <queue>
#include <set>

#include "battle/player/battle_player.h"
#include "lib/time/app_time.h"

class BattleInfo : public SmartPtr<BattleInfo>
{
public:
    typedef boost::intrusive_ptr<BattleInfo> ptr;
    
    static BattleInfo::ptr create(const std::string& _battle_key,
        BattlePlayer::ptr p1, BattlePlayer::ptr p2);
    
    const std::string& getBattleKey() const;
    BattlePlayer::ptr getPlayer1() const;
    BattlePlayer::ptr getPlayer2() const;
    
    BattlePlayer::ptr getPlayerFromId(int player_id);
    
    void updateTimer(const AppTime::Point& tp);
    int diffMillis(const AppTime::Point& other) const;
    
    int getBattleSeqId() const;
    void incBattleSeqId();
    
    bool validSeqId(int seq_id, BattlePlayer::ptr player);
    void setSeqId(int seq_id, BattlePlayer::ptr player);
    
private:
    BattleInfo();
    bool init(const std::string& _battle_key,
        BattlePlayer::ptr p1, BattlePlayer::ptr p2);
    
    std::string battle_key;
    BattlePlayer::ptr player1;
    BattlePlayer::ptr player2;
    
    std::set<int> player1_seq_ids;
    std::set<int> player2_seq_ids;
    
    AppTime::Point last_update;
    int battle_seq_id;
};

#endif
