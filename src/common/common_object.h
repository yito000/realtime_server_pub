#ifndef COMMON_OBJECT_H
#define COMMON_OBJECT_H

#include "smart_ptr.hpp"

#include "actor/actor_manager.h"
#include "cluster/cluster.h"
#include "error/error_handle_router.h"
#include "scheduler/app_scheduler.h"
#include "router/router.h"
#include "router/udp_router.h"
#include "db/redis/redis_service.h"

#ifdef USE_VOLTDB
#include "db/voltdb/voltdb_io_service.h"
#endif

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
    
    UdpRouter::ptr getUserUdpRouter();
    void setUserUdpRouter(UdpRouter::ptr r);
    
    UdpRouter::ptr getSystemUdpRouter();
    void setSystemUdpRouter(UdpRouter::ptr r);

    ErrorHandleRouter::ptr getUserErrorHandleRouter();
    void setUserErrorHandleRouter(ErrorHandleRouter::ptr eh_router);
    
    ErrorHandleRouter::ptr getClusterErrorHandleRouter();
    void setClusterErrorHandleRouter(ErrorHandleRouter::ptr eh_router);
    
    ErrorHandleRouter::ptr getServerErrorHandleRouter();
    void setServerErrorHandleRouter(ErrorHandleRouter::ptr eh_router);

#ifdef USE_VOLTDB
    VoltdbIOService::ptr getVoltdbIOService();
    void setVoltdbIOService(VoltdbIOService::ptr v_ios);
#endif

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
    
    UdpRouter::ptr user_router_udp;
    UdpRouter::ptr system_router_udp;
    
    ErrorHandleRouter::ptr user_err_handle_router;
    ErrorHandleRouter::ptr cluster_err_handle_router;
    ErrorHandleRouter::ptr server_err_handle_router;

#ifdef USE_VOLTDB
    VoltdbIOService::ptr volt_io_service;
#endif

    RedisService::ptr redis_service;
};

#endif
