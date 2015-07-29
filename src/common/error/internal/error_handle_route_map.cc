#include "error_handle_route_map.h"

void ErrorHandleRouteMap::addCallback(int error_num, ErrorCallback callback)
{
    error_func[error_num] = callback;
}

