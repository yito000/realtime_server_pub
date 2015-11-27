#include "udp_socket_proxy.h"

#include "atomic/atomic_operator.hpp"
#include <boost/thread.hpp>

#include "macros.h"
#include "log/logger.h"

UdpSocketProxy* UdpSocketProxy::create(
    boost::asio::io_service& _ios, AddressType addr_type,
    const std::string& host, unsigned short port)
{
    auto inst = new UdpSocketProxy(_ios);
    if (!inst->init(addr_type, host, port)) {
        delete inst;
        return nullptr;
    }
    
    return inst;
}

UdpSocketProxy* UdpSocketProxy::createServer(
    boost::asio::io_service& _ios, AddressType addr_type, 
    unsigned short port)
{
    auto inst = new UdpSocketProxy(_ios);
    if (!inst->initServer(addr_type, port)) {
        delete inst;
        return nullptr;
    }
    
    return inst;
}

void UdpSocketProxy::run()
{
    if (running) {
        return;
    }
    
    AtomicOperator<bool>::lock_test_and_set(&running, true);
    udp_socket->receive();
}

const boost::asio::ip::udp::endpoint& UdpSocketProxy::getEndpoint() const
{
    return endpoint;
}

void UdpSocketProxy::setReceiveCallback(UdpReceiveCallback callback)
{
    ios.dispatch([this, callback]() {
            udp_socket->setReceiveCallback([this, callback](
                const boost::system::error_code& ec,
                std::size_t size, const UdpPacketArray& packet, 
                const boost::asio::ip::udp::endpoint& ep, 
                const AsyncUdpSocket* const socket) {
                
                if (callback) {
                    callback(ec, size, packet, ep, socket);
                }
                
                udp_socket->receive();
            });
        });
}

void UdpSocketProxy::setSendCallback(UdpSendCallback callback)
{
    ios.dispatch([this, callback]() {
            udp_socket->setSendCallback(callback);
        });
}

void UdpSocketProxy::send(const ByteBuffer& packet) const
{
    ASSERT(!mode_server, "This method is Client Mode Only.");
    
    udp_socket->send(packet, endpoint);
}

void UdpSocketProxy::send(const ByteBuffer& packet,
    const boost::asio::ip::udp::endpoint& ep) const
{
    udp_socket->send(packet, ep);
}

// private member function
UdpSocketProxy::UdpSocketProxy(boost::asio::io_service& _ios) :
    ios(_ios),
    udp_socket(nullptr),
    running(false),
    mode_server(false)
{
    //
}

bool UdpSocketProxy::init(AddressType addr_type,
    const std::string& host, unsigned short port)
{
    auto ip_ver = boost::asio::ip::udp::v4();
    if (addr_type == UdpSocketProxy::AddressType::ipv6) {
        ip_ver = boost::asio::ip::udp::v6();
    }
    
    boost::asio::ip::udp::resolver resolver(ios);
    boost::asio::ip::udp::resolver::query query(
        ip_ver, host, std::to_string(port));
    
    try {
        endpoint = *resolver.resolve(query);
    } catch (std::exception& e) {
        Logger::log("%s", e.what());
        
        return false;
    }
    
    if (addr_type == UdpSocketProxy::AddressType::ipv6) {
        udp_socket = new AsyncUdpSocket(ios, true);
    } else {
        udp_socket = new AsyncUdpSocket(ios, false);
    }
    
    // set default callback
    udp_socket->setReceiveCallback([this](
            const boost::system::error_code& ec,
            std::size_t size, const UdpPacketArray& packet, 
            const boost::asio::ip::udp::endpoint& ep, 
            const AsyncUdpSocket* const socket) {
            
            udp_socket->receive();
        });
    
    return true;
}

bool UdpSocketProxy::initServer(AddressType addr_type,
    unsigned short port)
{
    auto ip_ver = boost::asio::ip::udp::v4();
    if (addr_type == UdpSocketProxy::AddressType::ipv6) {
        ip_ver = boost::asio::ip::udp::v6();
    }
    
    mode_server = true;
    
    if (addr_type == UdpSocketProxy::AddressType::ipv6) {
        udp_socket = new AsyncUdpSocket(ios, true, port);
    } else {
        udp_socket = new AsyncUdpSocket(ios, false, port);
    }
    
    // set default callback
    udp_socket->setReceiveCallback([this](
            const boost::system::error_code& ec,
            std::size_t size, const UdpPacketArray& packet, 
            const boost::asio::ip::udp::endpoint& ep, 
            const AsyncUdpSocket* const socket) {
            
            udp_socket->receive();
        });
    
    return true;
}
