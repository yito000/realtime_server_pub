#include "fb_event_uiid.h"

#include "flatbuffers/table/uiid_generated.h"

#include "redis/user_record_redis.h"

//#include "record/user_record_voltdb.h"
//#include "record/voltdb_record.h"

#include "app_director.h"

#include "event/receive_uiid.h"
#include "flatbuffers/table/uiid_generated.h"

#include "log/logger.h"

void FbEvUiid::addUser(long actor_key,
    const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!VerifyUiidDataBuffer(verifier)) {
        Logger::log("invalid packet data (Uiid)");
        return;
    }
    
    auto uiid_data = GetUiidData(data);
    std::string uiid = uiid_data->value()->str();
    
    auto u_r = UserRecordRedis::getInstance();
    u_r->add(uiid,
        [actor_key, uiid](const RedisValue& rv) {
            auto response = rv.toString();

            if (response != "NG") {
                AppDirector::getInstance()->postWorker(
                    std::bind(ReceiveUiid::exec, actor_key, uiid));
            } else {
                Logger::debug("user add ng.");

                // todo error event
            }
        });

/*
    auto u_r = UserRecordVoltdb::getInstance();
    u_r->add(uiid_data.value(), 
        [user_client, uiid_data](VoltdbResult& v_ret) {
            auto response = v_ret.res;

            if (response->success()) {
                auto ret = response->results();
                auto it = ret[0].iterator();

                auto row = it.next();
                std::string uiid = row.getString(0);

                AppDirector::getInstance()->postWorker(
                    std::bind(ReceiveUiid::exec, user_client, uiid));
                });
            } else {
                Logger::debug("user add ng.");

                // todo error event
            }
        });
*/
}
