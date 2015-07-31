#include "receive_uiid.h"

#include "protobuf/message/uiid.pb.h"

#include "common/network/websocket/ws_packet_data_helper.h"
#include "log/logger.h"

void ReceiveUiid::exec(const WsActor* actor,
    const std::string uiid)
{
    UiidPData u_data;
    u_data.set_value(uiid);

    int a_size = u_data.ByteSize();
    char* buf = new char[a_size];

    memset(buf, 0, a_size);
    u_data.SerializeToArray(buf, a_size);

    //
    using namespace boost::system;

    const int OP_CODE = 2;

    PacketData::ptr pd = 
        WsPacketDataHelper::buildPacket(OP_CODE, buf, a_size);

    pd->fin = true;
    pd->cont_frame = false;
    pd->packet_type = PACKET_TYPE_BINARY;

    delete[] buf;

    actor->write(pd, [](error_code ec) {
        if (!ec) {
            Logger::debug("receive ok!");
        } else {
            Logger::debug("receive ng...");
        }
    });
}
