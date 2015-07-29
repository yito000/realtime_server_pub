#include "server.h"
#include "network/websocket/server/websocket_session.h"

#include "log/logger.h"

using boost::asio::ip::tcp;

Server::Server(const AddrType addr_type, short port) : 
    session_delegate(NULL), timeout_millis(30 * 1000), retry(3)
{
    tcp addr = tcp::v4();
    if (addr_type == ADDR_V6) {
        addr = tcp::v6();
    }

    boost::asio::ip::tcp::endpoint ep(addr, port);
    acceptor = new tcp::acceptor(ios, ep);
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
    auto ss = new server::WebsocketSession(ios, timeout_millis, retry);
    ss->setDelegate(session_delegate);
    ss->setValidProtocol("realtime_battle");

    acceptor->async_accept(ss->getSocket(),
        [this, ss](boost::system::error_code ec) {
            if (!ec) {
                ss->start();
            } else {
                delete ss;
                Logger::log("server: accept error");
            }

            accept();
        });
}

void Server::start()
{
    ios.run();
}

