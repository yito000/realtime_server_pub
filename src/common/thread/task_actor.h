#ifndef TASK_ACTOR_H
#define TASK_ACTOR_H

#include "smart_ptr.hpp"

#include <future>
#include <boost/asio/io_service.hpp>

enum class ActThreadType {
    MAIN_THREAD,
    WORKER_THREAD
};

template<typename T>
class TaskActor : public SmartPtr< TaskActor<T> >
{
public:
    static boost::intrusive_ptr< TaskActor<T> > create();
    
    void send(ActThreadType t_type, std::function<void()> func);
    std::future<T> ask(ActThreadType t_type, std::function<T()> func);
    std::future<T> askIOS(boost::asio::io_service& ios, std::function<T()> func);
    void reset();
    
private:
    void postVoidTask(ActThreadType t_type, std::function<void()> func);
    void postPackagedTask(ActThreadType t_type, std::function<T()> func);
    
    std::packaged_task< T() > p_task;
};

#include "task_actor.tmpl"

#endif
