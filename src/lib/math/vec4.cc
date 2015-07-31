#include "vec4.h"

#include <math.h>

Vec4::Vec4() :
    x(0.0), y(0.0), z(0.0), w(0.0)
{
    //
}

Vec4::Vec4(float _x, float _y, float _z, float _w) :
    x(_x), y(_y), z(_z), w(_w)
{
    //
}

Vec4::Vec4(float* f) :
    x(f[0]), y(f[1]), z(f[2]), w(f[3])
{
    //
}

void Vec4::add(const Vec4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}

void Vec4::subtract(const Vec4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

void Vec4::clamp(const Vec4& min, const Vec4& max)
{
    clamp(min, max, this);
}

void Vec4::normalize()
{
    float f = x * x + y * y + z * z + w * w;
    if (f == 1.0f) {
        return;
    }

    f = sqrt(f);
    if (f <= 2e-37f) {
        return;
    }

    f = 1.0 / f;

    x *= f;
    y *= f;
    z *= f;
    w *= f;
}

Vec4 Vec4::getNormalized() const
{
    Vec4 out(*this);
    out.normalize();
    
    return out;
}

float Vec4::dot(const Vec4& v) const
{
    return Vec4::dot(*this, v);
}

float Vec4::angle(const Vec4& v1, const Vec4& v2)
{
    float dx = v1.w * v2.x - v1.x * v2.w - v1.y * v2.z + v1.z * v2.y;
    float dy = v1.w * v2.y - v1.y * v2.w - v1.z * v2.x + v1.x * v2.z;
    float dz = v1.w * v2.z - v1.z * v2.w - v1.x * v2.y + v1.y * v2.x;

    return atan2f(sqrt(dx * dx + dy * dy + dz * dz), dot(v1, v2));
}

float Vec4::distance(const Vec4& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;
    float dw = v.w - w;

    return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

float Vec4::length() const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

float Vec4::lengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}

void Vec4::negate()
{
    x = -x;
    y = -y;
    z = -z;
    w = -w;
}

void Vec4::set(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Vec4::set(const Vec4& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = v.w;
}

void Vec4::set(const float* f)
{
    this->x = f[0];
    this->y = f[1];
    this->z = f[2];
    this->w = f[3];
}

// static public function
void Vec4::add(const Vec4& v1, const Vec4& v2, Vec4* v)
{
    v->x = v1.x + v2.x;
    v->y = v1.y + v2.y;
    v->z = v1.z + v2.z;
    v->w = v1.w + v2.w;
}

void Vec4::subtract(const Vec4& v1, const Vec4& v2, Vec4* v)
{
    v->x = v1.x - v2.x;
    v->y = v1.y - v2.y;
    v->z = v1.z - v2.z;
    v->w = v1.w - v2.w;
}

void Vec4::clamp(const Vec4& min, const Vec4& max, Vec4* v)
{
    if (v->x < min.x) {
        v->x = min.x;
    } else if (v->x > max.x) {
        v->x = max.x;
    }
    
    if (v->y < min.y) {
        v->y = min.y;
    } else if (v->y > max.y) {
        v->y = max.y;
    }
    
    if (v->z < min.z) {
        v->z = min.z;
    } else if (v->z > max.z) {
        v->z = max.z;
    }
    
    if (v->w < min.w) {
        v->w = min.w;
    } else if (v->w > max.w) {
        v->w = max.w;
    }
}

float Vec4::dot(const Vec4& v1, const Vec4& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vec4 Vec4::lerp(const Vec4& v, float t) const
{
    return *this * (1.0 - t) + v * t;
}
    
// operator
const Vec4 Vec4::operator+(const Vec4& v) const
{
    Vec4 ret;
    Vec4::add(*this, v, &ret);
    
    return ret;
}

Vec4& Vec4::operator+=(const Vec4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    
    return *this;
}

const Vec4 Vec4::operator-(const Vec4& v) const
{
    Vec4 ret;
    Vec4::subtract(*this, v, &ret);
    
    return ret;
}

Vec4& Vec4::operator-=(const Vec4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    
    return *this;
}

const Vec4 Vec4::operator-() const
{
    Vec4 ret(*this);
    ret.negate();
    
    return ret;
}

const Vec4 Vec4::operator*(float f) const
{
    Vec4 ret(*this);
    ret.x *= f;
    ret.y *= f;
    ret.z *= f;
    ret.w *= f;
    
    return ret;
}

Vec4& Vec4::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    
    return *this;
}

const Vec4 Vec4::operator/(float f) const
{
    Vec4 ret(*this);
    ret.x /= f;
    ret.y /= f;
    ret.z /= f;
    ret.w /= f;
    
    return ret;
}

Vec4 Vec4::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    w /= f;
    
    return *this;
}

bool Vec4::operator<(const Vec4& v) const
{
    if (x == v.x) {
        if (y == v.y) {
            if (z == v.z) {
                return w < v.w;
            }
            
            return z < v.z;
        }
        
        return y < v.y;
    }
    
    return x < v.x;
}

bool Vec4::operator==(const Vec4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vec4::operator!=(const Vec4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}
