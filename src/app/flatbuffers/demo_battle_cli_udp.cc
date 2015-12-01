#include "demo_battle_cli_udp.h"

#include "battle/battle_manager.h"
#include "log/logger.h"

namespace DemoBattle {

void input_command_udp(const std::__1::string& battle_key, int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::CommandDetail *>* list, const boost::asio::ip::udp::endpoint& ep)
{
    BattleInputInfoUdp::ptr input = new BattleInputInfoUdp;
    input->battle_key = battle_key;
    input->seq_id = seq_id;
    input->player_id = player_id;
    input->ep = ep;
    
    for (auto p: *list) {
        input->list.push_back(BattleInputCommandDetail(p->characeter_id(), p->command_id()));
    }
    
    BattleManager::getInstance()->playerInputUdp(input);
}

void battle_entry_udp(const std::__1::string& battle_key, int player_id, const std::__1::string& access_token, const boost::asio::ip::udp::endpoint& ep)
{
    // not implemented
}

void battle_exit_udp(const std::__1::string& battle_key, int player_id, const boost::asio::ip::udp::endpoint& ep)
{
    // not implemented
}

};
