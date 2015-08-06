#ifndef A_RANDOM_H
#define A_RANDOM_H

#if defined(nil)
    #undef nil
#endif

#define nil cpp_nil

#include <string>
#include <boost/random.hpp>

class Random
{
public:
    Random();
    Random(unsigned long seed);
    
    int getInt(int min, int max);
    void getAllAscii(int length, std::string& ret);
    void getAscii(int length, std::string& ret);
    
    float getFloat(float min, float max);
    float getFloat0_1();
    float getFloatMinus1_1();
    
private:
    boost::mt19937 rand;
};

#undef nil
#define nil NULL

#endif
