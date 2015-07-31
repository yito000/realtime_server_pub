#ifndef TARGET_PLATFORM_QT

#include "file_stream_general.h"
#include "log/logger.h"

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
}

DataBuffer::ptr FileStream::readAll()
{
    if (!fp) {
        return nullptr;
    }

    seek(0);

    unsigned char* buf = new unsigned char[filesize];
    fread(buf, filesize, 1, fp);
    
    auto ret = new DataBuffer(buf, filesize);

    return ret;
}

size_t FileStream::readLine(char* buf, size_t read_size)
{
    if (!fp) {
        return 0;
    }
    
    fgets(buf, read_size, fp);
    
    return strlen(buf);
}

size_t FileStream::read(char* buf, size_t read_size)
{
    if (!fp) {
        return 0;
    }
    
    auto ret = fread(buf, read_size, 1, fp);

    return read_size * ret;
}

bool FileStream::seek(size_t pos)
{
    if (!fp) {
        return false;
    }
    
    return fseek(fp, pos, SEEK_SET) == 0;
}

size_t FileStream::pos() const
{
    if (!fp) {
        return -1;
    }
    
    return ftell(fp);
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
    
    return ferror(fp);
}

void FileStream::clearError()
{
    if (fp) {
        clearerr(fp);
    }
}

int FileStream::handle()
{
    return fileno(fp);
}

void FileStream::attachIstream(boost::iostreams::stream<boost::iostreams::file_descriptor_source>& is)
{
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
    filename = _filename;
    filesize = 0;
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
    }
}

#endif
