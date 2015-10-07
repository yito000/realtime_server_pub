#include "receive_uiid.h"

#include "flatbuffers/table/uiid_generated.h"
#include "common/common_object.h"

#include "network/websocket/ws_packet_data_helper.h"
#include "log/logger.h"

void ReceiveUiid::exec(long actor_key,
    const std::string uiid)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto fb_uiid = fbb.CreateString(uiid);
    
    auto data = CreateUiidData(fbb, fb_uiid);
    fbb.Finish(data);
    
    auto a_size = fbb.GetSize();
    const unsigned char* buf = fbb.GetBufferPointer();

    //
    using namespace boost::system;

    const int OP_CODE = 2;

    PacketData::ptr pd = 
        WsPacketDataHelper::buildPacket(OP_CODE, buf, a_size);

    pd->fin = true;
    pd->cont_frame = false;
    pd->packet_type = PACKET_TYPE_BINARY;

    auto am = CommonObject::getInstance()->getUpActorManager();
    am->getActorFromKey(actor_key, [pd, uiid](WsActor::const_ptr actor) {
        actor->write(pd, [](error_code ec) {
            if (!ec) {
                Logger::debug("receive ok!");
            } else {
                Logger::debug("receive ng...");
            }
        });
    });
}
