#if defined(TARGET_PLATFORM_IOS_SDK)

#import <Foundation/Foundation.h>

#include "../log_detail.h"

void LogDetail::output(const char* s)
{
    NSString* message = [NSString stringWithCString:s encoding:NSUTF8StringEncoding];
    NSLog(@"%@", message);
}

#endif
