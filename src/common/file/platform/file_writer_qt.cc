#ifdef TARGET_PLATFORM_QT

#include "file_writer_qt.h"

#include <QFile>
#include <QTextStream>
#include <QDataStream>

#include "log/logger.h"

BEGIN_NS

bool FileWriter::writeBinaryToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    if (data_buffer) {
        return writeBinaryToFile(filepath, (const char*)data_buffer->getBuffer(), data_buffer->getSize());
    } else {
        return writeBinaryToFile(filepath, nullptr, 0);
    }
}

bool FileWriter::writeBinaryToFile(const std::string& filepath, const char* buffer, size_t size)
{
    QFile f(filepath.c_str());
    
    f.open(QIODevice::WriteOnly);
    if (!f.isOpen()) {
        return false;
    }
    
    QDataStream out(&f);
    if (buffer && size > 0) {
        out.writeRawData(buffer, size);
    }
    
    auto write_status = out.status();
    f.close();
    
    if (write_status != QDataStream::WriteFailed) {
        return true;
    } else {
        return false;
    }
}

bool FileWriter::writeTextToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    if (data_buffer) {
        return writeTextToFile(filepath, (const char*)data_buffer->getBuffer(), data_buffer->getSize());
    } else {
        return writeTextToFile(filepath, std::string());
    }
}

bool FileWriter::writeTextToFile(const std::string& filepath, const std::string& text)
{
    QFile f(filepath.c_str());
    
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!f.isOpen()) {
        return false;
    }
    
    QTextStream out(&f);
    out << text.c_str();
    
    auto write_status = out.status();
    f.close();
    
    if (write_status != QTextStream::WriteFailed) {
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
