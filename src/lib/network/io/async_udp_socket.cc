#include "async_udp_socket.h"

#include "log/logger.h"

AsyncUdpSocket::AsyncUdpSocket(
    boost::asio::io_service& io_service, bool _ipv6_mode, int port) :
    ios(io_service),
    ios_st(ios), 
    server_mode(true),
    ipv6_mode(_ipv6_mode),
    socket(ios, boost::asio::ip::udp::endpoint(ipv6_mode ? 
        boost::asio::ip::udp::v6() : boost::asio::ip::udp::v4(), 
        port))
{
    //
}

AsyncUdpSocket::AsyncUdpSocket(
    boost::asio::io_service& io_service, bool _ipv6_mode,
    const boost::asio::ip::udp::endpoint& endpoint) :
    ios(io_service),
    ios_st(ios), 
    server_mode(false),
    ipv6_mode(_ipv6_mode),
    socket(ios, boost::asio::ip::udp::endpoint(ipv6_mode ?
		boost::asio::ip::udp::v6() : boost::asio::ip::udp::v4(), 0)),
    received_endpoint(endpoint)
{
    //
}

AsyncUdpSocket::~AsyncUdpSocket()
{
    if (socket.is_open()) {
        socket.close();
    }
}

void AsyncUdpSocket::setReceiveCallback(UdpReceiveCallback callback)
{
    receive_callback = callback;
}

void AsyncUdpSocket::setSendCallback(UdpSendCallback callback)
{
    send_callback = callback;
}

void AsyncUdpSocket::send(const ByteBuffer& packet, 
    const boost::asio::ip::udp::endpoint& endpoint) const
{
    socket.async_send_to(
        boost::asio::buffer(packet.data(), packet.size()), endpoint,
        ios_st.wrap(boost::bind(&AsyncUdpSocket::handleSend, this, _1, _2)));
}

void AsyncUdpSocket::send(const ByteBuffer& packet,
    const std::string& host, int port) const
{
    auto ip_ver = ipv6_mode ? 
        boost::asio::ip::udp::v6() : boost::asio::ip::udp::v4();

    boost::asio::ip::udp::resolver resolver(ios);
    boost::asio::ip::udp::resolver::query query(
        ip_ver, host, std::to_string(port));

    boost::asio::ip::udp::endpoint receiver_endpoint = 
        *resolver.resolve(query);

    send(packet, receiver_endpoint);
}

void AsyncUdpSocket::receive()
{
    socket.async_receive_from(
        boost::asio::buffer(buf.data(), buf.size()), received_endpoint,
        ios_st.wrap(boost::bind(&AsyncUdpSocket::handleReceive, this, _1, _2)));
}

// private member function
void AsyncUdpSocket::handleReceive(const boost::system::error_code& error,
    std::size_t packet_size) const
{
    if (receive_callback) {
        receive_callback(error, packet_size, buf, 
            received_endpoint, this);
    }
}

void AsyncUdpSocket::handleSend(const boost::system::error_code& error,
    std::size_t packet_size) const
{
    if (send_callback) {
        send_callback(error, packet_size, this);
    }
}
