#ifndef GZIP_H
#define GZIP_H

#include <vector>
#include "network/io/socket_buffer.h"

class Gzip
{
public:
    void decompress(const ByteBuffer& compressed, ByteBuffer& ret);
    void decompressToFile(const std::string& in_filepath, const std::string& out_filepath);
};

#endif
