#ifndef NODE_SERVER_H
#define NODE_SERVER_H

#include "smart_ptr.hpp"
#include <boost/asio.hpp>

namespace server {

class SessionDelegate;

};

class NodeServer : public SmartPtr<NodeServer>
{
public:
    typedef boost::intrusive_ptr<NodeServer> ptr;

    enum AddrType {
        ADDR_V4 = 1,
        ADDR_V6
    };

    NodeServer(const AddrType addr_type, short port);
    ~NodeServer();

    void accept();
    void run();

    server::SessionDelegate* getDelegate()
    {
        return session_delegate;
    }

    void setDelegate(server::SessionDelegate* sd)
    {
        session_delegate = sd;
    }

    int getTimeoutMillis()
    {
        return timeout_millis;
    }

    void setTimeoutMillis(int ms)
    {
        timeout_millis = ms;
    }

    int getRetry()
    {
        return retry;
    }

    void setRetry(int n)
    {
        retry = n;
    }

private:
    boost::asio::io_service ios;
    boost::asio::io_service::work work;

    boost::asio::ip::tcp::acceptor* acceptor;
    server::SessionDelegate* session_delegate;

    int timeout_millis;
    int retry;
};

#endif

