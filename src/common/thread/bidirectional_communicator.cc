#include "bidirectional_communicator.h"

BidirectionalCommunicator::ptr BidirectionalCommunicator::create(int worker_threads)
{
    auto inst = new BidirectionalCommunicator;
    if (!inst->init(worker_threads)) {
        delete inst;
        return nullptr;
    }

    return inst;
}

void BidirectionalCommunicator::update()
{
    master_thread->update();
}

// private member function
BidirectionalCommunicator::BidirectionalCommunicator() : init_flag(false)
{
    //
}

bool BidirectionalCommunicator::init(int worker_threads)
{
    if (init_flag) {
        return true;
    }

    master_thread = new TaskDispatcher;
    master_thread->initialize();

    worker_thread = new MultipleTaskDispatcher(worker_threads);
    worker_thread->initialize();

    init_flag = true;

    return true;
}
