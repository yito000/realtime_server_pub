#ifndef HTTPS_CLIENT_H
#define HTTPS_CLIENT_H

#include "smart_ptr.hpp"

#include "http_request_params.h"
#include "http_response_header.h"

#include "network/io/ssl_socket.h"

class HttpsClient : public SmartPtr<HttpsClient>
{
public:
    typedef boost::intrusive_ptr<HttpsClient> ptr;
    
    HttpResponseHeader::ptr sendGetRequest(HttpRequestParams::ptr params);
    HttpResponseHeader::ptr sendPostRequestAsJson(const std::string& json_str, HttpRequestParams::ptr params);
    
    void close();
    
private:
    SSLSocket::ptr socket;
};

#endif
