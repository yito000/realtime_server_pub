#include "tcp_server.h"
#include "network/websocket/server/websocket_session.h"

#include <boost/thread.hpp>
#include "log/logger.h"

#include "common/file/file_util.h"

using boost::asio::ip::tcp;

TcpServer::TcpServer(boost::asio::io_service& _ios,
    const AddrType addr_type, short port,
    const std::string& cert_filepath, const std::string& pkey_filepath,
    const std::string& tmp_dh_filepath) : 
    ios(_ios),
    ssl_context(ios, boost::asio::ssl::context::tlsv1),
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
    
    ssl_context.set_options(
        boost::asio::ssl::context::default_workarounds | 
        boost::asio::ssl::context::no_sslv2 |
        boost::asio::ssl::context::single_dh_use);
    ssl_context.set_password_callback(boost::bind(&TcpServer::passwordCallback, this));
    
    protocol = "default";
    
    //
    auto file_util = FileUtil::getInstance();
    
    // cert
    {
        FileStream::ptr st = file_util->getFileStream(cert_filepath);
        auto data = st->readAll();
        
        std::vector<char> cert;
        
        cert.reserve(data->getSize());
        auto p = data->getBuffer();
        
        for (int i = 0; i < data->getSize(); i++) {
            cert.push_back(p[i]);
        }
        
        ssl_context.use_certificate_chain(boost::asio::buffer(cert));
    }
    
    // pkey
    {
        FileStream::ptr st = file_util->getFileStream(pkey_filepath);
        auto data = st->readAll();
        
        std::vector<char> pkey;
        
        pkey.reserve(data->getSize());
        auto p = data->getBuffer();
        
        for (int i = 0; i < data->getSize(); i++) {
            pkey.push_back(p[i]);
        }
        
        ssl_context.use_private_key(boost::asio::buffer(pkey),
            boost::asio::ssl::context::pem);
    }
    
    // temp dh
    {
        FileStream::ptr st = file_util->getFileStream(tmp_dh_filepath);
        auto data = st->readAll();
        
        std::vector<char> tmp_dh;
        
        tmp_dh.reserve(data->getSize());
        auto p = data->getBuffer();
        
        for (int i = 0; i < data->getSize(); i++) {
            tmp_dh.push_back(p[i]);
        }
        
        ssl_context.use_tmp_dh(boost::asio::buffer(tmp_dh));
    }
}

TcpServer::~TcpServer()
{
    delete acceptor;
    if (session_delegate) {
        delete session_delegate;
    }
}

void TcpServer::accept()
{
    if (end_flag) {
        return;
    }
    
    auto ss = server::WebsocketSession::createSSL(ios, ssl_context, timeout_millis);
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

std::string TcpServer::getPassword() const
{
    return password_string;
}

void TcpServer::setPassword(const std::string& value)
{
    password_string = value;
}

// private member function
std::string TcpServer::passwordCallback() const
{
    return password_string;
}
