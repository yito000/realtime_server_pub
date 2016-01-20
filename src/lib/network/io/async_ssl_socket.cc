#include "async_ssl_socket.h"

#include <boost/bind.hpp>
#include "log/logger.h"

#include "openssl/ssl.h"

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios,
    boost::asio::ssl::context& _ssl_context) :
    ios(_ios),
    ios_st(ios),
    ssl_context(_ssl_context),
    socket(ios, ssl_context),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(true)
{
    initTimer();
}

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios,
    boost::asio::ssl::context& _ssl_context,
    const std::string& _host, unsigned int _port) :
    ios(_ios),
    ios_st(ios),
    ssl_context(_ssl_context),
    socket(ios, ssl_context),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(false)
{
    host = _host;
    protocol = std::to_string(_port);
    
    initTimer();
}

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios,
    boost::asio::ssl::context& _ssl_context,
    const std::string& _host, const std::string& _protocol) :
    ios(_ios),
    ios_st(ios),
    ssl_context(_ssl_context),
    socket(ios, ssl_context),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(false)
{
    host = _host;
    protocol = _protocol;
    
    initTimer();
}

AsyncSSLSocket::~AsyncSSLSocket()
{
    close();
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
    if (socket.lowest_layer().is_open()) {
        connect_timer.cancel();
        read_timer.cancel();
        write_timer.cancel();
        
        socket.lowest_layer().close();
    }
}

bool AsyncSSLSocket::isOpen()
{
    return socket.lowest_layer().is_open();
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
    
    socket.async_read_some(boost::asio::buffer(data, MAX_LENGTH), 
        ios_st.wrap(boost::bind(func, _1, data, _2)));
}

void AsyncSSLSocket::write(const ByteBuffer& buffer, 
    boost::posix_time::time_duration timeout, SocketWriteCallback callback)
{
    if (timeout == boost::posix_time::pos_infin) {
        write_timer.expires_at(boost::posix_time::pos_infin);
    } else {
        write_timer.expires_from_now(timeout);
    }
    
    socket.async_write_some(boost::asio::buffer(buffer),
        ios_st.wrap([this, callback](boost::system::error_code ec, std::size_t s) {
            if (callback) {
                callback(ec, s);
            }
            
            write_timer.expires_at(boost::posix_time::pos_infin);
        }));
}

void AsyncSSLSocket::handshake(boost::posix_time::time_duration timeout,
    SocketConnectCallback callback)
{
    connect_timer.expires_from_now(timeout);
    
    if (server_mode) {
        socket.async_handshake(boost::asio::ssl::stream_base::server,
            ios_st.wrap([this, callback](boost::system::error_code ec) {
                if (ec) {
                    Logger::debug("ssl handshake error: %s", ec.message().c_str());
                    close();
                } else {
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
                
                callback(ec);
            }));
    } else {
        socket.async_handshake(boost::asio::ssl::stream_base::client,
            ios_st.wrap([this, callback](boost::system::error_code ec) {
                if (ec) {
                    close();
                } else {
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
                
                callback(ec);
            }));
    }
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
    connect_timer.expires_at(boost::posix_time::pos_infin);
    read_timer.expires_at(boost::posix_time::pos_infin);
    write_timer.expires_at(boost::posix_time::pos_infin);
    
    connect_timer.async_wait(
        ios_st.wrap(boost::bind(&AsyncSSLSocket::checkConnectDeadline, this, _1)));
    
    read_timer.async_wait(
        ios_st.wrap(boost::bind(&AsyncSSLSocket::checkReadDeadline, this, _1)));

    write_timer.async_wait(
        ios_st.wrap(boost::bind(&AsyncSSLSocket::checkWriteDeadline, this, _1)));
}

void AsyncSSLSocket::connectInternal(boost::system::error_code err,
    boost::asio::ip::tcp::resolver::iterator endpoint_it,
    boost::posix_time::time_duration timeout,
    SocketConnectCallback callback)
{
    boost::asio::ip::tcp::resolver::iterator end;
    
    if (err && endpoint_it != end) {
        socket.lowest_layer().async_connect(*endpoint_it++, 
            ios_st.wrap([this, endpoint_it, timeout, callback](boost::system::error_code ec) {
                if (ec) {
                    connectInternal(ec, endpoint_it, timeout, callback);
                } else {
                    boost::asio::ip::tcp::no_delay option(true);
                    socket.lowest_layer().set_option(option);
                    
                    callback(ec);
                    
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
            }));
    } else {
        callback(err);
        
        connect_timer.expires_at(boost::posix_time::pos_infin);
    }
}

void AsyncSSLSocket::checkConnectDeadline(boost::system::error_code ec)
{
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }
    
    if (connect_timer.expires_at() <=
        boost::asio::deadline_timer::traits_type::now()) {
        
        if (connect_timeout_func && connect_timeout_func(*this)) {
            //
        } else {
            close();
        }
        
        connect_timer.expires_at(boost::posix_time::pos_infin);
    }
    
    if (socket.lowest_layer().is_open()) {
        connect_timer.async_wait(
            ios_st.wrap(boost::bind(&AsyncSSLSocket::checkConnectDeadline, this, _1)));
    }
}

void AsyncSSLSocket::checkReadDeadline(boost::system::error_code ec)
{
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }
    
    if (read_timer.expires_at() <=
        boost::asio::deadline_timer::traits_type::now()) {
        
        if (read_timeout_func && write_timeout_func(*this)) {
            //
        } else {
            close();
        }
        
        read_timer.expires_at(boost::posix_time::pos_infin);
    }
    
    if (socket.lowest_layer().is_open()) {
        read_timer.async_wait(
            ios_st.wrap(boost::bind(&AsyncSSLSocket::checkReadDeadline, this, _1)));
    }
}

void AsyncSSLSocket::checkWriteDeadline(boost::system::error_code ec)
{
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }
    
    if (write_timer.expires_at() <=
        boost::asio::deadline_timer::traits_type::now()) {
        
        if (write_timeout_func && write_timeout_func(*this)) {
            //
        } else {
            close();
        }
        
        write_timer.expires_at(boost::posix_time::pos_infin);
    }
    
    if (socket.lowest_layer().is_open()) {
        write_timer.async_wait(
            ios_st.wrap(boost::bind(&AsyncSSLSocket::checkWriteDeadline, this, _1)));
    }
}
