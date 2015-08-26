#include "router.h"

#include "internal/route_map.h"

Router::Router(RouteMap& pr_map) : 
    route_map(pr_map.route_map)
{
    //
}

Router::Callback Router::getRoute(int op_code)
{
    auto it = route_map.find(op_code);
    if (it != route_map.end()) {
        return it->second;
    }

    Router::Callback ret;
    return ret;
}
