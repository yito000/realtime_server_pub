#include "battle_processor.h"

#include <boost/thread.hpp>

#include "common_object.h"

#include "battle/packet/error_packet.h"
#include "battle/packet/join_packet.h"
#include "battle/packet/join2_packet.h"
#include "battle/packet/leave_packet.h"
#include "battle/packet/player_input_packet.h"

#include "lib/base64/base64.h"
#include "battle_info_generated.h"

#include "flatbuffers/demo_battle.h"

namespace {
    const int LOOP_COUNT = 1024;
};

BattleProcessor::ptr BattleProcessor::create(int queue_size)
{
    BattleProcessor::ptr inst = new BattleProcessor(queue_size);
    if (!inst->init()) {
        return nullptr;
    }
    
    return inst;
}

void BattleProcessor::runLoop()
{
    while (1) {
        BattlePacket* packet;
        
        for (int i = 0; i < LOOP_COUNT; i++) {
            if (packet_queue.pop(packet)) {
                dispatchPacket(packet);
                
                delete packet;
            } else {
                break;
            }
        }
        
        // TODO
        
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    }
}

void BattleProcessor::pushPacket(BattlePacket* packet)
{
    packet_queue.push(packet);
}

// private member function
BattleProcessor::BattleProcessor(int queue_size) :
    packet_queue(queue_size)
{
    //
}

bool BattleProcessor::init()
{
    return true;
}

void BattleProcessor::dispatchPacket(BattlePacket* packet)
{
    if (!packet) {
        return;
    }
    
    switch ((BattlePacketType)packet->packet_type) {
        case BattlePacketType::ERROR: {
            execError(dynamic_cast<ErrorPacket*>(packet));
            
            break;
        }
        
        case BattlePacketType::JOIN: {
            execJoin(dynamic_cast<JoinPacket*>(packet));
            
            break;
        }
        
        case BattlePacketType::JOIN2: {
            execJoin2(dynamic_cast<Join2Packet*>(packet));
            
            break;
        }
        
        case BattlePacketType::LEAVE: {
            execLeave(dynamic_cast<LeavePacket*>(packet));
            
            break;
        }
        
        case BattlePacketType::PLAYER_INPUT: {
            execPlayerInput(dynamic_cast<PlayerInputPacket*>(packet));
            
            break;
        }
        
        default:
            break;
    }
}

void BattleProcessor::execError(ErrorPacket* packet)
{
    if (!packet) {
        return;
    }
    
    //
}

void BattleProcessor::execJoin(JoinPacket* packet)
{
    if (!packet) {
        return;
    }
    
    JoinPacket* dup_packet = new JoinPacket;
    dup_packet->battle_key = packet->battle_key;
    dup_packet->player_id = packet->player_id;
    dup_packet->actor_key = packet->actor_key;
    dup_packet->access_token = packet->access_token;
    
    //
    auto redis_service =
        CommonObject::getInstance()->getRedisService();
    
    redis_service->get(dup_packet->battle_key, [this, dup_packet](const RedisValue& rv) {
            if (rv.isString()) {
                Join2Packet* next_packet = new Join2Packet;
                
                next_packet->battle_key = dup_packet->battle_key;
                next_packet->player_id = dup_packet->player_id;
                next_packet->actor_key = dup_packet->actor_key;
                next_packet->access_token = dup_packet->access_token;
                next_packet->base64_redis_value = rv.toString();
                
                this->pushPacket(next_packet);
            } else {
                Logger::log("Warning. result value is not String");
            }
            
            delete dup_packet;
        }, [dup_packet](const std::string& err_msg) {
            delete dup_packet;
            
            Logger::log("redis error: %s", err_msg.c_str());
            
            // TODO: error result
        });
}

