#include "main_loop.h"
#include "common/app_director.h"

#include "log/logger.h"

MainLoop::MainLoop() :
    first_update_flag(false)
{
    //
}

BidirectionalCommunicator::ptr MainLoop::getBidirectionalCommunicator()
{
    return task_comm;
}

void MainLoop::setBidirectionalCommunicator(BidirectionalCommunicator::ptr t_comm)
{
    task_comm = t_comm;
}

void MainLoop::addActorManager(ActorManager::ptr a_manager)
{
    actor_manager_list[a_manager.get()] = a_manager;
}

void MainLoop::removeActorManager(ActorManager::ptr a_manager)
{
    auto it = actor_manager_list.find(a_manager.get());
    if (it != actor_manager_list.end()) {
        actor_manager_list.erase(it);
    }
}

void MainLoop::update()
{
    if (!first_update_flag) {
        auto app_dir = AppDirector::getInstance();
        app_dir->setMainThreadId(boost::this_thread::get_id());
        
        first_update_flag = true;
    }
    
    task_comm->update();

    auto it = actor_manager_list.begin();
    for (; it != actor_manager_list.end(); ++it) {
        updateActor(it->second);
    }
}

// private member function
void MainLoop::updateActor(ActorManager::ptr actor_manager)
{
    std::list<WsActor::const_ptr> actor_list;
    
    actor_manager->map([actor_manager, &actor_list](WsActor::const_ptr actor,
        const ActorManager::ActorList& list) {

        if (!actor->isOpen()) {
            Logger::debug("remove actor");
            
            actor_list.push_back(actor);
        }
    });
    
    for (auto& a: actor_list) {
        actor_manager->removeActor(a);
    }
    
    actor_manager->update();
}
