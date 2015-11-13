#include "secret_key.h"

#include "base64/base64.h"
#include "crypt/crypt.h"
#include "random/random.h"

#include "log/logger.h"

namespace {
    Random s_rand(std::chrono::system_clock::now().
        time_since_epoch().count() / 1000 / 1000);

    const char* GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
};

std::string SecretKey::generate()
{
    std::string rand_str;
    s_rand.getAllAscii(16, rand_str);

    auto enc_str = Base64::encode(rand_str);

    Logger::debug("random generated string=%s", rand_str.c_str());
    Logger::debug("base64 encoded string=%s", enc_str.c_str());

    return enc_str;
}

std::string SecretKey::calcResponse(const std::string& sec_key)
{
    auto src_str = sec_key + GUID;

    std::string sha1_str;
    Crypt c;

    auto sha1_ret = c.sha1(src_str, sha1_str);
    if (!sha1_ret) {
        return "";
    }

    auto enc_str = Base64::encode(sha1_str);

    Logger::debug("sha1 string=%s", sha1_str.c_str());
    Logger::debug("calc base64 encoded string=%s", enc_str.c_str());

    return enc_str;
}
