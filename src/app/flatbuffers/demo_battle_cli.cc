#include "demo_battle_cli.h"

#include "battle/battle_manager.h"
#include "log/logger.h"

namespace DemoBattle {

void input_command(const std::string& battle_key, int player_id, 
    const flatbuffers::Vector<const CommandDetail *>* list,
    long actor_key)
{
    Logger::log("input battle key=%s, player id=%d, command size=%d", 
        battle_key.c_str(), player_id, list->size());
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
