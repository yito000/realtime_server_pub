#include "global_rand.h"

#include "log/logger.h"
#include <chrono>

GlobalRand::ptr GlobalRand::getInstance()
{
    static GlobalRand::ptr inst = new GlobalRand;
    
    return inst;
}

int GlobalRand::getInt(int min, int max)
{
    return rand->getInt(min, max);
}

void GlobalRand::getAllAscii(int length, std::string& ret)
{
    rand->getAllAscii(length, ret);
}

void GlobalRand::getAscii(int length, std::string& ret)
{
    rand->getAscii(length, ret);
}

float GlobalRand::getFloat(float min, float max)
{
    return rand->getFloat(min, max);
}

float GlobalRand::getFloat0_1()
{
    auto a = rand->getFloat0_1();
    return a;
}

float GlobalRand::getFloatMinus1_1()
{
    auto a = rand->getFloatMinus1_1();
    return a;
}

// private member function
GlobalRand::GlobalRand()
{
    auto now = std::chrono::system_clock::now();
    rand = new Random(now.time_since_epoch().count());
}

GlobalRand::~GlobalRand()
{
    //
}
