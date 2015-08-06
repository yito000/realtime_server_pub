#include "random.h"

Random::Random()
{
    //
}

Random::Random(unsigned long seed) : rand(seed)
{
    //
}

int Random::getInt(int min, int max)
{
    boost::uniform_int<> u(min, max);
    boost::variate_generator< boost::mt19937&, boost::uniform_int<> > r(rand, u);
    
    return r();
}

void Random::getAllAscii(int length, std::string& ret)
{
    ret = "";
    
    boost::uniform_smallint<> u(32, 126);
    boost::variate_generator< boost::mt19937&, boost::uniform_smallint<> > r(rand, u);
    
    for (int i = 0; i < length; i++) {
        char c[2];
        
        c[0] = r();
        c[1] = '\0';
        
        ret.append(c);
    }
}

void Random::getAscii(int length, std::string& ret)
{
    ret = "";
    
    boost::uniform_smallint<> u(48, 122);
    boost::variate_generator< boost::mt19937&, boost::uniform_smallint<> > r(rand, u);
    
    for (int i = 0; i < length; i++) {
        char c[2];
        char cc = 0;
        
        while (1) {
            cc = r();
            
            if ((48 <= cc && cc <= 57) || 
                (65 <= cc && cc <= 90) || 
                (97 <= cc && cc <= 122)) {
                
                break;
            }
        }
        
        c[0] = cc;
        c[1] = '\0';
        
        ret.append(c);
    }
}

float Random::getFloat(float min, float max)
{
    boost::uniform_real<> u(min, max);
    boost::variate_generator< boost::mt19937&, boost::uniform_real<> > r(rand, u);
    
    return r();
}

float Random::getFloat0_1()
{
    const int RAND_INT_MAX = 0x7fffffff;
    int a = getInt(0x0, 0x7fffffff);
    
    return (float)a / (float)RAND_INT_MAX;
}

float Random::getFloatMinus1_1()
{
    auto ret = getFloat0_1();
    
    return ret * 2.0 - 1.0;
}
