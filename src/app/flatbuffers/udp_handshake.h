#ifndef UDP_HANDSHAKE_H
#define UDP_HANDSHAKE_H

#include <boost/asio.hpp>

void glue_udp_handshake__(const boost::asio::ip::udp::endpoint& ep, const unsigned char* data, int size);

namespace DemoBattle {
void udp_handshake(const std::string& battle_key, int player_id, 
    const std::string& access_token, const boost::asio::ip::udp::endpoint& ep);
}

#endif
