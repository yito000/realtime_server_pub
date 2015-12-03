#ifndef VEC4_H
#define VEC4_H

#include <type_traits> 

#ifdef __SSE__
#include <xmmintrin.h>
#endif

#ifdef __ARM_NEON__
#include <arm_neon.h>
#endif

class Vec4
{
public:
#ifdef __SSE__
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        __m128 v;
    };
#elif __ARM_NEON__
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float32x4_t v;
    };
#else
    float x;
    float y;
    float z;
    float w;
#endif

    Vec4();
    Vec4(float _x, float _y, float _z, float _w);
    Vec4(float* f);
    Vec4(const Vec4& copy);
    
    void add(const Vec4& v);
    void subtract(const Vec4& v);
    
    void clamp(const Vec4& min, const Vec4& max);
    
    void normalize();
    Vec4 getNormalized() const;
    
    float dot(const Vec4& v) const;
    
    Vec4 lerp(const Vec4& v, float t) const;
    
    float distance(const Vec4& v) const;
    float length() const;
    float lengthSquared() const;
    
    void negate();
    
    void set(float x, float y, float z, float w);
    void set(const Vec4& v);
    void set(const float* f);

    // static method
    static void add(const Vec4& v1, const Vec4& v2, Vec4* v);
    static void subtract(const Vec4& v1, const Vec4& v2, Vec4* v);
    
    static void clamp(const Vec4& min, const Vec4& max, Vec4* v);

    static float dot(const Vec4& v1, const Vec4& v2);

    static float angle(const Vec4& v1, const Vec4& v2);
    
    // operator
    const Vec4 operator+(const Vec4& v) const;
    Vec4& operator+=(const Vec4& v);
    
    const Vec4 operator-(const Vec4& v) const;
    Vec4& operator-=(const Vec4& v);
    
    const Vec4 operator*(float f) const;
    Vec4& operator*=(float f);
    
    const Vec4 operator/(float f) const;
    Vec4 operator/=(float f);
    
    const Vec4 operator-() const;
    
    bool operator<(const Vec4& v) const;
    bool operator==(const Vec4& v) const;
    bool operator!=(const Vec4& v) const;
};

inline const Vec4 operator*(float x, const Vec4& v);

// inline function
inline const Vec4 operator*(float x, const Vec4& v)
{
    Vec4 result(v);
    result.x *= x;
    result.y *= x;
    result.z *= x;
    result.w *= x;
    return result;
}

#endif
