#include "common_object.h"

CommonObject* CommonObject::getInstance()
{
    static CommonObject inst;

    return &inst;
}

ActorManager::ptr CommonObject::getUserActorManager()
{
    return user_actor_manager;
}

void CommonObject::setUserActorManager(ActorManager::ptr a_manager)
{
    user_actor_manager = a_manager;
}

ActorManager::ptr CommonObject::getUpActorManager()
{
    return up_actor_manager;
}

void CommonObject::setUpActorManager(ActorManager::ptr a_manager)
{
    up_actor_manager = a_manager;
}

ActorManager::ptr CommonObject::getDownActorManager()
{
    return down_actor_manager;
}

void CommonObject::setDownActorManager(ActorManager::ptr a_manager)
{
    down_actor_manager = a_manager;
}

Cluster::ptr CommonObject::getCluster()
{
    return cluster;
}

void CommonObject::setCluster(Cluster::ptr a_cluster)
{
    cluster = a_cluster;
}

AppScheduler::ptr CommonObject::getDelayScheduler()
{
    return delay_scheduler;
}

void CommonObject::setDelayScheduler(AppScheduler::ptr sche)
{
    delay_scheduler = sche;
}

ProtobufRouter::ptr CommonObject::getUserProtobufRouter()
{
    return user_pb_router;
}

void CommonObject::setUserProtobufRouter(ProtobufRouter::ptr pb_r)
{
    user_pb_router = pb_r;
}

ProtobufRouter::ptr CommonObject::getSystemProtobufRouter()
{
    return system_pb_router;
}

void CommonObject::setSystemProtobufRouter(ProtobufRouter::ptr pb_r)
{
    system_pb_router = pb_r;
}

ErrorHandleRouter::ptr CommonObject::getErrorHandleRouter()
{
    return err_handle_router;
}

void CommonObject::setErrorHandleRouter(ErrorHandleRouter::ptr eh_router)
{
    err_handle_router = eh_router;
}

VoltdbIOService::ptr CommonObject::getVoltdbIOService()
{
    return volt_io_service;
}

void CommonObject::setVoltdbIOService(VoltdbIOService::ptr v_ios)
{
    volt_io_service = v_ios;
}

RedisService::ptr CommonObject::getRedisService()
{
    return redis_service;
}

void CommonObject::setRedisService(RedisService::ptr r_service)
{
    redis_service = r_service;
}

// private member function
CommonObject::CommonObject()
{
    //
}
