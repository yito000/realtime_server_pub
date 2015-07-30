#ifndef SSL_SOCKET_H
#define SSL_SOCKET_H

#include "smart_ptr.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "socket_buffer.h"

class SSLSocket : public SmartPtr<SSLSocket>
{
public:
    typedef boost::intrusive_ptr<SSLSocket> ptr;

    SSLSocket(const std::string& _hostname, unsigned int port);
    SSLSocket(const std::string& _hostname, const std::string& _protocol);
    ~SSLSocket();

    void setCAPath(const std::string& _ca_path);
    void connect(boost::posix_time::time_duration timeout);
    void close();

    void write(const std::string& request, 
        boost::posix_time::time_duration timeout);
    void write(const ByteBuffer& request, 
        boost::posix_time::time_duration timeout);
    void write_some(const std::string& request, 
        boost::posix_time::time_duration time_out);
    void write_some(const ByteBuffer& request, 
        boost::posix_time::time_duration timeout);

    bool read(int at_least, ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout);
    void read_until(const std::string delim, ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout);
    bool read_some(ByteBuffer& byte_buffer, 
        boost::posix_time::time_duration timeout);

    const std::string& getHostName();
    const std::string& getProtocol();
    const std::string& getCAPath();

    bool isErrorAfterClose();
    void setErrorAfterClose(bool b);

private:
    void handshake(boost::posix_time::time_duration timeout);

    void checkDeadline();
    void asyncCallback(const boost::system::error_code& error_code, 
        boost::system::error_code& ret);
    void asyncSomeCallback(const boost::system::error_code& error_code, 
        std::size_t bytes_transferred, boost::system::error_code& ret, 
        std::size_t& ret_bytes);

    boost::asio::io_service io_service;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* socket;
    boost::asio::deadline_timer timer;

    boost::asio::streambuf request_buffer;
    boost::asio::streambuf response_buffer;

    std::string hostname;
    std::string protocol;

    boost::system::error_code err;
    bool error_after_close;

    boost::asio::ssl::context* ssl_context;
    std::string ca_path;
};

#endif
