#include "demo_battle_cli_glue.h"
#include "demo_battle_cli.h"

#include "log/logger.h"

#include "input_command_generated.h"
#include "battle_entry_generated.h"
#include "battle_exit_generated.h"

void glue_input_command__(long actor_key, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!DemoBattle::VerifyInputCommandBuffer(verifier)) {
        return;
    }

    auto input_command_data = DemoBattle::GetInputCommand(data);
    DemoBattle::input_command(input_command_data->player_id(), const_cast<flatbuffers::Vector<const DemoBattle::CommandDetail *>*>(input_command_data->list()), actor_key);
}
void glue_battle_entry__(long actor_key, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!DemoBattle::VerifyBattleEntryBuffer(verifier)) {
        return;
    }

    auto battle_entry_data = DemoBattle::GetBattleEntry(data);
    DemoBattle::battle_entry(battle_entry_data->battle_key()->c_str(), battle_entry_data->player_id(), battle_entry_data->access_token()->c_str(), actor_key);
}
void glue_battle_exit__(long actor_key, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!DemoBattle::VerifyBattleExitBuffer(verifier)) {
        return;
    }

    auto battle_exit_data = DemoBattle::GetBattleExit(data);
    DemoBattle::battle_exit(battle_exit_data->battle_key()->c_str(), battle_exit_data->player_id(), actor_key);
}
