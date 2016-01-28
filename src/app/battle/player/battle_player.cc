#include "battle_player.h"

//USING_NS

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

BattleCharacter::ptr BattlePlayer::getCharacterFromId(int character_id)
{
    BattleCharacter::ptr ret;
    for (auto ch: characters) {
        if (ch->getCharacterId() == character_id) {
            return ch;
        }
    }
    
    return ret;
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

const boost::asio::ip::udp::endpoint& BattlePlayer::getUdpEndpoint() const
{
    return udp_endpoint;
}

void BattlePlayer::setUdpEndpoint(const boost::asio::ip::udp::endpoint& ep)
{
    udp_endpoint = ep;
    
    enable_udp_flag = true;
}

bool BattlePlayer::enableUdp() const
{
    return enable_udp_flag;
}

// private member function
BattlePlayer::BattlePlayer() :
    player_id(0),
    actor_key(0),
    joined(false),
    enable_udp_flag(false)
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
