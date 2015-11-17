#ifndef BATTLE_PLAYER_H
#define BATTLE_PLAYER_H

#include "smart_ptr.hpp"
#include <list>

#include "battle/player/character/battle_character.h"

class BattlePlayer : public SmartPtr<BattlePlayer>
{
public:
    typedef boost::intrusive_ptr<BattlePlayer> ptr;
    
    static BattlePlayer::ptr create(int _player_id, 
        long actor_key, const std::string& _name, 
        const std::list<BattleCharacter::ptr>& _characters);
    
    int getPlayerId() const;
    long getActorKey() const;
    std::string getName() const;
    
    const std::list<BattleCharacter::ptr>& getCharacters();
    
    void resetActoryKey(long _actor_key);
    
    bool isJoinedBattle() const;
    void setJoinFlag(bool b);
    
private:
    BattlePlayer();
    bool init(int _player_id, long actor_key, const std::string& _name, 
        const std::list<BattleCharacter::ptr>& _characters);
    
    int player_id;
    long actor_key;
    std::string name;
    std::list<BattleCharacter::ptr> characters;
    
    bool joined;
};

#endif
