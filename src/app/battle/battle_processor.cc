#include "battle_processor.h"

#include <boost/thread.hpp>

#include "common_object.h"

#include "battle/packet/error_packet.h"
#include "battle/packet/join_packet.h"
#include "battle/packet/leave_packet.h"
#include "battle/packet/player_input_packet.h"

#include "lib/base64/base64.h"
#include "battle_info_generated.h"

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
        while (1) {
            BattlePacket* packet;
            
            if (packet_queue.pop(packet)) {
                dispatchPacket(packet);
                
                delete packet;
            } else {
                break;
            }
        }
        
        // TODO
        
        boost::this_thread::sleep_for(boost::chrono::milliseconds(0));
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
    
    auto redis_service =
        CommonObject::getInstance()->getRedisService();
    
    redis_service->get(packet->battle_key, [packet](const RedisValue& rv) {
            if (rv.isString()) {
                auto ret = rv.toString();
                Logger::debug("result value is String: %s", ret.c_str());
                
                Base64::ByteBuffer out_buf;
                Base64::decode(ret, out_buf);
                
                Logger::debug("decode buffer size: %d", out_buf.size());
                
                //
                auto data = &out_buf[0];
                auto d_size = out_buf.size();
                
                flatbuffers::Verifier verifier((const uint8_t*)data, d_size);
                if (DemoBattle::VerifyBattleInfoBuffer(verifier)) {
                    auto b_info = DemoBattle::GetBattleInfo((const uint8_t*)data);
                    
                    // TODO: join battle
                    Logger::debug("host: %s", b_info->host()->c_str());
                }
            } else {
                Logger::log("Warning. result value is not String");
            }
        }, [](const std::string& err_msg) {
            Logger::log("redis error: %s", err_msg.c_str());
            
            // TODO: error result
        });
}

void BattleProcessor::execLeave(LeavePacket* packet)
{
    if (!packet) {
        return;
    }
    
    //
}

void BattleProcessor::execPlayerInput(PlayerInputPacket* packet)
{
    if (!packet) {
        return;
    }
    
    //
}
