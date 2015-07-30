#include "pb_ev_uiid.h"

#include "protobuf/message/uiid.pb.h"

#include "redis/user_record_redis.h"

#include "record/user_record_voltdb.h"
#include "record/voltdb_record.h"

#include "common_object.h"
#include "event/receive_uiid.h"

#include "log/logger.h"

void PbEvUiid::addUser(const WsActor* user_client,
    const char* data, int size)
{
    UiidPData uiid_data;
    uiid_data.ParseFromArray(data, size);

    //
    auto u_r = UserRecordRedis::getInstance();
    auto uiid = uiid_data.value();

    u_r->add(uiid,
        [user_client, uiid](const RedisValue& rv) {
            auto response = rv.toString();

            if (response != "NG") {
                CommonObject::getInstance()->postWorker(
                    std::bind(ReceiveUiid::exec, user_client, uiid));
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

                CommonObject::getInstance()->postWorker(
                    std::bind(ReceiveUiid::exec, user_client, uiid));
                });
            } else {
                Logger::debug("user add ng.");

                // todo error event
            }
        });
*/
}
