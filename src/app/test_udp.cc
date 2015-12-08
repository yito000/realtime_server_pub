#include "test_udp.h"

#include "network/io/async_udp_socket.h"
#include "log/logger.h"

#include "common/router/command_dispatcher.h"

void TestUdp::receiveCallback(const boost::system::error_code& ec,
    std::size_t size, const UdpPacketArray& packet, 
    const boost::asio::ip::udp::endpoint& ep, 
    const AsyncUdpSocket* const socket)
{
    if (ec) {
        Logger::log("udp receive error: %s", ec.message().c_str());
        return;
    }
    
    Logger::debug("receive udp packet size=%d", size);
    Logger::debug("addr=%s, port=%d", ep.address().to_string().c_str(), ep.port());
    
    UdpCommandInfo::ptr udp_info = new UdpCommandInfo;
    udp_info->ep = ep;
    udp_info->packet.reserve(size);
    
    for (int i = 0; i < size; i++) {
        udp_info->packet.push_back(packet[i]);
    }
    
    CommandDispatcher::bulkDispatchUDP(udp_info);
}

void TestUdp::sendCallback(const boost::system::error_code& ec,
    std::size_t size, const AsyncUdpSocket* const socket)
{
    if (ec) {
        Logger::log("udp send error: %s", ec.message().c_str());
        return;
    }
    
    Logger::debug("send udp packet");
}
