#include "udp_server.h"

#include "network/io/async_udp_socket.h"
#include "network/udp_socket_proxy.h"
#include "flatbuffers_socket_manager.h"

#include "log/logger.h"

BEGIN_NS

UdpServer::UdpServer(boost::asio::io_service& _ios,
    const AddrType addr_type, short port) :
    ios(_ios)
{
    bool ipv6_mode = addr_type == AddrType::ADDR_V6;
    
    if (ipv6_mode) {
        Logger::debug("setup ipv6 udp server");
    } else {
        Logger::debug("setup ipv4 udp server");
    }
    
    socket = UdpSocketProxy::createServer(ios,
        ipv6_mode ? UdpSocketProxy::AddressType::ipv6 : UdpSocketProxy::AddressType::ipv4,
        port);
    
    FlatbuffersSocketManager::getInstance()->setUdpSocket(socket);
}

UdpServer::~UdpServer()
{
    if (socket) {
        delete socket;
    }
}

void UdpServer::start()
{
    socket->run();
}

void UdpServer::setReceiveCallback(UdpPacketReceiveCallback callback)
{
    receive_callback = callback;
    socket->setReceiveCallback(receive_callback);
}

void UdpServer::setSendCallback(UdpPacketSendCallback callback)
{
    send_callback = callback;
    socket->setSendCallback(send_callback);
}

END_NS
