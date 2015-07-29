#ifndef ERROR_HANDLE_ROUTE_MAP_H
#define ERROR_HANDLE_ROUTE_MAP_H

#include "smart_ptr.hpp"

#include <boost/system/error_code.hpp>
#include <map>
#include <functional>

class WsActor;
class ErrorHandleRouter;

typedef std::function<void(const WsActor*)> ErrorCallback;

class ErrorHandleRouteMap : public SmartPtr<ErrorHandleRouteMap>
{
    friend class ErrorHandleRouter;

public:
    typedef boost::intrusive_ptr<ErrorHandleRouteMap> ptr;

    void addCallback(int error_num, ErrorCallback callback);

private:
    std::map<int, ErrorCallback> error_func;
};

#endif

