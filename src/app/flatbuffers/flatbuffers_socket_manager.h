#ifndef FLATBUFFERS_SOCKET_MANAGER_H
#define FLATBUFFERS_SOCKET_MANAGER_H

#include "fw_env.h"

#include <string>
#include <boost/asio.hpp>

#include "network/websocket/packet.h"

BEGIN_NS

class UdpSocketProxy;

END_NS

class FlatbuffersSocketManager : public SmartPtr<FlatbuffersSocketManager>
{
public:
    typedef boost::intrusive_ptr<FlatbuffersSocketManager> ptr;
    
    static FlatbuffersSocketManager::ptr getInstance();
    
    void writeTCPSocket(fwx::PacketData::ptr packet, long actor_key);
    
    fwx::UdpSocketProxy* getUdpSocket() const;
    void setUdpSocket(fwx::UdpSocketProxy* socket);
    
private:
    FlatbuffersSocketManager();
    
    fwx::UdpSocketProxy* udp_socket;
};

#endif
