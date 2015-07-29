#if defined(QT_PROJECT)

#include "log_detail.h"

#include <QDebug>

void LogDetail::output(const char* s)
{
    qDebug("%s", s);
}

#endif
