#ifndef ROUTE_MAP_H
#define ROUTE_MAP_H

#include "router/route_map_base.h"

class Router;

class RouteMap : public RouteMapBase
{
public:
    typedef boost::intrusive_ptr<RouteMap> ptr;

    void addRoute(int op_code, RouteCallback callback);
};

#endif
