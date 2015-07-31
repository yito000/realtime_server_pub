#ifndef COMMON_OBJECT_H
#define COMMON_OBJECT_H

#include "smart_ptr.hpp"

#include "actor/actor_manager.h"
#include "cluster/cluster.h"
#include "error/error_handle_router.h"
#include "scheduler/app_scheduler.h"
#include "protobuf/protobuf_router.h"

#include "db/voltdb/voltdb_io_service.h"
#include "db/redis/redis_service.h"

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

    VoltdbIOService::ptr getVoltdbIOService();
    void setVoltdbIOService(VoltdbIOService::ptr v_ios);

    RedisService::ptr getRedisService();
    void setRedisService(RedisService::ptr r_service);

private:
    CommonObject();

    ActorManager::ptr user_actor_manager;
    ActorManager::ptr up_actor_manager;
    ActorManager::ptr down_actor_manager;

    Cluster::ptr cluster;
    AppScheduler::ptr delay_scheduler;

    ProtobufRouter::ptr user_pb_router;
    ProtobufRouter::ptr system_pb_router;
    ErrorHandleRouter::ptr err_handle_router;

    VoltdbIOService::ptr volt_io_service;
    RedisService::ptr redis_service;
};

#endif
