#ifndef HTTP_STATUS_CODE_H
#define HTTP_STATUS_CODE_H

namespace http {
    const int OK = 200;
   
    const int MOVED_PERMANENTLY = 301;
    const int MOVED_TEMPORARILY = 302;
    const int NOT_MODIFIED = 304;
    
    const int BAD_REQUEST = 400;
    const int FORBIDDEN = 403;
    const int NOT_FOUND = 404;
    
    const int INTERNAL_SERVER_ERROR = 500;
    const int BAD_GATEWAY = 502;
    const int SERVICE_UNAVAILABLE = 503;
    const int GATEWAY_TIMEOUT = 504;
}

#endif
