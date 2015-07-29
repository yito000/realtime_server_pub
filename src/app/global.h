#ifndef GLOBAL_H
#define GLOBAL_H

#include "protobuf/internal/protobuf_route_map.h"

struct AppGlobalSetting
{
    ProtobufRouteMap* user_pb_route_map;
    ProtobufRouteMap* system_pb_route_map;

    AppGlobalSetting()
    {
        user_pb_route_map = NULL;
        system_pb_route_map = NULL;
    }
};

class Global
{
public:
    void onStart(AppGlobalSetting& g_setting);
    void onEnd();
};

#endif

