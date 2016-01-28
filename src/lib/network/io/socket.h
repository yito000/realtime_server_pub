#ifndef SOCKET_H
#define SOCKET_H

#include "socket_inf.h"
#include <boost/asio.hpp>

#include "network/io/socket_buffer.h"

class Socket : public SocketInf
{
public:
    typedef boost::intrusive_ptr<Socket> ptr;

    Socket(const std::string& _hostname, unsigned int port);
    Socket(const std::string& _hostname, const std::string& _protocol);
    ~Socket();

    virtual void connect(boost::posix_time::time_duration timeout) override;
    virtual void close() override;

    virtual void write(const std::string& request, 
        boost::posix_time::time_duration timeout) override;
    virtual void write(const ByteBuffer& request, 
        boost::posix_time::time_duration timeout) override;
    virtual void write_some(const std::string& request, 
        boost::posix_time::time_duration time_out) override;
    virtual void write_some(const ByteBuffer& request, 
        boost::posix_time::time_duration timeout) override;

    virtual bool read(int at_least, ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout) override;
    virtual void read_until(const std::string delim, ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout) override;
    virtual bool read_some(ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout) override;

    virtual const std::string& getHostName() override;
    virtual const std::string& getProtocol() override;

    bool isErrorAfterClose();
    void setErrorAfterClose(bool b);

private:
    void checkDeadline();
    void asyncCallback(const boost::system::error_code& error_code, 
        boost::system::error_code& ret);
    void asyncSomeCallback(const boost::system::error_code& error_code, 
        std::size_t bytes_transferred, boost::system::error_code& ret, 
        std::size_t& ret_bytes);

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;
    boost::asio::deadline_timer timer;

    boost::asio::streambuf request_buffer;
    boost::asio::streambuf response_buffer;

    std::string hostname;
    std::string protocol;

    boost::system::error_code err;
    bool error_after_close;
};

#endif
