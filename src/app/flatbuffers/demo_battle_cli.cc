#include "demo_battle_cli.h"

#include "battle/battle_manager.h"
#include "log/logger.h"

namespace DemoBattle {

void input_command(const std::string& battle_key, int player_id, 
    const flatbuffers::Vector<const CommandDetail *>* list,
    long actor_key)
{
    BattleInputInfo::ptr input = new BattleInputInfo;
    input->battle_key = battle_key;
    input->player_id = player_id;
    input->actor_key = actor_key;
    
    for (auto p: *list) {
        input->list.push_back(BattleInputCommandDetail(p->characeter_id(), p->command_id()));
    }
    
    BattleManager::getInstance()->playerInput(input);
}

void battle_entry(const std::string& battle_key, int player_id, 
    const std::string& access_token, long actor_key)
{
    BattleManager::getInstance()->joinPlayer(battle_key, player_id, access_token, actor_key);
}

void battle_exit(const std::string& battle_key, int player_id, 
    long actor_key)
{
    BattleManager::getInstance()->leavePlayer(battle_key, player_id, actor_key);
}

};
