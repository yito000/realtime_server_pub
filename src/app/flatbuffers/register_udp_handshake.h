#ifndef register_udp_handshake_H
#define register_udp_handshake_H

#include "fw_env.h"

BEGIN_NS

class UdpRouteMap;

END_NS

void register_udp_handshake(fwx::UdpRouteMap& route_map);

#endif
