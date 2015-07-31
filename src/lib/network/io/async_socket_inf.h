#ifndef ASYNC_SOCKET_INF_H
#define ASYNC_SOCKET_INF_H

#include "smart_ptr.hpp"

#include <boost/asio.hpp>
#include <functional>

#include "socket_buffer.h"

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
    
    virtual void setConnectTimeoutCallback(SocketTimeoutCallback callback) = 0;
    virtual void setReadTimeoutCallback(SocketTimeoutCallback callback) = 0;
    virtual void setWriteTimeoutCallback(SocketTimeoutCallback callback) = 0;
};

#endif
