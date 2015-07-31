#include "gzip.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace {
    const int MIN_GZIP_LEVEL = 0;
    const int MAX_GZIP_LEVEL = 9;
};

void Gzip::compressBufToBuf(int level, const ByteBuffer& src, 
    ByteBuffer& ret)
{
    compressBufToBuf(level, (const char*)&src[0], src.size(), ret);
}

void Gzip::compressBufToBuf(int level, const char* buf, size_t size,
    ByteBuffer& ret)
{
    if (level < MIN_GZIP_LEVEL) {
        level = MIN_GZIP_LEVEL;
    }

    if (level > MAX_GZIP_LEVEL) {
        level = MAX_GZIP_LEVEL;
    }

    boost::iostreams::filtering_ostream filter;
    filter.push(boost::iostreams::gzip_compressor(level) |
        boost::iostreams::back_inserter(ret));

    filter.write(buf, size);
}

/*
void Gzip::compressFileToFile(int level, const std::string& in_filepath,
    const std::string& out_filepath)
{
    if (level < MIN_GZIP_LEVEL) {
        level = MIN_GZIP_LEVEL;
    }

    if (level > MAX_GZIP_LEVEL) {
        level = MAX_GZIP_LEVEL;
    }

    boost::filesystem::ifstream in(in_filepath.c_str());
    boost::filesystem::ofstream out(out_filepath.c_str());

    boost::iostreams::filtering_istream filter;

    filter.push(boost::iostreams::gzip_compressor(level));
    filter.push(in);

    boost::iostreams::copy(filter, out);

    in.close();
    out.close();
}
*/

void Gzip::decompressBufToBuf(const ByteBuffer& compressed, ByteBuffer& ret)
{
    decompressBufToBuf((const char*)&compressed[0], 
        compressed.size(), ret);
}

void Gzip::decompressBufToBuf(const char* buf, size_t size,
    ByteBuffer& ret)
{
    boost::iostreams::filtering_ostream filter;
    filter.push(boost::iostreams::gzip_decompressor() |
        boost::iostreams::back_inserter(ret));

    filter.write(buf, size);
}

/*
void Gzip::decompressFileToFile(const std::string& in_filepath, 
    const std::string& out_filepath)
{
    boost::filesystem::ifstream in(in_filepath.c_str());
    boost::filesystem::ofstream out(out_filepath.c_str());
    
    boost::iostreams::filtering_istream filter;
    
    filter.push(boost::iostreams::gzip_decompressor());
    filter.push(in);
    
    boost::iostreams::copy(filter, out);
    
    in.close();
    out.close();
}
*/
