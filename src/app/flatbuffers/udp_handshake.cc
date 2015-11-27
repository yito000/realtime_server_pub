#include "udp_handshake.h"

#include "udp_handshake_generated.h"
#include "battle/battle_manager.h"

void glue_udp_handshake__(const boost::asio::ip::udp::endpoint& ep, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!DemoBattle::VerifyUdpHandshakeBuffer(verifier)) {
        return;
    }

    auto fb_data = DemoBattle::GetUdpHandshake(data);
    DemoBattle::udp_handshake(fb_data->battle_key()->c_str(), fb_data->player_id(), fb_data->access_token()->c_str(), ep);
}

namespace DemoBattle {

void udp_handshake(const std::string& battle_key, int player_id, 
    const std::string& access_token, const boost::asio::ip::udp::endpoint& ep)
{
    BattleManager::getInstance()->udpHandshake(battle_key, player_id, access_token, ep);
}

};
