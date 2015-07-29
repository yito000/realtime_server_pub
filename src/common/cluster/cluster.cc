#include "cluster.h"

#include "log/logger.h"
#include "common_object.h"

#include "cluster_actor.h"
#include "cluster_node_delegate.h"
#include "network/websocket/client/handshake_helper.h"

namespace {
    // todo setting
    const int TIMEOUT_MILLIS = 60 * 1000;
    const int RETRY_COUNT = 3;

    //
    const char* WS_CLUSTER_PATH = "/";
    const char* CLUSTER_PROTOCOL_NAME = "cluster";
};

void Cluster::addNode(long node_id, 
    const std::string& host, unsigned short port)
{
    try {
        long _node_id = node_id;
        std::string _host = host;
        unsigned short _port = port;

        task_comm->postMaster([this, _node_id, _host, _port]() {
            // todo
            client::HandShakeRequest::ptr hs_req = 
                new client::HandShakeRequest;
            hs_req->path = WS_CLUSTER_PATH;
            hs_req->secret_key = 
                client::HandshakeHelper::createSecret();

            hs_req->protocols.insert(CLUSTER_PROTOCOL_NAME);

            //
            Logger::debug("connect: %s:%d", _host.c_str(), _port);

            auto del = new ClusterNodeDelegate(_node_id, task_comm);
            auto ws = new client::WebsocketAsync(
                task_comm->getMasterIOService(), _host, _port, 
                TIMEOUT_MILLIS, RETRY_COUNT);

            ws->setDelegate(del);
            ws->connect(hs_req);

            addNewNodeInfo(_node_id, _host, _port);
        });
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void Cluster::removeNode(long node_id)
{
    auto am = CommonObject::getInstance()->getDownActorManager();

    am->removeActor(node_id);
}

void Cluster::cloneNodeList(NodeListCallback callback)
{
    if (!callback) {
        return;
    }

    std::list<ClusterNodeInfo::ptr> clone_list;

    for (auto node_info: node_list) {
        auto tmp_node = new ClusterNodeInfo(*node_info);

        clone_list.push_back(tmp_node);
    }

    callback(clone_list);
}

void Cluster::setNodeRouter(NodeRouter::ptr router)
{
    node_router = router;
    node_router->setNodeList(&node_list);
}

void Cluster::broadcast(PacketData::ptr pd, NodeSendCallback callback)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    using namespace boost::system;

    am->map([pd, callback](WsActor::const_ptr actor, 
        const ActorManager::ActorList& list) {

        if (actor) {
            actor->write(pd, [actor, callback](error_code ec) {
                long node_id = actor->getKey();

                callback(node_id, ec);
            });
        }
    });
}

void Cluster::sendOnce(long node_id, PacketData::ptr pd,
    NodeSendCallback callback)
{
    auto am = CommonObject::getInstance()->getDownActorManager();
    using namespace boost::system;

    am->getActorFromKey(node_id, 
        [pd, node_id, callback](WsActor::const_ptr actor) {

        if (actor) {
            actor->write(pd, [node_id, callback](error_code ec){
                callback(node_id, ec);
            });
        } else {
            callback(node_id, boost::asio::error::not_connected);
        }
    });
}

void Cluster::sendRouter(PacketData::ptr pd, NodeSendCallback callback)
{
    if (!node_router) {
        Logger::debug("warning: node router is not found");
        return;
    }

    task_comm->postMaster([this, pd, callback]() {
        if (node_router) {
            long node_id = node_router->getNode(pd);

            if (node_id != -1) {
                sendOnce(node_id, pd, callback);
            }
        }
    });
}

void Cluster::run()
{
    if (watch_mode && node_list.size() > 0) {
        Logger::debug("watch cluster node");

        auto am = CommonObject::getInstance()->getDownActorManager();

        for (auto node_info: node_list) {
            am->getActorFromKey(node_info->node_id, 
                [this](WsActor::const_ptr actor) {
                    //
                }, [this, node_info]() {
                    Logger::debug("add node");

                    this->addNode(node_info->node_id,
                        node_info->host, node_info->port);
                });
        }
    }
}

bool Cluster::isWatchMode()
{
    return watch_mode;
}

void Cluster::setWatchMode(bool flag)
{
    Logger::debug("set watch mode");

    // todo atomic operator
    watch_mode = flag;
}

// private member function
Cluster::Cluster(BidirectionalCommunicator::ptr t_comm) : 
    task_comm(t_comm), watch_mode(false)
{
    //
}

void Cluster::addNewNodeInfo(long node_id, 
    const std::string& host, unsigned short port)
{
    bool found = false;

    for (auto node_info: node_list) {
        if (node_info->node_id == node_id) {
            found = true;
            break;
        }
    }

    if (!found) {
        ClusterNodeInfo::ptr node_info = new ClusterNodeInfo;
        node_info->node_id = node_id;
        node_info->host = host;
        node_info->port = port;

        node_list.push_back(node_info);
    }
}

