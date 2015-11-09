#include "demo_battle_cli.h"

#include "log/logger.h"

namespace DemoBattle {

void input_command(int player_id, 
    const flatbuffers::Vector<const CommandDetail *>* list,
    long actor_key)
{
    Logger::log("input player id=%d, command size=%d", player_id, list->size());
}

};
