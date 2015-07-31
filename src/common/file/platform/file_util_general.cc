#ifndef TARGET_PLATFORM_QT

#include "common/file/file_util.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "log/logger.h"

FileUtil::ptr FileUtil::getInstance()
{
    static FileUtil::ptr file_util = new FileUtil;

    return file_util;
}

void FileUtil::addSearchPathPrefix(const std::string& path, int order)
{
    boost::mutex::scoped_lock(mutex);
    
    auto it = path_list.begin();
    for (; it != path_list.end(); ++it) {
        if (path == (*it)->path) {
            return;
        }
    }

    auto path_info = new PathInfo;
    path_info->path = path;
    path_info->order = order;

    path_list.push_back(path_info);
    path_list.sort([](const FileUtil::PathInfo::ptr& left,
            const FileUtil::PathInfo::ptr& right) {
            
            return left->order < right->order;
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
    boost::filesystem::path p(boost::filesystem::current_path());

    return p.native() + "/";
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
            boost::filesystem::path f(filepath);

            if (boost::filesystem::is_regular_file(f) && boost::filesystem::exists(f)) {
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
        boost::filesystem::path d(dirpath);
        
        if (boost::filesystem::is_directory(d) && boost::filesystem::exists(d)) {
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
    
    boost::filesystem::path path(d_path);
    auto cur = boost::filesystem::directory_iterator(path);
    auto end = boost::filesystem::directory_iterator();
    
    FilePathList ret;
    for (; cur != end; ++cur) {
        FilePath::ptr f = new FilePath;
        if (boost::filesystem::is_directory(*cur)) {
            f->file_type = FilePath::Type::DIRECTORY;
        } else if (boost::filesystem::is_regular_file(*cur)) {
            f->file_type = FilePath::Type::FILE;
        } else {
            continue;
        }
        
        f->name = cur->path().filename().string();
        f->fullpath = cur->path().string();
        
        Logger::log("filename: %s", f->name.c_str());
        Logger::log("filepath: %s", f->fullpath.c_str());
        
        ret.push_back(f);
    }
    
    return std::move(ret);
}

bool FileUtil::writeBinaryToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    return writeBinaryToFile(filepath, (const char*)data_buffer->getBuffer(), data_buffer->getSize());
}

bool FileUtil::writeBinaryToFile(const std::string& filepath, const char* buffer, size_t size)
{
    FILE* fp = fopen(filepath.c_str(), "wb");
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

bool FileUtil::writeTextToFile(const std::string& filepath, DataBuffer::ptr data_buffer)
{
    return writeTextToFile(filepath, (const char*)data_buffer->getBuffer(), data_buffer->getSize());
}

bool FileUtil::writeTextToFile(const std::string& filepath, const std::string& text)
{
    FILE* fp = fopen(filepath.c_str(), "wt");
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

bool FileUtil::writeTextToFile(const std::string& filepath, const char* buffer, size_t size)
{
    return writeTextToFile(filepath, std::string(buffer, size));
}

// private member function
FileUtil::FileUtil()
{
    //
}

bool FileUtil::isAbsolutePath(const std::string& path) const
{
    return path.find("/") == 0;
}

#endif
