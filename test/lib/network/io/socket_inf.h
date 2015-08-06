#ifndef SOCKET_INF_H
#define SOCKET_INF_H

#include "smart_ptr.hpp"

#include <string>
#include <boost/asio.hpp>

#include "socket_buffer.h"

class SocketInf : public SmartPtr<SocketInf>
{
public:
    typedef boost::intrusive_ptr<SocketInf> ptr;

    virtual ~SocketInf() {}

    virtual void connect(boost::posix_time::time_duration timeout) = 0;
    virtual void close() = 0;

    virtual void write(const std::string& request, 
        boost::posix_time::time_duration timeout) = 0;
    virtual void write(const ByteBuffer& request, 
        boost::posix_time::time_duration timeout) = 0;
    virtual void write_some(const std::string& request, 
        boost::posix_time::time_duration time_out) = 0;
    virtual void write_some(const ByteBuffer& request, 
        boost::posix_time::time_duration timeout) = 0;

    virtual bool read(int at_least, ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout) = 0;
    virtual void read_until(const std::string delim, ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout) = 0;
    virtual bool read_some(ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout) = 0;

    virtual const std::string& getHostName() = 0;
    virtual const std::string& getProtocol() = 0;
};

#endif
