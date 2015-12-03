#if defined(TARGET_PLATFORM_ANDROID_NDK)

#include "../log_detail.h"

#include <string>
#include <android/log.h>

#include "text/utf8_util.h"

namespace {
    const int MAX_LOG_LENGTH = 1000;
};

void LogDetail::output(const char* s)
{
    auto len = strlen(s);
    
    if (len > MAX_LOG_LENGTH) {
        int i = 0;
        char* current_p = const_cast<char*>(s);
        
        while (len > 0) {
            int tmp_len = 0;
            char* tmp_cur_p = current_p;
            
            while (tmp_len < MAX_LOG_LENGTH) {
                if (*tmp_cur_p == '\0') {
                    break;
                }
                
                int ul = utf8_get_bytes_per_unit((unsigned char*)tmp_cur_p);
                
                tmp_cur_p += ul;
                tmp_len += ul;
            }
            
            size_t ss_len = (size_t)(tmp_cur_p - current_p);
            std::string ss(current_p, ss_len);
            
            __android_log_print(ANDROID_LOG_INFO, "gameapp", "%s", ss.c_str());
            
            len -= ss_len;
            current_p += ss_len;
        }
    } else {
        __android_log_print(ANDROID_LOG_INFO, "gameapp", "%s", s);
    }
}

#endif
