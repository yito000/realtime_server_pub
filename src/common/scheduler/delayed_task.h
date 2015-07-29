#ifndef DELAYED_TASK_H
#define DELAYED_TASK_H

#include "smart_ptr.hpp"

#include <functional>

struct DelayedTask : public SmartPtr<DelayedTask>
{
    typedef boost::intrusive_ptr<DelayedTask> ptr;

    int interval;
    int max_loop;
    std::function<void()> func;

    DelayedTask()
    {
        interval = 1;
        max_loop = 0;
    }
};

#endif
