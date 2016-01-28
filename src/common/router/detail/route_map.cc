#include "route_map.h"

#include "log/logger.h"

BEGIN_NS

void RouteMap::addRoute(int op_code, RouteCallback callback)
{
    route_map[op_code] = callback;
}

END_NS
