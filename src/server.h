#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

namespace server {

class SessionDelegate;

};

class Server
{
public:
    enum AddrType {
        ADDR_V4 = 1,
        ADDR_V6
    };

    Server(const AddrType addr_type, short port);
    ~Server();

    void accept();
    void start();

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

    boost::asio::ip::tcp::acceptor* acceptor;
    server::SessionDelegate* session_delegate;

    int timeout_millis;
    int retry;
};

#endif
