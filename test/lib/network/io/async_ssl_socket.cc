#include "async_ssl_socket.h"

#include <boost/bind.hpp>
#include "log/logger.h"

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios) :
    ios(_ios),
    ios_st(ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(true)
{
    ssl_context = new boost::asio::ssl::context(ios, boost::asio::ssl::context::tlsv1);
    ssl_context->set_options(
        boost::asio::ssl::context::default_workarounds | 
        boost::asio::ssl::context::no_sslv2 |
        boost::asio::ssl::context::single_dh_use);
    
    ssl_context->set_password_callback(boost::bind(&AsyncSSLSocket::passwordCallback, this));
    socket = new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(ios, *ssl_context);
    
    initTimer();
}

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios,
    const std::string& _host, unsigned int _port) :
    ios(_ios),
    ios_st(ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(false)
{
    host = _host;
    protocol = std::to_string(_port);
    
    ssl_context = new boost::asio::ssl::context(ios, boost::asio::ssl::context::tlsv1);
    
    ssl_context->set_verify_callback(boost::bind(&AsyncSSLSocket::verifyCertificate, this, _1, _2));
    socket = new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(ios, *ssl_context);
    
    initTimer();
}

AsyncSSLSocket::AsyncSSLSocket(boost::asio::io_service& _ios,
    const std::string& _host, const std::string& _protocol) :
    ios(_ios),
    ios_st(ios),
    connect_timer(ios),
    read_timer(ios), 
    write_timer(ios),
    server_mode(false)
{
    host = _host;
    protocol = _protocol;
    
    ssl_context = new boost::asio::ssl::context(ios, boost::asio::ssl::context::tlsv1);
    
    ssl_context->set_verify_callback(boost::bind(&AsyncSSLSocket::verifyCertificate, this, _1, _2));
    socket = new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(ios, *ssl_context);
    
    initTimer();
}

AsyncSSLSocket::~AsyncSSLSocket()
{
    close();
    
    delete ssl_context;
    delete socket;
}

void AsyncSSLSocket::setVerifyMode(int verify_mode)
{
    if (!server_mode) {
        Logger::log("set verify mode: %d", verify_mode);
        ssl_context->set_verify_mode(verify_mode);
    }
}

void AsyncSSLSocket::loadCertificate(const ByteBuffer& buffer)
{
    if (server_mode) {
        Logger::log("load cert");
        
        // pem file only
//        ssl_context->use_certificate_chain(boost::asio::buffer(buffer));
//        ssl_context->use_certificate_chain_file("/Users/ito/qtproj/realtime_server/key/server.crt");
    }
}

void AsyncSSLSocket::loadPrivateKey(const ByteBuffer& buffer)
{
    if (server_mode) {
        Logger::log("load pkey");
        
        // pem file only
//        ssl_context->use_private_key(boost::asio::buffer(buffer),
//            boost::asio::ssl::context_base::pem);
        ssl_context->use_private_key_file("/Users/ito/qtproj/realtime_server/key/server.key",
            boost::asio::ssl::context_base::pem);
    }
}

void AsyncSSLSocket::loadTempDHParam(const ByteBuffer& buffer)
{
    if (server_mode) {
        Logger::log("load dh");
//        ssl_context->use_tmp_dh(boost::asio::buffer(buffer));
        ssl_context->use_tmp_dh_file("/Users/ito/qtproj/realtime_server/key/dh512.pem");
    }
}

void AsyncSSLSocket::loadVerifyCertificate(const ByteBuffer& buffer)
{
    if (!server_mode) {
        Logger::log("load verify file");
        
        // pem file only
//        ssl_context->add_certificate_authority(boost::asio::buffer(buffer));
        ssl_context->load_verify_file("/Users/ito/qtproj/realtime_server/key/server.crt");
    }
}

void AsyncSSLSocket::connect(boost::posix_time::time_duration timeout, 
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
    connectInternal(err, endpoint_it, timeout, callback);
}

void AsyncSSLSocket::close()
{
    ios.dispatch([this]() {
        if (socket->lowest_layer().is_open()) {
            connect_timer.cancel();
            read_timer.cancel();
            write_timer.cancel();
            
            socket->lowest_layer().close();
        }
    });
}

bool AsyncSSLSocket::isOpen()
{
    return socket->lowest_layer().is_open();
}

void AsyncSSLSocket::read(boost::posix_time::time_duration timeout, 
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
    
    socket->async_read_some(boost::asio::buffer(data, MAX_LENGTH), 
        boost::bind(func, _1, data, _2));
}

void AsyncSSLSocket::write(const ByteBuffer& buffer, 
    boost::posix_time::time_duration timeout, SocketWriteCallback callback)
{
    if (timeout == boost::posix_time::pos_infin) {
        write_timer.expires_at(boost::posix_time::pos_infin);
    } else {
        write_timer.expires_from_now(timeout);
    }
    
    socket->async_write_some(boost::asio::buffer(buffer),
        [this, callback](boost::system::error_code ec, std::size_t s) {
            if (callback) {
                callback(ec, s);
            }
            
            write_timer.expires_at(boost::posix_time::pos_infin);
        });
}

void AsyncSSLSocket::handshake(boost::posix_time::time_duration timeout,
    SocketConnectCallback callback)
{
    connect_timer.expires_from_now(timeout);
    
    if (server_mode) {
        Logger::log("handshake server");
        socket->async_handshake(boost::asio::ssl::stream_base::server,
            [this, callback](boost::system::error_code ec) {
                if (ec) {
                    Logger::log("error: %s", ec.message().c_str());
                    close();
                } else {
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
                
                callback(ec);
            });
    } else {
        socket->async_handshake(boost::asio::ssl::stream_base::client,
            [this, callback](boost::system::error_code ec) {
                if (ec) {
                    close();
                } else {
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
                
                callback(ec);
            });
    }
}

std::string AsyncSSLSocket::getHost()
{
    return host;
}

std::string AsyncSSLSocket::getProtocol()
{
    return protocol;
}

void AsyncSSLSocket::setConnectTimeoutCallback(SocketTimeoutCallback callback)
{
    connect_timeout_func = callback;
}

void AsyncSSLSocket::setReadTimeoutCallback(SocketTimeoutCallback callback)
{
    read_timeout_func = callback;
}

void AsyncSSLSocket::setWriteTimeoutCallback(SocketTimeoutCallback callback)
{
    write_timeout_func = callback;
}

std::string AsyncSSLSocket::getPassword()
{
    return password_string;
}

void AsyncSSLSocket::setPassword(const std::string& value)
{
    password_string = value;
}

// private member function
void AsyncSSLSocket::initTimer()
{
    connect_timer.expires_at(boost::posix_time::pos_infin);
    read_timer.expires_at(boost::posix_time::pos_infin);
    write_timer.expires_at(boost::posix_time::pos_infin);
    
    connect_timer.async_wait(
        ios_st.wrap(std::bind(&AsyncSSLSocket::checkConnectDeadline, this)));
    
    read_timer.async_wait(
        ios_st.wrap(std::bind(&AsyncSSLSocket::checkReadDeadline, this)));

    write_timer.async_wait(
        ios_st.wrap(std::bind(&AsyncSSLSocket::checkWriteDeadline, this)));
}

void AsyncSSLSocket::connectInternal(boost::system::error_code err,
    boost::asio::ip::tcp::resolver::iterator endpoint_it,
    boost::posix_time::time_duration timeout,
    SocketConnectCallback callback)
{
    boost::asio::ip::tcp::resolver::iterator end;
    
    if (err && endpoint_it != end) {
        Logger::log("connect");
        socket->lowest_layer().async_connect(*endpoint_it++, 
            [this, endpoint_it, timeout, callback](boost::system::error_code ec) {
                if (ec) {
                    Logger::log("error connect");
                    connectInternal(ec, endpoint_it, timeout, callback);
                    
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                } else {
                    Logger::log("handshake");
                    handshake(timeout, callback);
                    
                    connect_timer.expires_at(boost::posix_time::pos_infin);
                }
            });
    } else {
        callback(boost::asio::error::host_not_found);
    }
}

void AsyncSSLSocket::checkConnectDeadline()
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
    
    if (socket->lowest_layer().is_open()) {
        connect_timer.async_wait(
            ios_st.wrap(std::bind(&AsyncSSLSocket::checkConnectDeadline, this)));
    }
}

void AsyncSSLSocket::checkReadDeadline()
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
    
    if (socket->lowest_layer().is_open()) {
        read_timer.async_wait(
            ios_st.wrap(std::bind(&AsyncSSLSocket::checkReadDeadline, this)));
    }
}

void AsyncSSLSocket::checkWriteDeadline()
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
    
    if (socket->lowest_layer().is_open()) {
        write_timer.async_wait(
            ios_st.wrap(std::bind(&AsyncSSLSocket::checkWriteDeadline, this)));
    }
}

bool AsyncSSLSocket::verifyCertificate(bool preverified, 
    boost::asio::ssl::verify_context& ctx)
{
    Logger::log("verify callback");
    return preverified;
}

std::string AsyncSSLSocket::passwordCallback() const
{
    Logger::log("password");
    return password_string;
}
