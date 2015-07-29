#include "crypt.h"

#include "log/logger.h"

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>

#include <openssl/evp.h>

bool Crypt::sha1(const std::string& key, std::string& result)
{
    unsigned char hash[20] = {0};
    unsigned char* key_p = (unsigned char*)key.c_str();

    unsigned char* ret_p = SHA1(key_p, key.size(), hash);

    if (ret_p) {
        result = std::string((char*)ret_p, 20);

        return true;
    } else {
        return false;
    }
}

bool Crypt::hmacSha1(const std::string& key, 
    const std::string& data, std::string& result)
{
    unsigned int reslen;
    unsigned char res[SHA_DIGEST_LENGTH + 1];
    
    const unsigned char* c_data = (const unsigned char*)data.c_str();
    
    if (HMAC(EVP_sha1(), key.c_str(), key.size(), c_data, data.size(), 
        res, &reslen)) {

        result = "";
        res[SHA_DIGEST_LENGTH] = '\0';
        
        for (int i = 0; i < reslen; i++) {
            char buf[24];
            sprintf(buf, "%x", res[i]);
            
            result.append(buf);
        }
        
        return true;
    } else {
        return false;
    }
}

// private module
static bool aes256_enc_init(const std::string& str_key, 
    const unsigned char* salt, int nrounds, EVP_CIPHER_CTX* e_ctx)
{
    const unsigned char* key_data = (const unsigned char*)str_key.c_str();
    int key_data_len = str_key.size();

    unsigned char key[32], iv[32];

    int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), 
        salt, key_data, key_data_len, nrounds, key, iv);
    if (i != 32) {
        return false;
    }

    EVP_CIPHER_CTX_init(e_ctx);
    EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);

    return true;
}

static bool aes256_enc(EVP_CIPHER_CTX* e, const std::string& data, 
    Crypt::ByteBuffer& result)
{
    const unsigned char* plaintext = (const unsigned char*)data.c_str();
    int len = data.size() + 1;

    int c_len = len + AES_BLOCK_SIZE;
    int f_len = 0;

    unsigned char* ciphertext = new unsigned char[c_len];

    EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, len);
    EVP_EncryptFinal_ex(e, ciphertext + c_len, &f_len);

    len = c_len + f_len;
    result.clear();

    for (int i = 0; i < len; i++) {
        result.push_back(ciphertext[i]);
    }

    delete[] ciphertext;

    return true;
}

// private module
static bool aes256_dec_init(const std::string& str_key, 
    const unsigned char* salt, int nrounds, EVP_CIPHER_CTX* d_ctx)
{
    const unsigned char* key_data = (const unsigned char*)str_key.c_str();
    int key_data_len = str_key.size();

    unsigned char key[32], iv[32];

    int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), 
        salt, key_data, key_data_len, nrounds, key, iv);
    if (i != 32) {
        return false;
    }

    EVP_CIPHER_CTX_init(d_ctx);
    EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

    return true;
}

static bool aes256_dec(EVP_CIPHER_CTX* e, Crypt::ByteBuffer& encrypt_data, 
    std::string& result)
{
    int len = encrypt_data.size();
    int p_len = len;
    int f_len = 0;

    unsigned char* ciphertext = new unsigned char[len];
    unsigned char* plaintext = new unsigned char[len];

    for (int i = 0; i < encrypt_data.size(); i++) {
        ciphertext[i] = encrypt_data[i];
    }

    EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, len);
    EVP_DecryptFinal_ex(e, plaintext + p_len, &f_len);

    result = (const char*)plaintext;

    delete[] ciphertext;
    delete[] plaintext;

    return true;
}

// public member function
void Crypt::aes256KeyAndIV(const std::string& str_key, 
    const unsigned char* salt, int nrounds, 
    ByteBuffer& out_key, ByteBuffer& out_iv)
{
    const unsigned char* key_data = (const unsigned char*)str_key.c_str();
    int key_data_len = str_key.size();
    
    const int LENGTH = 32;
    const int IV_LENGTH = 16;
    unsigned char key[LENGTH], iv[LENGTH];
    
    int iret = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), 
        salt, key_data, key_data_len, nrounds, key, iv);
    if (iret != LENGTH) {
        return;
    }
    
    for (int i = 0; i < LENGTH; i++) {
        out_key.push_back(key[i]);
    }

    for (int i = 0; i < IV_LENGTH; i++) {
        out_iv.push_back(iv[i]);
    }
}

bool Crypt::aes256Encode(const std::string& str_key, const std::string& data, 
    const unsigned char* salt, int nrounds, ByteBuffer& result)
{
    EVP_CIPHER_CTX en;
    if (!aes256_enc_init(str_key, salt, nrounds, &en)) {
        return false;
    }

    aes256_enc(&en, data, result);

    return true;
}

bool Crypt::aes256Decode(const std::string& str_key, ByteBuffer& encrypt_data, 
    const unsigned char* salt, int nrounds, std::string& result)
{
    EVP_CIPHER_CTX de;
    if (!aes256_dec_init(str_key, salt, nrounds, &de)) {
        return false;
    }

    aes256_dec(&de, encrypt_data, result);

    return true;
}

