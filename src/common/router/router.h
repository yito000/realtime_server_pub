#ifndef ROUTER_H
#define ROUTER_H

#include "smart_ptr.hpp"

#include <map>
#include <functional>

class RouteMap;
class WsActor;
typedef boost::intrusive_ptr<WsActor> WsActorPtr;

class Router : public SmartPtr<Router>
{
public:
    typedef boost::intrusive_ptr<Router> ptr;
    typedef std::function<void(long, 
        const char*, int)> Callback;

    Router(RouteMap& pr_map);

    Callback getRoute(int op_code);

private:
    std::map<int, Callback>& route_map;
};

#endif
