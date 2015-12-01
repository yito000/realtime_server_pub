#ifndef demo_battle_cli_udp_H
#define demo_battle_cli_udp_H

#include <boost/asio.hpp>

#include "input_command_generated.h"
#include "battle_entry_generated.h"
#include "battle_exit_generated.h"

namespace DemoBattle {
void input_command_udp(const std::__1::string& battle_key, int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::CommandDetail *>* list, const boost::asio::ip::udp::endpoint& ep = boost::asio::ip::udp::endpoint());
};

namespace DemoBattle {
void battle_entry_udp(const std::__1::string& battle_key, int player_id, const std::__1::string& access_token, const boost::asio::ip::udp::endpoint& ep = boost::asio::ip::udp::endpoint());
};

namespace DemoBattle {
void battle_exit_udp(const std::__1::string& battle_key, int player_id, const boost::asio::ip::udp::endpoint& ep = boost::asio::ip::udp::endpoint());
};

#endif
