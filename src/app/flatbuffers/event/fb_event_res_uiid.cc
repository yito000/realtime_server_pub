#include "fb_event_res_uiid.h"

#include "flatbuffers/table/uiid_generated.h"

#include "log/logger.h"

void FbEvResponseUiid::response(long actor_key,
    const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    
    if (VerifyUiidDataBuffer(verifier)) {
        auto uiid_data = GetUiidData(data);
        auto uiid = uiid_data->value();

        Logger::debug("response ok: %s, data size=%d", uiid->c_str(), size);
    } else {
        Logger::log("response data NG");
    }
}
