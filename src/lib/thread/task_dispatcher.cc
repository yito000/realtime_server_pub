#include "task_dispatcher.h"

TaskDispatcher::TaskDispatcher() : work(io_service),
    init(false)
{
    //
}

TaskDispatcher::~TaskDispatcher()
{
    if (init) {
        io_service.poll();
        io_service.stop();
    }
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

