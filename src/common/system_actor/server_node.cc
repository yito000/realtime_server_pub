#include "server_node.h"

#include "common_object.h"
#include "router/command_dispatcher.h"

#include "network/websocket/packet.h"
#include "log/logger.h"

BEGIN_NS

ServerNode::ServerNode(server::WebsocketContext* s) : 
    session(s),
    first_process(true),
    read_cnt(0),
    write_cnt(0)
{
    //
}

ServerNode::~ServerNode()
{
    if (session) {
        session->destroyAsync();
    }
}

void ServerNode::update() const
{
    //
}

void ServerNode::write(PacketData::ptr pd,
    server::SendCallback send_callback) const
{
    if (!session->isOpen()) {
        return;
    }
    
    write_cnt.fetch_add(1);
    session->write(pd, send_callback);
}

void ServerNode::close() const
{
    session->close();
}

void ServerNode::onStart() const
{
    read_cnt.fetch_add(1);
    first_process.exchange(false);
    
    session->read();
}

void ServerNode::onReceive(PacketData::ptr r_pd) const
{
    if (r_pd->packet_type == PACKET_TYPE_BINARY) {
        CommandDispatcher::bulkDispatch(getKey(), r_pd->data);
    } else {
        Logger::log("server node: warning packet type=%d, data size=%ld\n",
            r_pd->packet_type, r_pd->data.size());
    }
}

void ServerNode::onReceiveFinish(boost::system::error_code ec) const
{
    if (!ec && session->isOpen()) {
        session->read();
    } else {
        read_cnt.fetch_sub(1);
    }
}

void ServerNode::onSendFinish(boost::system::error_code ec) const
{
    write_cnt.fetch_sub(1);
}

void ServerNode::onError(Operation operation, 
    boost::system::error_code ec) const
{
    auto router = CommonObject::getInstance()->getServerErrorHandleRouter();
    auto callback = router->getCallback(ec.value());

    if (callback) {
        callback(this);
    } else {
        callback = router->getCallback(-1);
        if (callback) {
            callback(this);
        } else {
            if (session->isOpen()) {
                session->close();
            }
        }
    }
}

bool ServerNode::isActive() const
{
    return first_process || read_cnt > 0 || write_cnt > 0;
}

long ServerNode::getKey() const
{
    return session->getKey();
}

END_NS
