#include "node_session_delegate.h"

#include "common_object.h"
#include "actor/ws_actor.h"
#include "network/websocket/server/websocket_session.h"

#include "system_actor/server_node.h"

#include "log/logger.h"

NodeSessionDelegate::NodeSessionDelegate(BidirectionalCommunicator::ptr t_comm) :
    task_comm(t_comm)
{
    //
}

void NodeSessionDelegate::onStart(server::WebsocketSession* session)
{
    auto ws_actor = new ServerNode(session);
    auto am = CommonObject::getInstance()->getUpActorManager();

    auto key = reinterpret_cast<long>(session);

    am->addActor(key, ws_actor);
    am->getActorFromKey(key, [](WsActor::const_ptr actor) {
        actor->onStart();
    });
}

void NodeSessionDelegate::onReceive(
    server::WebsocketSession* session, PacketData::ptr r_pd)
{
    auto am = CommonObject::getInstance()->getUpActorManager();
    auto key = reinterpret_cast<long>(session);

    am->getActorFromKey(key, [r_pd](WsActor::const_ptr actor) {
        actor->onReceive(r_pd);
    });
}

void NodeSessionDelegate::onReceiveFinish(
    server::WebsocketSession* session)
{
    auto am = CommonObject::getInstance()->getUpActorManager();
    auto key = reinterpret_cast<long>(session);

    am->getActorFromKey(key, [](WsActor::const_ptr actor) {
        actor->onReceiveFinish();
    });
}

void NodeSessionDelegate::onSendFinish(
    server::WebsocketSession* session)
{
    auto am = CommonObject::getInstance()->getUpActorManager();
    auto key = reinterpret_cast<long>(session);

    am->getActorFromKey(key, [](WsActor::const_ptr actor) {
        actor->onSendFinish();
    });
}

void NodeSessionDelegate::onError(
    server::WebsocketSession* session, boost::system::error_code ec)
{
    auto am = CommonObject::getInstance()->getUpActorManager();
    auto key = reinterpret_cast<long>(session);

    am->getActorFromKey(key, [ec](WsActor::const_ptr actor) {
        actor->onError(ec);
    }, [session]() {
        // TODO: session crush
        session->destroyAsync();
    });
}
