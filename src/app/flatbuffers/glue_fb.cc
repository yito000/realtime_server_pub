#include "glue_fb.h"

#include "log/logger.h"

namespace glue {

void test(int n, const std::string& str, long actor_key)
{
    Logger::log("n=%d, str=%s", n, str.c_str());
}

void monster(MyGame::Vec3* pos, short mana, short hp, 
    const std::string& name, flatbuffers::Vector<uint8_t>* inventory, 
    MyGame::Color color, long actor_key)
{
    Logger::log("monster");
}

};
