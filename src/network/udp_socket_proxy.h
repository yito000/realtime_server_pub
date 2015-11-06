#ifndef UDP_SOCKET_PROXY_H
#define UDP_SOCKET_PROXY_H

#include "smart_ptr.hpp"

#include <list>
#include <functional>

#include "network/io/async_udp_socket.h"

class UdpSocketProxy : public SmartPtr<UdpSocketProxy>
{
public:
    enum AddressType {
        ipv4,
        ipv6
    };
    
    typedef boost::intrusive_ptr<UdpSocketProxy> ptr;
    
    static UdpSocketProxy* create(
        boost::asio::io_service& _ios, AddressType addr_type, 
        const std::string& host, unsigned short port);
    static UdpSocketProxy* createServer(
        boost::asio::io_service& _ios, AddressType addr_type, 
        unsigned short port);
    
    void run();
    
    const boost::asio::ip::udp::endpoint& getEndpoint() const;
    
    void setReceiveCallback(UdpReceiveCallback callback);
    void setSendCallback(UdpSendCallback callback);
    
    void send(const ByteBuffer& packet) const;
    void send(const ByteBuffer& packet,
        const boost::asio::ip::udp::endpoint& ep) const;
    
private:
    UdpSocketProxy(boost::asio::io_service& _ios);
    bool init(AddressType addr_type,
        const std::string& host, unsigned short port);
    bool initServer(AddressType addr_type,
        unsigned short port);
    
    boost::asio::ip::udp::endpoint endpoint;
    
    boost::asio::io_service& ios;
    
    AsyncUdpSocket* udp_socket;
    bool running;
    bool mode_server;
};

#endif
