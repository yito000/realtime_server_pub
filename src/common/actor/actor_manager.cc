#include "actor_manager.h"
#include "common/app_director.h"

#include "log/logger.h"

ActorManager::ActorManager(BidirectionalCommunicator::ptr t_comm) : 
    task_comm(t_comm)
{
    //
}

void ActorManager::getActorFromKey(long key, GetCallback callback)
{
    auto func = [this, key, callback](){
        WsActor::ptr ret;
        auto it = actor_list.find(key);
        if (it != actor_list.end()) {
            ret = it->second;
        }

        if (ret) {
            callback(ret);
        } else {
            Logger::log("actor not found: %ld", key);
        }
    };
    
    if (AppDirector::getInstance()->isMainThread()) {
        func();
    } else {
        task_comm->postMaster(func);
    }
}

void ActorManager::getActorFromKey(long key, GetCallback callback,
    GetErrorCallback error_callback)
{
    auto func = [this, key, callback, error_callback](){
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
    };
    
    if (AppDirector::getInstance()->isMainThread()) {
        func();
    } else {
        task_comm->postMaster(func);
    }
}

void ActorManager::addActor(long key, WsActor::ptr actor)
{
    auto func = [this, key, actor]() {
        if (actor) {
            void* old_p = nullptr;
            auto a_it = actor_list.find(key);
            if (a_it != actor_list.end()) {
                old_p = a_it->second.get();
            }
            
            actor_list[key] = actor;
            
            auto ai_it = hazard_actor_list.find(key);
            if (ai_it != hazard_actor_list.end()) {
                ActorGabageMap& ag_map = ai_it->second;
                
                auto ag_it = ag_map.find(old_p);
                if (ag_it != ag_map.end()) {
                    auto old_ai = ag_it->second;
                    
                    old_ai->actor->close();
                    old_ai->deleted = true;
                    old_ai->tp = AppTime::now();
                }
                
                ActorInfo::ptr ai = new ActorInfo;
                ai->actor = actor;
                ai->deleted = false;
                
                ag_map[actor.get()] = ai;
            } else {
                ActorInfo::ptr ai = new ActorInfo;
                ai->actor = actor;
                ai->deleted = false;
                
                hazard_actor_list[key] = ActorGabageMap();
                hazard_actor_list[key][actor.get()] = ai;
            }
        }
    };
    
    if (AppDirector::getInstance()->isMainThread()) {
        func();
    } else {
        task_comm->postMaster(func);
    }
}

void ActorManager::removeActor(long key)
{
    auto func = [this, key]() {
        Logger::log("remove actor key=%ld", key);
        void* actor_pointer = nullptr;

        auto it = actor_list.find(key);
        if (it != actor_list.end()) {
            actor_pointer = it->second.get();
            actor_list.erase(it);
        }
        
        if (actor_pointer != nullptr) {
            auto h_it = hazard_actor_list.find(key);
            if (h_it != hazard_actor_list.end()) {
                auto ag_it = h_it->second.find(actor_pointer);
                if (ag_it != h_it->second.end()) {
                    Logger::debug("mark hazard pointer");
                    
                    ag_it->second->deleted = true;
                    ag_it->second->tp = AppTime::now();
                }
            }
        }
    };
    
    if (AppDirector::getInstance()->isMainThread()) {
        func();
    } else {
        task_comm->postMaster(func);
    }
}

void ActorManager::removeActor(WsActor::const_ptr actor)
{
    long key = actor->getKey();
    removeActor(key);
}

void ActorManager::map(MapCallback callback)
{
    auto func = [this, callback]() {
        auto it = actor_list.begin();
        for (; it != actor_list.end(); ++it) {
            callback(it->second, actor_list);
        }
    };
    
    if (AppDirector::getInstance()->isMainThread()) {
        func();
    } else {
        task_comm->postMaster(func);
    }
}

void ActorManager::getSize(SizeCallback callback)
{
    auto func = [this, callback]() {
        size_t s = actor_list.size();

        callback(s);
    };
    
    if (AppDirector::getInstance()->isMainThread()) {
        func();
    } else {
        task_comm->postMaster(func);
    }
}

void ActorManager::update()
{
    auto func = [this]() {
        auto now = AppTime::now();
        auto h_it = hazard_actor_list.begin();
        
        while (h_it != hazard_actor_list.end()) {
            auto& ag_map = h_it->second;
            auto ag_it = ag_map.begin();
            
            while (ag_it != ag_map.end()) {
                auto& ai = ag_it->second;
                
                if (ai->deleted && !ai->actor->isOpen() && !ai->actor->isActive()) {
                    auto du = now - ai->tp;
                    int sec = du.count() / 1000 / 1000;
                    
                    // TODO: second parameter
                    if (sec > 5) {
                        Logger::debug("remove hazard actor");
                        ag_map.erase(ag_it++);
                    } else {
                        ++ag_it;
                    }
                } else {
                    ++ag_it;
                }
            }
            
            if (ag_map.size() > 0) {
                ++h_it;
            } else {
                hazard_actor_list.erase(h_it++);
            }
        }
    };
    
    if (AppDirector::getInstance()->isMainThread()) {
        func();
    } else {
        task_comm->postMaster(func);
    }
}