void BattleProcessor::execJoin2(Join2Packet* packet)
{
    if (!packet) {
        return;
    }
    
    Base64::ByteBuffer out_buf;
    Base64::decode(packet->base64_redis_value, out_buf);
    
    Logger::debug("result value is String: %s", packet->base64_redis_value.c_str());
    Logger::debug("decoded buffer size: %d", out_buf.size());
    
    //
    auto data = &out_buf[0];
    auto d_size = out_buf.size();
    
    flatbuffers::Verifier verifier((const uint8_t*)data, d_size);
    if (DemoBattle::VerifyBattleInfoBuffer(verifier)) {
        auto b_info = DemoBattle::GetBattleInfo((const uint8_t*)data);
        
        auto b_it = battle_list.find(packet->battle_key);
        if (b_it != battle_list.end()) {
            BattleInfo::ptr battle_info = b_it->second;
            
            if (packet->player_id == b_info->player1_id()) {
                battle_info->player1->resetActoryKey(packet->actor_key);
                battle_info->player1->setJoinFlag(true);
            } else if (packet->player_id == b_info->player2_id()) {
                battle_info->player2->resetActoryKey(packet->actor_key);
                battle_info->player2->setJoinFlag(true);
            }
            
            startBattle(battle_info);
            
            return;
        }
        
        //
        BattleInfo::ptr new_battle = new BattleInfo;
        
        std::list<BattleCharacter::ptr> player1_characters;
        std::list<BattleCharacter::ptr> player2_characters;
        
        for (const DemoBattle::BattlePlayerCharacter* c: *(b_info->player1_characters())) {
            auto bc = BattleCharacter::create(c->character_id(), c->name()->c_str());
            
            bc->setBaseHp(c->base_hp());
            bc->setBaseMana(c->base_mana());
            bc->setBasePhysicalAttack(c->physical_attack());
            bc->setBaseMagicalAttack(c->magical_attack());
            bc->setBasePhysicalDefense(c->physical_defense());
            bc->setBaseMagicalDefense(c->magical_defense());
            bc->setSpeed(c->speed());
            
            bc->setHp(bc->getBaseHp());
            bc->setMana(bc->getBaseMana());
            
            player1_characters.push_back(bc);
        }
        
        for (const DemoBattle::BattlePlayerCharacter* c: *(b_info->player2_characters())) {
            auto bc = BattleCharacter::create(c->character_id(), c->name()->c_str());
            
            bc->setBaseHp(c->base_hp());
            bc->setBaseMana(c->base_mana());
            bc->setBasePhysicalAttack(c->physical_attack());
            bc->setBaseMagicalAttack(c->magical_attack());
            bc->setBasePhysicalDefense(c->physical_defense());
            bc->setBaseMagicalDefense(c->magical_defense());
            bc->setSpeed(c->speed());
            
            bc->setHp(bc->getBaseHp());
            bc->setMana(bc->getBaseMana());
            
            player2_characters.push_back(bc);
        }
        
        auto player1 = BattlePlayer::create(b_info->player1_id(), 
            0, b_info->player1_name()->c_str(), 
            player1_characters);
        auto player2 = BattlePlayer::create(b_info->player2_id(), 
            0, b_info->player2_name()->c_str(), player2_characters);
        
        if (packet->player_id == b_info->player1_id()) {
            player1->resetActoryKey(packet->actor_key);
            player1->setJoinFlag(true);
        } else if (packet->player_id == b_info->player2_id()) {
            player2->resetActoryKey(packet->actor_key);
            player2->setJoinFlag(true);
        }
        
        new_battle->player1 = player1;
        new_battle->player2 = player2;
        
        battle_list[packet->battle_key] = new_battle;
    }
}

void BattleProcessor::execLeave(LeavePacket* packet)
{
    if (!packet) {
        return;
    }
    
    auto it = battle_list.find(packet->battle_key);
    if (it != battle_list.end()) {
        BattleInfo::ptr battle_info = it->second;
        
        // TODO: validation packet
        
        DemoBattle::notify_end_phase(-1, battle_info->player1->getActorKey());
        DemoBattle::notify_end_phase(-1, battle_info->player2->getActorKey());
        
        // TODO: remove actor
        
        battle_list.erase(it);
    }
}

void BattleProcessor::execPlayerInput(PlayerInputPacket* packet)
{
    if (!packet) {
        return;
    }
    
    //
}

void BattleProcessor::startBattle(BattleInfo::ptr battle_info)
{
    Logger::log("start battle");
    Logger::log("player1 actor key: %ld", battle_info->player1->getActorKey());
    Logger::log("player2 actor key: %ld", battle_info->player2->getActorKey());
    
    DemoBattle::notify_start_phase(0, battle_info->player1->getActorKey());
    DemoBattle::notify_start_phase(0, battle_info->player2->getActorKey());
    
    // TODO
    
    DemoBattle::notify_end_phase(-1, battle_info->player1->getActorKey());
    DemoBattle::notify_end_phase(-1, battle_info->player2->getActorKey());
    
    // TODO: remove actor
}
