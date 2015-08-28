#ifndef GLUE_FB_H
#define GLUE_FB_H

#include <string>

#include "monster_generated.h"

namespace glue {

void test(int n, const std::string& str, long actor_key = -1);
void monster(MyGame::Vec3* pos, short mana, short hp, const std::string& name, 
    flatbuffers::Vector<uint8_t>* inventory, MyGame::Color color, long actor_key = 1);

};

#endif
