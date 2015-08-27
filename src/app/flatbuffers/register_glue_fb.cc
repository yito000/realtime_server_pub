#include "register_glue_fb.h"
#include "router/internal/route_map.h"

#include "glue_fb_glue.h"

void register_glue_fb(RouteMap& route_map)
{
    route_map.addRoute(1000, glue_test);
}
