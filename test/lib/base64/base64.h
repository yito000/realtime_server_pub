#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <vector>

class Base64
{
public:
    typedef std::vector<char> ByteBuffer;
    
    static std::string encode(const std::string& str);
    static std::string encode(const char* ctext, int len);
    
    static void decode(const std::string& text, ByteBuffer& out_buffer);
    static std::string decode(const std::string& text);
};

#endif
