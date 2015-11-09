#include "demo_battle_cli_glue.h"
#include "demo_battle_cli.h"

#include "log/logger.h"

#include "input_command_generated.h"

void glue_input_command__(long actor_key, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!DemoBattle::VerifyInputCommandBuffer(verifier)) {
        return;
    }

    auto input_command_data = DemoBattle::GetInputCommand(data);
    DemoBattle::input_command(input_command_data->player_id(), const_cast<flatbuffers::Vector<const DemoBattle::CommandDetail *>*>(input_command_data->list()), actor_key);
}
