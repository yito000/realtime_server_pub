#include "cluster_actor.h"

#include "common_object.h"
#include "router/command_dispatcher.h"

#include "network/websocket/ws_packet_data_helper.h"
#include "network/websocket/packet.h"
#include "log/logger.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

namespace {
    // TODO
    auto uuid_gen = boost::uuids::random_generator();
};

ClusterActor::ClusterActor(int _node_id, client::WebsocketAsync* ws) :
    node_id(_node_id), 
    websocket(ws),
    first_process(true),
    read_cnt(0),
    write_cnt(0)
{
    uuid = boost::lexical_cast<std::string>(boost::uuids::uuid(uuid_gen()));
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
    if (!websocket->isOpen()) {
        return;
    }
    
    auto mask_key = WsPacketDataHelper::buildMaskKey();
    
    write_cnt.fetch_add(1);
    websocket->write(pd, mask_key, send_callback);
}

void ClusterActor::close() const
{
    websocket->close();
}

void ClusterActor::onStart() const
{
    read_cnt.fetch_add(1);
    first_process.exchange(false);
    
    websocket->read();
}

void ClusterActor::onReceive(PacketData::ptr r_pd) const
{
    if (r_pd->packet_type == PACKET_TYPE_BINARY) {
        CommandDispatcher::bulkDispatch(getKey(), r_pd->data);
    } else {
        Logger::debug("cluster actor: warning packet type=%d, data size=%ld\n",
            r_pd->packet_type, r_pd->data.size());
    }
}

void ClusterActor::onReceiveFinish(boost::system::error_code ec) const
{
    if (!ec && websocket->isOpen()) {
        websocket->read();
    } else {
        read_cnt.fetch_sub(1);
    }
}

void ClusterActor::onSendFinish(boost::system::error_code ec) const
{
    write_cnt.fetch_sub(1);
}

void ClusterActor::onError(Operation operation, 
    boost::system::error_code ec) const
{
    auto router = CommonObject::getInstance()->getClusterErrorHandleRouter();
    auto callback = router->getCallback(ec.value());

    if (callback) {
        callback(this);
    } else {
        callback = router->getCallback(-1);
        if (callback) {
            callback(this);
        } else {
            if (websocket->isOpen()) {
                websocket->close();
            }
        }
    }
}

bool ClusterActor::isActive() const
{
    return first_process || read_cnt > 0 || write_cnt > 0;
}

long ClusterActor::getKey() const
{
    return node_id;
}
