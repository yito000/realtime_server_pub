#include "demo_battle_cli.h"

#include "battle/battle_manager.h"
#include "log/logger.h"

namespace DemoBattle {

void input_command(int player_id, 
    const flatbuffers::Vector<const CommandDetail *>* list,
    long actor_key)
{
    Logger::log("input player id=%d, command size=%d", player_id, list->size());
}

void battle_entry(const std::string& battle_key, int player_id, 
    long actor_key)
{
    BattleManager::getInstance()->joinPlayer(battle_key, player_id, actor_key);
}

// TODO: leave battle

};
