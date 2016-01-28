#include "user_record_redis.h"

#include "common_object.h"
#include "log/logger.h"

USING_NS

UserRecordRedis* UserRecordRedis::getInstance()
{
    static UserRecordRedis inst;

    return &inst;
}

void UserRecordRedis::add(const std::string& name,
    std::function<void(const RedisValue&)> result_callback)
{
    auto redis_service =
        CommonObject::getInstance()->getRedisService();

    const char* command =
        "caf0d409a95aea54d9b7732d73b42c15f03991a0";

    std::list<std::string> args = {
        command, "0", name
    };

    redis_service->command("evalsha", args, 
        [result_callback](const RedisValue& rv) {
//            Logger::log("%s", rv.toString().c_str());

            if (rv.toString() != "NG") {
                result_callback(rv);
            }
        }, [](const std::string& err_msg) {
            Logger::log("%s", err_msg.c_str());
        });
}

// private member function
UserRecordRedis::UserRecordRedis()
{
    //
}
