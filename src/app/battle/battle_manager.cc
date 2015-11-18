#include "battle_manager.h"

#include "common/app_director.h"
#include "common/actor/ws_actor.h"

#include <boost/bind.hpp>

#include "battle/packet/error_packet.h"
#include "battle/packet/join_packet.h"
#include "battle/packet/leave_packet.h"
#include "battle/packet/player_input_packet.h"

#include "log/logger.h"

namespace {
    const int QUEUE_SIZE = 20480;
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
        BattleProcessor::ptr bp = BattleProcessor::create(*this, QUEUE_SIZE);
        battle_processor_list.push_back(bp);
        
        app_dir->postWorker(boost::bind(&BattleProcessor::runLoop, bp));
    }
    
    init_flag = true;
}

void BattleManager::endAllBattle()
{
    for (auto b: battle_processor_list) {
        b->endLoop();
    }
}

static int CalcThreadIndex(const std::string& battle_key, int thread_size)
{
    int total = 0;
    for (int i = 0; i < battle_key.size(); i++) {
        total += (int)battle_key.at(i);
    }
    
    return total % thread_size;
}

void BattleManager::joinPlayer(const std::string& battle_key, int player_id, 
    const std::string& access_token, long actor_key)
{
    Logger::debug("battle entry: player id=%d, battle key=%s", player_id, battle_key.c_str());
    
    auto join_packet = new JoinPacket;
    join_packet->battle_key = battle_key;
    join_packet->player_id = player_id;
    join_packet->actor_key = actor_key;
    join_packet->access_token = access_token;
    
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

void BattleManager::addActorInfo(long actor_key, 
    const std::string& battle_key, int thread_index)
{
    ActorInfo::ptr actor_info = new ActorInfo;
    actor_info->actor_key = actor_key;
    actor_info->battle_key = battle_key;
    actor_info->thread_index = thread_index;
    
    auto app_dir = AppDirector::getInstance();
    app_dir->postMaster([this, actor_key, actor_info]() {
            actor_map[actor_key] = actor_info;
        });
}

void BattleManager::receiveError(const WsActor* actor)
{
    auto actor_key = actor->getKey();
    
    auto app_dir = AppDirector::getInstance();
    app_dir->postMaster([this, actor_key]() {
            auto it = actor_map.find(actor_key);
            if (it != actor_map.end()) {
                ActorInfo::ptr actor_info = it->second;
                
                ErrorPacket* err_packet = new ErrorPacket;
                err_packet->actor_key = actor_info->actor_key;
                err_packet->battle_key = actor_info->battle_key;
                err_packet->player_id = -1;
                err_packet->error_code = 0;
                
                battle_processor_list[actor_info->thread_index]->pushPacket(err_packet);
            }
        });
}

// private member function
BattleManager::BattleManager() :
    init_flag(false)
{
    //
}
