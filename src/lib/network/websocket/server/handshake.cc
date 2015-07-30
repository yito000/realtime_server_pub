#include "handshake.h"

#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

namespace {
    boost::regex upgrade("Upgrade", boost::regex::icase);
    boost::regex connection("Connection", boost::regex::icase);
    boost::regex sec_key("Sec-WebSocket-Key", boost::regex::icase);
    boost::regex sec_protocol("Sec-WebSocket-Protocol", boost::regex::icase);
    boost::regex sec_ws_version("Sec-WebSocket-Version", boost::regex::icase);
};

namespace server {

typedef boost::char_separator<char> char_separator;
typedef boost::tokenizer<char_separator> tokenizer;

HandShakeRequest::ptr HandShake::parseReqHeader(ByteBuffer* buf)
{
    HandShakeRequest::ptr req = new HandShakeRequest;

    std::string p_str(buf->begin(), buf->end());
    std::stringstream ss(p_str);

    std::string tmp_str;
    std::getline(ss, tmp_str);
//    std::cout << tmp_str << std::endl;

    if (!getFirstLine(tmp_str, req)) {
        return NULL;
    }

    while (std::getline(ss, tmp_str)) {
//        std::cout << tmp_str << std::endl;

        if (!readHeaderLine(tmp_str, req)) {
            return NULL;
        }
    }

    return req;
}

// private member function
bool HandShake::getFirstLine(const std::string& str, 
    HandShakeRequest::ptr req)
{
    char* pos = strstr(str.c_str(), " ");

    if (pos) {
        size_t m_len = pos - str.c_str();
        std::string method(str.c_str(), m_len);
        req->method = method;

        pos++;
        while (pos[0] == ' ') {
            pos++;
        }

        char* path_pos = strstr(pos, " ");
        if (path_pos) {
            int p_len = path_pos - pos;
            std::string path(pos, p_len);
            req->path = path;

            path_pos++;
            while (path_pos[0] == ' ') {
                path_pos++;
            }

            char* http_pos = path_pos;
            if (http_pos) {
                std::string http_ver(path_pos);

                if (http_ver.size() >= 8 && http_ver.substr(0, 5) == "HTTP/") {
                    http_ver = http_ver.substr(5, 3);
                    req->http_version = http_ver;
                }
            }
        }
    }

    return true;
}

bool HandShake::readHeaderLine(const std::string& line,
    HandShakeRequest::ptr req)
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

        if (!setHeaderValue(key, value, req)) {
            return false;
        }
    }

    return true;
}

bool HandShake::setHeaderValue(const std::string& key,
    const std::string& value, HandShakeRequest::ptr req)
{
    if (boost::regex_match(key, upgrade)) {
        std::string s = value;
        boost::algorithm::to_lower(s);

        req->upgrade_name = s;
    } else if (boost::regex_match(key, connection)) {
        std::string s = value;
        boost::algorithm::to_lower(s);

        req->connection_name = s;
    } else if (boost::regex_match(key, sec_key)) {
        req->secret_key = value;
    } else if (boost::regex_match(key, sec_protocol)) {
        std::string p_str = value;

        parseProtocols(p_str, req);
    } else if (boost::regex_match(key, sec_ws_version)) {
        req->ws_version = boost::lexical_cast<int>(value);
    }

    return true;
}

void HandShake::parseProtocols(const std::string& p_str, 
    HandShakeRequest::ptr req)
{
    char_separator sep(", ", "", boost::drop_empty_tokens);
    tokenizer tokens(p_str, sep);
    tokenizer::iterator it = tokens.begin();

    for (; it != tokens.end(); ++it) {
        req->protocols.insert(*it);
    }
}

};
