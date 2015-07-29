#include "server_node.h"

#include "common_object.h"
#include "protobuf/pb_command_dispatcher.h"

#include "network/websocket/packet.h"
#include "log/logger.h"

ServerNode::ServerNode(server::WebsocketContext* s) : session(s)
{
    //
}

ServerNode::~ServerNode()
{
    session->destroyAsync();
}

void ServerNode::update() const
{
    //
}

void ServerNode::write(PacketData::ptr pd,
    server::SendCallback send_callback) const
{
    session->write(pd, send_callback);
}

void ServerNode::onStart() const
{
    session->read();
}

void ServerNode::onReceive(PacketData::ptr r_pd) const
{
    if (r_pd->packet_type == PACKET_TYPE_BINARY) {
        PbCommandDispatcher::bulkDispatch(this, r_pd->data);
    } else {
        Logger::debug("warning packet type=%d, data size=%ld\n",
            r_pd->packet_type, r_pd->data.size());
    }
}

void ServerNode::onReceiveFinish() const
{
    session->read();
}

void ServerNode::onSendFinish() const
{
    //
}

void ServerNode::onError(boost::system::error_code ec) const
{
    auto router = CommonObject::getInstance()->getErrorHandleRouter();
    auto callback = router->getCallback(ec.value());

    if (callback) {
        callback(this);
    } else {
        if (session->isOpen()) {
            session->close();
        }
    }
}

long ServerNode::getKey() const
{
    return reinterpret_cast<long>(session);
}

