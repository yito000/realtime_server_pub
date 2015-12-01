#ifndef PLAYER_INPUT_UDP_PACKET_H
#define PLAYER_INPUT_UDP_PACKET_H

#include "battle/packet/battle_packet.h"
#include "battle/packet/character_input_packet_info.h"

#include "smart_ptr.hpp"

#include <string>
#include <list>
#include <boost/asio.hpp>

struct PlayerInputUdpPacket : public BattlePacket
{
    int seq_id;
    std::string battle_key;
    int player_id;
    boost::asio::ip::udp::endpoint ep;
    std::list<CharacterInputPacket::ptr> input_list;
    
    PlayerInputUdpPacket()
    {
        packet_type = (int)BattlePacketType::PLAYER_INPUT_UDP;
        
        seq_id = 0;
        player_id = -1;
    }
};

#endif
