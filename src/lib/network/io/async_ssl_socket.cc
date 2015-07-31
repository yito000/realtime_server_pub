#include "async_ssl_socket.h"

#include <boost/bind.hpp>

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios,
    const std::string& _host, unsigned int _port, bool _server_mode) :
    ios(_ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(_server_mode)
{
    host = _host;
    protocol = std::to_string(_port);
    
    if (server_mode) {
        ssl_context = new boost::asio::ssl::context(ios, boost::asio::ssl::context::tlsv12_server);
    } else {
        ssl_context = new boost::asio::ssl::context(ios, boost::asio::ssl::context::tlsv12_client);
    }
    
    ssl_context->set_password_callback(boost::bind(&AsyncSSLSocket::passwordCallback, this));
    socket = new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(ios, *ssl_context);
    
    initTimer();
}

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios,
    const std::string& _host, const std::string& _protocol, bool _server_mode) :
    ios(_ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(_server_mode)
{
    host = _host;
    protocol = _protocol;
    
    if (server_mode) {
        ssl_context = new boost::asio::ssl::context(ios, boost::asio::ssl::context::tlsv12_server);
    } else {
        ssl_context = new boost::asio::ssl::context(ios, boost::asio::ssl::context::tlsv12_client);
    }
    
    ssl_context->set_password_callback(boost::bind(&AsyncSSLSocket::passwordCallback, this));
    socket = new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(ios, *ssl_context);
    
    initTimer();
}

AsyncSSLSocket::~AsyncSSLSocket()
{
    close();
    
    delete ssl_context;
    delete socket;
}

void AsyncSSLSocket::setVerifyMode(VerifyMode verify_mode)
{
    ssl_context->set_verify_mode(
        (boost::asio::ssl::context::verify_mode)verify_mode);
}

void AsyncSSLSocket::loadCertificate(const ByteBuffer& buffer)
{
    // pem file only
    ssl_context->use_certificate(boost::asio::buffer(buffer),
        boost::asio::ssl::context_base::pem);
}

void AsyncSSLSocket::loadPrivateKey(const ByteBuffer& buffer)
{
    // pem file only
    ssl_context->use_private_key(boost::asio::buffer(buffer),
        boost::asio::ssl::context_base::pem);
}

void AsyncSSLSocket::connect(boost::posix_time::time_duration timeout, 
    SocketConnectCallback callback)
{
    boost::asio::ip::tcp::resolver resolver(ios);
    boost::asio::ip::tcp::resolver::query query(host, protocol);
    auto endpoint_it = resolver.resolve(query);
    
    connect_timer.expires_from_now(timeout);
    
    auto err = boost::asio::error::host_not_found;
    connectInternal(err, endpoint_it, timeout, callback);
}

void AsyncSSLSocket::close()
{
    if (socket->lowest_layer().is_open()) {
        socket->lowest_layer().close();
    }
}

bool AsyncSSLSocket::isOpen()
{
    return socket->lowest_layer().is_open();
}

void AsyncSSLSocket::read(boost::posix_time::time_duration timeout, 
    SocketReadCallback callback)
{
    read_timer.expires_from_now(timeout);
    
    socket->async_read_some(boost::asio::buffer(data, MAX_LENGTH), 
        boost::bind(callback, _1, data, _2));
}

void AsyncSSLSocket::write(const ByteBuffer& buffer, 
    boost::posix_time::time_duration timeout, SocketWriteCallback callback)
{
    write_timer.expires_from_now(timeout);
    
    socket->async_write_some(boost::asio::buffer(buffer),
        callback);
}

void AsyncSSLSocket::setConnectTimeoutCallback(SocketTimeoutCallback callback)
{
    connect_timeout_func = callback;
}

void AsyncSSLSocket::setReadTimeoutCallback(SocketTimeoutCallback callback)
{
    read_timeout_func = callback;
}

void AsyncSSLSocket::setWriteTimeoutCallback(SocketTimeoutCallback callback)
{
    write_timeout_func = callback;
}

// private member function
void AsyncSSLSocket::initTimer()
{
    connect_timer.async_wait(
        std::bind(&AsyncSSLSocket::checkConnectDeadline, this));
    
    read_timer.async_wait(
        std::bind(&AsyncSSLSocket::checkReadDeadline, this));

    write_timer.async_wait(
        std::bind(&AsyncSSLSocket::checkWriteDeadline, this));
}

void AsyncSSLSocket::connectInternal(boost::system::error_code err,
    boost::asio::ip::tcp::resolver::iterator endpoint_it,
    boost::posix_time::time_duration timeout,
    SocketConnectCallback callback)
{
    boost::asio::ip::tcp::resolver::iterator end;
    
    if (err && endpoint_it != end) {
        socket->lowest_layer().async_connect(*endpoint_it++, 
            [this, endpoint_it, timeout, callback](boost::system::error_code ec) {
                if (ec) {
                    connectInternal(ec, endpoint_it, timeout, callback);
                } else {
                    handshake(timeout, callback);
                }
            });
    } else {
        callback(boost::asio::error::host_not_found);
    }
}

void AsyncSSLSocket::checkConnectDeadline()
{
    if (connect_timeout_func) {
        if (connect_timeout_func(*this)) {
            if (connect_timer.expires_at() <=
                boost::asio::deadline_timer::traits_type::now()) {
            
                connect_timer.expires_at(boost::posix_time::pos_infin);
                connect_timer.async_wait(
                    std::bind(&AsyncSSLSocket::checkConnectDeadline, this));
            }
        } else {
            close();
        }
    } else {
        close();
    }
}

void AsyncSSLSocket::checkReadDeadline()
{
    if (read_timeout_func) {
        if (read_timeout_func(*this)) {
            if (read_timer.expires_at() <=
                boost::asio::deadline_timer::traits_type::now()) {
            
                read_timer.expires_at(boost::posix_time::pos_infin);
                read_timer.async_wait(
                    std::bind(&AsyncSSLSocket::checkReadDeadline, this));
            }
        } else {
            close();
        }
    } else {
        close();
    }
}

void AsyncSSLSocket::checkWriteDeadline()
{
    if (write_timeout_func) {
        if (write_timeout_func(*this)) {
            if (write_timer.expires_at() <=
                boost::asio::deadline_timer::traits_type::now()) {
            
                write_timer.expires_at(boost::posix_time::pos_infin);
                write_timer.async_wait(
                    std::bind(&AsyncSSLSocket::checkWriteDeadline, this));
            }
        } else {
            close();
        }
    } else {
        close();
    }
}

void AsyncSSLSocket::handshake(boost::posix_time::time_duration timeout,
    SocketConnectCallback callback)
{
    connect_timer.expires_from_now(timeout);
    
    if (server_mode) {
        socket->async_handshake(boost::asio::ssl::stream_base::server,
            [this, callback](boost::system::error_code ec) {
                callback(ec);
                
                if (ec) {
                    close();
                }
            });
    } else {
        socket->async_handshake(boost::asio::ssl::stream_base::client,
            [this, callback](boost::system::error_code ec) {
                callback(ec);
                
                if (ec) {
                    close();
                }
            });
    }
}

std::string AsyncSSLSocket::passwordCallback() const
{
    // TODO
    return "test";
}
