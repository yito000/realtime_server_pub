#include "glue_fb_glue.h"
#include "glue_fb.h"

#include "log/logger.h"

#include "test_generated.h"
#include "monster_generated.h"

void glue_test__(long actor_key, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!VerifyTestDataBuffer(verifier)) {
        return;
    }

    auto test_data = GetTestData(data);
    glue::test(test_data->n(), test_data->str()->c_str(), actor_key);
}
void glue_monster__(long actor_key, const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!MyGame::VerifyMonsterBuffer(verifier)) {
        return;
    }

    auto monster_data = MyGame::GetMonster(data);
    glue::monster(const_cast<MyGame::Vec3*>(monster_data->pos()), monster_data->mana(), monster_data->hp(), monster_data->name()->c_str(), const_cast<flatbuffers::Vector<unsigned char>*>(monster_data->inventory()), monster_data->color(), actor_key);
}
