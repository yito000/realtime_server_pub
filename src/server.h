#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "smart_ptr.hpp"

namespace server {
    class SessionDelegate;
};

class Server : public SmartPtr<Server>
{
public:
    typedef boost::intrusive_ptr<Server> ptr;
    
    enum AddrType {
        ADDR_V4 = 1,
        ADDR_V6
    };

    Server(const AddrType addr_type, short port);
    virtual ~Server();

    void accept();
    void start();

    server::SessionDelegate* getDelegate() const
    {
        return session_delegate;
    }

    void setDelegate(server::SessionDelegate* sd)
    {
        session_delegate = sd;
    }

    int getTimeoutMillis() const
    {
        return timeout_millis;
    }

    void setTimeoutMillis(int ms)
    {
        timeout_millis = ms;
    }
    
    std::string getValidProtocol() const
    {
        return protocol;
    }
    
    void setValidProtocol(const std::string& str)
    {
        protocol = str;
    }

    bool isEnd() const
    {
        return end_flag;
    }
    
    void setEndFlag(bool b)
    {
        end_flag = b;
    }

private:
    boost::asio::io_service ios;

    boost::asio::ip::tcp::acceptor* acceptor;
    server::SessionDelegate* session_delegate;

    int timeout_millis;
    int retry;
    
    std::string protocol;
    bool end_flag;
};

#endif
