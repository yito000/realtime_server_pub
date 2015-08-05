#ifndef PROTOBUF_ROUTER_H
#define PROTOBUF_ROUTER_H

#include "smart_ptr.hpp"

#include <map>
#include <functional>

class ProtobufRouteMap;
class WsActor;
typedef boost::intrusive_ptr<WsActor> WsActorPtr;

class ProtobufRouter : public SmartPtr<ProtobufRouter>
{
public:
    typedef boost::intrusive_ptr<ProtobufRouter> ptr;
    typedef std::function<void(long, 
        const char*, int)> Callback;

    ProtobufRouter(ProtobufRouteMap& pr_map);

    Callback getRoute(int op_code);

private:
    std::map<int, Callback>& route_map;
};

#endif
