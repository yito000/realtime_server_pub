#include "route_map.h"

#include "log/logger.h"

void RouteMap::addRoute(int op_code, RouteCallback callback)
{
    route_map[op_code] = callback;
}
