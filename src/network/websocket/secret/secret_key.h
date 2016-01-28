#ifndef SECRET_KEY_H
#define SECRET_KEY_H

#include "fw_env.h"

#include <string>

BEGIN_NS

class SecretKey
{
public:
    static std::string generate();
    static std::string calcResponse(const std::string& sec_key);
};

END_NS

#endif
