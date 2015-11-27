#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include "smart_ptr.hpp"

#include <vector>
#include <map>
#include <boost/asio.hpp>

#include "battle/battle_processor.h"
#include "battle/battle_input_info.h"

class WsActor;

class BattleManager : public SmartPtr<BattleManager>
{
public:
    typedef boost::intrusive_ptr<BattleManager> ptr;
    
    static BattleManager::ptr getInstance();
    void initialize(int worker_thread);
    void endAllBattle();
    
    void joinPlayer(const std::string& battle_key, int player_id, 
        const std::string& access_token, long actor_key);
    void leavePlayer(const std::string& battle_key, int player_id, long actor_key);
    void endBattle(const std::string& battle_key);
    void playerInput(BattleInputInfo::ptr input);
    void playerInputUdp(BattleInputInfoUdp::ptr input);
    
    void udpHandshake(const std::string& battle_key, int player_id, 
        const std::string& access_token, const boost::asio::ip::udp::endpoint& ep);
    
    void addActorInfo(long actor_key, const std::string& battle_key, int player_id);
    void removeActorInfo(long actor_key, const std::string& battle_key);
    void receiveError(const WsActor* actor);
    
private:
    BattleManager();
    
    bool init_flag;
    std::vector<BattleProcessor::ptr> battle_processor_list;
    
    struct ActorInfo : public SmartPtr<ActorInfo>
    {
        typedef boost::intrusive_ptr<ActorInfo> ptr;
        
        long actor_key;
        std::string battle_key;
        int player_id;
        
        boost::asio::ip::udp::endpoint udp_endpoint;
        bool enable_udp;
        
        int thread_index;
        
        ActorInfo()
        {
            actor_key = 0;
            player_id = 0;
            thread_index = 0;
            
            enable_udp = false;
        }
    };
    
    std::map<long, ActorInfo::ptr> actor_map;
};

#endif
