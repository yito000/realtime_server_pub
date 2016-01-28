#ifndef HTTP_REQUEST_PARAMS_H
#define HTTP_REQUEST_PARAMS_H

#include "fw_env.h"

#include <string>
#include <map>

#include <boost/function.hpp>
#include <boost/date_time.hpp>

#include "http_response_header.h"
#include "network/io/socket_buffer.h"

BEGIN_NS

struct HttpRequestParams : public SmartPtr<HttpRequestParams>
{
    typedef boost::intrusive_ptr<HttpRequestParams> ptr;
    
    typedef boost::function<void(const HttpResponseHeader&, const ByteBuffer&)> ResponseCallback;
    typedef boost::function<void(const HttpResponseHeader&)> FinishResponseCallback;
    typedef std::map<std::string, std::string> Headers;
    
    std::string hostname;
    std::string path;
    unsigned int port;
    
    int connect_timeout;
    int request_timeout;
    
    bool auto_redirect; // not used
    
    ResponseCallback response_callback;
    FinishResponseCallback finish_response_callback;
    Headers user_headers;
    
    HttpRequestParams()
    {
        port = -1;
        path = "/";
        
        connect_timeout = 60;
        request_timeout = 60;
        
        auto_redirect = false;
    }
};

END_NS

#endif
