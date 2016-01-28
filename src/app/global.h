#ifndef GLOBAL_H
#define GLOBAL_H

#include "fw_env.h"

BEGIN_NS

class RouteMap;
class UdpRouteMap;
class ErrorHandleRouteMap;

END_NS

struct AppGlobalSetting
{
    fwx::RouteMap* user_route_map;
    fwx::RouteMap* system_route_map;
    
    fwx::UdpRouteMap* user_route_map_udp;
    fwx::UdpRouteMap* system_route_map_udp;
    
    fwx::ErrorHandleRouteMap* user_err_route_map;
    fwx::ErrorHandleRouteMap* cluster_err_route_map;
    fwx::ErrorHandleRouteMap* server_err_route_map;

    AppGlobalSetting()
    {
        user_route_map = nullptr;
        system_route_map = nullptr;
        
        user_err_route_map = nullptr;
        cluster_err_route_map = nullptr;
        server_err_route_map = nullptr;
    }
};

BEGIN_NS

class App;

END_NS

class Global
{
public:
    void onStart(AppGlobalSetting& g_setting, fwx::App* app);
    void onEnd();
};

#endif
