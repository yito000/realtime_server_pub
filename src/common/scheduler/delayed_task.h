#ifndef DELAYED_TASK_H
#define DELAYED_TASK_H

#include "fw_env.h"

#include <functional>

BEGIN_NS

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

END_NS

#endif
