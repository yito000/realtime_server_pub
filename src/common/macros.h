#ifndef MACROS_H
#define MACROS_H

#include <assert.h>
#include "log/logger.h"

#define ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            Logger::debug(msg); \
            assert(cond); \
        } \
    } while (0)

#define LOGI(msg, ...) Logger::log(msg, __VA_ARGS__)
#define LOGD(msg, ...) Logger::debug(msg, __VA_ARGS__)

#endif
