#ifndef TASK_DISPACTHER_H
#define TASK_DISPACTHER_H

#include "fw_env.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class TaskDispatcher : public SmartPtr<TaskDispatcher>
{
public:
    typedef boost::intrusive_ptr<TaskDispatcher> ptr;

    TaskDispatcher();
    ~TaskDispatcher();

    void initialize();
    void update();
    void stop();

    template <typename F>
    void post(F f)
    {
        io_service.post(f);
    }

    boost::asio::io_service& getIOService()
    {
        return io_service;
    }

private:
    boost::asio::io_service io_service;
    boost::asio::io_service::work work;

    bool init;
    bool end_flag;
};

#endif
