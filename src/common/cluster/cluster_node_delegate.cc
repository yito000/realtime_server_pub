#include "cluster_node_delegate.h"

#include "log/logger.h"

#include "common_object.h"
#include "actor/ws_actor.h"
#include "protobuf/pb_command_dispatcher.h"

#include "cluster/cluster.h"
#include "cluster/cluster_actor.h"

ClusterNodeDelegate::ClusterNodeDelegate(int _node_id, 
    BidirectionalCommunicator::ptr t_comm, Cluster::ptr _cluster) : 
    node_id(_node_id), task_comm(t_comm), cluster(_cluster)
{
    //
}

void ClusterNodeDelegate::onStart(client::WebsocketAsync* ws)
{
    ClusterActor::ptr ws_actor = new ClusterActor(node_id, ws);
    auto am = CommonObject::getInstance()->getDownActorManager();

    auto key = node_id;
    
    cluster->addActiveNode(node_id, ws);
    
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

void ClusterNodeDelegate::onReceiveFinish(client::WebsocketAsync* ws,
    boost::system::error_code ec)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    auto key = node_id;

    am->getActorFromKey(key, [ec](WsActor::const_ptr actor) {
        actor->onReceiveFinish(ec);
    });
}

void ClusterNodeDelegate::onSendFinish(client::WebsocketAsync* ws,
    boost::system::error_code ec)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    auto key = node_id;

    am->getActorFromKey(key, [ec](WsActor::const_ptr actor) {
        actor->onSendFinish(ec);
    });
}

void ClusterNodeDelegate::onError(client::WebsocketAsync* ws, 
    Operation operation, boost::system::error_code ec)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    auto key = node_id;

    am->getActorFromKey(key, [operation, ec](WsActor::const_ptr actor) {
        actor->onError((WsActor::Operation)operation, ec);
    }, [ws]() {
//        ws->close();
    });
}
