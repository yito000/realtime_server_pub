#ifndef ERROR_HANDLE_ROUTER_H
#define ERROR_HANDLE_ROUTER_H

#include "fw_env.h"

#include <boost/system/error_code.hpp>
#include <map>
#include <functional>

BEGIN_NS

class WsActor;
class ErrorHandleRouteMap;

class ErrorHandleRouter : public SmartPtr<ErrorHandleRouter>
{
public:
    typedef boost::intrusive_ptr<ErrorHandleRouter> ptr;
    typedef std::function<void(const WsActor*)> Callback;

    ErrorHandleRouter(ErrorHandleRouteMap& eh_map);
    Callback getCallback(int error_num);

private:
    std::map<int, Callback>& error_func;
};

END_NS

#endif
