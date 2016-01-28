#include "error_handle_route_map.h"

BEGIN_NS

void ErrorHandleRouteMap::addCallback(int error_num, ErrorCallback callback)
{
    error_func[error_num] = callback;
}

END_NS
