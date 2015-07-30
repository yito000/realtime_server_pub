#include "main_loop.h"

#include "log/logger.h"

MainLoop::MainLoop()
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
    task_comm->update();

    auto it = actor_manager_list.begin();
    for (; it != actor_manager_list.end(); ++it) {
        updateActor(it->second);
    }
}

// private member function
void MainLoop::updateActor(ActorManager::ptr actor_manager)
{
    actor_manager->map([actor_manager](WsActor::const_ptr actor,
        const ActorManager::ActorList& list) {

        if (!actor->isOpen()) {
            Logger::debug("remove actor");
            actor_manager->removeActor(actor);
        }
    });
}
