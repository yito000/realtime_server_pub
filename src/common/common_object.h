#ifndef COMMON_OBJECT_H
#define COMMON_OBJECT_H

#include "smart_ptr.hpp"

#include "actor/actor_manager.h"
#include "cluster/cluster.h"
#include "error/error_handle_router.h"
#include "scheduler/app_scheduler.h"
#include "router/router.h"

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

    Router::ptr getUserRouter();
    void setUserRouter(Router::ptr r);

    Router::ptr getSystemRouter();
    void setSystemRouter(Router::ptr r);

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

    Router::ptr user_router;
    Router::ptr system_router;
    ErrorHandleRouter::ptr err_handle_router;

    VoltdbIOService::ptr volt_io_service;
    RedisService::ptr redis_service;
};

#endif
