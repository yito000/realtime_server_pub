#ifndef WEBSOCKET_EXCEPTION_H
#define WEBSOCKET_EXCEPTION_H

#include "exception/base_exception.h"

class WebsocketException;

#define THROW_WEBSOCKET_EXCEPTION(message) \
    throw WebsocketException(message, __FILE__, __func__, __LINE__)

class WebsocketException : public BaseException
{
public:
    WebsocketException(const char* _message, const char* _filename, 
        const char* _function, int _line) : 
        BaseException(_message, _filename, _function, _line)
    {
        //
    }
};

#endif

