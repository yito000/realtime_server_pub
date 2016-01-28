#ifndef TARGET_PLATFORM_QT

#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include "fw_env.h"

#include <stdio.h>
#include <string>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

#include "data_buffer.h"

#ifdef TARGET_PLATFORM_ANDROID_NDK
#include <android/native_activity.h>
#include <android/asset_manager.h>
#endif

BEGIN_NS

// read only class
class FileStream : public SmartPtr<FileStream>
{
    friend class FileUtil;

public:
    typedef boost::intrusive_ptr<FileStream> ptr;

    ~FileStream();

    bool isOpen() const;
    void close();

    DataBuffer::ptr readAll();
    size_t readLine(char* buf, size_t read_size);
    size_t read(char* buf, size_t read_size);

    bool seek(size_t pos);
    size_t pos() const;
    bool reset();

    size_t getSize() const;
    int error() const;
    void clearError();
    
    int handle();
    void attachIstream(boost::iostreams::stream<boost::iostreams::file_descriptor_source>& is);

private:
    FileStream(const std::string& _filename);
    void open();

    FILE* fp;
    std::string filename;
    size_t filesize;
    
#ifdef TARGET_PLATFORM_ANDROID_NDK
    AAsset* android_asset;
#endif
};

END_NS

#endif

#endif
