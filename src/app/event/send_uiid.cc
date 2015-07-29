#include "send_uiid.h"

#include "log/logger.h"

void SendUiid::exec(const WsActor* user_client,
    const std::string uiid)
{
    Logger::debug("uiid is %s", uiid.c_str());

    //
    using namespace boost::system;

    PacketData::ptr pd = new PacketData;
    pd->fin = true;
    pd->packet_type = PACKET_TYPE_TEXT;

    pd->data.insert(pd->data.end(), uiid.begin(), uiid.end());

    user_client->write(pd, [](error_code ec) {
        if (!ec) {
            Logger::debug("send ok!");
        } else {
            Logger::debug("send ng...");
        }
    });
}

