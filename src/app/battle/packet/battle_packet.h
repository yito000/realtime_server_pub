#ifndef BATTLE_PACKET_H
#define BATTLE_PACKET_H

enum class BattlePacketType {
    END_BATTLE,
    ERROR,
    JOIN,
    JOIN2,
    LEAVE,
    PLAYER_INPUT,
    PLAYER_INPUT_UDP,
    UDP_HANDSHAKE,
    UNKNOWN = -1
};

struct BattlePacket
{
    int packet_type;
    
    BattlePacket()
    {
        packet_type = (int)BattlePacketType::UNKNOWN;
    }
    
    virtual ~BattlePacket() {}
};

#endif
