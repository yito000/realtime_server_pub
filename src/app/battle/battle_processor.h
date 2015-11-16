#ifndef BATTLE_PROCESSOR_H
#define BATTLE_PROCESSOR_H

#include "smart_ptr.hpp"
#include "battle/packet/battle_packet.h"

#include <boost/lockfree/queue.hpp>

class ErrorPacket;
class JoinPacket;
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
    BattleProcessor(int queue_size);
    bool init();
    
    void dispatchPacket(BattlePacket* packet);
    
    void execError(ErrorPacket* packet);
    void execJoin(JoinPacket* packet);
    void execLeave(LeavePacket* packet);
    void execPlayerInput(PlayerInputPacket* packet);
    
    boost::lockfree::queue<BattlePacket*> packet_queue;
};

#endif
