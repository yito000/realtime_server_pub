#include "pb_ev_res_uiid.h"

#include "protobuf/message/uiid.pb.h"

#include "log/logger.h"

void PbEvResponseUiid::response(long actor_key,
    const unsigned char* data, int size)
{
    UiidPData uiid_data;
    uiid_data.ParseFromArray(data, size);

    auto uiid = uiid_data.value();

    Logger::debug("response ok: %s, data size=%d", uiid.c_str(), size);
}
