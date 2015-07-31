#if defined(TARGET_PLATFORM_QT)

#include "../log_detail.h"

#include <QDebug>

void LogDetail::output(const char* s)
{
    qDebug("%s", s);
}

#endif
