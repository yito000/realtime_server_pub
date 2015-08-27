#ifndef ASYNC_SSL_SOCKET_H
#define ASYNC_SSL_SOCKET_H

#include "async_socket_inf.h"

#include <string>
#include <boost/asio/ssl.hpp>

class AsyncSSLSocket : public AsyncSocketInf
{
public:
    // server mode
    AsyncSSLSocket(boost::asio::io_service& _ios,
        boost::asio::ssl::context& _ssl_context);
    
    // client mode
    AsyncSSLSocket(boost::asio::io_service& _ios, 
        boost::asio::ssl::context& _ssl_context, 
        const std::string& _host, unsigned int _port);
    AsyncSSLSocket(boost::asio::io_service& _ios, 
        boost::asio::ssl::context& _ssl_context,
        const std::string& _host, const std::string& _protocol);
    
    ~AsyncSSLSocket();
    
    virtual void connect(boost::posix_time::time_duration timeout, 
        SocketConnectCallback callback) override;
    virtual void close() override;
    
    virtual bool isOpen() override;
    
    virtual void read(boost::posix_time::time_duration timeout, 
        SocketReadCallback callback) override;
    virtual void write(const ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        SocketWriteCallback callback) override;
    
    void handshake(boost::posix_time::time_duration timeout,
        SocketConnectCallback callback);
    
    virtual std::string getHost() override;
    virtual std::string getProtocol() override;
    
    virtual boost::asio::ip::tcp::socket& getDetail() override
    {
        return (boost::asio::ip::tcp::socket&)socket.lowest_layer();
    }
    
    virtual void setConnectTimeoutCallback(SocketTimeoutCallback callback) override;
    virtual void setReadTimeoutCallback(SocketTimeoutCallback callback) override;
    virtual void setWriteTimeoutCallback(SocketTimeoutCallback callback) override;
    
private:
    void initTimer();
    void connectInternal(boost::system::error_code err,
        boost::asio::ip::tcp::resolver::iterator endpoint_it,
        boost::posix_time::time_duration timeout,
        SocketConnectCallback callback);
    
    void checkConnectDeadline(boost::system::error_code ec);
    void checkReadDeadline(boost::system::error_code ec);
    void checkWriteDeadline(boost::system::error_code ec);
    
    std::string host;
    std::string protocol;
    bool server_mode;

    boost::asio::io_service& ios;
    boost::asio::io_service::strand ios_st;
    boost::asio::ssl::context& ssl_context;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
    
    boost::asio::deadline_timer connect_timer;
    boost::asio::deadline_timer read_timer;
    boost::asio::deadline_timer write_timer;
    
    SocketTimeoutCallback connect_timeout_func;
    SocketTimeoutCallback read_timeout_func;
    SocketTimeoutCallback write_timeout_func;

    enum { MAX_LENGTH = 8192 };
    char data[MAX_LENGTH];
};

#endif
