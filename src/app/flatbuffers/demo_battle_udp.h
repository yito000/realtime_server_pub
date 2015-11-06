#ifndef demo_battle_udp_H
#define demo_battle_udp_H

#include "notify_attack_action_generated.h"
#include "notify_buff_action_generated.h"
#include "notify_debuff_action_generated.h"
#include "notify_heal_action_generated.h"
#include "notify_meditation_action_generated.h"
#include "notify_start_phase_generated.h"
#include "notify_end_phase_generated.h"

namespace DemoBattle {
void notify_attack_action_udp(int seq_id, const flatbuffers::Vector<const DemoBattle::AttackDetail *>* actions, long actor_key = -1);
};

namespace DemoBattle {
void notify_buff_action_udp(int seq_id, const flatbuffers::Vector<const DemoBattle::BuffDetail *>* actions, long actor_key = -1);
};

namespace DemoBattle {
void notify_debuff_action_udp(int seq_id, const flatbuffers::Vector<const DemoBattle::DebuffDetail *>* actions, long actor_key = -1);
};

namespace DemoBattle {
void notify_heal_action_udp(int seq_id, const flatbuffers::Vector<const DemoBattle::HealDetail *>* actions, long actor_key = -1);
};

namespace DemoBattle {
void notify_meditation_action_udp(int seq_id, const flatbuffers::Vector<const DemoBattle::MeditationDetail *>* actions, long actor_key = -1);
};

namespace DemoBattle {
void notify_start_phase_udp(int seq_id, long actor_key = -1);
};

namespace DemoBattle {
void notify_end_phase_udp(int seq_id, long actor_key = -1);
};

#endif
