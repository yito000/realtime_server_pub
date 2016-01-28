#ifdef TARGET_PLATFORM_QT

#include "file/file_util.h"

#include <QFile>
#include <QDir>
#include <QDirIterator>

#include <QTextStream>
#include <QDataStream>

#include "log/logger.h"

BEGIN_NS

FileUtil::ptr FileUtil::getInstance()
{
    static FileUtil::ptr file_util = new FileUtil;

    return file_util;
}

void AddNewPath()
{
    //
}

void FileUtil::addSearchRootPath(const std::string& path, int order)
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

void FileUtil::removeSearchRootPath(const std::string& path)
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

void FileUtil::addWritablePath(const std::string& path, int order)
{
    boost::mutex::scoped_lock(mutex);
    
    auto it = writable_path_list.begin();
    for (; it != writable_path_list.end(); ++it) {
        if (path == (*it)->path) {
            return;
        }
    }

    auto path_info = new PathInfo;
    path_info->path = path;
    path_info->order = order;

    writable_path_list.push_back(path_info);
    writable_path_list.sort([](const FileUtil::PathInfo::ptr& left,
            const FileUtil::PathInfo::ptr& right) {
            
            return left->order < right->order;
        });
}

void FileUtil::removeWritablePath(const std::string& path)
{
    boost::mutex::scoped_lock(mutex);
    
    auto it = writable_path_list.begin();
    for (; it != writable_path_list.end(); ++it) {
        if (path == (*it)->path) {
            writable_path_list.erase(it);
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

    FileStream::ptr fs = new FileStream(path);
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

void FileUtil::getWritablePaths(std::list<std::string>& out_list)
{
    for (auto p: writable_path_list) {
        out_list.push_back(p->path);
    }
}

bool FileUtil::writeBinaryToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    FileWriter::ptr file_writer = new FileWriter;
    
    return file_writer->writeBinaryToFile(filepath, data_buffer);
}

bool FileUtil::writeBinaryToFile(const std::string& filepath, const char* buffer, size_t size)
{
    FileWriter::ptr file_writer = new FileWriter;
    
    return file_writer->writeBinaryToFile(filepath, buffer, size);
}

bool FileUtil::writeTextToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    FileWriter::ptr file_writer = new FileWriter;
    
    return file_writer->writeTextToFile(filepath, data_buffer);
}

bool FileUtil::writeTextToFile(const std::string& filepath, const std::string& text)
{
    FileWriter::ptr file_writer = new FileWriter;
    
    return file_writer->writeTextToFile(filepath, text);
}

bool FileUtil::writeTextToFile(const std::string& filepath, const char* buffer, size_t size)
{
    FileWriter::ptr file_writer = new FileWriter;
    
    return file_writer->writeTextToFile(filepath, buffer, size);
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

END_NS

#endif
