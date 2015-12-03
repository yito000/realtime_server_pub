#include "redis_service.h"
#include "log/logger.h"

#include <boost/asio/ip/address.hpp>

namespace {
    const int CONNECTION_WATCH_TIME = 1000;
};

RedisService::RedisService() : cli(nullptr), work(ios), connected(false)
{
    port = 0;
    signal = NO_SIGNAL;
    
    last_watch = AppTime::now();

    cli = new RedisClient(ios);
    cli->installErrorHandler(std::bind(&RedisService::errorHandle,
        this, std::placeholders::_1));
}

RedisService::~RedisService()
{
    ios.poll();
    ios.stop();

    if (cli) {
        delete cli;
    }
}

bool RedisService::connect(const std::string& address, 
    unsigned short _port)
{
    auto addr = boost::asio::ip::address::from_string(address);

    host = address;
    port = _port;

    Logger::debug("connection %s:%d", address.c_str(), _port);
    ios.reset();

    if (cli) {
        delete cli;

        cli = new RedisClient(ios);
        cli->installErrorHandler(std::bind(&RedisService::errorHandle,
            this, std::placeholders::_1));

        execQueueTask();
    }
    
    if (cli->connect(addr, _port)) {
        Logger::log("connection success!");

        signal.exchange(NO_SIGNAL);
        connected = true;

        return true;
    } else {
        Logger::log("connection failure!");
        
        signal.exchange(SIG_CONNECTION);

        return false;
    }

    return true;
}

void RedisService::subscribe(const std::string& channel, 
    RedisSubscribeCallback callback, RedisErrorCallback err_callback)
{
    std::string c = channel;

    auto callback_func = [this, callback](const std::string& s) {
        item_queue.pop();

        callback(s);
    };

    invoke([this, c, callback_func, err_callback]() {
        addItem(err_callback);

        cli->subscribe(c, callback_func);
    });
}

void RedisService::publish(const std::string& channel, 
    const std::string& message, RedisPublishCallback callback,
    RedisErrorCallback err_callback)
{
    std::string c = channel;
    std::string m = message;

    auto callback_func = [this, callback](const RedisValue& rv) {
        item_queue.pop();

        callback(rv);
    };

    invoke([this, c, m, callback_func, err_callback]() {
        addItem(err_callback);

        cli->publish(c, m, callback_func);
    });
}

void RedisService::get(const std::string& key, 
    RedisCommandCallback callback, RedisErrorCallback err_callback)
{
    std::string k = key;

    auto callback_func = [this, callback](const RedisValue& rv) {
        item_queue.pop();

        callback(rv);
    };

    invoke([this, k, callback_func, err_callback]() {
        addItem(err_callback);

        cli->command("GET", k, callback_func);
    });
}

void RedisService::set(const std::string& key, const std::string& value,
    RedisCommandCallback callback, RedisErrorCallback err_callback)
{
    std::string k = key;
    std::string v = value;

    auto callback_func = [this, callback](const RedisValue& rv) {
        item_queue.pop();

        callback(rv);
    };

    invoke([this, k, v, callback_func, err_callback]() {
        addItem(err_callback);

        cli->command("SET", k, v, callback_func);
    });
}

void RedisService::del(const std::string& key, 
    RedisCommandCallback callback, RedisErrorCallback err_callback)
{
    std::string k = key;

    auto callback_func = [this, callback](const RedisValue& rv) {
        item_queue.pop();

        callback(rv);
    };

    invoke([this, k, callback_func, err_callback]() {
        addItem(err_callback);

        cli->command("DEL", k, callback_func);
    });
}

void RedisService::exist(const std::string& key, 
    RedisCommandCallback callback, RedisErrorCallback err_callback)
{
    std::string k = key;

    auto callback_func = [this, callback](const RedisValue& rv) {
        item_queue.pop();

        callback(rv);
    };

    invoke([this, k, callback_func, err_callback]() {
        addItem(err_callback);

        cli->command("EXIST", k, callback_func);
    });
}

void RedisService::command(const std::string& cmd, 
    const std::list<std::string>& args, RedisCommandCallback callback,
    RedisErrorCallback err_callback)
{
    std::string c = cmd;
    std::list<std::string> a = args;

    auto callback_func = [this, callback](const RedisValue& rv) {
        item_queue.pop();

        callback(rv);
    };

    invoke([this, c, a, callback_func, err_callback]() {
        addItem(err_callback);

        cli->command(c, a, callback_func);
    });
}

void RedisService::update()
{
    watchConnection();

    try {
        ios.poll();
    } catch (std::exception& e) {
        Logger::debug("%s", e.what());

        signal.exchange(SIG_CONNECTION);
        connected = false;
    }
}

// private member function
void RedisService::invoke(std::function<void()> exec_func)
{
    ios.dispatch(exec_func);
}

void RedisService::watchConnection()
{
    auto now = AppTime::now();
    auto diff_time = (int)((float)(now - last_watch).count() / 1000.0);
    
    if (diff_time < CONNECTION_WATCH_TIME) {
        return;
    }
    
    last_watch = now;
    
    if (signal == SIG_CONNECTION) {
        try {
            connect(host, port);
        } catch (...) {}
    }
}

RedisService::Item::ptr RedisService::addItem(
    RedisErrorCallback err_callback)
{
    Item::ptr item = new Item;
    item->error_callback = err_callback;

    item_queue.push(item);

    return item;
}

void RedisService::errorHandle(const std::string& s)
{
    Logger::debug("connection error!");

    while (!item_queue.empty()) {
        auto item = item_queue.front();
        item->error_callback(s);

        item_queue.pop();
    }

    signal.exchange(SIG_CONNECTION);
}

void RedisService::execQueueTask()
{
    while (!item_queue.empty()) {
        auto item = item_queue.front();
        item->error_callback("connection error");

        item_queue.pop();
    }
}
