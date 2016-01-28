#if defined(TARGET_OS_WIN64)

#include "app_path_win.h"
#include "log/logger.h"

#include <windows.h>

BEGIN_NS

std::string AppPath::getResourceDirectory()
{
    wchar_t s[MAX_PATH];
    GetCurrentDirectory(sizeof(s) / sizeof(s[0]), s);
    
    char cs[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, s, MAX_PATH, cs, MAX_PATH, NULL, NULL);

	auto ret = std::string(cs) + "\\resources";

    return ret;
}

std::string AppPath::getSaveDirectory()
{
    wchar_t s[MAX_PATH];
    GetCurrentDirectory(sizeof(s) / sizeof(s[0]), s);
    
    char cs[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, s, MAX_PATH, cs, MAX_PATH, NULL, NULL);
    
	auto ret = std::string(cs) + "\\save";

    return ret;
}

END_NS

#endif
