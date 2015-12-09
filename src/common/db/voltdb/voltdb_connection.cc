#ifdef USE_VOLTDB

#include "voltdb_connection.h"
#include "log/logger.h"

class ResponseCallback : public voltdb::ProcedureCallback
{
public:
    typedef boost::shared_ptr<ResponseCallback> sptr;

    ResponseCallback(VoltQueryCallback _func) : func(_func) {}

    bool callback(voltdb::InvocationResponse response)
        throw (voltdb::Exception)
    {
        if (response.success()) {
            func(response);
        } else {
            Logger::debug("failure:");
            Logger::debug("%s", response.toString().c_str());

            func(response);
        }

        return true;
    }

private:
    VoltQueryCallback func;
};

class VoltLogger : public voltdb::ClientLogger
{
public:
    void log(voltdb::ClientLogger::CLIENT_LOG_LEVEL severity, 
        const std::string& msg)
    {
        Logger::debug("%d: %s", severity, msg.c_str());
    }
};

VoltdbConnection::VoltdbConnection(const Params& params) : 
    config(params.username, params.password), 
    cli(voltdb::Client::create(config)), connected(false)
{
    port = 0;
    signal = NO_SIGNAL;

    v_logger = new VoltLogger;
    cli.setLoggerCallback(v_logger);
}

VoltdbConnection::~VoltdbConnection()
{
    if (v_logger) {
        delete v_logger;
    }
}

void VoltdbConnection::connect(const std::string& _host, 
    unsigned short _port)
{
    host = _host;
    port = _port;

    try {
        cli.createConnection(_host, _port);

        signal.exchange(NO_SIGNAL);
        connected = true;
    } catch (voltdb::NoConnectionsException& e) {
        signal.exchange(SIG_CONNECTION);
        connected = false;

        Logger::debug("error: no connection");
    } catch (std::exception& e) {
        connected = false;
        // todo error event

        Logger::debug("error: cannot connect database");
    }
}

bool VoltdbConnection::isConnected()
{
    return connected;
}

void VoltdbConnection::invoke(voltdb::Procedure& procedure,
    VoltQueryCallback callback)
{
    try {
        ResponseCallback::sptr res_callback(new ResponseCallback(callback));

        cli.invoke(procedure, res_callback);
        cli.runOnce();
    } catch (voltdb::NoConnectionsException& e) {
        signal.exchange(SIG_CONNECTION);

        connected = false;
    } catch (std::exception& e) {
        Logger::debug("%s", e.what());
    }
}

void VoltdbConnection::invoke(voltdb::Procedure& procedure,
    VoltQueryCallback callback, VoltErrorCallback err_callback)
{
    try {
        ResponseCallback::sptr res_callback(new ResponseCallback(callback));

        cli.invoke(procedure, res_callback);
        cli.runOnce();
    } catch (voltdb::NoConnectionsException& e) {
        signal.exchange(SIG_CONNECTION);

        if (err_callback) {
            err_callback(e);
        }
    } catch (std::exception& e) {
        if (err_callback) {
            err_callback(e);
        }

        Logger::debug("%s", e.what());
    }
}

void VoltdbConnection::watchConnection()
{
    if (signal == SIG_CONNECTION) {
        connect(host, port);
    }
}

void VoltdbConnection::drain()
{
    while (!cli.drain()) {}
}

#endif
