#include "battle_character.h"

BattleCharacter::ptr BattleCharacter::create(int _character_id,
    const std::string& _name)
{
    BattleCharacter::ptr inst = new BattleCharacter;
    if (!inst->init(_character_id, _name)) {
        return nullptr;
    }
    
    return inst;
}

void BattleCharacter::regenHp(int value)
{
    hp += value;
    if (hp > base_hp) {
        hp = base_hp;
    }
}

void BattleCharacter::regenMana(int value)
{
    mana += value;
    if (mana > base_mana) {
        mana = base_mana;
    }
}

void BattleCharacter::damageHp(int value)
{
    hp -= value;
    if (hp < 0) {
        hp = 0;
    }
}

void BattleCharacter::damageMana(int value)
{
    mana -= value;
    if (mana < 0) {
        mana = 0;
    }
}

void BattleCharacter::update(float delta_time)
{
    //
}

bool BattleCharacter::isDead() const
{
    return hp <= 0;
}

int BattleCharacter::getCharacterId() const
{
    return character_id;
}

std::string BattleCharacter::getCharacterName() const
{
    return name;
}

int BattleCharacter::getBaseHp() const
{
    return base_hp;
}

int BattleCharacter::getBaseMana() const
{
    return base_mana;
}

int BattleCharacter::getPhysicalAttack() const
{
    return physical_attack;
}

int BattleCharacter::getMagicalAttack() const
{
    return magical_attack;
}

int BattleCharacter::getPhysicalDefense() const
{
    return physical_defense;
}

int BattleCharacter::getMagicalDefense() const
{
    return magical_defense;
}

int BattleCharacter::getSpeed() const
{
    return speed;
}

void BattleCharacter::setBaseHp(int value)
{
    base_hp = value;
}

void BattleCharacter::setBaseMana(int value)
{
    base_mana = value;
}

void BattleCharacter::setBasePhysicalAttack(int value)
{
    physical_attack = value;
}

void BattleCharacter::setBaseMagicalAttack(int value)
{
    magical_attack = value;
}

void BattleCharacter::setBasePhysicalDefense(int value)
{
    physical_defense = value;
}

void BattleCharacter::setBaseMagicalDefense(int value)
{
    magical_defense = value;
}

void BattleCharacter::setSpeed(int value)
{
    speed = value;
}

int BattleCharacter::getHp() const
{
    return hp;
}

int BattleCharacter::getMana() const
{
    return mana;
}

void BattleCharacter::setHp(int value)
{
    hp = value;
}

void BattleCharacter::setMana(int value)
{
    mana = value;
}

// private member function
BattleCharacter::BattleCharacter() :
    character_id(0),
    base_hp(0),
    base_mana(0),
    physical_attack(0),
    magical_attack(0),
    physical_defense(0),
    magical_defense(0),
    speed(0),
    hp(0),
    mana(0)
{
    //
}

bool BattleCharacter::init(int _character_id, const std::string& _name)
{
    if (_name == "") {
        return false;
    }
    
    character_id = _character_id;
    name = _name;
    
    return true;
}
