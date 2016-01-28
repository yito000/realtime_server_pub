#include "error_handle_router.h"

#include "internal/error_handle_route_map.h"

BEGIN_NS

ErrorHandleRouter::ErrorHandleRouter(ErrorHandleRouteMap& eh_map) : 
    error_func(eh_map.error_func)
{
    //
}

ErrorHandleRouter::Callback ErrorHandleRouter::getCallback(int error_num)
{
    auto it = error_func.find(error_num);
    if (it != error_func.end()) {
        return it->second;
    }

    ErrorHandleRouter::Callback ret;
    return ret;
}

END_NS
