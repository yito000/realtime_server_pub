#ifndef DEMO_BATTLE_CLI_H
#define DEMO_BATTLE_CLI_H

#include <string>

#include "input_command_generated.h"

namespace DemoBattle {

void input_command(int player_id, 
    const flatbuffers::Vector<const CommandDetail *>* list,
    long actor_key = -1);

void battle_entry(const std::string& battle_key, int player_id, 
    const std::string& access_token, long actor_key = -1);

void battle_exit(const std::string& battle_key, int player_id, 
    long actor_key = -1);

};

#endif
