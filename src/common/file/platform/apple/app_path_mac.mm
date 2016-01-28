#if defined(TARGET_OS_MACOSX)

#include "app_path_mac.h"

#import <Foundation/Foundation.h>

BEGIN_NS

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

std::string AppPath::getApplicationSupportDirectory()
{
    NSString* str = [NSSearchPathForDirectoriesInDomains(
        NSApplicationSupportDirectory, NSUserDomainMask, YES) lastObject];

    return std::string([str UTF8String]);
}

END_NS

#endif
