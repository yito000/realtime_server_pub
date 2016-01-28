#ifndef UDP_ROUTE_MAP_H
#define UDP_ROUTE_MAP_H

#include "router/udp_route_map_base.h"

BEGIN_NS

class UdpRouter;

class UdpRouteMap : public UdpRouteMapBase
{
public:
    typedef boost::intrusive_ptr<UdpRouteMap> ptr;

    void addRoute(int op_code, UdpRouteCallback callback);
};

END_NS

#endif
