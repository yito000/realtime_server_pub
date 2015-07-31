#include "https_client.h"

#include <boost/lexical_cast.hpp>

#include "http_helper.hpp"
#include "network/io/ssl_socket.h"

HttpResponseHeader::ptr HttpsClient::sendGetRequest(HttpRequestParams::ptr params)
{
    if (params->hostname == "" || params->path == "") {
        return HttpResponseHeader::ptr();
    }
    
    if (params->port == -1) {
        params->port = 443;
        socket = new SSLSocket(params->hostname, "https");
    } else {
        std::string port = boost::lexical_cast<std::string>(params->port);
        socket = new SSLSocket(params->hostname, port);
    }
    
    socket->connect(boost::posix_time::seconds(params->connect_timeout));
    
    HttpHelper<SSLSocket> helper;
    HttpResponseHeader::ptr result = helper.sendGetRequest(*socket, params);
    
    socket->close();
    
    return result;
}

HttpResponseHeader::ptr HttpsClient::sendPostRequestAsJson(const std::string& json_str, HttpRequestParams::ptr params)
{
    if (params->hostname == "" || params->path == "") {
        return HttpResponseHeader::ptr();
    }
    
    if (params->port == -1) {
        params->port = 443;
        socket = new SSLSocket(params->hostname, "https");
    } else {
        std::string port = boost::lexical_cast<std::string>(params->port);
        socket = new SSLSocket(params->hostname, port);
    }
    
    socket->connect(boost::posix_time::seconds(params->connect_timeout));
    
    HttpHelper<SSLSocket> helper;
    HttpResponseHeader::ptr result = helper.sendPostRequestAsJson(json_str, *socket, params);
    
    socket->close();
    
    return result;
}

void HttpsClient::close()
{
    if (socket) {
        socket->close();
    }
}
