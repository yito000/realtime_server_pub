#if defined(ANDROID_PROJECT)

#include "log_detail.h"

void LogDetail::output(const char* s)
{
    printf("%s\n", s);
}

#endif
