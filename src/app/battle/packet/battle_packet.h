#ifndef BATTLE_PACKET_H
#define BATTLE_PACKET_H

enum class BattlePacketType {
    ERROR,
    JOIN,
    LEAVE,
    PLAYER_INPUT,
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
