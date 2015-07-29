#ifndef DELAYED_TASK_PROCESSOR_H
#define DELAYED_TASK_PROCESSOR_H

#include "smart_ptr.hpp"
#include "delayed_task.h"

#include <map>
#include <chrono>

#include <boost/asio.hpp>

class DelayedTaskProcessor : public SmartPtr<DelayedTaskProcessor>
{
public:
    typedef boost::intrusive_ptr<DelayedTaskProcessor> ptr;

    static DelayedTaskProcessor::ptr create();
    ~DelayedTaskProcessor();

    void poll();

    void addTask(const std::string name, DelayedTask::ptr task);
    void removeTask(const std::string name);

private:
    struct TaskInfo : public SmartPtr<TaskInfo>
    {
        typedef boost::intrusive_ptr<TaskInfo> ptr;

        std::chrono::system_clock::time_point last_time;
        int current_step;

        DelayedTask::ptr task;

        TaskInfo()
        {
            last_time = std::chrono::system_clock::now();
            current_step = 0;
        }
    };

    DelayedTaskProcessor();
    bool init();

    boost::asio::io_service io_service;
    boost::asio::io_service::work work;

    std::map<std::string, TaskInfo::ptr> task_list;

    bool init_flag;
};

#endif

