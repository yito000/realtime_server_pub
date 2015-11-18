#ifndef MULTIPLE_TASK_DISPATCHER_H
#define MULTIPLE_TASK_DISPATCHER_H

#include "smart_ptr.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <map>

typedef std::map< int, boost::thread::id > ThreadIdList;
typedef std::map< boost::thread::id, boost::thread* > ThreadList;

class MultipleTaskDispatcher : public SmartPtr<MultipleTaskDispatcher>
{
public:
    typedef boost::intrusive_ptr<MultipleTaskDispatcher> ptr;

    MultipleTaskDispatcher(int threads);
    ~MultipleTaskDispatcher();

    void initialize();
    void poll();
    void stop();

    void addWorker();
    int size();

    template <typename F>
    void post(F f)
    {
        io_service.post(f);
    }

    boost::asio::io_service& getIOService()
    {
        return io_service;
    }

private:
    boost::asio::io_service io_service;
    boost::asio::io_service::work work;

    boost::thread_group group;

    ThreadIdList thread_id_list;
    ThreadList thread_list;

    int next_index;
    int init_threads;
    
    bool init;
    bool end_flag;
};

#endif
