#include "ssl_socket.h"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

SSLSocket::SSLSocket(const std::string& _hostname, unsigned int port) : 
    timer(io_service), error_after_close(true)
{
    ssl_context = new boost::asio::ssl::context(io_service, boost::asio::ssl::context::sslv23);
    socket = new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(io_service, *ssl_context);
    
    hostname = _hostname;
    protocol = boost::lexical_cast<std::string>(port);
    
    boost::asio::ip::tcp::no_delay option(true);
    socket->lowest_layer().set_option(option);
    
    checkDeadline();
}

SSLSocket::SSLSocket(const std::string& _hostname, const std::string& _protocol) : 
    timer(io_service), error_after_close(true)
{
    ssl_context = new boost::asio::ssl::context(io_service, boost::asio::ssl::context::sslv23);
    socket = new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(io_service, *ssl_context);
    
    hostname = _hostname;
    protocol = _protocol;
    
    boost::asio::ip::tcp::no_delay option(true);
    socket->lowest_layer().set_option(option);
    
    checkDeadline();
}

SSLSocket::~SSLSocket()
{
    close();
    
    delete ssl_context;
    delete socket;
}

void SSLSocket::setCAPath(const std::string& _ca_path)
{
    ca_path = _ca_path;
    ssl_context->set_verify_mode(boost::asio::ssl::context::verify_peer);
    ssl_context->load_verify_file(ca_path.c_str());
}

void SSLSocket::connect(boost::posix_time::time_duration timeout)
{
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(hostname.c_str(), protocol.c_str());
    boost::asio::ip::tcp::resolver::iterator endpoint_it = resolver.resolve(query);
    
    boost::asio::ip::tcp::resolver::iterator end;
    err = boost::asio::error::host_not_found;
    
    timer.expires_from_now(timeout);
    
    while (err && endpoint_it != end) {
        err = boost::asio::error::would_block;
        
        socket->lowest_layer().close();
        socket->lowest_layer().async_connect(*endpoint_it++, boost::bind(&SSLSocket::asyncCallback, this, _1, boost::ref(err)));
        
        do {
            io_service.run_one();
        } while (err == boost::asio::error::would_block);
    }
    
    if (err) {
        throw boost::system::system_error(err);
    }
    
    handshake(timeout);
}

void SSLSocket::close()
{
    if (socket->lowest_layer().is_open()) {
        socket->lowest_layer().close();
    }
}

void SSLSocket::write(const std::string& request, 
    boost::posix_time::time_duration timeout)
{
    err = boost::asio::error::would_block;

    timer.expires_from_now(timeout);
    
    std::ostream request_stream(&request_buffer);
    request_stream << request;
    
    boost::asio::async_write(*socket, request_buffer, 
        boost::bind(&SSLSocket::asyncCallback, this, _1, boost::ref(err)));
    
    do {
        io_service.run_one();
    } while (err == boost::asio::error::would_block);
    
    if (err) {
        throw boost::system::system_error(err);
    }
}

void SSLSocket::write(const ByteBuffer& request, 
    boost::posix_time::time_duration timeout)
{
    err = boost::asio::error::would_block;
    std::size_t bytes = 0;

    timer.expires_from_now(timeout);

    boost::asio::async_write(*socket, boost::asio::buffer(request),
        boost::bind(&SSLSocket::asyncCallback, this, _1, boost::ref(err)));

    do {
        io_service.run_one();
    } while (err == boost::asio::error::would_block);

    if (err) {
        throw boost::system::system_error(err);
    }
}

void SSLSocket::write_some(const std::string& request,
    boost::posix_time::time_duration timeout)
{
    err = boost::asio::error::would_block;
    std::size_t bytes = 0;

    timer.expires_from_now(timeout);

    ByteBuffer req;
    req.insert(req.end(), request.begin(), request.end());

    socket->async_write_some(boost::asio::buffer(req),
        boost::bind(&SSLSocket::asyncSomeCallback, this, _1, _2,
            boost::ref(err), boost::ref(bytes)));

    do {
        io_service.run_one();
    } while (err == boost::asio::error::would_block);

    if (err) {
        throw boost::system::system_error(err);
    }
}

void SSLSocket::write_some(const ByteBuffer& request,
    boost::posix_time::time_duration timeout)
{
    err = boost::asio::error::would_block;
    std::size_t bytes = 0;

    timer.expires_from_now(timeout);

    socket->async_write_some(boost::asio::buffer(request),
        boost::bind(&SSLSocket::asyncSomeCallback, this, _1, _2,
            boost::ref(err), boost::ref(bytes)));

    do {
        io_service.run_one();
    } while (err == boost::asio::error::would_block);

    if (err) {
        throw boost::system::system_error(err);
    }
}

