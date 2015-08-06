#include "base64.h"

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

using namespace boost::archive::iterators;
typedef base64_from_binary< transform_width<std::string::const_iterator, 6, 8> > base64_encoder;
typedef transform_width< binary_from_base64<std::string::const_iterator>, 8, 6> base64_decoder;

std::string Base64::encode(const std::string& str)
{
    return encode(str.c_str(), str.size());
}

std::string Base64::encode(const char* ctext, int len)
{
    std::string buf(ctext, len);
    std::string ret(base64_encoder(buf.begin()), base64_encoder(buf.end()));
    
    return ret;
}

void Base64::decode(const std::string& text, ByteBuffer& out_buffer)
{
    out_buffer.clear();
    out_buffer.insert(out_buffer.begin(), base64_decoder(text.begin()), base64_decoder(text.end()));
}

std::string Base64::decode(const std::string& text)
{
    ByteBuffer out_buffer;
    decode(text, out_buffer);
    
    std::string ret(out_buffer.begin(), out_buffer.end());
    
    return ret;
}
