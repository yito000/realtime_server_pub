#ifdef TARGET_PLATFORM_QT

#include "file_stream_qt.h"
#include <QFile>

#include "log/logger.h"

FileStream::~FileStream()
{
    close();
}

bool FileStream::isOpen() const
{
    return fp && fp->isOpen();
}

void FileStream::close()
{
    if (fp) {
        fp->close();
        
        delete fp;
        fp = nullptr;
    }
}

DataBuffer::ptr FileStream::readAll()
{
    if (!fp || !fp->isOpen() || fp->size() <= 0) {
        return nullptr;
    }
    
    seek(0);
    
    unsigned char* buf = new unsigned char[fp->size()];
    size_t file_size = fp->size();
    
    fp->read((char*)buf, file_size);
    
    auto ret = new DataBuffer(buf, file_size);

    return ret;
}

size_t FileStream::readLine(char* buf, size_t read_size)
{
    if (!fp) {
        return 0;
    }
    
    return fp->readLine(buf, read_size);
}

size_t FileStream::read(char* buf, size_t read_size)
{
    if (!fp) {
        return 0;
    }
    
    return fp->read(buf, read_size);
}

bool FileStream::seek(size_t pos)
{
    if (!fp) {
        return false;
    }
    
    return fp->seek(pos);
}

size_t FileStream::pos() const
{
    if (!fp) {
        return -1;
    }
    
    return fp->pos();
}

bool FileStream::reset()
{
    if (fp) {
        fp->reset();
    }
    
    return true;
}

size_t FileStream::getSize() const
{
    if (!fp) {
        return -1;
    }
    
    return fp->size();
}

int FileStream::error() const
{
    if (!fp) {
        return 0;
    }
    
    return (int)fp->error();
}

void FileStream::clearError()
{
    if (fp) {
        fp->unsetError();
    }
}

int FileStream::handle()
{
    return fp->handle();
}

void FileStream::attachIstream(boost::iostreams::stream<boost::iostreams::file_descriptor_source>& is)
{
    boost::iostreams::file_descriptor_source src;
    src.open(fp->handle(), boost::iostreams::never_close_handle);
    
    if (!is.is_open()) {
        is.open(src);
    }
}

// private member function
FileStream::FileStream(const std::string& _filename)
{
    fp = new QFile(_filename.c_str());
}

void FileStream::open()
{
    if (fp) {
        fp->open(QIODevice::ReadOnly);
    }
}

#endif
