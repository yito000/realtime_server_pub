#ifndef VEC2_H
#define VEC2_H

class Vec2
{
public:
    float x;
    float y;

    Vec2();
    Vec2(float _x, float _y);
    Vec2(float* f);
    
    void add(const Vec2& v);
    void subtract(const Vec2& v);
    
    void clamp(const Vec2& min, const Vec2& max);
    
    void normalize();
    Vec2 getNormalized() const;
    
    float dot(const Vec2& v) const;
    float cross(const Vec2& v) const;
    
    Vec2 lerp(const Vec2& v, float t) const;
    
    float distance(const Vec2& v) const;
    float length() const;
    float lengthSquared() const;
    
    void negate();
    
    void set(float x, float y);
    void set(const Vec2& v);
    void set(const float* f);

    // static method
    static void add(const Vec2& v1, const Vec2& v2, Vec2* v);
    static void subtract(const Vec2& v1, const Vec2& v2, Vec2* v);
    
    static void clamp(const Vec2& min, const Vec2& max, Vec2* v);

    static float dot(const Vec2& v1, const Vec2& v2);
    static float cross(const Vec2& v1, const Vec2& v2);

    static float angle(const Vec2& v1, const Vec2& v2);
    
    // operator
    const Vec2 operator+(const Vec2& v) const;
    Vec2& operator+=(const Vec2& v);
    
    const Vec2 operator-(const Vec2& v) const;
    Vec2& operator-=(const Vec2& v);
    
    const Vec2 operator*(float f) const;
    Vec2& operator*=(float f);
    
    const Vec2 operator/(float f) const;
    Vec2 operator/=(float f);
    
    const Vec2 operator-() const;
    
    bool operator<(const Vec2& v) const;
    bool operator==(const Vec2& v) const;
    bool operator!=(const Vec2& v) const;
};

inline const Vec2 operator*(float x, const Vec2& v);

// inline function
inline const Vec2 operator*(float x, const Vec2& v)
{
    Vec2 result(v);
    result.x *= x;
    result.y *= x;
    return result;
}

#endif
