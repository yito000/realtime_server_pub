#include "key_value_cache_storage.h"

void KeyValueCacheStorage::set(const std::string& key, const std::string& value)
{
    string_cache[key] = value;
}

KeyValueCacheStorage::Option KeyValueCacheStorage::get(const std::string& key)
{
    std::map<std::string, std::string>::iterator it = string_cache.find(key);
    
    if (it != string_cache.end()) {
        return Option(it->second);
    } else {
        return Option();
    }
}

void KeyValueCacheStorage::setObject(const std::string& key, void* data)
{
    obj_cache[key] = data;
}

KeyValueCacheStorage::ObjOpt KeyValueCacheStorage::getObject(const std::string& key)
{
    std::map<std::string, void*>::iterator it = obj_cache.find(key);
    
    if (it != obj_cache.end()) {
        return ObjOpt(it->second);
    } else {
        return ObjOpt();
    }
}
