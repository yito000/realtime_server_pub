#include "battle_player.h"

BattlePlayer::ptr BattlePlayer::create(
    int _player_id, long _actor_key, const std::string& _name,
    const std::list<BattleCharacter::ptr>& _characters)
{
    BattlePlayer::ptr inst = new BattlePlayer;
    if (!inst->init(_player_id, _actor_key, _name, _characters)) {
        return nullptr;
    }
    
    return inst;
}

int BattlePlayer::getPlayerId() const
{
    return player_id;
}

long BattlePlayer::getActorKey() const
{
    return actor_key;
}

std::string BattlePlayer::getName() const
{
    return name;
}

const std::list<BattleCharacter::ptr>& BattlePlayer::getCharacters()
{
    return characters;
}

void BattlePlayer::resetActoryKey(long _actor_key)
{
    actor_key = _actor_key;
}

bool BattlePlayer::isJoinedBattle() const
{
    return joined;
}

void BattlePlayer::setJoinFlag(bool b)
{
    joined = b;
}

// private member function
BattlePlayer::BattlePlayer() :
    player_id(0),
    actor_key(0),
    joined(false)
{
    //
}

bool BattlePlayer::init(int _player_id, long _actor_key, 
    const std::string& _name, const std::list<BattleCharacter::ptr>& _characters)
{
    player_id = _player_id;
    actor_key = _actor_key;
    name = _name;
    characters = _characters;
    
    return true;
}
