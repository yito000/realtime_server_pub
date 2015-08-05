#ifndef PROTOBUF_ROUTE_MAP_H
#define PROTOBUF_ROUTE_MAP_H

#include "smart_ptr.hpp"

#include <map>
#include <functional>

class WsActor;
class ProtobufRouter;

typedef boost::intrusive_ptr<WsActor> WsActorPtr;

typedef std::function<void(long, const char*, int)> RouteCallback;

class ProtobufRouteMap : public SmartPtr<ProtobufRouteMap>
{
    friend class ProtobufRouter;

public:
    typedef boost::intrusive_ptr<ProtobufRouteMap> ptr;

    void addRoute(int op_code, RouteCallback callback);

private:
    std::map<int, RouteCallback> route_map;
};

#endif
