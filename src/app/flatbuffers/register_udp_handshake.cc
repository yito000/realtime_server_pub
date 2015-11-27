#include "register_udp_handshake.h"
#include "router/detail/udp_route_map.h"

#include "udp_handshake.h"

void register_udp_handshake(UdpRouteMap& route_map)
{
    route_map.addRoute(1, glue_udp_handshake__);
}
