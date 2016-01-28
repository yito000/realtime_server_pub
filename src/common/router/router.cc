#include "router.h"

BEGIN_NS

Router::Router(RouteMapBase& pr_map) : 
    route_map(pr_map)
{
    //
}

Router::Callback Router::getRoute(int op_code)
{
    auto it = route_map.route_map.find(op_code);
    if (it != route_map.route_map.end()) {
        return it->second;
    }

    Router::Callback ret;
    return ret;
}

END_NS
