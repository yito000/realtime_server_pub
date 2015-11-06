#ifndef GLOBAL_H
#define GLOBAL_H

#include "router/detail/route_map.h"

struct AppGlobalSetting
{
    RouteMap* user_route_map;
    RouteMap* system_route_map;

    AppGlobalSetting()
    {
        user_route_map = NULL;
        system_route_map = NULL;
    }
};

class App;

class Global
{
public:
    void onStart(AppGlobalSetting& g_setting, App* app);
    void onEnd();
};

#endif
