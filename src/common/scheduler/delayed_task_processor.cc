#include "delayed_task_processor.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "log/logger.h"

DelayedTaskProcessor::ptr DelayedTaskProcessor::create()
{
    auto inst = new DelayedTaskProcessor;
    if (!inst->init()) {
        delete inst;
        return nullptr;
    }

    return inst;
}

DelayedTaskProcessor::~DelayedTaskProcessor()
{
    io_service.poll();
    io_service.stop();
}

void DelayedTaskProcessor::poll()
{
    if (!init_flag) {
        return;
    }

    io_service.poll();

    auto it = task_list.begin();
    while (it != task_list.end()) {
        TaskInfo::ptr t = it->second;
        auto now = std::chrono::system_clock::now();
        auto du = std::chrono::milliseconds(t->task->interval);

        if (t->last_time + du < now) {
            if (t->task->max_loop >= 1) {
                t->current_step++;

                if (t->current_step <= t->task->max_loop) {
                    t->task->func();
                    t->last_time = std::chrono::system_clock::now();

                    ++it;
                } else {
                    Logger::debug("remove task");
                    task_list.erase(it++);
                }
            } else {
                t->task->func();
                t->last_time = std::chrono::system_clock::now();

                ++it;
            }
        } else {
            ++it;
        }
    }
}

void DelayedTaskProcessor::addTask(const std::string name,
    DelayedTask::ptr task)
{
    io_service.post([this, name, task]() {
        auto it = task_list.find(name);
        if (it != task_list.end()) {
            // warning
        }

        if (task->interval < 1) {
            task->interval = 1;
        }

        auto task_info = new DelayedTaskProcessor::TaskInfo;
        task_info->task = task;

        task_list[name] = task_info;
    });
}

void DelayedTaskProcessor::removeTask(const std::string name)
{
    io_service.post([this, name]() {
        auto it = task_list.find(name);
        if (it != task_list.end()) {
            task_list.erase(it);
        }
    });
}

// private member function
DelayedTaskProcessor::DelayedTaskProcessor() : work(io_service),
    init_flag(false)
{
    //
}

bool DelayedTaskProcessor::init()
{
    if (init_flag) {
        return true;
    }

    init_flag = true;

    return true;
}
