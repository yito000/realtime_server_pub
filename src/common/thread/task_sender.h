#ifndef TASK_SENDER_H
#define TASK_SENDER_H

#include "fw_env.h"

#include <future>
#include <boost/asio/io_service.hpp>

BEGIN_NS

enum class ActThreadType {
    MAIN_THREAD,
    WORKER_THREAD
};

template<typename T>
class TaskSender : public SmartPtr< TaskSender<T> >
{
public:
    static boost::intrusive_ptr< TaskSender<T> > create();
    
    void send(ActThreadType t_type, std::function<void()> func);
    std::future<T> sendAsync(ActThreadType t_type, std::function<T()> func);
    std::future<T> sendAsyncIOS(boost::asio::io_service& ios, std::function<T()> func);
    void reset();
    
private:
    void postVoidTask(ActThreadType t_type, std::function<void()> func);
    void postPackagedTask(ActThreadType t_type, std::function<T()> func);
    
    std::packaged_task< T() > p_task;
};

END_NS

#include "task_sender.ipp"

#endif
