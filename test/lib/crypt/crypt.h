#ifndef MOD_CRYPT_H
#define MOD_CRYPT_H

#include <vector>

class Crypt
{
public:
    typedef std::vector< unsigned char > ByteBuffer;

    bool sha1(const std::string& key, std::string& result);
    bool hmacSha1(const std::string& key, const std::string& data, 
        std::string& result);
    bool aes256Encode(const std::string& str_key, const std::string& data, 
        const unsigned char* salt, int nrounds, ByteBuffer& result);
    bool aes256Decode(const std::string& str_key, ByteBuffer& encrypt_data, 
        const unsigned char* salt, int nrounds, std::string& result);
    
    void aes256KeyAndIV(const std::string& str_key, 
        const unsigned char* salt, int nrounds, 
        ByteBuffer& out_key, ByteBuffer& out_iv);
};

#endif
