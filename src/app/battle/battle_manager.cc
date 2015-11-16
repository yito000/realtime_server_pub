#include "battle_manager.h"

#include "app_director.h"

#include <boost/bind.hpp>

#include "battle/packet/error_packet.h"
#include "battle/packet/join_packet.h"
#include "battle/packet/leave_packet.h"
#include "battle/packet/player_input_packet.h"

#include "log/logger.h"

namespace {
    const int QUEUE_SIZE = 10240;
};

BattleManager::ptr BattleManager::getInstance()
{
    static BattleManager::ptr inst = new BattleManager;
    
    return inst;
}

void BattleManager::initialize(int worker_thread)
{
    if (init_flag) {
        return;
    }
    
    auto app_dir = AppDirector::getInstance();
    
    for (int i = 0; i < worker_thread; i++) {
        BattleProcessor::ptr bp = BattleProcessor::create(QUEUE_SIZE);
        battle_processor_list.push_back(bp);
        
        app_dir->postWorker(boost::bind(&BattleProcessor::runLoop, bp));
    }
    
    init_flag = true;
}

static int CalcThreadIndex(const std::string& battle_key, int thread_size)
{
    int total = 0;
    for (int i = 0; i < battle_key.size(); i++) {
        total += (int)battle_key.at(i);
    }
    
    return total % thread_size;
}

void BattleManager::joinPlayer(const std::string& battle_key, int player_id, long actor_key)
{
    Logger::debug("battle entry: player id=%d, battle key=%s", player_id, battle_key.c_str());
    
    auto join_packet = new JoinPacket;
    join_packet->battle_key = battle_key;
    join_packet->player_id = player_id;
    join_packet->actor_key = actor_key;
    
    auto thread_index = CalcThreadIndex(battle_key, battle_processor_list.size());
    battle_processor_list[thread_index]->pushPacket(join_packet);
}

void BattleManager::leavePlayer(const std::string& battle_key, int player_id, long actor_key)
{
    auto leave_packet = new LeavePacket;
    leave_packet->battle_key = battle_key;
    leave_packet->player_id = player_id;
    leave_packet->actor_key = actor_key;
    
    auto thread_index = CalcThreadIndex(battle_key, battle_processor_list.size());
    battle_processor_list[thread_index]->pushPacket(leave_packet);
}

void BattleManager::endBattle(const std::string& battle_key)
{
    // TODO
}

// private member function
BattleManager::BattleManager() :
    init_flag(false)
{
}
