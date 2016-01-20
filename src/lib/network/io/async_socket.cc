#include "async_socket.h"

#include <boost/bind.hpp>

#include "log/logger.h"

AsyncSocket::AsyncSocket(boost::asio::io_service& _ios) :
    ios(_ios),
    ios_st(ios),
    socket(ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios)
{
    initTimer();
}

AsyncSocket::AsyncSocket(boost::asio::io_service& _ios,
    const std::string& _host, unsigned int _port) :
    ios(_ios),
    ios_st(ios),
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
    ios_st(ios),
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
    
    if (timeout == boost::posix_time::pos_infin) {
        connect_timer.expires_at(boost::posix_time::pos_infin);
    } else {
        connect_timer.expires_from_now(timeout);
    }
    
    auto err = boost::asio::error::host_not_found;
    connectInternal(err, endpoint_it, callback);
}

void AsyncSocket::close()
{
    if (socket.is_open()) {
        connect_timer.cancel();
        read_timer.cancel();
        write_timer.cancel();
        
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

void AsyncSocket::write(const ByteBuffer& buffer, 
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

std::string AsyncSocket::getHost()
{
    return host;
}

std::string AsyncSocket::getProtocol()
{
    return protocol;
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
    connect_timer.expires_at(boost::posix_time::pos_infin);
    read_timer.expires_at(boost::posix_time::pos_infin);
    write_timer.expires_at(boost::posix_time::pos_infin);
    
    connect_timer.async_wait(
        ios_st.wrap(boost::bind(&AsyncSocket::checkConnectDeadline, this, _1)));
    
    read_timer.async_wait(
        ios_st.wrap(boost::bind(&AsyncSocket::checkReadDeadline, this, _1)));

    write_timer.async_wait(
        ios_st.wrap(boost::bind(&AsyncSocket::checkWriteDeadline, this, _1)));
}

void AsyncSocket::connectInternal(boost::system::error_code err,
    boost::asio::ip::tcp::resolver::iterator endpoint_it,
    SocketConnectCallback callback)
{
    boost::asio::ip::tcp::resolver::iterator end;
    
    if (err && endpoint_it != end) {
        socket.async_connect(*endpoint_it++, 
            ios_st.wrap([this, endpoint_it, callback](boost::system::error_code ec) {
                if (ec) {
                    connectInternal(ec, endpoint_it, callback);
                } else {
                    boost::asio::ip::tcp::no_delay option(true);
                    socket.set_option(option);
                    
                    callback(ec);
                    
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
            }));
    } else {
        callback(err);
        
        connect_timer.expires_at(boost::posix_time::pos_infin);
    }
}

void AsyncSocket::checkConnectDeadline(boost::system::error_code ec)
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
    
    if (socket.is_open()) {
        connect_timer.async_wait(
            ios_st.wrap(boost::bind(&AsyncSocket::checkConnectDeadline, this, _1)));
    }
}

void AsyncSocket::checkReadDeadline(boost::system::error_code ec)
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
    
    if (socket.is_open()) {
        read_timer.async_wait(
            ios_st.wrap(boost::bind(&AsyncSocket::checkReadDeadline, this, _1)));
    }
}

void AsyncSocket::checkWriteDeadline(boost::system::error_code ec)
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
    
    if (socket.is_open()) {
        write_timer.async_wait(
            ios_st.wrap(boost::bind(&AsyncSocket::checkWriteDeadline, this, _1)));
    }
}
