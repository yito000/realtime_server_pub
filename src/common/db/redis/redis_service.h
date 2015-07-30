#ifndef REDIS_SERVICE_H
#define REDIS_SERVICE_H

#include "db/redis/lib/redisclient.h"

#include "smart_ptr.hpp"

#include <string>
#include <queue>
#include <boost/function.hpp>

typedef std::function<void(const std::string&)> RedisSubscribeCallback;
typedef std::function<void(const RedisValue&)> RedisPublishCallback;
typedef std::function<void(const RedisValue&)> RedisCommandCallback;
typedef std::function<void(const std::string&)> RedisErrorCallback;

class RedisService : public SmartPtr<RedisService>
{
public:
    typedef boost::intrusive_ptr<RedisService> ptr;

    RedisService();
    ~RedisService();

    bool connect(const std::string& address, unsigned short port);

    void subscribe(const std::string& channel, 
        RedisSubscribeCallback callback, RedisErrorCallback err_callback);
    void publish(const std::string& channel, 
        const std::string& message, RedisPublishCallback callback,
        RedisErrorCallback err_callback);

    void get(const std::string& key, RedisCommandCallback callback,
        RedisErrorCallback err_callback);
    void set(const std::string& key, const std::string& value,
        RedisCommandCallback callback, RedisErrorCallback err_callback);
    void del(const std::string& key, RedisCommandCallback callback,
        RedisErrorCallback err_callback);
    void exist(const std::string& key, RedisCommandCallback callback,
        RedisErrorCallback err_callback);

    void command(const std::string& cmd, const std::list<std::string>& args, 
        RedisCommandCallback callback, RedisErrorCallback err_callback);

    void update();

private:
    struct Item : public SmartPtr<Item>
    {
        typedef boost::intrusive_ptr<Item> ptr;

        RedisErrorCallback error_callback;

        Item()
        {
            //
        }
    };

    enum Signal {
        SIG_CONNECTION = 0,
        NO_SIGNAL
    };

    void invoke(std::function<void()> exec_func);
    void watchConnection();

    Item::ptr addItem(RedisErrorCallback err_callback);
    void errorHandle(const std::string& s);
    void execQueueTask();

    boost::asio::io_service ios;
    boost::asio::io_service::work work;

    RedisClient* cli;
    std::queue<Item::ptr> item_queue;

    std::string host;
    unsigned short port;

    bool connected;
    int signal;
};

#endif
