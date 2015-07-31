#include "async_ssl_socket.h"

#include <boost/bind.hpp>

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios) :
    ios(_ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(false)
{
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
    const std::string& _host, unsigned int _port) :
    ios(_ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(false)
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
    const std::string& _host, const std::string& _protocol) :
    ios(_ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(false)
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
    
    if (timeout == boost::posix_time::pos_infin) {
        connect_timer.expires_at(boost::posix_time::pos_infin);
    } else {
        connect_timer.expires_from_now(timeout);
    }
    
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
    if (timeout == boost::posix_time::pos_infin) {
        read_timer.expires_at(boost::posix_time::pos_infin);
    } else {
        read_timer.expires_from_now(timeout);
    }
    
    SocketReadCallback func = [this, callback](
        boost::system::error_code ec, char* data, std::size_t s) {
        
        if (callback) {
            callback(ec, data, s);
        }
        
        read_timer.expires_at(boost::posix_time::pos_infin);
    };
    
    socket->async_read_some(boost::asio::buffer(data, MAX_LENGTH), 
        boost::bind(func, _1, data, _2));
}

void AsyncSSLSocket::write(const ByteBuffer& buffer, 
    boost::posix_time::time_duration timeout, SocketWriteCallback callback)
{
    if (timeout == boost::posix_time::pos_infin) {
        write_timer.expires_at(boost::posix_time::pos_infin);
    } else {
        write_timer.expires_from_now(timeout);
    }
    
    socket->async_write_some(boost::asio::buffer(buffer),
        [this, callback](boost::system::error_code ec, std::size_t s) {
            if (callback) {
                callback(ec, s);
            }
            
            write_timer.expires_at(boost::posix_time::pos_infin);
        });
}

std::string AsyncSSLSocket::getHost()
{
    return host;
}

std::string AsyncSSLSocket::getProtocol()
{
    return protocol;
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
                    
                    connect_timer.expires_at(boost::posix_time::pos_infin);
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
    if (connect_timer.expires_at() <=
        boost::asio::deadline_timer::traits_type::now()) {
        
        if (connect_timeout_func && connect_timeout_func(*this)) {
            //
        } else {
            close();
        }
        
        connect_timer.expires_at(boost::posix_time::pos_infin);
    }
    
    if (socket->lowest_layer().is_open()) {
        connect_timer.async_wait(
            std::bind(&AsyncSSLSocket::checkConnectDeadline, this));
    }
}

void AsyncSSLSocket::checkReadDeadline()
{
    if (read_timer.expires_at() <=
        boost::asio::deadline_timer::traits_type::now()) {
        
        if (read_timeout_func && write_timeout_func(*this)) {
            //
        } else {
            close();
        }
        
        read_timer.expires_at(boost::posix_time::pos_infin);
    }
    
    if (socket->lowest_layer().is_open()) {
        read_timer.async_wait(
            std::bind(&AsyncSSLSocket::checkReadDeadline, this));
    }
}

void AsyncSSLSocket::checkWriteDeadline()
{
    if (write_timer.expires_at() <=
        boost::asio::deadline_timer::traits_type::now()) {
        
        if (write_timeout_func && write_timeout_func(*this)) {
            //
        } else {
            close();
        }
        
        write_timer.expires_at(boost::posix_time::pos_infin);
    }
    
    if (socket->lowest_layer().is_open()) {
        write_timer.async_wait(
            std::bind(&AsyncSSLSocket::checkWriteDeadline, this));
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
                } else {
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
            });
    } else {
        socket->async_handshake(boost::asio::ssl::stream_base::client,
            [this, callback](boost::system::error_code ec) {
                callback(ec);
                
                if (ec) {
                    close();
                } else {
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
            });
    }
}

std::string AsyncSSLSocket::passwordCallback() const
{
    // TODO
    return "test";
}
