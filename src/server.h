#ifndef SERVER_H
#define SERVER_H

#include "fw_env.h"

#include "server/tcp_server.h"
#include "server/udp_server.h"

BEGIN_NS

class Server : public SmartPtr<Server>
{
public:
    typedef boost::intrusive_ptr<Server> ptr;
    
    Server();
    virtual ~Server();

    void start();
    boost::asio::io_service& getIOService();

    bool isEnd() const;
    void setEndFlag(bool b);
    
    TcpServer::ptr getTcpServer() const;
    void setTcpServer(TcpServer::ptr s);
    
    UdpServer::ptr getUdpServer() const;
    void setUdpServer(UdpServer::ptr s);
    
private:
    boost::asio::io_service ios;
    TcpServer::ptr tcp_server;
    UdpServer::ptr udp_server;

    bool end_flag;
};

END_NS

#endif
