#include "logger.h"
#include "detail/log_detail.h"

#include <stdarg.h>

const int LOGBUF_SIZE = 20480;

void Logger::log(const char* str, ...)
{
    char buf[LOGBUF_SIZE];

    va_list args;

    va_start(args, str);
    vsnprintf(buf, sizeof(buf), str, args);
    va_end(args);

    LogDetail::output(buf);
}

void Logger::log(const std::string str, ...)
{
    char buf[LOGBUF_SIZE];

    va_list args;

    va_start(args, str);
    vsnprintf(buf, sizeof(buf), str.c_str(), args);
    va_end(args);

    LogDetail::output(buf);
}

void Logger::debug(const char* str, ...)
{
#ifndef NDEBUG
    char buf[LOGBUF_SIZE];

    va_list args;

    va_start(args, str);
    vsnprintf(buf, sizeof(buf), str, args);
    va_end(args);

    LogDetail::output(buf);
#endif
}

void Logger::debug(const std::string str, ...)
{
#ifndef NDEBUG
    char buf[LOGBUF_SIZE];

    va_list args;

    va_start(args, str);
    vsnprintf(buf, sizeof(buf), str.c_str(), args);
    va_end(args);

    LogDetail::output(buf);
#endif
}
