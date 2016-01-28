#if defined(TARGET_OS_WIN64)

#ifndef APP_PATH_WIN_H
#define APP_PATH_WIN_H

#include "fw_env.h"

#include <string>

BEGIN_NS

class AppPath
{
public:
    static std::string getResourceDirectory();
    static std::string getSaveDirectory();
};

END_NS

#endif

#endif
