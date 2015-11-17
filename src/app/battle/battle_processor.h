#ifndef BATTLE_PROCESSOR_H
#define BATTLE_PROCESSOR_H

#include "smart_ptr.hpp"
#include "battle/packet/battle_packet.h"

#include <string>
#include <list>

#include <unordered_map>
#include <boost/lockfree/queue.hpp>

#include "battle/player/battle_player.h"

class ErrorPacket;
class JoinPacket;
class Join2Packet;
class LeavePacket;
class PlayerInputPacket;

class BattleProcessor : public SmartPtr<BattleProcessor>
{
public:
    typedef boost::intrusive_ptr<BattleProcessor> ptr;
    
    static BattleProcessor::ptr create(int queue_size);
    
    void runLoop();
    void pushPacket(BattlePacket* packet);
    
private:
    // TODO: GvG
    struct BattleInfo : public SmartPtr<BattleInfo>
    {
        typedef boost::intrusive_ptr<BattleInfo> ptr;
        
        BattlePlayer::ptr player1;
        BattlePlayer::ptr player2;
        
        BattleInfo()
        {
            //
        }
    };
    
    BattleProcessor(int queue_size);
    bool init();
    
    void dispatchPacket(BattlePacket* packet);
    
    void execError(ErrorPacket* packet);
    void execJoin(JoinPacket* packet);
    void execJoin2(Join2Packet* packet);
    void execLeave(LeavePacket* packet);
    void execPlayerInput(PlayerInputPacket* packet);
    
    void startBattle(BattleInfo::ptr battle_info);
    
    boost::lockfree::queue<BattlePacket*> packet_queue;
    std::unordered_map<std::string, BattleInfo::ptr> battle_list;
};

#endif
