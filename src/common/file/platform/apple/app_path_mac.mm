#if defined(TARGET_OS_MACOSX)

#include "app_path_mac.h"

#import <Foundation/Foundation.h>

std::string AppPath::getHomeDirectory()
{
    NSString* home = NSHomeDirectory();
    
    return std::string([home UTF8String]);
}

std::string AppPath::getResourceDirectory()
{
    NSString* str = [[NSBundle mainBundle] resourcePath];
    
    return std::string([str UTF8String]);
}

#endif
