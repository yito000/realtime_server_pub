#include "app_session_delegate.h"

#include "common_object.h"
#include "actor/ws_actor.h"
#include "network/websocket/server/websocket_session.h"

#include "user_actor/user_client.h"
#include "log/logger.h"

BEGIN_NS

AppSessionDelegate::AppSessionDelegate(BidirectionalCommunicator::ptr t_comm) : 
    task_comm(t_comm)
{
    //
}

void AppSessionDelegate::onStart(server::WebsocketSession* session)
{
    UserClient::ptr ws_actor = new UserClient(session);
    auto am = CommonObject::getInstance()->getUserActorManager();

    auto key = session->getKey();

    am->addActor(key, ws_actor);
    am->getActorFromKey(key, [](WsActor::const_ptr actor) {
        actor->onStart();
    });
}

void AppSessionDelegate::onReceive(
    server::WebsocketSession* session, PacketData::ptr r_pd)
{
    auto am = CommonObject::getInstance()->getUserActorManager();
    auto key = session->getKey();

    am->getActorFromKey(key, [r_pd](WsActor::const_ptr actor) {
        actor->onReceive(r_pd);
    });
}

void AppSessionDelegate::onReceiveFinish(
    server::WebsocketSession* session, boost::system::error_code ec)
{
    auto am = CommonObject::getInstance()->getUserActorManager();
    auto key = session->getKey();

    am->getActorFromKey(key, [ec](WsActor::const_ptr actor) {
        actor->onReceiveFinish(ec);
    });
}

void AppSessionDelegate::onSendFinish(server::WebsocketSession* session,
    boost::system::error_code ec)
{
    auto am = CommonObject::getInstance()->getUserActorManager();
    auto key = session->getKey();

    am->getActorFromKey(key, [ec](WsActor::const_ptr actor) {
        actor->onSendFinish(ec);
    });
}

void AppSessionDelegate::onError(server::WebsocketSession* session, 
    Operation operation, boost::system::error_code ec)
{
    if (operation == Operation::CONNECT) {
        session->destroyAsync();
        return;
    }
    
    auto am = CommonObject::getInstance()->getUserActorManager();
    auto key = session->getKey();

    am->getActorFromKey(key, [operation, ec](WsActor::const_ptr actor) {
        actor->onError((WsActor::Operation)operation, ec);
    }, [session]() {
//        session->close();
    });
}

END_NS
