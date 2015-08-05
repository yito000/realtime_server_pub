#include "server.h"
#include "network/websocket/server/websocket_session.h"

#include <boost/thread.hpp>
#include "log/logger.h"

using boost::asio::ip::tcp;

Server::Server(const AddrType addr_type, short port) : 
    session_delegate(NULL), 
    timeout_millis(30 * 1000), 
    end_flag(false)
{
    tcp addr = tcp::v4();
    if (addr_type == ADDR_V6) {
        addr = tcp::v6();
    }

    boost::asio::ip::tcp::endpoint ep(addr, port);
    acceptor = new tcp::acceptor(ios, ep);
    
    protocol = "default";
}

Server::~Server()
{
    delete acceptor;
    if (session_delegate) {
        delete session_delegate;
    }
}

void Server::accept()
{
    if (end_flag) {
        return;
    }
    
    auto ss = server::WebsocketSession::create(ios, timeout_millis);
    ss->setDelegate(session_delegate);
    ss->setValidProtocol(protocol);

    acceptor->async_accept(ss->getSocket(),
        [this, ss](boost::system::error_code ec) {
            if (!ec) {
                ss->start();
            } else {
                ss->destroyAsync();
                Logger::log("server: accept error");
            }

            accept();
        });
}

void Server::start()
{
    while (!end_flag) {
        ios.poll();
        
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    }
}
