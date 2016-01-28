#if defined(TARGET_OS_MACOSX)

#ifndef APP_PATH_IOS_H
#define APP_PATH_IOS_H

#include "fw_env.h"

#include <string>

BEGIN_NS

class AppPath
{
public:
    static std::string getHomeDirectory();
    static std::string getResourceDirectory();
    static std::string getApplicationSupportDirectory();
};

END_NS

#endif

#endif
