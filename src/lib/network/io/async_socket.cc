#include "async_socket.h"

#include <boost/bind.hpp>

AsyncSocket::AsyncSocket(boost::asio::io_service& _ios,
    const std::string& _host, unsigned int _port) :
    ios(_ios),
    socket(ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios)
{
    host = _host;
    protocol = std::to_string(_port);
    
    initTimer();
}

AsyncSocket::AsyncSocket(boost::asio::io_service& _ios,
    const std::string& _host, const std::string& _protocol) :
    ios(_ios),
    socket(ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios)
{
    host = _host;
    protocol = _protocol;
    
    initTimer();
}

AsyncSocket::~AsyncSocket()
{
    close();
}

void AsyncSocket::connect(boost::posix_time::time_duration timeout, 
    SocketConnectCallback callback)
{
    boost::asio::ip::tcp::resolver resolver(ios);
    boost::asio::ip::tcp::resolver::query query(host, protocol);
    auto endpoint_it = resolver.resolve(query);
    
    connect_timer.expires_from_now(timeout);
    
    auto err = boost::asio::error::host_not_found;
    connectInternal(err, endpoint_it, callback);
}

void AsyncSocket::close()
{
    if (socket.is_open()) {
        socket.close();
    }
}

bool AsyncSocket::isOpen()
{
    return socket.is_open();
}

void AsyncSocket::read(boost::posix_time::time_duration timeout, 
    SocketReadCallback callback)
{
    read_timer.expires_from_now(timeout);
    
    socket.async_read_some(boost::asio::buffer(data, MAX_LENGTH), 
        boost::bind(callback, _1, data, _2));
}

void AsyncSocket::write(const ByteBuffer& buffer, 
    boost::posix_time::time_duration timeout, SocketWriteCallback callback)
{
    write_timer.expires_from_now(timeout);
    
    socket.async_write_some(boost::asio::buffer(buffer),
        callback);
}

void AsyncSocket::setConnectTimeoutCallback(SocketTimeoutCallback callback)
{
    connect_timeout_func = callback;
}

void AsyncSocket::setReadTimeoutCallback(SocketTimeoutCallback callback)
{
    read_timeout_func = callback;
}

void AsyncSocket::setWriteTimeoutCallback(SocketTimeoutCallback callback)
{
    write_timeout_func = callback;
}

// private member function
void AsyncSocket::initTimer()
{
    connect_timer.async_wait(
        std::bind(&AsyncSocket::checkConnectDeadline, this));
    
    read_timer.async_wait(
        std::bind(&AsyncSocket::checkReadDeadline, this));

    write_timer.async_wait(
        std::bind(&AsyncSocket::checkWriteDeadline, this));
}

void AsyncSocket::connectInternal(boost::system::error_code err,
    boost::asio::ip::tcp::resolver::iterator endpoint_it,
    SocketConnectCallback callback)
{
    boost::asio::ip::tcp::resolver::iterator end;
    
    if (err && endpoint_it != end) {
        socket.async_connect(*endpoint_it++, 
            [this, endpoint_it, callback](boost::system::error_code ec) {
                if (ec) {
                    connectInternal(ec, endpoint_it, callback);
                } else {
                    callback(ec);
                }
            });
    } else {
        callback(boost::asio::error::host_not_found);
    }
}

void AsyncSocket::checkConnectDeadline()
{
    if (connect_timeout_func) {
        if (connect_timeout_func(*this)) {
            if (connect_timer.expires_at() <=
                boost::asio::deadline_timer::traits_type::now()) {
            
                connect_timer.expires_at(boost::posix_time::pos_infin);
                connect_timer.async_wait(
                    std::bind(&AsyncSocket::checkConnectDeadline, this));
            }
        } else {
            close();
        }
    } else {
        close();
    }
}

void AsyncSocket::checkReadDeadline()
{
    if (read_timeout_func) {
        if (read_timeout_func(*this)) {
            if (read_timer.expires_at() <=
                boost::asio::deadline_timer::traits_type::now()) {
            
                read_timer.expires_at(boost::posix_time::pos_infin);
                read_timer.async_wait(
                    std::bind(&AsyncSocket::checkReadDeadline, this));
            }
        } else {
            close();
        }
    } else {
        close();
    }
}

void AsyncSocket::checkWriteDeadline()
{
    if (write_timeout_func) {
        if (write_timeout_func(*this)) {
            if (write_timer.expires_at() <=
                boost::asio::deadline_timer::traits_type::now()) {
            
                write_timer.expires_at(boost::posix_time::pos_infin);
                write_timer.async_wait(
                    std::bind(&AsyncSocket::checkWriteDeadline, this));
            }
        } else {
            close();
        }
    } else {
        close();
    }
}
