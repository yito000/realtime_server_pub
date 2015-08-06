#include "server.h"
#include "network/websocket/server/websocket_session.h"

#include <boost/thread.hpp>
#include "log/logger.h"

#include "common/file/file_util.h"

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
    
    //
    auto file_util = FileUtil::getInstance();
    
    // cert
    {
        // TODO
        FileStream::ptr st = file_util->getFileStream("/Users/ito/qtproj/realtime_server/key/server.crt");
        auto data = st->readAll();
        
        cert.reserve(data->getSize());
        auto p = data->getBuffer();
        
        for (int i = 0; i < data->getSize(); i++) {
            cert.push_back(p[i]);
        }
    }
    
    // pkey
    {
        // TODO
        FileStream::ptr st = file_util->getFileStream("/Users/ito/qtproj/realtime_server/key/server.key");
        auto data = st->readAll();
        
        pkey.reserve(data->getSize());
        auto p = data->getBuffer();
        
        for (int i = 0; i < data->getSize(); i++) {
            pkey.push_back(p[i]);
        }
    }
    
    // temp dh
    {
        // TODO
        FileStream::ptr st = file_util->getFileStream("/Users/ito/qtproj/realtime_server/key/dh512.pem");
        auto data = st->readAll();
        
        pkey.reserve(data->getSize());
        auto p = data->getBuffer();
        
        for (int i = 0; i < data->getSize(); i++) {
            tmp_dh.push_back(p[i]);
        }
    }
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
    
    auto ss = server::WebsocketSession::createSSL(ios, timeout_millis, cert, pkey, tmp_dh);
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
