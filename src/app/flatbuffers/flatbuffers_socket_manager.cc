#include "flatbuffers_socket_manager.h"

#include "common_object.h"
#include "network/udp_socket_proxy.h"

#include "log/logger.h"

FlatbuffersSocketManager::ptr FlatbuffersSocketManager::getInstance()
{
    static FlatbuffersSocketManager::ptr inst = new FlatbuffersSocketManager;
    
    return inst;
}

void FlatbuffersSocketManager::writeTCPSocket(PacketData::ptr packet)
{
    auto cluster = CommonObject::getInstance()->getCluster();
    cluster->sendRouter(packet, [](long node_id, boost::system::error_code ec) {
        // TODO: error callback
        if (!ec) {
            Logger::debug("node%d send ok!", node_id);
        } else {
            Logger::debug("node%d send ng...", node_id);
        }
    });
}

UdpSocketProxy* FlatbuffersSocketManager::getUdpSocket() const
{
    return udp_socket;
}

void FlatbuffersSocketManager::setUdpSocket(UdpSocketProxy* socket)
{
    udp_socket = socket;
}

// private member function
FlatbuffersSocketManager::FlatbuffersSocketManager() :
    udp_socket(nullptr)
{
    //
}
