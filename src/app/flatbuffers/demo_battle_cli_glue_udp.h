#ifndef demo_battle_cli_glue_udp_H
#define demo_battle_cli_glue_udp_H

#include <boost/asio.hpp>

void glue_udp_input_command__(const boost::asio::ip::udp::endpoint& ep, const unsigned char* data, int size);
void glue_udp_battle_entry__(const boost::asio::ip::udp::endpoint& ep, const unsigned char* data, int size);
void glue_udp_battle_exit__(const boost::asio::ip::udp::endpoint& ep, const unsigned char* data, int size);

#endif
