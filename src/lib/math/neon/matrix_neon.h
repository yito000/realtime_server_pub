#ifdef __ARM_NEON__

#ifndef MATRIX_NEON_H

#include <arm_neon.h>

class MatrixNeon
{
public:
    static void multiply(const float32x4x4_t& m1, const float32x4x4_t& m2, float32x4x4_t& dst);
    static void transformVector4(const float32x4x4_t& m, const float32x4_t& v, float32x4_t& dst);
};

#endif

#else
#error unsupported platform
#endif
