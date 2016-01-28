#ifndef UDP_ROUTE_MAP_BASE_H
#define UDP_ROUTE_MAP_BASE_H

#include "fw_env.h"

#include <map>
#include <functional>
#include <boost/asio.hpp>

BEGIN_NS

typedef std::function<void(const boost::asio::ip::udp::endpoint&, const unsigned char*, int)> UdpRouteCallback;

class UdpRouteMapBase : public SmartPtr<UdpRouteMapBase>
{
    friend class UdpRouter;
    
public:
    typedef boost::intrusive_ptr<UdpRouteMapBase> ptr;

    virtual ~UdpRouteMapBase() {}
    
protected:
    std::map<int, UdpRouteCallback> route_map;
};

END_NS

#endif
