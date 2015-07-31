#include "math_util.h"

float MathUtil::degreesToRadians(float degrees)
{
    return degrees * 0.01745329252f;
}

float MathUtil::radiansToDegrees(float radians)
{
    return radians * 57.29577951f;
}

float MathUtil::epsilon()
{
    return FLT_EPSILON;
}
