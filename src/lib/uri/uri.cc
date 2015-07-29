#include "uri.h"

#include <string>
#include <sstream>
#include <string>

#include <boost/regex.hpp>

std::string Uri::encode(const std::string& text)
{
    boost::regex r("[^\\*\\-\\.@_A-Za-z0-9 ]");
    boost::smatch m;
    
    std::string str = text.c_str();
    
    std::string::const_iterator start = str.begin();
    std::string::const_iterator end = str.end();
    
    PosList list;
    size_t base_pos = 0;
    
    while (boost::regex_search(start, end, m, r)) {
        start = m[0].second;
        list.push_back(StrPair(base_pos + m.position(), m.length()));
        
        base_pos += m.position() + m.length();
    }
    
    PosList::reverse_iterator it = list.rbegin();
    
    for (; it != list.rend(); ++it) {
        StrPair& pair = *it;
        std::string sub = str.substr(pair.first, pair.second);
        
        unsigned char c = sub.c_str()[0];
        std::stringstream in;
        std::string hex_str;
        
        in << std::hex << static_cast<unsigned int>(c);
        in >> hex_str;
        
        str.replace(pair.first, pair.second, "%" + hex_str);
    }
    
    std::string format = "+";
    boost::regex r2(" ");
    str = boost::regex_replace(str, r2, format);
    
    return str;
}

std::string Uri::decode(const std::string& text)
{
    boost::regex r("%[0-9A-Za-z][0-9A-Za-z]");
    boost::smatch m;
    
    std::string str = text.c_str();
    
    std::string::const_iterator start = str.begin();
    std::string::const_iterator end = str.end();
    
    PosList list;
    size_t base_pos = 0;
    
    while (boost::regex_search(start, end, m, r)) {
        start = m[0].second;
        list.push_back(StrPair(base_pos + m.position(), m.length()));
        
        base_pos += m.position() + m.length();
    }
    
    PosList::reverse_iterator it = list.rbegin();
    
    for (; it != list.rend(); ++it) {
        StrPair& pair = *it;
        std::string sub = str.substr(pair.first + 1, pair.second - 1);
        
        int c = 0;
        std::stringstream in;
        
        in << std::hex << sub;
        in >> c;
        
        char ca[2] = {(char)c, 0};
        str.replace(pair.first, pair.second, ca);
    }
    
    std::string format = " ";
    boost::regex r2("\\+");
    str = boost::regex_replace(str, r2, format);
    
    return str;
}
