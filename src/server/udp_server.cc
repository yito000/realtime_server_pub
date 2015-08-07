#include "udp_server.h"

#include "network/io/async_udp_socket.h"

UdpServer::UdpServer(boost::asio::io_service& _ios,
    const AddrType addr_type, short port) :
    ios(_ios),
    end_flag(false)
{
    bool ipv6_mode = addr_type == AddrType::ADDR_V6;
    
    socket = new AsyncUdpSocket(ios, ipv6_mode, port);
    socket->setReceiveCallback([this](const boost::system::error_code& ec,
        std::size_t size, const UdpPacketArray& packet, 
        const boost::asio::ip::udp::endpoint& ep, 
        const AsyncUdpSocket* const socket) {
        
        this->receive();
    });
}

UdpServer::~UdpServer()
{
    if (socket) {
        delete socket;
    }
}

void UdpServer::receive()
{
    if (end_flag) {
        return;
    }
    
    socket->receive();
}

void UdpServer::setReceiveCallback(UdpPacketReceiveCallback callback)
{
    receive_callback = callback;
    socket->setReceiveCallback([this](const boost::system::error_code& ec,
        std::size_t size, const UdpPacketArray& packet, 
        const boost::asio::ip::udp::endpoint& ep, 
        const AsyncUdpSocket* const socket) {
        
        if (receive_callback) {
            receive_callback(ec, size, packet, ep, socket);
        }
        
        this->receive();
    });
}

bool UdpServer::isEnd() const
{
    return end_flag;
}

void UdpServer::setEndFlag(bool b)
{
    end_flag = b;
}
