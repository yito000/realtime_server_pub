#include "cluster_node_delegate.h"

#include "log/logger.h"

#include "common_object.h"
#include "actor/ws_actor.h"
#include "protobuf/pb_command_dispatcher.h"

#include "cluster/cluster_actor.h"

ClusterNodeDelegate::ClusterNodeDelegate(int _node_id, 
    BidirectionalCommunicator::ptr t_comm) : 
    node_id(_node_id), task_comm(t_comm)
{
    //
}

void ClusterNodeDelegate::onStart(client::WebsocketAsync* ws)
{
    auto ws_actor = new ClusterActor(node_id, ws);
    auto am = CommonObject::getInstance()->getDownActorManager();

    auto key = node_id;

    am->addActor(key, ws_actor);
    am->getActorFromKey(key, [](WsActor::const_ptr actor) {
        actor->onStart();
    });
}

void ClusterNodeDelegate::onReceive(
    client::WebsocketAsync* ws, PacketData::ptr pd)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    auto key = node_id;

    am->getActorFromKey(key, [pd](WsActor::const_ptr actor) {
        actor->onReceive(pd);
    });
}

void ClusterNodeDelegate::onReceiveFinish(client::WebsocketAsync* ws)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    auto key = node_id;

    am->getActorFromKey(key, [](WsActor::const_ptr actor) {
        actor->onReceiveFinish();
    });
}

void ClusterNodeDelegate::onSendFinish(client::WebsocketAsync* ws)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    auto key = node_id;

    am->getActorFromKey(key, [](WsActor::const_ptr actor) {
        actor->onSendFinish();
    });
}

void ClusterNodeDelegate::onError(
    client::WebsocketAsync* ws, boost::system::error_code ec)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    auto key = node_id;

    am->getActorFromKey(key, [ec](WsActor::const_ptr actor) {
        actor->onError(ec);
    });
}

