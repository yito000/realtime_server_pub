#include "file_util.h"

#include <boost/filesystem.hpp>

std::string FileUtil::getCurrentPath()
{
    boost::filesystem::path p(boost::filesystem::current_path());

    return p.native() + "/";
}

