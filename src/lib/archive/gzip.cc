#include "gzip.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

void Gzip::decompress(const ByteBuffer& compressed, ByteBuffer& ret)
{
    boost::iostreams::filtering_ostream filter;
    filter.push(boost::iostreams::gzip_decompressor() | boost::iostreams::back_inserter(ret));
    filter.write(&compressed[0], compressed.size());
}

void Gzip::decompressToFile(const std::string& in_filepath, const std::string& out_filepath)
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
