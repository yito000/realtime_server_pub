#include "send_uiid.h"

#include "common/common_object.h"
#include "log/logger.h"

void SendUiid::exec(long actor_key, const std::string uiid)
{
    Logger::debug("uiid is %s", uiid.c_str());

    //
    using namespace boost::system;

    PacketData::ptr pd = new PacketData;
    pd->fin = true;
    pd->packet_type = PACKET_TYPE_TEXT;

    pd->data.insert(pd->data.end(), uiid.begin(), uiid.end());
    
    auto am = CommonObject::getInstance()->getDownActorManager();
    am->getActorFromKey(actor_key, [pd, uiid](WsActor::const_ptr actor) {
        actor->write(pd, [](error_code ec) {
            if (!ec) {
                Logger::debug("send ok!");
            } else {
                Logger::debug("send ng...");
            }
        });
    });
}
