#include "demo_battle_cli_glue_udp.h"
#include "demo_battle_cli_udp.h"

#include "log/logger.h"

#include "input_command_generated.h"
#include "battle_entry_generated.h"
#include "battle_exit_generated.h"

void glue_udp_input_command__(const boost::asio::ip::udp::endpoint& ep, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!DemoBattle::VerifyInputCommandBuffer(verifier)) {
        return;
    }
    
    Logger::log("input!! glue_udp_input_command__");

    auto input_command_data = DemoBattle::GetInputCommand(data);
    DemoBattle::input_command_udp(input_command_data->battle_key()->c_str(), input_command_data->seq_id(), input_command_data->player_id(), const_cast<flatbuffers::Vector<const DemoBattle::CommandDetail *>*>(input_command_data->list()), ep);
}
void glue_udp_battle_entry__(const boost::asio::ip::udp::endpoint& ep, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!DemoBattle::VerifyBattleEntryBuffer(verifier)) {
        return;
    }

    auto battle_entry_data = DemoBattle::GetBattleEntry(data);
    DemoBattle::battle_entry_udp(battle_entry_data->battle_key()->c_str(), battle_entry_data->player_id(), battle_entry_data->access_token()->c_str(), ep);
}
void glue_udp_battle_exit__(const boost::asio::ip::udp::endpoint& ep, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!DemoBattle::VerifyBattleExitBuffer(verifier)) {
        return;
    }

    auto battle_exit_data = DemoBattle::GetBattleExit(data);
    DemoBattle::battle_exit_udp(battle_exit_data->battle_key()->c_str(), battle_exit_data->player_id(), ep);
}
