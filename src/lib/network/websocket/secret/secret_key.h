#ifndef SECRET_KEY_H
#define SECRET_KEY_H

#include <string>

class SecretKey
{
public:
    static std::string generate();
    static std::string calcResponse(const std::string& sec_key);
};

#endif

