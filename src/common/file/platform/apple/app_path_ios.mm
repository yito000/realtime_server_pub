#if defined(TARGET_OS_IOS)

#include "app_path_ios.h"

#import <Foundation/Foundation.h>

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

#endif
