#include "demo_battle_cli.h"

#include "common_object.h"
#include "lib/base64/base64.h"

#include "battle_info_generated.h"

#include "log/logger.h"

namespace DemoBattle {

void input_command(int player_id, 
    const flatbuffers::Vector<const CommandDetail *>* list,
    long actor_key)
{
    Logger::log("input player id=%d, command size=%d", player_id, list->size());
}

void battle_entry(const std::string& battle_key, int player_id, 
    long actor_key)
{
    Logger::log("battle entry: player id=%d, battle key=%s", player_id, battle_key.c_str());
    
    auto redis_service =
        CommonObject::getInstance()->getRedisService();
    
    redis_service->get(battle_key, [](const RedisValue& rv) {
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
                    
                    // TODO: battle start
                    Logger::debug("host: %s", b_info->host()->c_str());
                }
            } else {
                Logger::log("Warning. result value is not String");
            }
        }, [](const std::string& err_msg) {
            Logger::log("redis error: %s", err_msg.c_str());
        });
}

};
