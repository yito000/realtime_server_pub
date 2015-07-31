#include "websocket.h"

#include <sstream>
#include <regex>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/thread.hpp>

namespace {
    const int DEFAULT_TIMEOUT = 60 * 1000;

    std::regex upgrade("Upgrade", std::regex_constants::icase);
    std::regex connection("Connection", std::regex_constants::icase);
    std::regex sec_accept("Sec-WebSocket-Accept", std::regex_constants::icase);
    std::regex sec_protocol("Sec-WebSocket-Protocol", std::regex_constants::icase);
};

namespace client {

typedef boost::char_separator<char> char_separator;
typedef boost::tokenizer<char_separator> tokenizer;

Websocket::Websocket(const std::string& _host, 
    unsigned short _port) : host(_host), port(_port), 
    connected(false), handshake_ok(false)
{
    //
}

Websocket::~Websocket()
{
    if (socket) {
        socket->close();
    }
}

void Websocket::connect(int timeout_millis)
{
    if (host == "") {
        return;
    }

    int t = timeout_millis < 1 ? DEFAULT_TIMEOUT : timeout_millis;

    socket = new Socket(host, port);
    socket->setErrorAfterClose(false);
    socket->connect(boost::posix_time::milliseconds(t));

    connected = true;
}

HandShakeResponse::ptr Websocket::handShake(
    HandShakeRequest::ptr handshake_req, int timeout_millis, int retry)
{
    if (!connected || handshake_req->path == "" || 
        handshake_req->secret_key == "" || 
        handshake_req->protocols.size() <= 0) {

        return NULL;
    }

    int t = timeout_millis < 1 ? DEFAULT_TIMEOUT : timeout_millis;

    std::string path = handshake_req->path;;
    std::string key = handshake_req->secret_key;
    std::string version = 
        boost::lexical_cast<std::string>(handshake_req->version);
    std::string protocol;

    bool p_first = true;
    std::set<std::string>::iterator p_it = handshake_req->protocols.begin();
    for (; p_it != handshake_req->protocols.end(); ++p_it) {
        if (p_first) {
            protocol = *p_it;
        } else {
            protocol += ", " + *p_it;
        }
    }

    boost::posix_time::time_duration timeout = 
        boost::posix_time::milliseconds(t);

    std::string req;
    req += "GET " + path + " HTTP/1.1\r\n";
    req += "Host: " + host + "\r\n";
    req += "Upgrade: websocket\r\n";
    req += "Connection: Upgrade\r\n";
    req += "Sec-WebSocket-Key: " + key + "\r\n";
    req += "Sec-WebSocket-Protocol: " + protocol + "\r\n";
    req += "Sec-WebSocket-Version: " + version + "\r\n";
    req += "\r\n";

    socket->write(req, timeout);

    HandShakeResponse::ptr ret = receiveHandShake(retry, t);
    if (ret && ret->status_code == 101) {
        handshake_ok = true;
    }

    return ret;
}

void Websocket::close()
{
    if (socket) {
        socket->close();
        connected = false;
        handshake_ok = false;
    }
}

void Websocket::write(PacketData::ptr packet_data, 
    const char* mask_key, int timeout_millis, int retry)
{
    if (!handshake_ok) {
        return;
    }

    std::string mask(mask_key, 0, 4);

    std::vector<char> ser_packet_data;
    serializeFramingData(packet_data->fin, packet_data->packet_type, 
        mask, packet_data->data, ser_packet_data);

    boost::posix_time::time_duration timeout = 
        boost::posix_time::milliseconds(timeout_millis);

    int cnt = 0;

    while (1) {
        try {
            socket->write(ser_packet_data, timeout);
            break;
        } catch (std::exception& e) {
            if (cnt >= retry) {
                throw e;
            }

            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
            cnt++;
        }
    }
}

PacketData::ptr Websocket::read(int timeout_millis, int retry)
{
    if (!handshake_ok) {
        return NULL;
    }

    boost::posix_time::time_duration timeout = 
        boost::posix_time::milliseconds(timeout_millis);

    SocketFrame socket_frame;
    std::vector<char> buf;
    std::vector<char> out_data;

    int cnt = 0;

    while (1) {
        buf.clear();

        try {
            bool can_read_buffer = socket->read_some(buf, timeout);
            if (can_read_buffer) {
                out_data.insert(out_data.end(), buf.begin(), buf.end());

                bool parse_complete = 
                    deserializeFramingData(out_data, socket_frame);

                if (parse_complete) {
                    break;
                }
            } else {
                break;
            }
        } catch (std::exception& e) {
            if (cnt >= retry) {
                throw e;
            }

            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
            cnt++;
        }
    }

    //
    PacketData::ptr pd_ret = new PacketData;
    pd_ret->fin = socket_frame.fin;

    if (socket_frame.opcode == 0x0) {
        pd_ret->cont_frame = true;
    } else {
        switch (socket_frame.opcode) {
            case 0x1:
                pd_ret->packet_type = PACKET_TYPE_BINARY;
                break;

            case 0x2:
                pd_ret->packet_type = PACKET_TYPE_TEXT;
                break;

            case 0x8:
                pd_ret->packet_type = PACKET_TYPE_CLOSE;
                break;

            case 0x9:
                pd_ret->packet_type = PACKET_TYPE_PING;
                break;

            case 0xa:
                pd_ret->packet_type = PACKET_TYPE_PONG;
                break;

            default:
                break;
        }
    }

    pd_ret->data.insert(pd_ret->data.end(), 
        socket_frame.body.begin(), socket_frame.body.end());

    return pd_ret;
}

// private member function
HandShakeResponse::ptr Websocket::receiveHandShake(int retry, 
    int timeout_millis)
{
    boost::posix_time::time_duration timeout = 
        boost::posix_time::milliseconds(timeout_millis);

    std::vector<char> buf;
    socket->read_some(buf, timeout);

    //
    std::string p_str(buf.begin(), buf.end());
    std::stringstream ss(p_str);

    std::string tmp_str;
    HandShakeResponse::ptr res = new HandShakeResponse;

    std::getline(ss, tmp_str);
    std::cout << tmp_str << std::endl;

    if (!getResponseCode(tmp_str, res)) {
        return NULL;
    }

    if (res->status_code != 101) {
        return res;
    }

    while (std::getline(ss, tmp_str)) {
        std::cout << tmp_str << std::endl;

        if (!readHeaderLine(tmp_str, res)) {
            return NULL;
        }
    }

    return res;
}

bool Websocket::getResponseCode(const std::string& line, 
    HandShakeResponse::ptr response)
{
    std::string tmp = line.c_str();

    char_separator sep(" ", "", boost::drop_empty_tokens);
    tokenizer tokens(tmp, sep);
    tokenizer::iterator it = tokens.begin();

    if (it != tokens.end()) {
        if ((*it).size() >= 8 && (*it).substr(0, 5) == "HTTP/") {
            response->http_version = (*it).substr(5, 3).c_str();
        } else {
            return false;
        }

        ++it;

        if (it != tokens.end()) {
            try {
                response->status_code = boost::lexical_cast<int>(*it);
            } catch (boost::bad_lexical_cast& e) {
                return false;
            }
        }
    } else {
        return false;
    }

    return true;
}

bool Websocket::readHeaderLine(const std::string& line, 
    HandShakeResponse::ptr response)
{
    char* pos = strstr(line.c_str(), ":");

    if (pos) {
        size_t len = pos - line.c_str();
        std::string key(line, 0, len);
        std::string value;

        pos++;

        while (pos[0] == ' ') {
            pos++;
        }

        size_t value_size = strlen(pos);

        if (pos[value_size - 1] == '\r') {
            value.assign(pos, strlen(pos) - 1);
        } else {
            value = pos;
        }

        if (!setHeaderValue(key, value, response)) {
            return false;
        }
    }

    return true;
}

bool Websocket::setHeaderValue(const std::string& key, 
    const std::string& value, HandShakeResponse::ptr response)
{
    if (std::regex_match(key, upgrade)) {
        response->upgrade = value;
    } else if (std::regex_match(key, connection)) {
        response->connection = value;
    } else if (std::regex_match(key, sec_accept)) {
        response->secret_accept = value;
    } else if (std::regex_match(key, sec_protocol)) {
        response->protocol = value;
    }

    return true;
}

void Websocket::serializeFramingData(bool end, PacketType packet_type, 
    const std::string& mask, const std::vector<char>& body, 
    std::vector<char>& out_data)
{
    char* pa = NULL;
    int len_type = 3;
    int header_size = -1;

    if (body.size() > 125) {
        if (body.size() > 65535) {
            pa = new char[14];
            len_type = 1;
            header_size = 14;
        } else {
            pa = new char[8];
            len_type = 2;
            header_size = 8;
        }
    } else {
        pa = new char[6];
        header_size = 6;
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

    // enable mask
    pa[1] |= (0x01 << 7);

    pa[cur_index + 1] = mask[0];
    pa[cur_index + 2] = mask[1];
    pa[cur_index + 3] = mask[2];
    pa[cur_index + 4] = mask[3];

    int mask_len = mask.size();
    if (mask_len != 4) {
        delete[] pa;
        return;
    }

    //
    for (int i = 0; i < header_size; i++) {
        out_data.push_back(pa[i]);
    }

    for (int i = 0; i < body.size(); i++) {
        out_data.push_back(body[i] ^ mask[i % 4]);
    }

    delete[] pa;
}

bool Websocket::deserializeFramingData(std::vector<char>& data,
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
