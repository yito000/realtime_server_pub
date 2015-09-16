#include "multiple_task_dispatcher.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

MultipleTaskDispatcher::MultipleTaskDispatcher(int threads) : 
    work(io_service), init_threads(threads), init(false)
{
    //
}

MultipleTaskDispatcher::~MultipleTaskDispatcher()
{
    if (init) {
        io_service.stop();
    }
}

void MultipleTaskDispatcher::initialize()
{
    if (init) {
        return;
    }

    try {
        int i = 0;

        for (; i < init_threads; i++) {
            boost::thread* t = group.create_thread(
                boost::bind(&boost::asio::io_service::run, &io_service));
            t->detach();
            
            thread_id_list[i] = t->get_id();
            thread_list[t->get_id()] = t;
        }

        next_index = i;
        init = true;
    } catch (std::exception& e) {
        // error
        throw e;
    }
}

void MultipleTaskDispatcher::poll()
{
    io_service.poll();
}

void MultipleTaskDispatcher::addWorker()
{
    boost::thread* t = group.create_thread(
        boost::bind(&boost::asio::io_service::run, &io_service));

    thread_id_list[next_index] = t->get_id();
    thread_list[t->get_id()] = t;

    next_index++;
}

int MultipleTaskDispatcher::size()
{
    return group.size();
}

