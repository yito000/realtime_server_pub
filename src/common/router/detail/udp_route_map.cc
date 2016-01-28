#include "udp_route_map.h"

#include "log/logger.h"

BEGIN_NS

void UdpRouteMap::addRoute(int op_code, UdpRouteCallback callback)
{
    route_map[op_code] = callback;
}

END_NS
