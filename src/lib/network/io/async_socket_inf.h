#ifndef ASYNC_SOCKET_INF_H
#define ASYNC_SOCKET_INF_H

#include "fw_env.h"

#include <string>
#include <boost/asio.hpp>
#include <functional>

#include "network/io/socket_buffer.h"

typedef std::function<void(boost::system::error_code)> SocketConnectCallback;
typedef std::function<void(boost::system::error_code, char*, std::size_t)> SocketReadCallback;
typedef std::function<void(boost::system::error_code, std::size_t)> SocketWriteCallback;

class AsyncSocketInf;

typedef std::function<bool(AsyncSocketInf&)> SocketTimeoutCallback;

class AsyncSocketInf : public SmartPtr<AsyncSocketInf>
{
public:
    typedef boost::intrusive_ptr<AsyncSocketInf> ptr;
    
    virtual void connect(boost::posix_time::time_duration timeout,
        SocketConnectCallback callback) = 0;
    virtual void close() = 0;
    
    virtual bool isOpen() = 0;
    
    virtual void read(boost::posix_time::time_duration timeout,
        SocketReadCallback callback) = 0;
    virtual void write(const ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        SocketWriteCallback callback) = 0;
    
    virtual std::string getHost() = 0;
    virtual std::string getProtocol() = 0;
    
    virtual boost::asio::ip::tcp::socket& getDetail() = 0;
    
    virtual void setConnectTimeoutCallback(SocketTimeoutCallback callback) = 0;
    virtual void setReadTimeoutCallback(SocketTimeoutCallback callback) = 0;
    virtual void setWriteTimeoutCallback(SocketTimeoutCallback callback) = 0;
};

#endif
