#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

const int LOGBUF_SIZE = 2048;

void Logger::log(const char* str, ...)
{
    char buf[LOGBUF_SIZE];

    va_list args;

    va_start(args, str);
    vsnprintf(buf, sizeof(buf), str, args);
    va_end(args);

    printf("%s\n", buf);
}

void Logger::log(const std::string str, ...)
{
    char buf[LOGBUF_SIZE];

    va_list args;

    va_start(args, str);
    vsnprintf(buf, sizeof(buf), str.c_str(), args);
    va_end(args);

    printf("%s\n", buf);
}

void Logger::debug(const char* str, ...)
{
#ifdef LIB_DEBUG
    char buf[LOGBUF_SIZE];

    va_list args;

    va_start(args, str);
    vsnprintf(buf, sizeof(buf), str, args);
    va_end(args);

    printf("%s\n", buf);
#endif
}

void Logger::debug(const std::string str, ...)
{
#ifdef LIB_DEBUG
    char buf[LOGBUF_SIZE];

    va_list args;

    va_start(args, str);
    vsnprintf(buf, sizeof(buf), str.c_str(), args);
    va_end(args);

    printf("%s\n", buf);
#endif
}

