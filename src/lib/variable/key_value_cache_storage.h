#ifndef KEY_VALUE_CACHE_STORAGE_H
#define KEY_VALUE_CACHE_STORAGE_H

#include <string>
#include <map>

#include <boost/optional.hpp>

#include "smart_ptr.hpp"

class KeyValueCacheStorage : public SmartPtr<KeyValueCacheStorage>
{
public:
    typedef boost::intrusive_ptr<KeyValueCacheStorage> ptr;
    typedef boost::optional<std::string> Option;
    typedef boost::optional<void*> ObjOpt;
    
    void set(const std::string& key, const std::string& value);
    Option get(const std::string& key);
    
    void setObject(const std::string& key, void* data);
    ObjOpt getObject(const std::string& key);
    
private:
    std::map<std::string, std::string> string_cache;
    std::map<std::string, void*> obj_cache;
};

#endif
