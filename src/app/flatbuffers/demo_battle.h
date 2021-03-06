#ifndef DEMO_BATTLE_H
#define DEMO_BATTLE_H

#include <string>

#include "notify_attack_action_generated.h"
#include "notify_buff_action_generated.h"
#include "notify_debuff_action_generated.h"
#include "notify_heal_action_generated.h"
#include "notify_meditation_action_generated.h"

#include "notify_start_phase_generated.h"
#include "notify_end_phase_generated.h"

namespace DemoBattle {

void notify_attack_action(int seq_id, int player_id,
    const flatbuffers::Vector<const DemoBattle::AttackDetail*>* actions, 
    long actor_key = -1);
void notify_buff_action(int seq_id, int player_id,
    const flatbuffers::Vector<const DemoBattle::BuffDetail*>* actions, 
    long actor_key = -1);
void notify_debuff_action(int seq_id, int player_id,
    const flatbuffers::Vector<const DemoBattle::DebuffDetail*>* actions, 
    long actor_key = -1);
void notify_heal_action(int seq_id, int player_id,
    const flatbuffers::Vector<const DemoBattle::HealDetail*>* actions, 
    long actor_key = -1);
void notify_meditation_action(int seq_id, int player_id,
    const flatbuffers::Vector<const DemoBattle::MeditationDetail*>* actions, 
    long actor_key = -1);

void notify_start_phase(int seq_id, long actor_key = -1);
void notify_end_phase(int seq_id, long actor_key = -1);
void notify_leave_player(int player_id, long actor_key = -1);

};

#endif
