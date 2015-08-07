#include "test_udp.h"

#include "network/io/async_udp_socket.h"
#include "log/logger.h"

void TestUdp::receiveCallback(const boost::system::error_code& ec,
    std::size_t size, const UdpPacketArray& packet, 
    const boost::asio::ip::udp::endpoint& ep, 
    const AsyncUdpSocket* const socket)
{
    Logger::log("receive udp packet");
    
    std::string s = "ok dayo!";
    std::vector<char> res_packet(s.begin(), s.end());
    
    socket->send(res_packet, ep);
}

void TestUdp::sendCallback(const boost::system::error_code& ec,
    std::size_t size, const AsyncUdpSocket* const socket)
{
    Logger::log("send udp packet");
}
