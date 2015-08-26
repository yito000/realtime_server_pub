#include "route_map.h"

void RouteMap::addRoute(int op_code, RouteCallback callback)
{
    route_map[op_code] = callback;
}
