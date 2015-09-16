#ifndef APP_DIRECTOR_H
#define APP_DIRECTOR_H

#include "smart_ptr.hpp"

#include "thread/bidirectional_communicator.h"
#include "variable/key_value_cache_storage.h"

#include <boost/thread.hpp>
#include <functional>

typedef std::function<void(const std::string&)> VarGetCallback;
typedef std::function<void(void*)> VarObjGetCallback;

class AppDirector : public SmartPtr<AppDirector>
{
public:
    typedef boost::intrusive_ptr<AppDirector> ptr;
    
    static AppDirector::ptr getInstance();
    void finalize();
    
    boost::thread::id getMainThreadId();
    void setMainThreadId(const boost::thread::id& tid);
    bool isMainThread();
    bool isMainThread(const boost::thread::id& tid);
    
    void setBidirectionalCommunicator(BidirectionalCommunicator::ptr t_comm)
    {
        thread_conn = t_comm;
    }
    
    template <typename F>
    void postMaster(F f)
    {
        thread_conn->postMaster(f);
    }

    template <typename F>
    void postWorker(F f)
    {
        thread_conn->postWorker(f);
    }
    
    KeyValueCacheStorage::ptr getKeyValueCacheStorage();
    void setKeyValueCacheStorage(KeyValueCacheStorage::ptr kv_cache);
    
    void getVariable(const std::string& key, VarGetCallback callback);
    void setVariable(const std::string& key, const std::string& value);

    void getVarObject(const std::string& key, VarObjGetCallback callback);
    void setVarObject(const std::string& key, void* data);
    
private:
    AppDirector();
    
    BidirectionalCommunicator::ptr thread_conn;
    KeyValueCacheStorage::ptr key_value_cache;
    
    boost::thread::id render_thread_id;
};

#endif
