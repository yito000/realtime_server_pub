#include "http_header_parser.h"

#include <iostream>
#include <regex>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "time/time_formatter.h"

namespace {
    std::regex last_modified("Last-Modified", std::regex_constants::icase);
    std::regex expires("Expires", std::regex_constants::icase);
    std::regex etag("ETag", std::regex_constants::icase);
    std::regex content_length("Content-Length", std::regex_constants::icase);
    std::regex location("Location", std::regex_constants::icase);
    std::regex transfer_encondig("Transfer-Encoding", std::regex_constants::icase);
    std::regex content_type("Content-Type", std::regex_constants::icase);
    std::regex cache_control("Cache-Control", std::regex_constants::icase);
    std::regex content_encoding("Content-Encoding", std::regex_constants::icase);

    std::regex cache_no_cache("no-cache", std::regex_constants::icase);
    std::regex cache_no_store("no-store", std::regex_constants::icase);
    std::regex cache_must_revalidate("must-revalidate", std::regex_constants::icase);
    std::regex cache_private("private", std::regex_constants::icase);
    std::regex cache_public("public", std::regex_constants::icase);
    
    std::regex rgx_content_type(".+;.+");
    
    std::regex date_format_rgx("[A-Z][a-z]+, \\d+ [A-Z][a-z]+ \\d+ \\d+:\\d\\d:\\d\\d");
}

typedef boost::char_separator<char> char_separator;
typedef boost::tokenizer<char_separator> tokenizer;

HttpHeaderParser::HttpHeaderParser() : maxage_delimiter("[^ =]+=[\\d]+")
{
    //
}

bool HttpHeaderParser::getHeader(const std::string& str, HttpResponseHeader& header)
{
    std::stringstream stream;
    stream << str;
    
    std::string line;
    std::getline(stream, line);
    
    if (!getResponseCode(line, header)) {
        return false;
    }
    
    if (header.status_code != 200) {
        return false;
    }
    
    while (std::getline(stream, line)) {
        if (!readLine(line, header)) {
            return false;
        }
    }
    
    return true;
}

// private member function
bool HttpHeaderParser::getResponseCode(const std::string& str, HttpResponseHeader& header)
{
    std::string tmp = str.c_str();
    
    char_separator sep(" ", "", boost::drop_empty_tokens);
    tokenizer tokens(tmp, sep);
    tokenizer::iterator it = tokens.begin();
    
    if (it != tokens.end()) {
        if ((*it).size() >= 8 && (*it).substr(0, 5) == "HTTP/") {
            header.http_version = (*it).substr(5, 3).c_str();
        } else {
            return false;
        }
        
        ++it;
        
        if (it != tokens.end()) {
            try {
                header.status_code = boost::lexical_cast<int>(*it);
            } catch (boost::bad_lexical_cast& e) {
                return false;
            }
        }
    } else {
        return false;
    }
    
    return true;
}

bool HttpHeaderParser::readLine(const std::string& line, HttpResponseHeader& header)
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
        
        if (!setHeaderValue(key, value, header)) {
            return false;
        }
    } else {
        return false;
    }
    
    return true;
}

bool HttpHeaderParser::setHeaderValue(const std::string& key, const std::string& value, HttpResponseHeader& header)
{
//    Logger::debug("%s: %s", key.c_str(), value.c_str());
    
    if (std::regex_match(key, last_modified)) {
        header.last_modified = getDate(value);
    } else if (std::regex_match(key, expires)) {
        header.expires = getDate(value);
    } else if (std::regex_match(key, etag)) {
        header.etag = value;
    } else if (std::regex_match(key, content_length)) {
        header.content_length = boost::lexical_cast<size_t>(value);
    } else if (std::regex_match(key, location)) {
        header.location = value;
    } else if (std::regex_match(key, transfer_encondig)) {
        header.encoding_chunked = true;
    } else if (std::regex_match(key, content_type)) {
        setContentType(header, value);
    } else if (std::regex_match(key, content_encoding)) {
        setContentEncoding(header, value);
    } else if (std::regex_match(key, cache_control)) {
        header.cache_control |= getCacheControl(value, header);
    }
    
    return true;
}

boost::posix_time::ptime HttpHeaderParser::getDate(const std::string& str)
{
    std::smatch m;
    boost::posix_time::ptime ptime;
    
    if (std::regex_search(str, m, date_format_rgx)) {
        std::string ret = m.str().c_str();
        
        ptime = TimeFormatter::ptimeFromString("%a, %d %b %Y %H:%M:%S", ret);
    } else {
//        Logger::debug("unmatch");
    }
    
    return ptime;
}

HttpResponseHeader::CacheControl HttpHeaderParser::getCacheControl(const std::string& cache_control, HttpResponseHeader& header)
{
    std::string ss = cache_control.c_str();
    
    if (std::regex_match(cache_control, cache_no_cache)) {
        return HttpResponseHeader::NO_CACHE;
    } else if (std::regex_match(cache_control, cache_no_store)) {
        return HttpResponseHeader::NO_STORE;
    } else if (std::regex_match(ss, maxage_delimiter) ) {
        char* pos = strstr(ss.c_str(), "=");
        pos++;
        
        header.cache_maxage = boost::lexical_cast<int>(pos);
        
        return HttpResponseHeader::CACHE_CONTROLER_MAXAGE;
    } else if (std::regex_match(cache_control, cache_must_revalidate)) {
        return HttpResponseHeader::CACHE_MUST_REVALIDATE;
    } else if (std::regex_match(cache_control, cache_private)) {
        return HttpResponseHeader::CACHE_PRIVATE;
    } else if (std::regex_match(cache_control, cache_public)) {
        return HttpResponseHeader::CACHE_PUBLIC;
    } else {
        return HttpResponseHeader::UNKNOWN_CACHE_CONTROL;
    }
}

void HttpHeaderParser::setContentType(HttpResponseHeader& header, const std::string& value)
{
    if (std::regex_match(value, rgx_content_type)) {
        std::string s = value.c_str();
        
        char_separator sep(";");
        tokenizer tokenizer(s, sep);
        tokenizer::iterator it = tokenizer.begin();
        int cnt = 0;
        
        for (; it != tokenizer.end(); ++it) {
            cnt++;
        }
        
        if (cnt < 2) {
            header.content_type = value;
        } else {
            it = tokenizer.begin();
            
            header.content_type = (*it).c_str();
            ++it;
            
            std::string charset = *it;
            setCharSet(header, charset);
        }
    } else {
        header.content_type = value;
    }
}

void HttpHeaderParser::setCharSet(HttpResponseHeader& header, const std::string& charset_str)
{
    char_separator sep("=");
    tokenizer tokenizer(charset_str, sep);
    tokenizer::iterator it = tokenizer.begin();
    int cnt = 0;
    
    for (; it != tokenizer.end(); ++it) {
        cnt++;
    }
    
    if (cnt != 2) {
        return;
    }
    
    it = tokenizer.begin();
    if (*it == "charset") {
        ++it;
        
        // todo shift_jis and euc-jp
    }
}

void HttpHeaderParser::setContentEncoding(HttpResponseHeader& header, const std::string& value)
{
    if (value == "gzip") {
        header.content_encoding = HttpResponseHeader::TYPE_GZIP;
    }
}
