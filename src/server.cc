#include "server.h"
#include <boost/thread.hpp>

#include "log/logger.h"

Server::Server() : 
    end_flag(false)
{
    //
}

Server::~Server()
{
    //
}

void Server::start()
{
    while (!end_flag) {
        ios.poll();
        
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    }
}

boost::asio::io_service& Server::getIOService()
{
    return ios;
}

bool Server::isEnd() const
{
    return end_flag;
}

void Server::setEndFlag(bool b)
{
    end_flag = b;
    
    if (tcp_server) {
        tcp_server->setEndFlag(b);
    }
}

TcpServer::ptr Server::getTcpServer() const
{
    return tcp_server;
}

void Server::setTcpServer(TcpServer::ptr s)
{
    tcp_server = s;
}
