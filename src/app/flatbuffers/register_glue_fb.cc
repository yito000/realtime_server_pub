#include "register_glue_fb.h"
#include "router/detail/route_map.h"

#include "glue_fb_glue.h"

USING_NS

void register_glue_fb(RouteMap& route_map)
{
    route_map.addRoute(1000, glue_test__);
    route_map.addRoute(1001, glue_monster__);
}
