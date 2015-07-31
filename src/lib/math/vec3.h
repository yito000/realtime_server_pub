#ifndef VEC3_H
#define VEC3_H

class Vec3
{
public:
    float x;
    float y;
    float z;

    Vec3();
    Vec3(float _x, float _y, float _z);
    Vec3(float* f);

    void add(const Vec3& v);
    void subtract(const Vec3& v);
    
    void clamp(const Vec3& min, const Vec3& max);
    
    void normalize();
    Vec3 getNormalized() const;
    
    float dot(const Vec3& v) const;
    void cross(const Vec3& v, Vec3* out) const;
    
    Vec3 lerp(const Vec3& v, float t) const;
    
    float distance(const Vec3& v) const;
    float length() const;
    float lengthSquared() const;
    
    void negate();
    
    void set(float x, float y, float z);
    void set(const Vec3& v);
    void set(const float* f);

    // static method
    static void add(const Vec3& v1, const Vec3& v2, Vec3* v);
    static void subtract(const Vec3& v1, const Vec3& v2, Vec3* v);
    
    static void clamp(const Vec3& min, const Vec3& max, Vec3* v);

    static float dot(const Vec3& v1, const Vec3& v2);
    static void cross(const Vec3& v1, const Vec3& v2, Vec3* v);

    static float angle(const Vec3& v1, const Vec3& v2);
    
    // operator
    const Vec3 operator+(const Vec3& v) const;
    Vec3& operator+=(const Vec3& v);
    
    const Vec3 operator-(const Vec3& v) const;
    Vec3& operator-=(const Vec3& v);
    
    const Vec3 operator*(float f) const;
    Vec3& operator*=(float f);
    
    const Vec3 operator/(float f) const;
    Vec3 operator/=(float f);
    
    const Vec3 operator-() const;
    
    bool operator<(const Vec3& v) const;
    bool operator==(const Vec3& v) const;
    bool operator!=(const Vec3& v) const;
};

inline const Vec3 operator*(float x, const Vec3& v);

// inline function
inline const Vec3 operator*(float x, const Vec3& v)
{
    Vec3 result(v);
    result.x *= x;
    result.y *= x;
    result.z *= x;
    return result;
}

#endif
