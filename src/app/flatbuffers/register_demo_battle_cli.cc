#include "register_demo_battle_cli.h"
#include "router/detail/route_map.h"

#include "demo_battle_cli_glue.h"

USING_NS

void register_demo_battle_cli(RouteMap& route_map)
{
    route_map.addRoute(10000, glue_input_command__);
    route_map.addRoute(20000, glue_battle_entry__);
    route_map.addRoute(20001, glue_battle_exit__);
}
