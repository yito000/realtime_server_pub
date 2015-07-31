#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "smart_ptr.hpp"

#include "http_request_params.h"
#include "http_response_header.h"

#include "../io/socket.h"

class HttpClient : public SmartPtr<HttpClient>
{
public:
    typedef boost::intrusive_ptr<HttpClient> ptr;
    
    HttpResponseHeader::ptr sendGetRequest(HttpRequestParams::ptr params);
    HttpResponseHeader::ptr sendPostRequestAsJson(const std::string& json_str, HttpRequestParams::ptr params);
    
    void close();
    
private:
    Socket::ptr socket;
};

#endif
