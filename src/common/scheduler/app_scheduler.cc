#include "app_scheduler.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "lib/atomic/atomic_operator.hpp"
#include "log/logger.h"

AppScheduler::AppScheduler() : 
    interval_millis(-1), 
    init(false), end(false)
{
    //
}

AppScheduler::~AppScheduler()
{
    //
}

void AppScheduler::initialize(int interval)
{
    if (init) {
        return;
    }

    interval_millis = interval;
    if (interval_millis < 1) {
        interval_millis = 1;
    }

    task_processor = DelayedTaskProcessor::create();

    init = true;
}

void AppScheduler::run()
{
    if (!init) {
        return;
    }

    auto du = boost::chrono::milliseconds(interval_millis);

    while (!end) {
        task_processor->poll();

        boost::this_thread::sleep_for(du);
    }
}

void AppScheduler::addTask(const std::string name, 
    DelayedTask::ptr task)
{
    task_processor->addTask(name, task);
}

void AppScheduler::removeTask(const std::string name)
{
    task_processor->removeTask(name);
}

void AppScheduler::endScheduler()
{
    AtomicOperator<bool>::lock_test_and_set(&end, true);
}
