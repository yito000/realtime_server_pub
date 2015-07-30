#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger
{
public:
    static void log(const char* str, ...);
    static void log(const std::string str, ...);
    
    static void debug(const char* str, ...);
    static void debug(const std::string str, ...);
};

#endif
