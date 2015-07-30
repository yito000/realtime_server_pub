#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include "smart_ptr.hpp"
#include <string>
#include <vector>
#include <set>

#include "../../io/socket.h"
#include "../packet.h"
#include "handshake.h"

namespace client {

class Websocket : public SmartPtr<Websocket>
{
public:
    typedef boost::intrusive_ptr<Websocket> ptr;

    Websocket(const std::string& _host, unsigned short _port);
    ~Websocket();

    void connect(int timeout_millis);
    HandShakeResponse::ptr handShake(HandShakeRequest::ptr handshake_req,
        int timeout_millis, int retry);
    void close();

    void write(PacketData::ptr packet_data, const char* mask_key, 
        int timeout_millis, int retry);
    PacketData::ptr read(int timeout_millis, int retry);

private:
    struct SocketFrame
    {
        bool fin;
        bool rsv1;
        bool rsv2;
        bool rsv3;

        int opcode;
        bool mask;
        int payload_length;

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

    HandShakeResponse::ptr receiveHandShake(int retry, int timeou_millis);
    bool getResponseCode(const std::string& line, HandShakeResponse::ptr response);
    bool readHeaderLine(const std::string& line, HandShakeResponse::ptr response);
    bool setHeaderValue(const std::string& key, const std::string& value,
        HandShakeResponse::ptr response);

    void serializeFramingData(bool end, PacketType packet_type, 
        const std::string& mask, const std::vector<char>& body, 
        std::vector<char>& out_data);
    bool deserializeFramingData(std::vector<char>& data,
        SocketFrame& socket_frame);

    std::string host;
    unsigned short port;

    bool connected;
    bool handshake_ok;
    Socket::ptr socket;
};

};

#endif

