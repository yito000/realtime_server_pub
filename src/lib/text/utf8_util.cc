#include "utf8_util.h"

size_t utf8_strlen(const unsigned char* str)
{
    unsigned char* pos = const_cast<unsigned char*>(str);
    size_t len = 0;
    
    while (pos[0] != '\0') {
        if (pos[0] >= 0x00 && pos[0] <= 0x7f) {
            pos += 1;
            len += 1;
        } else if (pos[0] >= 0xc0 && pos[0] <= 0xdf &&
            pos[1] >= 0x80 && pos[1] <= 0xbf) {
            
            pos += 2;
            len += 1;
        } else if (pos[0] >= 0xe0 && pos[0] <= 0xef &&
            pos[1] >= 0x80 && pos[1] <= 0xbf &&
            pos[2] >= 0x80 && pos[2] <= 0xbf) {
            
            pos += 3;
            len += 1;
        } else {
            pos += 4;
            len += 1;
        }
    }
    
    return len;
}

void utf8_strncpy(unsigned char* str1, const unsigned char* str2, size_t n)
{
    unsigned char* src_pos = const_cast<unsigned char*>(str2);
    size_t len = 0;
    int pos = 0;
    
    while (len < n) {
        if (src_pos[0] == '\0') {
            break;
        }
        
        if (src_pos[0] >= 0x00 && src_pos[0] <= 0x7f) {
            str1[pos] = src_pos[0];
            
            src_pos += 1;
            pos += 1;
            len += 1;
        } else if (src_pos[0] >= 0xc0 && src_pos[0] <= 0xdf &&
            src_pos[1] >= 0x80 && src_pos[1] <= 0xbf) {
            
            str1[pos] = src_pos[0];
            str1[pos + 1] = src_pos[1];
            
            src_pos += 2;
            pos += 2;
            len += 1;
        } else if (src_pos[0] >= 0xe0 && src_pos[0] <= 0xef &&
            src_pos[1] >= 0x80 && src_pos[1] <= 0xbf &&
            src_pos[2] >= 0x80 && src_pos[2] <= 0xbf) {
            
            str1[pos] = src_pos[0];
            str1[pos + 1] = src_pos[1];
            str1[pos + 2] = src_pos[2];
            
            src_pos += 3;
            pos += 3;
            len += 1;
        } else {
            str1[pos] = src_pos[0];
            str1[pos + 1] = src_pos[1];
            str1[pos + 2] = src_pos[2];
            str1[pos + 3] = src_pos[3];
            
            src_pos += 4;
            pos += 4;
            len += 1;
        }
    }
    
    str1[pos] = '\0';
}

int utf8_get_bytes_per_unit(const unsigned char* character)
{
    if (character[0] >= 0x00 && character[0] <= 0x7f) {
        return 1;
    } else if (character[0] >= 0xc0 && character[0] <= 0xdf &&
        character[1] >= 0x80 && character[1] <= 0xbf) {
        
        return 2;
    } else if (character[0] >= 0xe0 && character[0] <= 0xef &&
        character[1] >= 0x80 && character[1] <= 0xbf &&
        character[2] >= 0x80 && character[2] <= 0xbf) {
        
        return 3;
    } else {
        return 4;
    }
}
