#ifndef PACKET_H
#define PACKET_H

#include "smart_ptr.hpp"

#include <vector>

enum PacketType {
    PACKET_TYPE_TEXT = 1,
    PACKET_TYPE_BINARY = 2,
    PACKET_TYPE_CLOSE = 8,
    PACKET_TYPE_PING = 9,
    PACKET_TYPE_PONG = 10,
    PACKET_TYPE_ERROR = -1,
};

struct PacketData : public SmartPtr<PacketData>
{
    typedef boost::intrusive_ptr<PacketData> ptr;

    bool fin;
    bool cont_frame;

    std::vector<unsigned char> data;
    PacketType packet_type;

    PacketData()
    {
        fin = true;
        cont_frame = false;

        packet_type = PACKET_TYPE_TEXT;
    }
};

#endif

