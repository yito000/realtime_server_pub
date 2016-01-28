#ifndef WEBSOCKET_EXCEPTION_H
#define WEBSOCKET_EXCEPTION_H

#include "fw_env.h"

#include "exception/base_exception.h"

BEGIN_NS

class WebsocketException : public BaseException
{
public:
    WebsocketException(const char* _message, const char* _filename, 
        const char* _function, int _line) : 
        BaseException(_message, _filename, _function, _line)
    {
        //
    }
    
    WebsocketException(const char* _message) : 
        BaseException(_message, __FILE__, __func__, __LINE__)
    {
        //
    }
};

END_NS

#endif

