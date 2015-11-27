#ifndef UDP_HANDSHAKE_PACKET_H
#define UDP_HANDSHAKE_PACKET_H

#include "battle/packet/battle_packet.h"

#include <string>
#include <boost/asio.hpp>

struct UdpHandshakePacket : public BattlePacket
{
    std::string battle_key;
    int player_id;
    boost::asio::ip::udp::endpoint ep;
    std::string access_token;
    
    UdpHandshakePacket()
    {
        packet_type = (int)BattlePacketType::UDP_HANDSHAKE;
        
        player_id = -1;
    }
};

#endif
