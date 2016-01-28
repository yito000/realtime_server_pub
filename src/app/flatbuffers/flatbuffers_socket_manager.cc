#include "flatbuffers_socket_manager.h"

#include "common_object.h"
#include "network/udp_socket_proxy.h"

#include "log/logger.h"

USING_NS

FlatbuffersSocketManager::ptr FlatbuffersSocketManager::getInstance()
{
    static FlatbuffersSocketManager::ptr inst = new FlatbuffersSocketManager;
    
    return inst;
}

void FlatbuffersSocketManager::writeTCPSocket(PacketData::ptr packet, long actor_key)
{
    auto am = CommonObject::getInstance()->getUserActorManager();
    am->getActorFromKey(actor_key, [packet](WsActor::const_ptr actor) {
        actor->write(packet, [](boost::system::error_code ec) {
            if (!ec) {
                Logger::debug("send ok!");
            } else {
                Logger::debug("send ng...");
            }
        });
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
