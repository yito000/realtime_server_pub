#ifndef UTF8_UTIL_H
#define UTF8_UTIL_H

#include <stddef.h>

size_t utf8_strlen(const unsigned char* str);
void utf8_strncpy(unsigned char* str1, const unsigned char* str2, size_t n);

int utf8_get_bytes_per_unit(const unsigned char* character);

#endif
