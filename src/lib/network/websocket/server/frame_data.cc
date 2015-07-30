#include "frame_data.h"

namespace server {

void FrameData::serialize(bool end, PacketType packet_type,
    const std::vector<char>& body, std::vector<char>& out_data)
{
    char* pa = NULL;
    int len_type = 3;
    int header_size = -1;

    if (body.size() > 125) {
        if (body.size() > 65535) {
            pa = new char[10];
            len_type = 1;
            header_size = 10;
        } else {
            pa = new char[4];
            len_type = 2;
            header_size = 4;
        }
    } else {
        pa = new char[2];
        header_size = 2;
    }

    //
    pa[0] = 0x02;
    switch (packet_type) {
        case PACKET_TYPE_BINARY:
            pa[0] = 0x02;
            break;

        case PACKET_TYPE_TEXT:
            pa[0] = 0x01;
            break;

        case PACKET_TYPE_CLOSE:
            pa[0] = 0x08;
            break;

        case PACKET_TYPE_PING:
            pa[0] = 0x09;
            break;

        case PACKET_TYPE_PONG:
            pa[0] = 0x0a;
            break;

        default:
            break;
    }

    if (end) {
        pa[0] |= (0x01 << 7);
    }

    //
    int cur_index = 1;
    if (len_type == 1) {
        pa[1] = 127;
        long long n = body.size();

        pa[2] = (n >> 56) & 255;
        pa[3] = (n >> 48) & 255;
        pa[4] = (n >> 40) & 255;
        pa[5] = (n >> 32) & 255;
        pa[6] = (n >> 24) & 255;
        pa[7] = (n >> 16) & 255;
        pa[8] = (n >> 8) & 255;
        pa[9] = n & 255;

        cur_index = 9;
    } else if (len_type == 2) {
        pa[1] = 126;
        unsigned short n = body.size();

        pa[2] = (n >> 8) & 255;
        pa[3] = n & 255;

        cur_index = 3;
    } else {
        pa[1] = body.size();
    }

    //
    for (int i = 0; i < header_size; i++) {
        out_data.push_back(pa[i]);
    }

    for (int i = 0; i < body.size(); i++) {
        out_data.push_back(body[i]);
    }

    delete[] pa;
}

bool FrameData::deserialize(std::vector<char>& data,
    SocketFrame& socket_frame)
{
    if (data.size() < 2) {
        return false;
    }

    char c1 = data[0];
    char c2 = data[1];

    socket_frame.fin = c1 & (1 << 7);
    socket_frame.opcode = c1 & 0x0f;
    socket_frame.mask = c2 & (1 << 7);

    if (!socket_frame.mask) {
        // todo
        throw std::exception();
    }

    switch (socket_frame.opcode) {
        case PACKET_TYPE_BINARY:
        case PACKET_TYPE_TEXT:
        case PACKET_TYPE_CLOSE:
        case PACKET_TYPE_PING:
        case PACKET_TYPE_PONG:
            break;

        default:
            // todo
            throw std::exception();
            break;
    }

    int raw_data_byte_index = 0;

    char p_len = c2;
    if (p_len & 0x80) {
        p_len = c2 ^ 0x80;
    }

    if (p_len == 127) {
        if (data.size() < 10) {
            return false;
        }

        unsigned char c3 = data[2];
        unsigned char c4 = data[3];
        unsigned char c5 = data[4];
        unsigned char c6 = data[5];
        unsigned char c7 = data[6];
        unsigned char c8 = data[7];
        unsigned char c9 = data[8];
        unsigned char c10 = data[9];

        socket_frame.payload_length = c3 << 56;
        socket_frame.payload_length |= c4 << 48;
        socket_frame.payload_length |= c5 << 40;
        socket_frame.payload_length |= c6 << 32;
        socket_frame.payload_length |= c7 << 24;
        socket_frame.payload_length |= c8 << 16;
        socket_frame.payload_length |= c9 << 8;
        socket_frame.payload_length |= c10;

        raw_data_byte_index = 9;
    } else if (p_len == 126) {
        if (data.size() < 4) {
            return false;
        }

        unsigned char c3 = data[2];
        unsigned char c4 = data[3];

        socket_frame.payload_length = c3 << 8;
        socket_frame.payload_length |= c4;

        raw_data_byte_index = 3;
    } else {
        socket_frame.payload_length = p_len;

        raw_data_byte_index = 1;
    }

    if (socket_frame.mask) {
        if (data.size() < (raw_data_byte_index + 1) + 4) {
            return false;
        }

        socket_frame.mask_key[0] = data[raw_data_byte_index + 1];
        socket_frame.mask_key[1] = data[raw_data_byte_index + 2];
        socket_frame.mask_key[2] = data[raw_data_byte_index + 3];
        socket_frame.mask_key[3] = data[raw_data_byte_index + 4];

        raw_data_byte_index += 4;
    }

    int body_size = socket_frame.payload_length;
    int all_data_size = body_size + (raw_data_byte_index + 1);

    if (data.size() < all_data_size) {
        return false;
    }

    int start_body_index = raw_data_byte_index + 1;

    if (socket_frame.mask) {
        int cnt = 0;
        for (int i = start_body_index; i < data.size(); i++) {
            char mask_key = socket_frame.mask_key[cnt % 4];
            socket_frame.body.push_back(data[i] ^ mask_key);

            cnt++;
        }
    } else {
        for (int i = start_body_index; i < data.size(); i++) {
            socket_frame.body.push_back(data[i]);
        }
    }

    return true;
}

};
