#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include "smart_ptr.hpp"

#include <vector>

#include "battle/battle_processor.h"

class BattleManager : public SmartPtr<BattleManager>
{
public:
    typedef boost::intrusive_ptr<BattleManager> ptr;
    
    static BattleManager::ptr getInstance();
    void initialize(int worker_thread);
    
    void joinPlayer(const std::string& battle_key, int player_id, 
        const std::string& access_token, long actor_key);
    void leavePlayer(const std::string& battle_key, int player_id, long actor_key);
    void endBattle(const std::string& battle_key);
    
private:
    BattleManager();
    
    bool init_flag;
    std::vector<BattleProcessor::ptr> battle_processor_list;
};

#endif
