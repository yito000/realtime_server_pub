#ifdef __SSE__

#ifndef MATRIX_SSE_H

#include <xmmintrin.h>

class MatrixSSE
{
public:
    static void add(const __m128 m[4], float scalar, __m128 dst[4]);
    static void add(const __m128 m1[4], const __m128 m2[4], __m128 dst[4]);
    static void subtract(const __m128 m[4], float scalar, __m128 dst[4]);
    static void subtract(const __m128 m1[4], const __m128 m2[4], __m128 dst[4]);
    static void multiply(const __m128 m[4], float scalar, __m128 dst[4]);
    static void multiply(const __m128 m1[4], const __m128 m2[4], __m128 dst[4]);
    static void negate(const __m128 m[4], __m128 dst[4]);
    static void transpose(const __m128 m[4], __m128 dst[4]);
    static void transformVector4(const __m128 m[4], const __m128& v, __m128& dst);
};

#endif

#else
#error unsupported platform
#endif
