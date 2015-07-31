#include "mat3.h"

Mat3::Mat3()
{
    m[0] = 1.0;
    m[1] = 0.0;
    m[2] = 0.0;
    m[3] = 0.0;
    m[4] = 1.0;
    m[5] = 0.0;
    m[6] = 0.0;
    m[7] = 0.0;
    m[8] = 1.0;
}

Mat3::Mat3(float* f)
{
    for (int i = 0; i < 9; i++) {
        m[i] = f[i];
    }
}
