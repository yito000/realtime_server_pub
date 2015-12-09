#ifdef USE_VOLTDB

#ifndef VOLTDB_CONNECTION_H
#define VOLTDB_CONNECTION_H

#include "voltdb.h"
#include "smart_ptr.hpp"

#include <functional>
#include <atomic>

typedef std::function<void(voltdb::InvocationResponse&)> VoltQueryCallback;
typedef std::function<void(std::exception&)> VoltErrorCallback;

class VoltLogger;

class VoltdbConnection : public SmartPtr<VoltdbConnection>
{
public:
    struct Params
    {
        std::string username;
        std::string password;

        Params()
        {
            //
        }
    };

    typedef boost::intrusive_ptr<VoltdbConnection> ptr;

    VoltdbConnection(const Params& params);
    ~VoltdbConnection();

    void connect(const std::string& _host, unsigned short _port = 21212);
    bool isConnected();

    void invoke(voltdb::Procedure& procedure,
        VoltQueryCallback callback);
    void invoke(voltdb::Procedure& procedure,
        VoltQueryCallback callback, VoltErrorCallback err_callback);

    void watchConnection();
    void drain();

private:
    enum Signal {
        SIG_CONNECTION = 0,
        NO_SIGNAL
    };

    voltdb::ClientConfig config;
    voltdb::Client cli;

    std::string host;
    unsigned short port;

    bool connected;
    std::atomic<int> signal;
    VoltLogger* v_logger;
};

#endif

#endif
