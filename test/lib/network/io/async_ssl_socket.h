#ifndef ASYNC_SSL_SOCKET_H
#define ASYNC_SSL_SOCKET_H

#include "async_socket_inf.h"

#include <string>
#include <boost/asio/ssl.hpp>

class AsyncSSLSocket : public AsyncSocketInf
{
public:
    enum class VerifyMode {
        VERIFY_NODE = boost::asio::ssl::context::verify_none,
        VERIFY_PEER = boost::asio::ssl::context::verify_peer,
        VERIFY_CLIENT_ONCE = boost::asio::ssl::context::verify_client_once,
        VERIFY_PEER_FAIL_IF_NO_PEER_CERT = boost::asio::ssl::context::verify_fail_if_no_peer_cert
    };
    
    // server mode
    AsyncSSLSocket(boost::asio::io_service& _ios);
    
    // client mode
    AsyncSSLSocket(boost::asio::io_service& _ios, 
        const std::string& _host, unsigned int _port);
    AsyncSSLSocket(boost::asio::io_service& _ios, 
        const std::string& _host, const std::string& _protocol);
    
    ~AsyncSSLSocket();
    
    // server mode
    void setVerifyMode(int verify_mode);
    void loadCertificate(const ByteBuffer& buffer);
    void loadPrivateKey(const ByteBuffer& buffer);
    void loadTempDHParam(const ByteBuffer& buffer);
    
    // client mode
    void loadVerifyCertificate(const ByteBuffer& buffer);
    
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
        return (boost::asio::ip::tcp::socket&)socket->lowest_layer();
    }
    
    virtual void setConnectTimeoutCallback(SocketTimeoutCallback callback) override;
    virtual void setReadTimeoutCallback(SocketTimeoutCallback callback) override;
    virtual void setWriteTimeoutCallback(SocketTimeoutCallback callback) override;
    
    std::string getPassword();
    void setPassword(const std::string& value);
    
private:
    void initTimer();
    void connectInternal(boost::system::error_code err,
        boost::asio::ip::tcp::resolver::iterator endpoint_it,
        boost::posix_time::time_duration timeout,
        SocketConnectCallback callback);
    
    void checkConnectDeadline();
    void checkReadDeadline();
    void checkWriteDeadline();
    
    bool verifyCertificate(bool preverified, 
        boost::asio::ssl::verify_context& ctx);
    std::string passwordCallback() const;
    
    std::string host;
    std::string protocol;
    bool server_mode;
    std::string password_string;

    boost::asio::io_service& ios;
    boost::asio::io_service::strand ios_st;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* socket;
    boost::asio::ssl::context* ssl_context;
    
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
