#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "smart_ptr.hpp"

class AsyncUdpSocket;

typedef boost::array<char, 65507> UdpPacketArray;

typedef std::function<void(const boost::system::error_code&,
    std::size_t, const UdpPacketArray&, 
    const boost::asio::ip::udp::endpoint&, const AsyncUdpSocket* const)> 
    UdpPacketReceiveCallback;
typedef std::function<void(const boost::system::error_code&,
    std::size_t, const AsyncUdpSocket* const)> 
    UdpPacketSendCallback;

class UdpServer : public SmartPtr<UdpServer>
{
public:
    typedef boost::intrusive_ptr<UdpServer> ptr;
    
    enum AddrType {
        ADDR_V4 = 1,
        ADDR_V6
    };
    
    UdpServer(boost::asio::io_service& _ios,
        const AddrType addr_type, short port);
    ~UdpServer();
    
    void receive();
    void setReceiveCallback(UdpPacketReceiveCallback callback);
    void setSendCallback(UdpPacketSendCallback callback);
    
    bool isEnd() const;
    void setEndFlag(bool b);
    
private:
    boost::asio::io_service& ios;
    UdpPacketReceiveCallback receive_callback;
    UdpPacketSendCallback send_callback;
    
    AsyncUdpSocket* socket;
    bool end_flag;
};

#endif
