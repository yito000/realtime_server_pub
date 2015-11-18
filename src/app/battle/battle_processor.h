#ifndef BATTLE_PROCESSOR_H
#define BATTLE_PROCESSOR_H

#include "smart_ptr.hpp"
#include "battle/packet/battle_packet.h"

#include <string>
#include <list>

#include <unordered_map>
#include <boost/lockfree/queue.hpp>

#include "battle/player/battle_player.h"

class EndBattlePacket;
class ErrorPacket;
class JoinPacket;
class Join2Packet;
class LeavePacket;
class PlayerInputPacket;

class BattleManager;

class BattleProcessor : public SmartPtr<BattleProcessor>
{
public:
    typedef boost::intrusive_ptr<BattleProcessor> ptr;
    
    static BattleProcessor::ptr create(BattleManager& _battle_manager, int queue_size);
    
    void runLoop();
    void endLoop();
    void pushPacket(BattlePacket* packet);
    
private:
    // TODO: GvG
    struct BattleInfo : public SmartPtr<BattleInfo>
    {
        typedef boost::intrusive_ptr<BattleInfo> ptr;
        
        std::string battle_key;
        BattlePlayer::ptr player1;
        BattlePlayer::ptr player2;
        
        BattleInfo()
        {
            //
        }
    };
    
    BattleProcessor(BattleManager& _battle_manager, int queue_size);
    bool init();
    
    void dispatchPacket(BattlePacket* packet);
    
    void execEndBattle(EndBattlePacket* packet);
    void execError(ErrorPacket* packet);
    void execJoin(JoinPacket* packet);
    void execJoin2(Join2Packet* packet);
    void execLeave(LeavePacket* packet);
    void execPlayerInput(PlayerInputPacket* packet);
    
    void startBattle(BattleInfo::ptr battle_info);
    void endBattle(BattleInfo::ptr battle_info);
    
    boost::lockfree::queue<BattlePacket*> packet_queue;
    std::unordered_map<std::string, BattleInfo::ptr> battle_list;
    
    BattleManager& battle_manager;
    bool end_flag;
};

#endif
