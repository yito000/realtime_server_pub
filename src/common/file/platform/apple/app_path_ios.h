#if defined(TARGET_OS_IOS)

#ifndef APP_PATH_IOS_H
#define APP_PATH_IOS_H

#include <string>

class AppPath
{
public:
    static std::string getHomeDirectory();
    static std::string getBundleDirectory();
};

#endif

#endif
