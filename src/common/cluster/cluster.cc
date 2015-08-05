#include "cluster.h"

#include "log/logger.h"
#include "common_object.h"

#include "cluster_actor.h"
#include "cluster_node_delegate.h"
#include "network/websocket/client/handshake_helper.h"

namespace {
    // todo setting
    const int TIMEOUT_MILLIS = 60 * 1000;

    //
    const char* WS_CLUSTER_PATH = "/";
    const char* CLUSTER_PROTOCOL_NAME = "cluster";
};

void Cluster::addNode(long node_id, 
    const std::string& host, unsigned short port)
{
    try {
        std::string a_host = host;
        Logger::log("add new node");

        task_comm->postMaster([this, node_id, a_host, port]() {
            client::HandShakeRequest::ptr hs_req = 
                new client::HandShakeRequest;
            hs_req->path = WS_CLUSTER_PATH;
            hs_req->secret_key = 
                client::HandshakeHelper::createSecret();

            hs_req->protocols.insert(CLUSTER_PROTOCOL_NAME);

            //
            Logger::log("child node connect: %s:%d", a_host.c_str(), port);

            auto del = new ClusterNodeDelegate(node_id, task_comm, this);
            auto ws = client::WebsocketAsync::create(
                task_comm->getMasterIOService(), a_host, port, 
                TIMEOUT_MILLIS);
            
            ws->setDelegate(del);
            ws->connect(hs_req);

            addNewNodeInfo(node_id, a_host, port);
        });
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void Cluster::removeNode(long node_id)
{
    removeActiveNode(node_id);
    
    task_comm->postMaster([this, node_id]() {
        auto it = node_list.begin();
        for (; it != node_list.end(); ++it) {
            if ((*it)->node_id == node_id) {
                node_list.erase(it);
                break;
            }
        }
    });
}

void Cluster::setNodeRouter(NodeRouter::ptr router)
{
    task_comm->postMaster([this, router]() {
        node_router = router;
        node_router->setNodeList(&active_node_list);
    });
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
    task_comm->postMaster([this]() {
        auto am = CommonObject::getInstance()->getDownActorManager();
        
        if (watch_mode && node_list.size() > 0) {
            std::list<WsActor::const_ptr> remove_list;
            
            for (auto node_info: node_list) {
                am->getActorFromKey(node_info->node_id, 
                    [this, am, node_info, &remove_list](WsActor::const_ptr actor) {
                        if (!actor->isOpen()) {
                            Logger::log("process reconnect cluster");
                            
                            remove_list.push_back(actor);
                            
                            removeActiveNode(node_info->node_id);
                            addNode(node_info->node_id,
                                node_info->host, node_info->port);
                        }
                    }, [this, node_info]() {
                        // error callback
                        Logger::log("process connect cluster");
                        
                        removeActiveNode(node_info->node_id);
                        addNode(node_info->node_id,
                            node_info->host, node_info->port);
                    });
            }
            
            for (auto a: remove_list) {
                am->removeActor(a);
            }
        }
        
        am->update();
    });
}

bool Cluster::isWatchMode()
{
    return watch_mode;
}

void Cluster::setWatchMode(bool flag)
{
    Logger::debug("set watch mode");

    // TODO: atomic operator
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
    std::string a_host = host;
    Logger::log("add new node info1");
    
    task_comm->postMaster([this, a_host, node_id, port]() {
        bool found = false;
        Logger::log("add new node info2");

        for (auto node_info: node_list) {
            if (node_info->node_id == node_id) {
                found = true;
                break;
            }
        }

        if (!found) {
            ClusterNodeInfo::ptr node_info = new ClusterNodeInfo;
            node_info->node_id = node_id;
            node_info->host = a_host;
            node_info->port = port;
            
            node_list.push_back(node_info);
        }
    });
}

void Cluster::addActiveNode(long node_id,
    client::WebsocketAsync* websocket)
{
    task_comm->postMaster([this, node_id, websocket]() {
        bool found = false;

        for (auto n: active_node_list) {
            if (n->node_id == node_id) {
                n->websocket = websocket;
                
                found = true;
                break;
            }
        }

        if (!found) {
            auto node_info = new ActiveClusterNodeInfo;
            node_info->node_id = node_id;
            node_info->websocket = websocket;
            
            active_node_list.push_back(node_info);
        }
    
        node_router->setNodeList(&active_node_list);
    });
}

void Cluster::removeActiveNode(long node_id)
{
    Logger::log("remove active node");
    task_comm->postMaster([this, node_id]() {
        Logger::log("remove active node: Process");
        auto it = active_node_list.begin();
        for (; it != active_node_list.end(); ++it) {
            if ((*it)->node_id == node_id) {
                active_node_list.erase(it);
                break;
            }
        }
    });
}
