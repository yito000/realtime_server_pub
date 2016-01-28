#ifndef TARGET_PLATFORM_QT

#include "file_stream_general.h"

#ifdef TARGET_PLATFORM_ANDROID_NDK
#include "platform_common.h"

#include <android/asset_manager.h>
#endif

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

FileStream::~FileStream()
{
    close();
}

bool FileStream::isOpen() const
{
    return fp;
}

void FileStream::close()
{
    if (fp) {
        fclose(fp);
        filesize = 0;
        
        fp = nullptr;
    }

#ifdef TARGET_PLATFORM_ANDROID_NDK
    if (android_asset) {
        AAsset_close(android_asset);
        android_asset = nullptr;
    }
#endif
}

DataBuffer::ptr FileStream::readAll()
{
    if (!fp) {
        return nullptr;
    }

    seek(0);
    
    // TODO: compressed asset (Android)
    unsigned char* buf = new unsigned char[filesize];
    fread(buf, filesize, 1, fp);
    
    seek(0);
    
    auto ret = new DataBuffer(buf, filesize);

    return ret;
}

size_t FileStream::readLine(char* buf, size_t read_size)
{
    if (!fp) {
        return 0;
    }
    
    if (read_size > filesize) {
        read_size = filesize;
    }
    
    if (read_size <= 0) {
        return 0;
    }
    
    // TODO: compressed asset (Android)
    fgets(buf, read_size, fp);
    
    return strlen(buf);
}

size_t FileStream::read(char* buf, size_t read_size)
{
    if (!fp) {
        return 0;
    }
    
    if (read_size > filesize) {
        read_size = filesize;
    }
    
    if (read_size <= 0) {
        return 0;
    }
    
    // TODO: compressed asset (Android)
    auto ret = fread(buf, 1, read_size, fp);
    return ret;
}

bool FileStream::seek(size_t pos)
{
    if (!fp) {
        return false;
    }
    
    // TODO: compressed asset (Android)
    bool ret = fseek(fp, pos, SEEK_SET) == 0;
    return ret;
}

size_t FileStream::pos() const
{
    if (!fp) {
        return -1;
    }
    
    // TODO: compressed asset (Android)
    auto ret = ftell(fp);
    return ret;
}

bool FileStream::reset()
{
    return true;
}

size_t FileStream::getSize() const
{
    if (!fp) {
        return -1;
    }
    
    return filesize;
}

int FileStream::error() const
{
    if (!fp) {
        return 0;
    }
    
    // TODO: compressed asset (Android)
    return ferror(fp);
}

void FileStream::clearError()
{
    // TODO: compressed asset (Android)
    if (fp) {
        clearerr(fp);
    }
}

int FileStream::handle()
{
    // TODO: compressed asset (Android)
    return fileno(fp);
}

void FileStream::attachIstream(boost::iostreams::stream<boost::iostreams::file_descriptor_source>& is)
{
    // TODO: compressed asset (Android)
    boost::iostreams::file_descriptor_source src;
    src.open(fileno(fp), boost::iostreams::never_close_handle);
    
    if (!is.is_open()) {
        is.open(src);
    }
}

// private member function
FileStream::FileStream(const std::string& _filename)
{
    fp = nullptr;
    filename = CONV_PATH(_filename);
    filesize = 0;
    
#ifdef TARGET_PLATFORM_ANDROID_NDK
    android_asset = nullptr;
#endif
}

void FileStream::open()
{
    if (!fp) {
        fp = fopen(filename.c_str(), "rb");
        
        if (fp) {
            fseek(fp, 0, SEEK_END);
            
            filesize = ftell(fp);
            fseek(fp, 0, SEEK_SET);
        }
        
#ifdef TARGET_PLATFORM_ANDROID_NDK
        /*
        if (!fp) {
            AAssetManager* asset_manager = PlatformCommon::getInstance()->getAAssetManager();
            
            android_asset = AAssetManager_open(asset_manager, filename.c_str(), AASSET_MODE_UNKNOWN);
            if (android_asset) {
                off_t start, length;
                
                auto fd = AAsset_openFileDescriptor(android_asset, &start, &length);
                fp = fdopen(fd, "rb");
                
                if (fp) {
//                    data_offset = start;
                    filesize = length;
                    
                    seek(0);
                }
            }
        }
        */
#endif
    }
}

END_NS

#endif
