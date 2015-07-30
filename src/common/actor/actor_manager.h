#ifndef ACTOR_MANAGER_H
#define ACTOR_MANAGER_H

#include "smart_ptr.hpp"
#include <map>
#include <functional>

#include "ws_actor.h"
#include "atomic/spin_lock.hpp"

#include "thread/bidirectional_communicator.h"

class ActorManager : public SmartPtr<ActorManager>
{
public:
    typedef boost::intrusive_ptr<ActorManager> ptr;

    typedef std::map<long, WsActor::ptr> ActorList;
    typedef std::map<long, WsActor::const_ptr> ConstActorList;
    typedef std::function<void(WsActor::const_ptr)> GetCallback;
    typedef std::function<void()> GetErrorCallback;
    typedef std::function<void(WsActor::const_ptr, const ActorList&)>
        MapCallback;
    typedef std::function<void(size_t)> SizeCallback;

    ActorManager(BidirectionalCommunicator::ptr t_comm);

    void getActorFromKey(long key, GetCallback callback);
    void getActorFromKey(long key, GetCallback callback,
        GetErrorCallback error_callback);
    void addActor(long key, WsActor::ptr actor);
    void removeActor(long key);
    void removeActor(WsActor::const_ptr actor);

    void map(MapCallback callback);

    void getSize(SizeCallback callback);

private:
    ActorList actor_list;
    Spinlock<100> spin_lock;

    BidirectionalCommunicator::ptr task_comm;
};

#endif
