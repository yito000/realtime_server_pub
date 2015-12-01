#ifndef BATTLE_INPUT_INFO_H
#define BATTLE_INPUT_INFO_H

#include "smart_ptr.hpp"

#include <string>
#include <vector>
#include <boost/asio.hpp>

struct BattleInputCommandDetail
{
    int character_id;
    int command_id;
    
    BattleInputCommandDetail() :
        character_id(-1),
        command_id(-1)
    {
        //
    }
    
    BattleInputCommandDetail(int _character_id, int _command_id) :
        character_id(_character_id),
        command_id(_command_id)
    {
        //
    }
};

struct BattleInputInfo : public SmartPtr<BattleInputInfo>
{
    typedef boost::intrusive_ptr<BattleInputInfo> ptr;
    
    std::string battle_key;
    int seq_id;
    int player_id;
    std::vector<BattleInputCommandDetail> list;
    long actor_key;
    
    BattleInputInfo()
    {
        seq_id = 0;
        player_id = -1;
        actor_key = -1;
    }
};

struct BattleInputInfoUdp : public SmartPtr<BattleInputInfoUdp>
{
    typedef boost::intrusive_ptr<BattleInputInfoUdp> ptr;
    
    std::string battle_key;
    int seq_id;
    int player_id;
    std::vector<BattleInputCommandDetail> list;
    boost::asio::ip::udp::endpoint ep;
    
    BattleInputInfoUdp()
    {
        seq_id = 0;
        player_id = -1;
    }
};

#endif
