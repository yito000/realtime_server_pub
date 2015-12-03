#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <float.h>
#include <math.h>

class MathUtil
{
public:
    static float degreesToRadians(float degrees);
    static float radiansToDegrees(float radians);

    static float epsilon();
};

#endif
