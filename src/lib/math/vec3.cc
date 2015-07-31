#include "vec3.h"

#include <math.h>

Vec3::Vec3() :
    x(0.0), y(0.0), z(0.0)
{
    //
}

Vec3::Vec3(float _x, float _y, float _z) :
    x(_x), y(_y), z(_z)
{
    //
}

Vec3::Vec3(float* f) :
    x(f[0]), y(f[1]), z(f[2])
{
    //
}

void Vec3::add(const Vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vec3::subtract(const Vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vec3::clamp(const Vec3& min, const Vec3& max)
{
    clamp(min, max, this);
}

void Vec3::normalize()
{
    float f = x * x + y * y + z * z;
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
}

Vec3 Vec3::getNormalized() const
{
    Vec3 out(*this);
    out.normalize();
    
    return out;
}

float Vec3::dot(const Vec3& v) const
{
    return Vec3::dot(*this, v);
}

void Vec3::cross(const Vec3& v, Vec3* out) const
{
    Vec3::cross(*this, v, out);
}

float Vec3::angle(const Vec3& v1, const Vec3& v2)
{
    float dx = v1.y * v2.z - v1.z * v2.y;
    float dy = v1.z * v2.x - v1.x * v2.z;
    float dz = v1.x * v2.y - v1.y * v2.x;

    return atan2f(sqrt(dx * dx + dy * dy + dz * dz), dot(v1, v2));
}

Vec3 Vec3::lerp(const Vec3& v, float t) const
{
    return *this * (1.0 - t) + v * t;
}

float Vec3::distance(const Vec3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

float Vec3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

float Vec3::lengthSquared() const
{
    return x * x + y * y + z * z;
}

void Vec3::negate()
{
    x = -x;
    y = -y;
    z = -z;
}

void Vec3::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vec3::set(const Vec3& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

void Vec3::set(const float* f)
{
    this->x = f[0];
    this->y = f[1];
    this->z = f[2];
}

// static public function
void Vec3::add(const Vec3& v1, const Vec3& v2, Vec3* v)
{
    v->x = v1.x + v2.x;
    v->y = v1.y + v2.y;
    v->z = v1.z + v2.z;
}

void Vec3::subtract(const Vec3& v1, const Vec3& v2, Vec3* v)
{
    v->x = v1.x - v2.x;
    v->y = v1.y - v2.y;
    v->z = v1.z - v2.z;
}

void Vec3::clamp(const Vec3& min, const Vec3& max, Vec3* v)
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
}

float Vec3::dot(const Vec3& v1, const Vec3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void Vec3::cross(const Vec3& v1, const Vec3& v2, Vec3* v)
{
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;

    v->x = x;
    v->y = y;
    v->z = z;
}

// operator
const Vec3 Vec3::operator+(const Vec3& v) const
{
    Vec3 ret;
    Vec3::add(*this, v, &ret);
    
    return ret;
}

Vec3& Vec3::operator+=(const Vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    
    return *this;
}

const Vec3 Vec3::operator-(const Vec3& v) const
{
    Vec3 ret;
    Vec3::subtract(*this, v, &ret);
    
    return ret;
}

Vec3& Vec3::operator-=(const Vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    
    return *this;
}

const Vec3 Vec3::operator*(float f) const
{
    Vec3 ret(*this);
    ret.x *= f;
    ret.y *= f;
    ret.z *= f;
    
    return ret;
}

Vec3& Vec3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    
    return *this;
}

const Vec3 Vec3::operator/(float f) const
{
    Vec3 ret(*this);
    ret.x /= f;
    ret.y /= f;
    ret.z /= f;
    
    return ret;
}

Vec3 Vec3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    
    return *this;
}

const Vec3 Vec3::operator-() const
{
    Vec3 ret(*this);
    ret.negate();
    
    return ret;
}

bool Vec3::operator<(const Vec3& v) const
{
    if (x == v.x) {
        if (y == v.y) {
            return z < v.z;
        }
        
        return y < v.y;
    }
    
    return x < v.x;
}

bool Vec3::operator==(const Vec3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

bool Vec3::operator!=(const Vec3& v) const
{
    return x != v.x || y != v.y || z != v.z;
}
