#include "glue_fb_glue.h"
#include "glue_fb.h"

#include "test_generated.h"

void glue_test(long actor_key, const char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!VerifyTestDataBuffer(verifier)) {
        return;
    }

    auto test_data = GetTestData(data);
    test(test_data->n(), test_data->str()->c_str(), actor_key);
}
