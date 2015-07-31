#ifndef QUATERNION_H
#define QUATERNION_H

#include "vec3.h"

class Mat4;

class Quaternion
{
public:
    float x;
    float y;
    float z;
    float w;

    Quaternion();
    Quaternion(float _x, float _y, float _z, float _w);
    Quaternion(float* f);
    
    void multiply(const Quaternion& q);
    void normalize();
    
    float toAxisAngle(Vec3* out) const;
    
    void set(float xx, float yy, float zz, float ww);
    void set(float* array);
    void set(const Mat4& m);
    void set(const Vec3& axis, float angle);
    void set(const Quaternion& q);
    
    bool inverse();
    Quaternion getInverse() const;
    
    // static method
    static void createFromMatrix(const Mat4& m, Quaternion* out);
    static void createFromAxisAngle(const Vec3& axis, float angle, Quaternion* out);
    
    static Quaternion identity();
    static void multiply(const Quaternion& q1, const Quaternion& q2, Quaternion* dst);
    
    static void lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
    static void slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
    
    // operator
    const Quaternion operator*(const Quaternion& q) const;
    Quaternion& operator*=(const Quaternion& q);
    
private:
    static void slerp(float q1x, float q1y, float q1z, float q1w, float q2x, float q2y, float q2z, float q2w, float t, float* dstx, float* dsty, float* dstz, float* dstw);
};

#endif
