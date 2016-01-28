#ifndef UDP_ROUTER_H
#define UDP_ROUTER_H

#include "fw_env.h"

#include <map>
#include <functional>
#include <boost/asio.hpp>

#include "udp_route_map_base.h"

BEGIN_NS

class UdpRouter : public SmartPtr<UdpRouter>
{
public:
    typedef boost::intrusive_ptr<UdpRouter> ptr;
    typedef std::function<void(const boost::asio::ip::udp::endpoint&, 
        const unsigned char*, int)> Callback;

    UdpRouter(UdpRouteMapBase& pr_map);

    Callback getRoute(int op_code);

private:
    UdpRouteMapBase& route_map;
};

END_NS

#endif
