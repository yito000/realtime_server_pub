#include "base64.h"

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

using namespace boost::archive::iterators;
typedef base64_from_binary< transform_width<std::string::const_iterator, 6, 8> > base64_encoder;
typedef transform_width< binary_from_base64<std::string::const_iterator>, 8, 6> base64_decoder;

namespace {
    const std::string base64_padding[] = {"", "===", "==", "="};
};

std::string Base64::encode(const std::string& str)
{
    return encode(str.c_str(), (int)str.size());
}

std::string Base64::encode(const char* ctext, int len)
{
    std::string buf(ctext, len);
    std::string ret(base64_encoder(buf.begin()), base64_encoder(buf.end()));
    
    if (base64_padding[ret.size() % 4] != "") {
        ret += base64_padding[ret.size() % 4];
    }
    
    return ret;
}

void Base64::decode(const std::string& text, ByteBuffer& out_buffer)
{
    out_buffer.clear();
    
    auto s_size = text.size();
    if (s_size && text[s_size - 1] == '=') {
        s_size--;
        if (s_size && text[s_size - 1] == '=') {
            s_size--;
            if (s_size && text[s_size - 1] == '=') {
                s_size--;
            }
        }
    }
    
    if (s_size == 0) {
        return;
    }
    
    std::string out_text(text.c_str(), s_size);
    
    out_buffer.insert(out_buffer.begin(), base64_decoder(out_text.begin()), base64_decoder(out_text.end()));
}

std::string Base64::decode(const std::string& text)
{
    ByteBuffer out_buffer;
    decode(text, out_buffer);
    
    std::string ret(out_buffer.begin(), out_buffer.end());
    
    return ret;
}
