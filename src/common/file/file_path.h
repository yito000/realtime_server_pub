#ifndef FILE_PATH_H
#define FILE_PATH_H

#include "smart_ptr.hpp"

#include <string>
#include <list>

struct FilePath : public SmartPtr<FilePath>
{
    typedef boost::intrusive_ptr<FilePath> ptr;
    
    enum class Type {
        DIRECTORY,
        FILE
    };
    
    std::string name;
    std::string fullpath;
    FilePath::Type file_type;
    
    FilePath()
    {
        file_type = Type::FILE;
    }
};

#endif
