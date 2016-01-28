#ifdef TARGET_PLATFORM_QT

#ifndef FILE_WRITER_QT_H
#define FILE_WRITER_QT_H

#include "fw_env.h"

#include <string>
#include "data_buffer.h"

BEGIN_NS

class FileWriter : public SmartPtr<FileWriter>
{
    friend class FileUtil;

public:
    typedef boost::intrusive_ptr<FileWriter> ptr;
    
    bool writeBinaryToFile(const std::string& filepath, DataBuffer::ptr data_buffer = nullptr);
    bool writeBinaryToFile(const std::string& filepath, const char* buffer, size_t size);
    
    bool writeTextToFile(const std::string& filepath, DataBuffer::ptr data_buffer = nullptr);
    bool writeTextToFile(const std::string& filepath, const std::string& text);
    bool writeTextToFile(const std::string& filepath, const char* buffer, size_t size);
};

END_NS

#endif

#endif
