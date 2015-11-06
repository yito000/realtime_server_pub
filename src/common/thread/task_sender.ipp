#ifndef TASK_SENDER_IPP
#define TASK_SENDER_IPP

#include "common/app_director.h"

template <typename T>
boost::intrusive_ptr< TaskSender<T> > TaskSender<T>::create()
{
    auto inst = new TaskSender<T>;
    return inst;
}

template <typename T>
void TaskSender<T>::send(ActThreadType t_type, std::function<void()> func)
{
    postVoidTask(t_type, func);
}

template <typename T>
std::future<T> TaskSender<T>::sendAsyncIOS(boost::asio::io_service& ios, std::function<T()> func)
{
    p_task = std::packaged_task< T() >(func);
    
    ios.post([this]() {
        p_task();
    });
    
    std::future<T> future = p_task.get_future();
    future.wait();
    
    return future;
}

template <typename T>
std::future<T> TaskSender<T>::sendAsync(ActThreadType t_type, std::function<T()> func)
{
    postPackagedTask(t_type, func);
    
    std::future<T> future = p_task.get_future();
    future.wait();
    
    return future;
}

template <typename T>
void TaskSender<T>::reset()
{
    p_task.reset();
}

// private member function
template <typename T>
void TaskSender<T>::postVoidTask(ActThreadType t_type, std::function<void()> func)
{
    auto app_dir = AppDirector::getInstance();
    
    switch (t_type) {
        case ActThreadType::MAIN_THREAD: {
                if (app_dir->isMainThread()) {
                    func();
                } else {
                   app_dir->postMaster([func]() {
                        func();
                    });
                }
            }
            break;
        
        case ActThreadType::WORKER_THREAD: {
                app_dir->postWorker([func]() {
                    func();
                });
            }
            break;
    }
}

template <typename T>
void TaskSender<T>::postPackagedTask(ActThreadType t_type, std::function<T()> func)
{
    auto app_dir = AppDirector::getInstance();
    
    switch (t_type) {
        case ActThreadType::MAIN_THREAD: {
                p_task = std::packaged_task< T() >(func);
                if (app_dir->isMainThread()) {
                    p_task();
                } else {
                    app_dir->postMaster([this]() {
                        p_task();
                    });
                }
            }
            break;
        
        case ActThreadType::WORKER_THREAD: {
                p_task = std::packaged_task< T() >(func);
                app_dir->postWorker([this]() {
                    p_task();
                });
            }
            break;
    }
}

#endif
