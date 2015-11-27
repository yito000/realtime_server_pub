#include "udp_router.h"

UdpRouter::UdpRouter(UdpRouteMapBase& pr_map) : 
    route_map(pr_map)
{
    //
}

UdpRouter::Callback UdpRouter::getRoute(int op_code)
{
    auto it = route_map.route_map.find(op_code);
    if (it != route_map.route_map.end()) {
        return it->second;
    }

    UdpRouter::Callback ret;
    return ret;
}
