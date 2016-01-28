#if defined(TARGET_OS_APPLE_IOS)

#include "app_path_ios.h"

#import <Foundation/Foundation.h>

BEGIN_NS

std::string AppPath::getHomeDirectory()
{
    NSString* home = NSHomeDirectory();
    
    return std::string([home UTF8String]);
}

std::string AppPath::getBundleDirectory()
{
    NSString* str = [[NSBundle mainBundle] bundlePath];
    
    return std::string([str UTF8String]);
}

END_NS

#endif
