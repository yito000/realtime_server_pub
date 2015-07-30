#include "cluster_actor.h"

#include "common_object.h"
#include "protobuf/pb_command_dispatcher.h"

#include "network/websocket/packet.h"
#include "log/logger.h"

ClusterActor::ClusterActor(int _node_id, client::WebsocketAsync* ws) :
    node_id(_node_id), websocket(ws)
{
    //
}

ClusterActor::~ClusterActor()
{
    if (websocket) {
        Logger::debug("destroy cluster actor");
        websocket->destroyAsync();
    }
}

void ClusterActor::update() const
{
    //
}

void ClusterActor::write(PacketData::ptr pd,
    client::SendCallback send_callback) const
{
    // todo helper class
    char mask_key[4] = {0x1, 0x2, 0x3, 0x4};

    websocket->write(pd, mask_key, send_callback);
}

void ClusterActor::onStart() const
{
    websocket->read();
}

void ClusterActor::onReceive(PacketData::ptr r_pd) const
{
    if (r_pd->packet_type == PACKET_TYPE_BINARY) {
        PbCommandDispatcher::bulkDispatch(this, r_pd->data);
    } else {
        Logger::debug("warning packet type=%d, data size=%ld\n",
            r_pd->packet_type, r_pd->data.size());
    }
}

void ClusterActor::onReceiveFinish() const
{
    websocket->read();
}

void ClusterActor::onSendFinish() const
{
    //
}

void ClusterActor::onError(boost::system::error_code ec) const
{
    auto router = CommonObject::getInstance()->getErrorHandleRouter();
    auto callback = router->getCallback(ec.value());

    if (callback) {
        callback(this);
    } else {
        if (websocket->isOpen()) {
            websocket->close();
        }
    }
}

long ClusterActor::getKey() const
{
    return node_id;
}
