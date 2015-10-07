#ifndef SERVER_FRAME_DATA_H
#define SERVER_FRAME_DATA_H

#include <vector>

#include "../packet.h"

namespace server {

struct SocketFrame
{
    bool fin;
    bool rsv1;
    bool rsv2;
    bool rsv3;

    int opcode;
    bool mask;
    size_t payload_length;

    char mask_key[4];
    std::vector<char> body;

    SocketFrame()
    {
        fin = false;
        rsv1 = false;
        rsv2 = false;
        rsv3 = false;

        opcode = 0;
        mask = false;
        payload_length = 0;

        mask_key[0] = 0;
        mask_key[1] = 0;
        mask_key[2] = 0;
        mask_key[3] = 0;
    }
};

class FrameData
{
public:
    static void serialize(bool end, PacketType packet_type,
        const std::vector<unsigned char>& body, std::vector<unsigned char>& out_data);

    static bool deserialize(std::vector<unsigned char>& data,
        SocketFrame& socket_frame);
};

};

#endif
