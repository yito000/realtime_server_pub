#ifndef TEST_UDP_H
#define TEST_UDP_H

#include "fw_env.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <functional>

class AsyncUdpSocket;

typedef boost::array<char, 65507> UdpPacketArray;

typedef std::function<void(const boost::system::error_code&,
    std::size_t, const UdpPacketArray&, 
    const boost::asio::ip::udp::endpoint&, const AsyncUdpSocket* const)> 
    UdpPacketReceiveCallback;
typedef std::function<void(const boost::system::error_code&,
    std::size_t, const AsyncUdpSocket* const)> 
    UdpPacketSendCallback;

class TestUdp : public SmartPtr<TestUdp>
{
public:
    typedef boost::intrusive_ptr<TestUdp> ptr;
    
    void receiveCallback(const boost::system::error_code& ec,
        std::size_t size, const UdpPacketArray& packet, 
        const boost::asio::ip::udp::endpoint& ep, 
        const AsyncUdpSocket* const socket);
    void sendCallback(const boost::system::error_code& ec,
        std::size_t size, const AsyncUdpSocket* const socket);
};

#endif
