#ifndef TARGET_PLATFORM_QT

#include "file_writer_general.h"

#include "log/logger.h"

BEGIN_NS

#if defined(TARGET_OS_WIN64)
static std::string ReplacePathSeparator(std::string s1, std::string s2, std::string s3)
{
    std::string::size_type pos(s1.find(s2));
    while (pos != std::string::npos) {
        s1.replace(pos, s2.length(), s3);
        pos = s1.find(s2, pos + s3.length());
    }

    return s1;
}
#endif

#if defined(TARGET_OS_WIN64)
#define CONV_PATH(s) ReplacePathSeparator(s, "/", "\\")
#else
#define CONV_PATH(s) s
#endif

bool FileWriter::writeBinaryToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    return writeBinaryToFile(filepath, (const char*)data_buffer->getBuffer(), data_buffer->getSize());
}

bool FileWriter::writeBinaryToFile(const std::string& filepath, const char* buffer, size_t size)
{
    FILE* fp = fopen(CONV_PATH(filepath).c_str(), "wb");
    if (!fp) {
        return false;
    }
    
    int ret = fwrite(buffer, size, 1, fp);
    fclose(fp);
    
    if (ret == 1) {
        return true;
    } else {
        return false;
    }
}

bool FileWriter::writeTextToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    return writeTextToFile(filepath, (const char*)data_buffer->getBuffer(), data_buffer->getSize());
}

bool FileWriter::writeTextToFile(const std::string& filepath, const std::string& text)
{
    FILE* fp = fopen(CONV_PATH(filepath).c_str(), "wt");
    if (!fp) {
        return false;
    }
    
    int ret = fwrite(text.c_str(), text.size(), 1, fp);
    fclose(fp);
    
    if (ret == 1) {
        return true;
    } else {
        return false;
    }
}

bool FileWriter::writeTextToFile(const std::string& filepath, const char* buffer, size_t size)
{
    return writeTextToFile(filepath, std::string(buffer, size));
}

END_NS

#endif
