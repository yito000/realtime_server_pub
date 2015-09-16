#include "app_director.h"

#include "log/logger.h"

AppDirector::ptr AppDirector::getInstance()
{
    static AppDirector::ptr inst = new AppDirector;
    
    return inst;
}

void AppDirector::finalize()
{
    thread_conn.reset();
}

boost::thread::id AppDirector::getMainThreadId()
{
    return render_thread_id;
}

void AppDirector::setMainThreadId(const boost::thread::id& tid)
{
    render_thread_id = tid;
}

bool AppDirector::isMainThread()
{
    return render_thread_id == boost::this_thread::get_id();
}

bool AppDirector::isMainThread(const boost::thread::id& tid)
{
    return render_thread_id == tid;
}

KeyValueCacheStorage::ptr AppDirector::getKeyValueCacheStorage()
{
    return key_value_cache;
}

void AppDirector::setKeyValueCacheStorage(KeyValueCacheStorage::ptr kv_cache)
{
    key_value_cache = kv_cache;
}

void AppDirector::getVariable(const std::string& key, 
    VarGetCallback callback)
{
    thread_conn->postMaster([this, key, callback]() {
        auto v = key_value_cache->get(key);

        if (v) {
            callback(v.get());
        } else {
            callback("");
        }
    });
}

void AppDirector::setVariable(const std::string& key, 
    const std::string& value)
{
    std::string str = value;

    thread_conn->postMaster([this, key, str]() {
        key_value_cache->set(key, str);
    });
}

void AppDirector::getVarObject(const std::string& key, 
    VarObjGetCallback callback)
{
    thread_conn->postMaster([this, key, callback]() {
        auto data = key_value_cache->getObject(key);

        if (data) {
            callback(data.get());
        } else {
            callback(nullptr);
        }
    });
}

void AppDirector::setVarObject(const std::string& key, 
    void* data)
{
    thread_conn->postMaster([this, key, data]() {
        key_value_cache->setObject(key, data);
    });
}

// private member function
AppDirector::AppDirector()
{
    //
}
