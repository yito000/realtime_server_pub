#if !defined(QT_PROJECT) && !defined(ANDROID_PROJECT)

#include "log_detail.h"
#include <stdio.h>

void LogDetail::output(const char* s)
{
    printf("%s\n", s);
}

#endif
