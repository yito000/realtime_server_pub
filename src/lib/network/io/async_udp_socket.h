#ifndef ASYNC_UDP_SOCKET_H
#define ASYNC_UDP_SOCKET_H

#include "socket_buffer.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include <functional>

class AsyncUdpSocket;

typedef boost::array<char, 65507> UdpPacketArray;

typedef std::function<void(const boost::system::error_code&,
    std::size_t, const UdpPacketArray&, 
    const boost::asio::ip::udp::endpoint&, const AsyncUdpSocket* const)> 
    UdpReceiveCallback;
typedef std::function<void(const boost::system::error_code&,
    std::size_t, const AsyncUdpSocket* const)> UdpSendCallback;

class AsyncUdpSocket
{
public:
    // server mode
    AsyncUdpSocket(boost::asio::io_service& io_service, 
        bool _ipv6_mode, int port);

    // client mode
    AsyncUdpSocket(boost::asio::io_service& io_service,
        bool _ipv6_mode, const boost::asio::ip::udp::endpoint& endpoint);

    ~AsyncUdpSocket();

    void setReceiveCallback(UdpReceiveCallback callback);
    void setSendCallback(UdpSendCallback callback);

    void send(const ByteBuffer& packet, 
        const boost::asio::ip::udp::endpoint& endpoint) const;
    void send(const ByteBuffer& packet,
        const std::string& host, int port) const;

    void receive();

private:
    void handleReceive(const boost::system::error_code& error,
        std::size_t packet_size) const;
    void handleSend(const boost::system::error_code& error,
        std::size_t packet_size) const;

    bool server_mode;
    bool ipv6_mode;

    boost::asio::io_service& ios;
    mutable boost::asio::io_service::strand ios_st;
    mutable boost::asio::ip::udp::socket socket;

    boost::asio::ip::udp::endpoint received_endpoint;

    UdpReceiveCallback receive_callback;
    UdpSendCallback send_callback;

    UdpPacketArray buf;
};

#endif
