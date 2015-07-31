#include "http_client.h"

#include <boost/lexical_cast.hpp>

#include "http_helper.hpp"
#include "network/io/socket.h"

HttpResponseHeader::ptr HttpClient::sendGetRequest(HttpRequestParams::ptr params)
{
    if (params->hostname == "" || params->path == "") {
        return HttpResponseHeader::ptr();
    }
    
    if (params->port == -1) {
        params->port = 80;
        socket = new Socket(params->hostname, "http");
    } else {
        std::string port = boost::lexical_cast<std::string>(params->port);
        socket = new Socket(params->hostname, port);
    }
    
    socket->connect(boost::posix_time::seconds(params->connect_timeout));
    
    HttpHelper<Socket> helper;
    HttpResponseHeader::ptr result = helper.sendGetRequest(*socket, params);
    
    socket->close();
    
    return result;
}

HttpResponseHeader::ptr HttpClient::sendPostRequestAsJson(const std::string& json_str, HttpRequestParams::ptr params)
{
    if (params->hostname == "" || params->path == "") {
        return HttpResponseHeader::ptr();;
    }
    
    if (params->port == -1) {
        params->port = 80;
        socket = new Socket(params->hostname, "http");
    } else {
        std::string port = boost::lexical_cast<std::string>(params->port);
        socket = new Socket(params->hostname, port);
    }
    
    socket->connect(boost::posix_time::seconds(params->connect_timeout));
    
    HttpHelper<Socket> helper;
    HttpResponseHeader::ptr result = helper.sendPostRequestAsJson(json_str, *socket, params);
    
    socket->close();
    
    return result;
}

void HttpClient::close()
{
    if (socket) {
        socket->close();
    }
}