bool SSLSocket::read(int at_least, ByteBuffer& byte_buffer, 
    boost::posix_time::time_duration timeout)
{
    err = boost::asio::error::would_block;

    timer.expires_from_now(timeout);
    
    boost::asio::async_read(*socket, response_buffer, 
        boost::asio::transfer_at_least(at_least), 
        boost::bind(&SSLSocket::asyncCallback, this, _1, boost::ref(err)));
    
    do {
        io_service.run_one();
    } while (err == boost::asio::error::would_block);
    
    if (err) {
        throw boost::system::system_error(err);
    }
    
    if (response_buffer.size() > 0) {
        size_t size = response_buffer.size();
        const char* data = 
            boost::asio::buffer_cast<const char*>(response_buffer.data());
        
        for (int i = 0; i < size; i++) {
            byte_buffer.push_back(data[i]);
        }
    }
    
    return err != boost::asio::error::eof;
}

void SSLSocket::read_until(const std::string delim, 
    ByteBuffer& byte_buffer, boost::posix_time::time_duration timeout)
{
    err = boost::asio::error::would_block;

    timer.expires_from_now(timeout);
    
    boost::asio::async_read_until(*socket, response_buffer, delim.c_str(), 
        boost::bind(&SSLSocket::asyncCallback, this, _1, boost::ref(err)));
    
    do {
        io_service.run_one();
    } while (err == boost::asio::error::would_block);
    
    if (err) {
        throw boost::system::system_error(err);
    }
    
    if (response_buffer.size() > 0) {
        size_t size = response_buffer.size();
        const char* data = 
            boost::asio::buffer_cast<const char*>(response_buffer.data());
        
        for (int i = 0; i < size; i++) {
            byte_buffer.push_back(data[i]);
        }
    }
}

bool SSLSocket::read_some(ByteBuffer& byte_buffer, 
    boost::posix_time::time_duration timeout)
{
    err = boost::asio::error::would_block;
    std::size_t bytes = 0;

    timer.expires_from_now(timeout);
    
    boost::array<char, 8192> array_buffer;
    
    socket->async_read_some(boost::asio::buffer(array_buffer), 
        boost::bind(&SSLSocket::asyncSomeCallback, this, _1, _2,
            boost::ref(err), boost::ref(bytes)));
    
    do {
        io_service.run_one();
    } while (err == boost::asio::error::would_block);
    
    if (err) {
        throw boost::system::system_error(err);
    }
    
    if (bytes > 0) {
        byte_buffer.reserve(byte_buffer.size() + bytes);
        
        for (int i = 0; i < bytes; i++) {
            byte_buffer.push_back(array_buffer[i]);
        }
    }
    
    return err != boost::asio::error::eof;
}

const std::string& SSLSocket::getHostName()
{
    return hostname;
}

const std::string& SSLSocket::getProtocol()
{
    return protocol;
}

const std::string& SSLSocket::getCAPath()
{
    return ca_path;
}

bool SSLSocket::isErrorAfterClose()
{
    return error_after_close;
}

void SSLSocket::setErrorAfterClose(bool b)
{
    error_after_close = b;
}

// private member function
void SSLSocket::handshake(boost::posix_time::time_duration timeout)
{
    boost::system::error_code err = boost::asio::error::would_block;
    timer.expires_from_now(timeout);
    
    socket->async_handshake(boost::asio::ssl::stream_base::client, 
        boost::bind(&SSLSocket::asyncCallback, this, _1, boost::ref(err)));
    
    do {
        io_service.run_one();
    } while (err == boost::asio::error::would_block);
    
    if (err) {
        throw boost::system::system_error(err);
    }
}

void SSLSocket::checkDeadline()
{
    if (timer.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
        if (error_after_close) {
            close();
        }

        err = boost::asio::error::timed_out;
        timer.expires_at(boost::posix_time::pos_infin);
    }
    
    timer.async_wait(boost::bind(&SSLSocket::checkDeadline, this));
}

void SSLSocket::asyncCallback(const boost::system::error_code& error_code, boost::system::error_code& ret)
{
    ret = error_code;
}

void SSLSocket::asyncSomeCallback(const boost::system::error_code& error_code, std::size_t bytes_transferred, 
    boost::system::error_code& ret, std::size_t& ret_bytes)
{
    ret = error_code;
    ret_bytes = bytes_transferred;
}
