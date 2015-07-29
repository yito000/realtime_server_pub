#ifndef SERVER_HANDSHAKE_H
#define SERVER_HANDSHAKE_H

#include "smart_ptr.hpp"

#include <string>
#include <set>

#include "../socket_buffer.h"

namespace server {

struct HandShakeRequest : public SmartPtr<HandShakeRequest>
{
    typedef boost::intrusive_ptr<HandShakeRequest> ptr;

    std::string method;
    std::string path;
    std::string http_version;

    std::string upgrade_name;
    std::string connection_name;
    std::string secret_key;
    std::set<std::string> protocols;
    int ws_version;

    HandShakeRequest()
    {
        http_version = "0";

        ws_version = -1;
    }
};

struct HandShakeResponse : public SmartPtr<HandShakeResponse>
{
    typedef boost::intrusive_ptr<HandShakeResponse> ptr;

    std::string http_version;
    int status_code;

    std::string upgrade;
    std::string connection;
    std::string secret_accept;
    std::string protocol;

    HandShakeResponse()
    {
        status_code = 0;
    }
};

class HandShake
{
public:
    static HandShakeRequest::ptr parseReqHeader(ByteBuffer* buf);

private:
    static bool getFirstLine(const std::string& str, 
        HandShakeRequest::ptr req);
    static bool readHeaderLine(const std::string& line,
        HandShakeRequest::ptr req);

    static bool setHeaderValue(const std::string& key,
        const std::string& value, HandShakeRequest::ptr req);
    static void parseProtocols(const std::string& p_str, 
        HandShakeRequest::ptr req);
};

};

#endif

