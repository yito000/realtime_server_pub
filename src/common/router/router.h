#ifndef ROUTER_H
#define ROUTER_H

#include "smart_ptr.hpp"

#include <map>
#include <functional>

#include "route_map_base.h"

class Router : public SmartPtr<Router>
{
public:
    typedef boost::intrusive_ptr<Router> ptr;
    typedef std::function<void(long, 
        const unsigned char*, int)> Callback;

    Router(RouteMapBase& pr_map);

    Callback getRoute(int op_code);

private:
    RouteMapBase& route_map;
};

#endif
