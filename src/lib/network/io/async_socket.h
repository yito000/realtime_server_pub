#ifndef ASYNC_SOCKET_H
#define ASYNC_SOCKET_H

#include "async_socket_inf.h"

#include <string>

class AsyncSocket : public AsyncSocketInf
{
public:
    // server mode
    AsyncSocket(boost::asio::io_service& _ios);
    
    // client mode
    AsyncSocket(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned int _port);
    AsyncSocket(boost::asio::io_service& _ios, 
        const std::string& _host, const std::string& _protocol);
    
    ~AsyncSocket();
    
    virtual void connect(boost::posix_time::time_duration timeout, 
        SocketConnectCallback callback) override;
    virtual void close() override;
    
    virtual bool isOpen() override;
    
    virtual void read(boost::posix_time::time_duration timeout, 
        SocketReadCallback callback) override;
    virtual void write(const ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        SocketWriteCallback callback) override;
    
    virtual std::string getHost() override;
    virtual std::string getProtocol() override;
    
    virtual boost::asio::ip::tcp::socket& getDetail() override
    {
        return socket;
    }
    
    virtual void setConnectTimeoutCallback(SocketTimeoutCallback callback) override;
    virtual void setReadTimeoutCallback(SocketTimeoutCallback callback) override;
    virtual void setWriteTimeoutCallback(SocketTimeoutCallback callback) override;
    
private:
    void initTimer();
    void connectInternal(boost::system::error_code err,
        boost::asio::ip::tcp::resolver::iterator endpoint_it,
        SocketConnectCallback callback);
    
    void checkConnectDeadline();
    void checkReadDeadline();
    void checkWriteDeadline();
    
    std::string host;
    std::string protocol;

    boost::asio::io_service& ios;
    boost::asio::ip::tcp::socket socket;
    
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