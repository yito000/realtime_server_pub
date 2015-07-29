#ifndef COMMON_OBJECT_H
#define COMMON_OBJECT_H

#include "smart_ptr.hpp"

#include <functional>

#include "actor/actor_manager.h"
#include "cluster/cluster.h"
#include "error/error_handle_router.h"
#include "scheduler/app_scheduler.h"
#include "protobuf/protobuf_router.h"
#include "thread/bidirectional_communicator.h"
#include "variable/key_value_cache_storage.h"
#include "random/random.h"

#include "db/voltdb/voltdb_io_service.h"
#include "db/redis/redis_service.h"

typedef std::function<void(const std::string&)> VarGetCallback;
typedef std::function<void(void*)> VarObjGetCallback;

class CommonObject
{
public:
    static CommonObject* getInstance();

    ActorManager::ptr getUserActorManager();
    void setUserActorManager(ActorManager::ptr a_manager);

    ActorManager::ptr getUpActorManager();
    void setUpActorManager(ActorManager::ptr a_manager);

    ActorManager::ptr getDownActorManager();
    void setDownActorManager(ActorManager::ptr a_manager);

    Cluster::ptr getCluster();
    void setCluster(Cluster::ptr cluster);

    AppScheduler::ptr getDelayScheduler();
    void setDelayScheduler(AppScheduler::ptr sche);

    ProtobufRouter::ptr getUserProtobufRouter();
    void setUserProtobufRouter(ProtobufRouter::ptr pb_r);

    ProtobufRouter::ptr getSystemProtobufRouter();
    void setSystemProtobufRouter(ProtobufRouter::ptr pb_r);

    ErrorHandleRouter::ptr getErrorHandleRouter();
    void setErrorHandleRouter(ErrorHandleRouter::ptr eh_router);

    KeyValueCacheStorage::ptr getKeyValueCacheStorage();
    void setKeyValueCacheStorage(KeyValueCacheStorage::ptr kv_cache);

    Random* getRandomGenerator();
    void setRandomGenerator(Random* r);

    void setBidirectionalCommunicator(BidirectionalCommunicator::ptr t_comm)
    {
        task_comm = t_comm;
    }

    template <typename F>
    void postMaster(F f)
    {
        task_comm->postMaster(std::move(f));
    }

    template <typename F>
    void postWorker(F f)
    {
        task_comm->postWorker(std::move(f));
    }

    void getVariable(const std::string& key, VarGetCallback callback);
    void setVariable(const std::string& key, const std::string& value);

    void getVarObject(const std::string& key, VarObjGetCallback callback);
    void setVarObject(const std::string& key, void* data);

    int getRandomInt(int min, int max);
    void getRandomAllAscii(int length, std::string& ret);
    void getRandomAscii(int length, std::string& ret);

    VoltdbIOService::ptr getVoltdbIOService();
    void setVoltdbIOService(VoltdbIOService::ptr v_ios);

    RedisService::ptr getRedisService();
    void setRedisService(RedisService::ptr r_service);

private:
    CommonObject();
    void initialize();

    ActorManager::ptr user_actor_manager;
    ActorManager::ptr up_actor_manager;
    ActorManager::ptr down_actor_manager;

    Cluster::ptr cluster;
    AppScheduler::ptr delay_scheduler;

    ProtobufRouter::ptr user_pb_router;
    ProtobufRouter::ptr system_pb_router;
    ErrorHandleRouter::ptr err_handle_router;
    BidirectionalCommunicator::ptr task_comm;
    KeyValueCacheStorage::ptr key_value_cache;

    VoltdbIOService::ptr volt_io_service;
    RedisService::ptr redis_service;

    Random* rand;
};

#endif

