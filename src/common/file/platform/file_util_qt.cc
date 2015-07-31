#ifdef TARGET_PLATFORM_QT

#include "file_util.h"

#include <QFile>
#include <QDir>
#include <QDirIterator>

#include <QTextStream>
#include <QDataStream>

#include "log/logger.h"

FileUtil::ptr FileUtil::getInstance()
{
    static FileUtil::ptr file_util = new FileUtil;

    return file_util;
}

void AddNewPath()
{
    //
}

void FileUtil::addSearchPathPrefix(const std::string& path, int order)
{
    boost::mutex::scoped_lock(mutex);
    
    // must be absolute path
    if (!isAbsolutePath(path)) {
        return;
    }
    
    auto path_info = new PathInfo;
    path_info->path = path;
    path_info->order = order;
    
    path_list.push_back(path_info);
    path_list.sort([](const FileUtil::PathInfo::ptr& left,
            const FileUtil::PathInfo::ptr& right) {
            
            return left->order > right->order;
        });
}

void FileUtil::removeSeachPathPrefix(const std::string& path)
{
    boost::mutex::scoped_lock(mutex);
    
    auto it = path_list.begin();
    for (; it != path_list.end(); ++it) {
        if (path == (*it)->path) {
            path_list.erase(it);
            return;
        }
    }
}

void FileUtil::addRelativeSearchDirectory(const std::string& dir, int order)
{
    boost::mutex::scoped_lock(mutex);
    
    // must be relative path
    if (isAbsolutePath(dir)) {
        return;
    }
    
    auto path_info = new PathInfo;
    path_info->path = dir;
    path_info->order = order;
    
    dir_list.push_back(path_info);
    dir_list.sort([](const FileUtil::PathInfo::ptr& left,
            const FileUtil::PathInfo::ptr& right) {
            
            return left->order > right->order;
        });
}

void FileUtil::removeRelativeSearchDirectory(const std::string& dir)
{
    boost::mutex::scoped_lock(mutex);
    
    auto it = dir_list.begin();
    for (; it != dir_list.end(); ++it) {
        if (dir == (*it)->path) {
            dir_list.erase(it);
            return;
        }
    }
}

std::string FileUtil::getCurrentPath()
{
    return "";
}

std::string FileUtil::getFilePath(const std::string& filename) const
{
    if (isAbsolutePath(filename)) {
        return filename;
    }
    
    boost::mutex::scoped_lock(mutex);

    auto it = path_list.begin();
    for (; it != path_list.end(); ++it) {
        for (auto& d_it: dir_list) {
            std::string filepath = (*it)->path + "/" + d_it->path + "/" + filename;
            QFile f(filepath.c_str());

            if (f.exists()) {
                return filepath;
            }
        }
    }

    return filename;
}

FileStream::ptr FileUtil::getFileStream(const std::string& filename) const
{
    auto path = getFilePath(filename);
    if (path == "") {
        return nullptr;
    }

    auto fs = new FileStream(path);
    fs->open();

    if (fs->isOpen()) {
        return fs;
    } else {
        return nullptr;
    }
}

std::string FileUtil::getDirectoryPath(const std::string& dirname) const
{
    if (isAbsolutePath(dirname)) {
        return dirname;
    }
    
    boost::mutex::scoped_lock(mutex);
    
    auto it = path_list.begin();
    for (; it != path_list.end(); ++it) {
        std::string dirpath = (*it)->path + "/" + dirname;
        QDir d(dirpath.c_str());

        if (d.exists()) {
            return dirpath;
        }
    }
    
    return "";
}

FilePathList FileUtil::getFileListFromDir(const std::string& dirpath, bool recursive) const
{
    auto d_path = getDirectoryPath(dirpath);
    if (d_path == "") {
        return FilePathList();
    }
    
    QDirIterator::IteratorFlags it_flags = recursive ? 
        QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags;
    QDirIterator it(d_path.c_str(), it_flags);
    
    FilePathList ret;
    while (it.hasNext()) {
        auto file = it.next();
        auto f_info = it.fileInfo();
        
        FilePath::ptr f = new FilePath;
        if (f_info.isDir()) {
            f->file_type = FilePath::Type::DIRECTORY;
        } else if (f_info.isFile()) {
            f->file_type = FilePath::Type::FILE;
        } else {
            continue;
        }
        
        f->name = file.toUtf8().data();
        f->fullpath = f_info.filePath().toUtf8().data();
        
        Logger::debug("filename: %s", f->name.c_str());
        Logger::debug("filepath: %s", f->fullpath.c_str());
        
        ret.push_back(f);
    }
    
    return std::move(ret);
}

bool FileUtil::writeBinaryToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    if (data_buffer) {
        return writeBinaryToFile(filepath, (const char*)data_buffer->getBuffer(), data_buffer->getSize());
    } else {
        return writeBinaryToFile(filepath, nullptr, 0);
    }
}

bool FileUtil::writeBinaryToFile(const std::string& filepath, const char* buffer, size_t size)
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

bool FileUtil::writeTextToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    if (data_buffer) {
        return writeTextToFile(filepath, (const char*)data_buffer->getBuffer(), data_buffer->getSize());
    } else {
        return writeTextToFile(filepath, std::string());
    }
}

bool FileUtil::writeTextToFile(const std::string& filepath, const std::string& text)
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

bool FileUtil::writeTextToFile(const std::string& filepath, const char* buffer, size_t size)
{
    return writeTextToFile(filepath, std::string(buffer, size));
}

// private member function
FileUtil::FileUtil()
{
    addRelativeSearchDirectory("", 0);
}

bool FileUtil::isAbsolutePath(const std::string& path) const
{
    return path.find("/") == 0 || path.find(":/") == 0;
}

#endif
