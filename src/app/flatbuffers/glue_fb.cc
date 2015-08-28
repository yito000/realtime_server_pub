#include "glue_fb.h"

#include "log/logger.h"

void test(int n, const std::string& str, long actor_key)
{
    Logger::log("n=%d, str=%s", n, str.c_str());
}
