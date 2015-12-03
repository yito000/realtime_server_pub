#if !defined(TARGET_PLATFORM_QT) && !defined(TARGET_PLATFORM_IOS_SDK) && !defined(TARGET_PLATFORM_ANDROID_NDK)

#include "../log_detail.h"
#include <stdio.h>

void LogDetail::output(const char* s)
{
    printf("%s\n", s);
}

#endif
