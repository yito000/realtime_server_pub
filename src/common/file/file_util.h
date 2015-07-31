#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include "smart_ptr.hpp"
#include <string>

class FileUtil : public SmartPtr<FileUtil>
{
public:
    typedef boost::intrusive_ptr<FileUtil> ptr;
    
    static FileUtil::ptr getInstance();
    
    std::string getCurrentPath();
};

#endif
