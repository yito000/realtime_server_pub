#include "fb_master.h"

#include "app_director.h"
#include "common_object.h"
#include "network/websocket/ws_packet_data_helper.h"

#include "flatbuffers/table/uiid_generated.h"

#include "event/send_uiid.h"
#include "log/logger.h"

USING_NS

void FbMaster::sendWorker(long actor_key,
    const unsigned char* data, int size)
{
    flatbuffers::Verifier verifier((const uint8_t*)data, size);
    if (!VerifyUiidDataBuffer(verifier)) {
        Logger::log("invalid packet data");
        return;
    }
    
    const int OP_CODE = 1;

    PacketData::ptr packet = 
        WsPacketDataHelper::buildPacket(OP_CODE, data, size);
    packet->fin = true;
    packet->cont_frame = false;
    packet->packet_type = PACKET_TYPE_BINARY;

    using namespace boost::system;

    auto cluster = CommonObject::getInstance()->getCluster();

/*
    cluster->broadcast(packet, [](long node_id, error_code ec) {
        if (!ec) {
            Logger::debug("node%d send ok!", node_id);
        } else {
            Logger::debug("node%d send ng...", node_id);
        }
    });
*/

    cluster->sendRouter(packet, [](long node_id, error_code ec) {
        if (!ec) {
            Logger::debug("node%d send ok!", node_id);
        } else {
            Logger::debug("node%d send ng...", node_id);
        }
    });

    //
    AppDirector::getInstance()->postWorker(
        std::bind(SendUiid::exec, actor_key, "ok_dayo"));
}
