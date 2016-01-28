#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include "fw_env.h"

#include <string>
#include <list>
#include <boost/thread.hpp>

#include "common/file/file_stream.h"
#include "common/file/file_path.h"
#include "common/file/file_writer.h"

BEGIN_NS

typedef std::list<FilePath::ptr> FilePathList;

class FileUtil : public SmartPtr<FileUtil>
{
public:
    typedef boost::intrusive_ptr<FileUtil> ptr;

    static FileUtil::ptr getInstance();
    
    void addSearchRootPath(const std::string& path, int order = 0);
    void removeSearchRootPath(const std::string& path);
    
    void addRelativeSearchDirectory(const std::string& dir, int order = 0);
    void removeRelativeSearchDirectory(const std::string& dir);
    
    void addWritablePath(const std::string& path, int order = 0);
    void removeWritablePath(const std::string& path);
    
    std::string getCurrentPath();

    std::string getFilePath(const std::string& filename) const;
    FileStream::ptr getFileStream(const std::string& filename) const;
    
    std::string getDirectoryPath(const std::string& dirname) const;
    FilePathList getFileListFromDir(const std::string& dirpath, bool recursive = false) const;
    
    void getWritablePaths(std::list<std::string>& out_list);
    
    bool writeBinaryToFile(const std::string& filepath, DataBuffer::ptr data_buffer = nullptr);
    bool writeBinaryToFile(const std::string& filepath, const char* buffer, size_t size);
    
    bool writeTextToFile(const std::string& filepath, DataBuffer::ptr data_buffer = nullptr);
    bool writeTextToFile(const std::string& filepath, const std::string& text);
    bool writeTextToFile(const std::string& filepath, const char* buffer, size_t size);
    
    // TODO: append mode
    
private:
    FileUtil();

    struct PathInfo : public SmartPtr<PathInfo>
    {
        typedef boost::intrusive_ptr<PathInfo> ptr;

        int order;
        std::string path;

        PathInfo()
        {
            order = 0;
        }
    };
    
    bool isAbsolutePath(const std::string& path) const;
    
    std::list<PathInfo::ptr> path_list;
    std::list<PathInfo::ptr> dir_list;
    
    std::list<PathInfo::ptr> writable_path_list;
    
    boost::mutex mutex;
};

END_NS

#endif
