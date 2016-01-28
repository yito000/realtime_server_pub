#include "handshake_helper.h"

#include "../secret/secret_key.h"

#include <iostream>
#include <sstream>
#include <regex>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "log/logger.h"

BEGIN_NS

namespace {
    std::regex upgrade("Upgrade", std::regex_constants::icase);
    std::regex connection("Connection", std::regex_constants::icase);
    std::regex sec_accept("Sec-WebSocket-Accept", 
        std::regex_constants::icase);
    std::regex sec_protocol("Sec-WebSocket-Protocol", 
        std::regex_constants::icase);
};

typedef boost::char_separator<char> char_separator;
typedef boost::tokenizer<char_separator> tokenizer;

namespace client {

// function
bool getResponseCode(const std::string& line, HandShakeResponse::ptr response);
bool readHeaderLine(const std::string& line, HandShakeResponse::ptr response);
bool setHeaderValue(const std::string& key, 
    const std::string& value, HandShakeResponse::ptr response);

HandShakeResponse::ptr createResponseObject(ByteBuffer* packet)
{
    std::string p_str(packet->begin(), packet->end());
    std::stringstream ss(p_str);

    std::string tmp_str;
    HandShakeResponse::ptr res = new HandShakeResponse;

    std::getline(ss, tmp_str);
    
#ifndef NDEBUG
    std::cout << tmp_str << std::endl;
#endif

    if (!getResponseCode(tmp_str, res)) {
        return NULL;
    }
    
    if (res->status_code != 101) {
        return res;
    }

    while (std::getline(ss, tmp_str)) {
#ifndef NDEBUG
        std::cout << tmp_str << std::endl;
#endif

        if (!readHeaderLine(tmp_str, res)) {
            return NULL;
        }
    }
    
    return res;
}

//
std::string HandshakeHelper::createSecret()
{
    return SecretKey::generate();
}

std::string HandshakeHelper::calcResponseSecret(const std::string secret)
{
    return SecretKey::calcResponse(secret);
}

bool HandshakeHelper::validate(ByteBuffer* packet,
    HandShakeRequest::ptr handshake_req)
{
    auto response = createResponseObject(packet);

    if (!response) {
        return false;
    }
    
    if (response->status_code != 101) {
        return false;
    }

    //
    auto u_name = response->upgrade;
    std::transform(response->upgrade.begin(), 
        response->upgrade.end(), u_name.begin(), tolower);

    if (u_name != "websocket") {
        return false;
    }

    //
    auto c_name = response->connection;
    std::transform(response->connection.begin(),
        response->connection.end(), c_name.begin(), tolower);
    
    if (c_name != "upgrade") {
        return false;
    }

    //
    auto secret = calcResponseSecret(handshake_req->secret_key);
    
    if (response->secret_accept != secret) {
        return false;
    }
    
    return true;
}

//
bool getResponseCode(const std::string& line,
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

bool readHeaderLine(const std::string& line,
    HandShakeResponse::ptr response)
{
    char* pos = (char*)strstr(line.c_str(), ":");

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

bool setHeaderValue(const std::string& key,
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

};

END_NS
