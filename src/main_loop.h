#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include "smart_ptr.hpp"
#include <map>

#include "thread/bidirectional_communicator.h"
#include "actor/actor_manager.h"

class MainLoop : public SmartPtr<MainLoop>
{
public:
    typedef boost::intrusive_ptr<MainLoop> ptr;

    MainLoop();

    BidirectionalCommunicator::ptr getBidirectionalCommunicator();
    void setBidirectionalCommunicator(BidirectionalCommunicator::ptr t_comm);

    void addActorManager(ActorManager::ptr a_manager);
    void removeActorManager(ActorManager::ptr a_manager);
    void update();

private:
    void updateActor(ActorManager::ptr actor_manager);

    BidirectionalCommunicator::ptr task_comm;

    std::map<void*, ActorManager::ptr> actor_manager_list;
    bool first_update_flag;
};

#endif
