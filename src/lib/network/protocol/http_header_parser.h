#ifndef HTTP_HEADER_PARSER_H
#define HTTP_HEADER_PARSER_H

#include "http_response_header.h"
#include <string>
#include <regex>

#include <boost/date_time.hpp>

class HttpHeaderParser
{
public:
    HttpHeaderParser();
    bool getHeader(const std::string& str, HttpResponseHeader& header);
    
private:
    bool getResponseCode(const std::string& str, HttpResponseHeader& header);
    bool readLine(const std::string& line, HttpResponseHeader& header);
    bool setHeaderValue(const std::string& key, const std::string& value, HttpResponseHeader& header);

    boost::posix_time::ptime getDate(const std::string& str);
    HttpResponseHeader::CacheControl getCacheControl(const std::string& cache_control, HttpResponseHeader& header);
    void setContentType(HttpResponseHeader& header, const std::string& value);
    void setCharSet(HttpResponseHeader& header, const std::string& charset_str);
    void setContentEncoding(HttpResponseHeader& header, const std::string& value);
    
    std::regex maxage_delimiter;
};

#endif
