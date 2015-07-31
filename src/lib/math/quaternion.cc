#include "quaternion.h"

#include <string.h>
#include <math.h>
#include "mat4.h"

#include "log/logger.h"

Quaternion::Quaternion() :
    x(0.0), y(0.0), z(0.0), w(1.0)
{
    //
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w) :
    x(_x), y(_y), z(_z), w(_w)
{
    //
}

Quaternion::Quaternion(float* f) :
    x(f[0]), y(f[1]), z(f[2]), w(f[3])
{
    //
}

void Quaternion::multiply(const Quaternion& q)
{
    multiply(*this, q, this);
}

void Quaternion::normalize()
{
    float n = x * x + y * y + z * z + w * w;
    
    if (n == 1.0f)
        return;
    
    n = sqrt(n);
    if (n < 0.000001f)
        return;
    
    n = 1.0f / n;
    x *= n;
    y *= n;
    z *= n;
    w *= n;
}

float Quaternion::toAxisAngle(Vec3* out) const
{
    Quaternion q(*this);
    q.normalize();
    
    out->x = q.x;
    out->y = q.y;
    out->z = q.z;
    out->normalize();
    
    return 2.0f * acos(q.w);
}

void Quaternion::set(float xx, float yy, float zz, float ww)
{
    x = xx;
    y = yy;
    z = zz;
    w = ww;
}

void Quaternion::set(float* array)
{
    x = array[0];
    y = array[1];
    z = array[2];
    w = array[3];
}

void Quaternion::set(const Mat4& m)
{
    Quaternion::createFromMatrix(m, this);
}

void Quaternion::set(const Vec3& axis, float angle)
{
    Quaternion::createFromAxisAngle(axis, angle, this);
}

void Quaternion::set(const Quaternion& q)
{
    *this = q;
}

bool Quaternion::inverse()
{
    float n = x * x + y * y + z * z + w * w;
    if (n == 1.0f) {
        x = -x;
        y = -y;
        z = -z;

        return true;
    }
    
    //
    if (n < 0.000001f) {
        return false;
    }
    
    n = 1.0f / n;
    x = -x * n;
    y = -y * n;
    z = -z * n;
    w = w * n;
    
    return true;
}

Quaternion Quaternion::getInverse() const
{
    Quaternion q(*this);
    q.inverse();
    return q;
}

void Quaternion::createFromMatrix(const Mat4& m, Quaternion* out)
{
    m.getRotation(out);
}

void Quaternion::createFromAxisAngle(const Vec3& axis, float angle, Quaternion* out)
{
    float half_angle = angle * 0.5;
    float sin_half_angle = sinf(half_angle);
    
    Vec3 normal(axis);
    normal.normalize();
    
    out->x = normal.x * sin_half_angle;
    out->y = normal.y * sin_half_angle;
    out->z = normal.z * sin_half_angle;
    out->w = cosf(half_angle);
}

Quaternion Quaternion::identity()
{
    static Quaternion q;
    
    return q;
}

void Quaternion::multiply(const Quaternion& q1, const Quaternion& q2, Quaternion* dst)
{
    float x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    float y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    float z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    float w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

    dst->x = x;
    dst->y = y;
    dst->z = z;
    dst->w = w;
}

void Quaternion::lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst)
{
    if (t == 0.0f)
    {
        memcpy(dst, &q1, sizeof(float) * 4);
        return;
    }
    else if (t == 1.0f)
    {
        memcpy(dst, &q2, sizeof(float) * 4);
        return;
    }

    float t1 = 1.0f - t;

    dst->x = t1 * q1.x + t * q2.x;
    dst->y = t1 * q1.y + t * q2.y;
    dst->z = t1 * q1.z + t * q2.z;
    dst->w = t1 * q1.w + t * q2.w;
}

void Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst)
{
    slerp(q1.x, q1.y, q1.z, q1.w, q2.x, q2.y, q2.z, q2.w, t, &dst->x, &dst->y, &dst->z, &dst->w);
}

const Quaternion Quaternion::operator*(const Quaternion& q) const
{
    Quaternion result(*this);
    result.multiply(q);
    return result;
}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{
    multiply(q);
    return *this;
}

// private member function
void Quaternion::slerp(float q1x, float q1y, float q1z, float q1w, float q2x, float q2y, 
    float q2z, float q2w, float t, float* dstx, float* dsty, float* dstz, float* dstw)
{
    if (t == 0.0f)
    {
        *dstx = q1x;
        *dsty = q1y;
        *dstz = q1z;
        *dstw = q1w;
        return;
    }
    else if (t == 1.0f)
    {
        *dstx = q2x;
        *dsty = q2y;
        *dstz = q2z;
        *dstw = q2w;
        return;
    }

    if (q1x == q2x && q1y == q2y && q1z == q2z && q1w == q2w)
    {
        *dstx = q1x;
        *dsty = q1y;
        *dstz = q1z;
        *dstw = q1w;
        return;
    }

    float halfY, alpha, beta;
    float u, f1, f2a, f2b;
    float ratio1, ratio2;
    float halfSecHalfTheta, versHalfTheta;
    float sqNotU, sqU;

    float cosTheta = q1w * q2w + q1x * q2x + q1y * q2y + q1z * q2z;

    //
    alpha = cosTheta >= 0 ? 1.0f : -1.0f;
    halfY = 1.0f + alpha * cosTheta;

    //
    f2b = t - 0.5f;
    u = f2b >= 0 ? f2b : -f2b;
    f2a = u - f2b;
    f2b += u;
    u += u;
    f1 = 1.0f - u;
    
    //
    halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
    halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
    versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

    //
    sqNotU = f1 * f1;
    ratio2 = 0.0000440917108f * versHalfTheta;
    ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
    ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
    ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
    ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

    sqU = u * u;
    ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
    ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
    ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
    ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;
    
    //
    f1 *= ratio1 * halfSecHalfTheta;
    f2a *= ratio2;
    f2b *= ratio2;
    alpha *= f1 + f2a;
    beta = f1 + f2b;
    
    //
    float w = alpha * q1w + beta * q2w;
    float x = alpha * q1x + beta * q2x;
    float y = alpha * q1y + beta * q2y;
    float z = alpha * q1z + beta * q2z;
    
    //
    f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
    *dstw = w * f1;
    *dstx = x * f1;
    *dsty = y * f1;
    *dstz = z * f1;
}
