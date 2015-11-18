#include "task_dispatcher.h"

TaskDispatcher::TaskDispatcher() : work(io_service),
    init(false),
    end_flag(false)
{
    //
}

TaskDispatcher::~TaskDispatcher()
{
    stop();
}

void TaskDispatcher::initialize()
{
    if (init) {
        return;
    }

    init = true;
}

void TaskDispatcher::update()
{
    if (init) {
        io_service.poll();
    }
}

void TaskDispatcher::stop()
{
    if (end_flag) {
        return;
    }
    
    if (init) {
        io_service.stop();
        
        end_flag = true;
    }
}
