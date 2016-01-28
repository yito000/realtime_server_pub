#ifndef GZIP_H
#define GZIP_H

#include <string>
#include <vector>

typedef std::vector<char> GzipByteBuffer;

class Gzip
{
public:
    static void compressBufToBuf(int level, const GzipByteBuffer& src, 
        GzipByteBuffer& ret);
    static void compressBufToBuf(int level, const char* buf, size_t size,
        GzipByteBuffer& ret);
//    static void compressFileToFile(int level, const std::string& in_filepath,
//        const std::string& out_filepath);

    static void decompressBufToBuf(const GzipByteBuffer& compressed, 
        GzipByteBuffer& ret);
    static void decompressBufToBuf(const char* buf, size_t size,
        GzipByteBuffer& ret);
//    static void decompressFileToFile(const std::string& in_filepath, 
//        const std::string& out_filepath);
};

#endif
