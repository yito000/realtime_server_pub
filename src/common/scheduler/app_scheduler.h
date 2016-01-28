#ifndef APP_SCHEDULER_H
#define APP_SCHEDULER_H

#include "fw_env.h"

#include "delayed_task.h"
#include "delayed_task_processor.h"

#include <atomic>

BEGIN_NS

class AppScheduler : public SmartPtr<AppScheduler>
{
public:
    typedef boost::intrusive_ptr<AppScheduler> ptr;

    AppScheduler();
    ~AppScheduler();

    void initialize(int interval);
    void run(); // execute sp thread

    void addTask(const std::string name, DelayedTask::ptr task);
    void removeTask(const std::string name);

    void endScheduler();

private:
    DelayedTaskProcessor::ptr task_processor;

    int interval_millis;
    bool init;
    std::atomic<bool> end;
};

END_NS

#endif
