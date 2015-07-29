#ifndef URI_H
#define URI_H

#include <string>
#include <list>
#include <utility>

class Uri
{
    typedef std::pair<int, int> StrPair;
    typedef std::list<StrPair> PosList;
    
public:
    static std::string encode(const std::string& text);
    static std::string decode(const std::string& text);
};

#endif
