#ifndef GZIP_H
#define GZIP_H

#include <string>
#include <vector>

typedef std::vector<char> ByteBuffer;

class Gzip
{
public:
    static void compressBufToBuf(int level, const ByteBuffer& src, 
        ByteBuffer& ret);
    static void compressBufToBuf(int level, const char* buf, size_t size,
        ByteBuffer& ret);
//    static void compressFileToFile(int level, const std::string& in_filepath,
//        const std::string& out_filepath);

    static void decompressBufToBuf(const ByteBuffer& compressed, 
        ByteBuffer& ret);
    static void decompressBufToBuf(const char* buf, size_t size,
        ByteBuffer& ret);
//    static void decompressFileToFile(const std::string& in_filepath, 
//        const std::string& out_filepath);
};

#endif
