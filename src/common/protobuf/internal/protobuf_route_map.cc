#include "protobuf_route_map.h"

void ProtobufRouteMap::addRoute(int op_code, RouteCallback callback)
{
    route_map[op_code] = callback;
}
