#include "register_demo_battle_cli_udp.h"
#include "router/detail/udp_route_map.h"

#include "demo_battle_cli_glue_udp.h"

USING_NS

void register_demo_battle_cli_udp(UdpRouteMap& route_map)
{
    route_map.addRoute(10000, glue_udp_input_command__);
    route_map.addRoute(20000, glue_udp_battle_entry__);
    route_map.addRoute(20001, glue_udp_battle_exit__);
}
