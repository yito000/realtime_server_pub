#ifndef FLATBUFFERS_SOCKET_MANAGER_H
#define FLATBUFFERS_SOCKET_MANAGER_H

#include "smart_ptr.hpp"

#include <string>
#include <boost/asio.hpp>

#include "network/websocket/packet.h"

class UdpSocketProxy;

class FlatbuffersSocketManager : public SmartPtr<FlatbuffersSocketManager>
{
public:
    typedef boost::intrusive_ptr<FlatbuffersSocketManager> ptr;
    
    static FlatbuffersSocketManager::ptr getInstance();
    
    void writeTCPSocket(PacketData::ptr packet);
    
    UdpSocketProxy* getUdpSocket() const;
    void setUdpSocket(UdpSocketProxy* socket);
    
private:
    FlatbuffersSocketManager();
    
    UdpSocketProxy* udp_socket;
};

#endif
