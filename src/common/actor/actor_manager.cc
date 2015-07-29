#include "actor_manager.h"

#include "log/logger.h"

ActorManager::ActorManager(BidirectionalCommunicator::ptr t_comm) : 
    task_comm(t_comm)
{
    //
}

void ActorManager::getActorFromKey(long key, GetCallback callback)
{
    task_comm->postMaster([this, key, callback](){
        spin_lock.lock();

        WsActor::ptr ret;
        auto it = actor_list.find(key);
        if (it != actor_list.end()) {
            ret = it->second;
        }

        if (ret) {
            callback(ret);
        }

        spin_lock.unlock();
    });
}

void ActorManager::getActorFromKey(long key, GetCallback callback,
    GetErrorCallback error_callback)
{
    task_comm->postMaster([this, key, callback, error_callback](){
        spin_lock.lock();

        WsActor::ptr ret;
        auto it = actor_list.find(key);
        if (it != actor_list.end()) {
            ret = it->second;
        }

        if (ret) {
            callback(ret);
        } else {
            error_callback();
        }

        spin_lock.unlock();
    });
}

void ActorManager::addActor(long key, WsActor::ptr actor)
{
    task_comm->postMaster([this, key, actor]() {
        if (actor) {
            spin_lock.lock();
            actor_list[key] = actor;
            spin_lock.unlock();
        }
    });
}

void ActorManager::removeActor(long key)
{
    task_comm->postMaster([this, key]() {
        spin_lock.lock();

        Logger::debug("actor key=%ld", key);

        auto it = actor_list.find(key);
        if (it != actor_list.end()) {
            actor_list.erase(it);
        }

        spin_lock.unlock();
    });
}

void ActorManager::removeActor(WsActor::const_ptr actor)
{
    long key = actor->getKey();
    removeActor(key);
}

void ActorManager::map(MapCallback callback)
{
    task_comm->postMaster([this, callback]() {
        spin_lock.lock();

        auto it = actor_list.begin();
        for (; it != actor_list.end(); ++it) {
            callback(it->second, actor_list);
        }

        spin_lock.unlock();
    });
}

void ActorManager::getSize(SizeCallback callback)
{
    task_comm->postMaster([this, callback]() {
        spin_lock.lock();
        size_t s = actor_list.size();

        callback(s);
        spin_lock.unlock();
    });
}

