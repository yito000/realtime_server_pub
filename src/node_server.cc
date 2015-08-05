#include "node_server.h"
#include "network/websocket/server/websocket_session.h"

#include "log/logger.h"

using boost::asio::ip::tcp;

NodeServer::NodeServer(const AddrType addr_type, short port) : 
    work(ios), 
    session_delegate(NULL), 
    timeout_millis(30 * 1000)
{
    tcp addr = tcp::v4();
    if (addr_type == ADDR_V6) {
        addr = tcp::v6();
    }

    boost::asio::ip::tcp::endpoint ep(addr, port);
    acceptor = new tcp::acceptor(ios, ep);
}

NodeServer::~NodeServer()
{
    delete acceptor;
    if (session_delegate) {
        delete session_delegate;
    }
}

void NodeServer::accept()
{
    auto ss = server::WebsocketSession::create(ios, timeout_millis);
    ss->setDelegate(session_delegate);
    ss->setValidProtocol(protocol);

    acceptor->async_accept(ss->getSocket(),
        [this, ss](boost::system::error_code ec) {
            if (!ec) {
                ss->start();
            } else {
                ss->destroyAsync();
                Logger::debug("node server: accept error");
            }

            accept();
        });
}

void NodeServer::run()
{
    ios.poll();
}
