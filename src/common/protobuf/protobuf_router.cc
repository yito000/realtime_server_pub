#include "protobuf_router.h"

#include "internal/protobuf_route_map.h"

ProtobufRouter::ProtobufRouter(ProtobufRouteMap& pr_map) : 
    route_map(pr_map.route_map)
{
    //
}

ProtobufRouter::Callback ProtobufRouter::getRoute(int op_code)
{
    auto it = route_map.find(op_code);
    if (it != route_map.end()) {
        return it->second;
    }

    ProtobufRouter::Callback ret;
    return ret;
}

