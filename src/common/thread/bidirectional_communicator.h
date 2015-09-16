#ifndef BIDIRECTIONAL_COMMUNICATOR_H
#define BIDIRECTIONAL_COMMUNICATOR_H

#include "smart_ptr.hpp"

#include "thread/task_dispatcher.h"
#include "thread/multiple_task_dispatcher.h"

class BidirectionalCommunicator : public SmartPtr<BidirectionalCommunicator>
{
public:
    typedef boost::intrusive_ptr<BidirectionalCommunicator> ptr;

    static BidirectionalCommunicator::ptr create(int worker_threads);
    void update();
    void stop();

    template <typename F>
    void postMaster(F f)
    {
        master_thread->post(std::move(f));
    }

    template <typename F>
    void postWorker(F f)
    {
        worker_thread->post(std::move(f));
    }

    boost::asio::io_service& getMasterIOService()
    {
        return master_thread->getIOService();
    }

    boost::asio::io_service& getWorkerIOService()
    {
        return worker_thread->getIOService();
    }

private:
    BidirectionalCommunicator();

    bool init(int worker_threads);

    TaskDispatcher::ptr master_thread;
    MultipleTaskDispatcher::ptr worker_thread;

    bool init_flag;
};

#endif

