#ifndef TARGET_PLATFORM_QT

#include "common/file/file_util.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

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
#define PATH_SEP "\\"
#define PATH_PREFIX_C "C:\\"
#define PATH_PREFIX_D "D:\\"
#else
#define CONV_PATH(s) s
#define PATH_SEP "/"
#define PATH_PREFIX "/"
#endif

//
FileUtil::ptr FileUtil::getInstance()
{
    static FileUtil::ptr file_util = new FileUtil;

    return file_util;
}

void FileUtil::addSearchRootPath(const std::string& path, int order)
{
    boost::mutex::scoped_lock(mutex);

	auto a_path = CONV_PATH(path);
    
    auto it = path_list.begin();
    for (; it != path_list.end(); ++it) {
        if (a_path == (*it)->path) {
            return;
        }
    }

    auto path_info = new PathInfo;
    path_info->path = a_path;
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

	auto a_path = CONV_PATH(path);
    
    auto it = path_list.begin();
    for (; it != path_list.end(); ++it) {
        if (a_path == (*it)->path) {
            path_list.erase(it);
            return;
        }
    }
}

void FileUtil::addRelativeSearchDirectory(const std::string& dir, int order)
{
    boost::mutex::scoped_lock(mutex);

	auto a_dir = CONV_PATH(dir);
    
    // must be relative path
    if (isAbsolutePath(a_dir)) {
        return;
    }
    
    auto path_info = new PathInfo;
    path_info->path = a_dir;
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
    
    auto a_dir = CONV_PATH(dir);
    
    auto it = dir_list.begin();
    for (; it != dir_list.end(); ++it) {
        if (a_dir == (*it)->path) {
            dir_list.erase(it);
            return;
        }
    }
}

void FileUtil::addWritablePath(const std::string& path, int order)
{
    boost::mutex::scoped_lock(mutex);
    
    auto a_path = CONV_PATH(path);
    
    auto it = writable_path_list.begin();
    for (; it != writable_path_list.end(); ++it) {
        if (a_path == (*it)->path) {
            return;
        }
    }
    
    auto path_info = new PathInfo;
    path_info->path = a_path;
    path_info->order = order;

    writable_path_list.push_back(path_info);
    writable_path_list.sort([](const FileUtil::PathInfo::ptr& left,
            const FileUtil::PathInfo::ptr& right) {
            
            return left->order > right->order;
        });
}

void FileUtil::removeWritablePath(const std::string& path)
{
    boost::mutex::scoped_lock(mutex);
    
    auto a_path = CONV_PATH(path);
    
    auto it = writable_path_list.begin();
    for (; it != writable_path_list.end(); ++it) {
        if (a_path == (*it)->path) {
            writable_path_list.erase(it);
            return;
        }
    }
}

std::string FileUtil::getCurrentPath()
{
    boost::filesystem::path p(boost::filesystem::current_path());
	std::string p_native = p.string();

    return p_native + std::string(PATH_SEP);
}

std::string FileUtil::getFilePath(const std::string& filename) const
{
	auto a_filename = CONV_PATH(filename);

    if (isAbsolutePath(a_filename)) {
        return a_filename;
    }
    
    boost::mutex::scoped_lock(mutex);
    
    auto it = path_list.begin();
    for (; it != path_list.end(); ++it) {
        for (auto& d_it: dir_list) {
            std::string filepath = (*it)->path + PATH_SEP + d_it->path + PATH_SEP + a_filename;
            boost::filesystem::path f(filepath);
            
            if (boost::filesystem::exists(f) && boost::filesystem::is_regular_file(f)) {
                return filepath;
            }
        }
    }
    
    return a_filename;
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
    
    auto a_dirname = CONV_PATH(dirname);
    
    auto it = path_list.begin();
    for (; it != path_list.end(); ++it) {
        std::string dirpath = (*it)->path + PATH_SEP + a_dirname;
        boost::filesystem::path d(dirpath);
        
        if (boost::filesystem::exists(d) && boost::filesystem::is_directory(d)) {
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
        
//        Logger::debug("filename: %s", f->name.c_str());
//        Logger::debug("filepath: %s", f->fullpath.c_str());
        
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
#if defined(TARGET_OS_WIN64)
    // TODO
    return path.find(PATH_PREFIX_C) == 0 || path.find(PATH_PREFIX_D) == 0;
#else
    return path.find("/") == 0;
#endif
}

END_NS

#endif
