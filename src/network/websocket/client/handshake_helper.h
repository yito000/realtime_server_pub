#ifndef HANDSHAKE_HELPER_H
#define HANDSHAKE_HELPER_H

#include "fw_env.h"

#include <string>

#include "network/io/socket_buffer.h"
#include "handshake.h"

BEGIN_NS

namespace client {

class HandshakeHelper
{
public:
    static std::string createSecret();
    static std::string calcResponseSecret(const std::string secret);

    static bool validate(ByteBuffer* packet,
        HandShakeRequest::ptr handshake_req);
};

};

END_NS

#endif
