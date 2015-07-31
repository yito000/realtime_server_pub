#include "file_util.h"

#include <boost/filesystem.hpp>

FileUtil::ptr FileUtil::getInstance()
{
    static FileUtil::ptr file_util = new FileUtil;

    return file_util;
}

std::string FileUtil::getCurrentPath()
{
    boost::filesystem::path p(boost::filesystem::current_path());

    return p.native() + "/";
}
