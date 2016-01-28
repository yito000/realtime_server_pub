#ifndef CLIENT_HANDSHAKE_H
#define CLIENT_HANDSHAKE_H

#include "fw_env.h"

#include <string>
#include <set>

BEGIN_NS

namespace client {

struct HandShakeRequest : public SmartPtr<HandShakeRequest>
{
    typedef boost::intrusive_ptr<HandShakeRequest> ptr;

    std::string path;
    std::string secret_key;
    std::set<std::string> protocols;
    int version;

    HandShakeRequest()
    {
        version = 13;
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

};

END_NS

#endif

