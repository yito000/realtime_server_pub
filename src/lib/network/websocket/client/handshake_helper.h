#ifndef HANDSHAKE_HELPER_H
#define HANDSHAKE_HELPER_H

#include <string>

#include "../socket_buffer.h"
#include "handshake.h"

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

#endif

