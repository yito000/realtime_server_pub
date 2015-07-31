#if defined(TARGET_OS_MACOSX)

#ifndef APP_PATH_IOS_H
#define APP_PATH_IOS_H

#include <string>

class AppPath
{
public:
    static std::string getHomeDirectory();
    static std::string getResourceDirectory();
};

#endif

#endif
