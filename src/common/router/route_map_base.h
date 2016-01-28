#ifndef ROUTE_MAP_BASE_H
#define ROUTE_MAP_BASE_H

#include "fw_env.h"

#include <map>
#include <functional>

BEGIN_NS

typedef std::function<void(long, const unsigned char*, int)> RouteCallback;

class RouteMapBase : public SmartPtr<RouteMapBase>
{
    friend class Router;
    
public:
    typedef boost::intrusive_ptr<RouteMapBase> ptr;

    virtual ~RouteMapBase() {}
    
protected:
    std::map<int, RouteCallback> route_map;
};

END_NS

#endif
