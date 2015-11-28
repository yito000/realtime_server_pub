#ifndef demo_battle_udp_H
#define demo_battle_udp_H

#include "notify_attack_action_generated.h"
#include "notify_buff_action_generated.h"
#include "notify_debuff_action_generated.h"
#include "notify_heal_action_generated.h"
#include "notify_meditation_action_generated.h"
#include "notify_start_phase_generated.h"
#include "notify_end_phase_generated.h"
#include "notify_leave_player_generated.h"

#include <boost/asio.hpp>

namespace DemoBattle {
void notify_attack_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::AttackDetail *>* actions, const boost::asio::ip::udp::endpoint& ep = boost::asio::ip::udp::endpoint());
};

namespace DemoBattle {
void notify_buff_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::BuffDetail *>* actions, const boost::asio::ip::udp::endpoint& ep = boost::asio::ip::udp::endpoint());
};

namespace DemoBattle {
void notify_debuff_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::DebuffDetail *>* actions, const boost::asio::ip::udp::endpoint& ep = boost::asio::ip::udp::endpoint());
};

namespace DemoBattle {
void notify_heal_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::HealDetail *>* actions, const boost::asio::ip::udp::endpoint& ep = boost::asio::ip::udp::endpoint());
};

namespace DemoBattle {
void notify_meditation_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::MeditationDetail *>* actions, const boost::asio::ip::udp::endpoint& ep = boost::asio::ip::udp::endpoint());
};

namespace DemoBattle {
void notify_start_phase_udp(int seq_id, const boost::asio::ip::udp::endpoint& ep);
};

namespace DemoBattle {
void notify_end_phase_udp(int seq_id, const boost::asio::ip::udp::endpoint& ep);
};

namespace DemoBattle {
void notify_leave_player_udp(int player_id, const boost::asio::ip::udp::endpoint& ep);
};

#endif
