#ifndef ROUTE_MAP_H
#define ROUTE_MAP_H

#include "smart_ptr.hpp"

#include <map>
#include <functional>

class WsActor;
class Router;

typedef boost::intrusive_ptr<WsActor> WsActorPtr;

typedef std::function<void(long, const unsigned char*, int)> RouteCallback;

class RouteMap : public SmartPtr<RouteMap>
{
    friend class Router;

public:
    typedef boost::intrusive_ptr<RouteMap> ptr;

    void addRoute(int op_code, RouteCallback callback);

private:
    std::map<int, RouteCallback> route_map;
};

#endif
