#ifndef HTTP_RESPONSE_HEADER_H
#define HTTP_RESPONSE_HEADER_H

#include "smart_ptr.hpp"

#include <boost/date_time.hpp>

struct HttpResponseHeader : public SmartPtr<HttpResponseHeader>
{
    typedef boost::intrusive_ptr<HttpResponseHeader> ptr;
    
    enum ConnectionType {
        TYPE_CLOSE = 0, 
        TYPE_KEEPALIVE, 
        TYPE_UPGRADE, 
        UNKNOWN_TYPE
    };
    
    enum ContentEncoding {
        TYPE_GZIP = 0,
        TYPE_NO_ENCODING
    };
    
    enum CacheControl {
        UNKNOWN_CACHE_CONTROL = 0,
        NO_CACHE = 1, 
        CACHE_NO_STORE = 2, 
        CACHE_PUBLIC = 4, 
        CACHE_PRIVATE = 8, 
        CACHE_CONTROLER_MAXAGE = 16,
        CACHE_MUST_REVALIDATE = 32
    };
    
    enum CharSet {
        UTF8 = 0,
        SHIFT_JIS, 
        EUC_JP
    };
    
    std::string http_version;
    int status_code;
    
    boost::posix_time::ptime last_modified;
    boost::posix_time::ptime expires;
    std::string etag;
    
    size_t content_length;
    ContentEncoding content_encoding;
    bool encoding_chunked;
    
    CharSet char_set;
    std::string content_type;
    std::string location;
    
    ConnectionType connect_type;
    
    int cache_control;
    int cache_maxage;
    
    HttpResponseHeader()
    {
        status_code = 0;
        content_length = 0;
        content_encoding = TYPE_NO_ENCODING;
        encoding_chunked = false;

        char_set = UTF8;
        
        connect_type = TYPE_CLOSE;
        cache_control = UNKNOWN_CACHE_CONTROL;
        
        cache_maxage = 0;
    }
};

#endif
