#ifndef BATTLE_CHARACTER_H
#define BATTLE_CHARACTER_H

#include "smart_ptr.hpp"

#include <string>

class BattleCharacter : public SmartPtr<BattleCharacter>
{
public:
    typedef boost::intrusive_ptr<BattleCharacter> ptr;
    
    static BattleCharacter::ptr create(int _character_id,
        const std::string& _name);
    
    void regenHp(int value);
    void regenMana(int value);
    void damageHp(int value);
    void damageMana(int value);
    
    void update(float delta_time);
    bool isDead() const;
    
    int getCharacterId() const;
    std::string getCharacterName() const;
    
    int getBaseHp() const;
    int getBaseMana() const;
    int getPhysicalAttack() const;
    int getMagicalAttack() const;
    int getPhysicalDefense() const;
    int getMagicalDefense() const;
    int getSpeed() const;
    
    void setBaseHp(int value);
    void setBaseMana(int value);
    void setBasePhysicalAttack(int value);
    void setBaseMagicalAttack(int value);
    void setBasePhysicalDefense(int value);
    void setBaseMagicalDefense(int value);
    void setSpeed(int value);
    
    int getHp() const;
    int getMana() const;
    
    void setHp(int value);
    void setMana(int value);

private:
    BattleCharacter();
    bool init(int _character_id, const std::string& _name);
    
    int character_id;
    std::string name;
    
    int base_hp;
    int base_mana;
    int physical_attack;
    int magical_attack;
    int physical_defense;
    int magical_defense;
    int speed;
    
    int hp;
    int mana;
};

#endif
