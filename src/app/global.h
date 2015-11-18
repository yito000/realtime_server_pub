#ifndef GLOBAL_H
#define GLOBAL_H

class RouteMap;
class ErrorHandleRouteMap;

struct AppGlobalSetting
{
    RouteMap* user_route_map;
    RouteMap* system_route_map;
    
    ErrorHandleRouteMap* user_err_route_map;
    ErrorHandleRouteMap* cluster_err_route_map;
    ErrorHandleRouteMap* server_err_route_map;

    AppGlobalSetting()
    {
        user_route_map = nullptr;
        system_route_map = nullptr;
        
        user_err_route_map = nullptr;
        cluster_err_route_map = nullptr;
        server_err_route_map = nullptr;
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
