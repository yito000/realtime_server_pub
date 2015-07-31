#include "vec2.h"

#include <math.h>

Vec2::Vec2() :
    x(0.0), y(0.0)
{
    //
}

Vec2::Vec2(float _x, float _y) :
    x(_x), y(_y)
{
    //
}

Vec2::Vec2(float* f) :
    x(f[0]), y(f[1])
{
    //
}

void Vec2::add(const Vec2& v)
{
    x += v.x;
    y += v.y;
}

void Vec2::subtract(const Vec2& v)
{
    x -= v.x;
    y -= v.y;
}

void Vec2::clamp(const Vec2& min, const Vec2& max)
{
    clamp(min, max, this);
}

void Vec2::normalize()
{
    float f = x * x + y * y;
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
}

Vec2 Vec2::getNormalized() const
{
    Vec2 out(*this);
    out.normalize();
    
    return out;
}

float Vec2::dot(const Vec2& v) const
{
    return Vec2::dot(*this, v);
}

Vec2 Vec2::lerp(const Vec2& v, float t) const
{
    return *this * (1.0 - t) + v * t;
}

float Vec2::cross(const Vec2& v) const
{
    return Vec2::cross(*this, v);
}

float Vec2::angle(const Vec2& v1, const Vec2& v2)
{
    float dz = v1.x * v2.y - v1.y * v2.x;

    return atan2f(fabsf(dz), dot(v1, v2));
}

float Vec2::distance(const Vec2& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;

    return sqrt(dx * dx + dy * dy);
}

float Vec2::length() const
{
    return sqrt(x * x + y * y);
}

float Vec2::lengthSquared() const
{
    return x * x + y * y;
}

void Vec2::negate()
{
    x = -x;
    y = -y;
}

void Vec2::set(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vec2::set(const Vec2& v)
{
    this->x = v.x;
    this->y = v.y;
}

void Vec2::set(const float* f)
{
    this->x = f[0];
    this->y = f[1];
}

// static method
void Vec2::add(const Vec2& v1, const Vec2& v2, Vec2* v)
{
    v->x = v1.x + v2.x;
    v->y = v1.y + v2.y;
}

void Vec2::subtract(const Vec2& v1, const Vec2& v2, Vec2* v)
{
    v->x = v1.x - v2.x;
    v->y = v1.y - v2.y;
}

void Vec2::clamp(const Vec2& min, const Vec2& max, Vec2* v)
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
}

float Vec2::dot(const Vec2& v1, const Vec2& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float Vec2::cross(const Vec2& v1, const Vec2& v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

// operator
const Vec2 Vec2::operator+(const Vec2& v) const
{
    Vec2 ret;
    Vec2::add(*this, v, &ret);
    
    return ret;
}

Vec2& Vec2::operator+=(const Vec2& v)
{
    x += v.x;
    y += v.y;
    
    return *this;
}

const Vec2 Vec2::operator-(const Vec2& v) const
{
    Vec2 ret;
    Vec2::subtract(*this, v, &ret);
    
    return ret;
}

Vec2& Vec2::operator-=(const Vec2& v)
{
    x -= v.x;
    y -= v.y;
    
    return *this;
}

const Vec2 Vec2::operator*(float f) const
{
    Vec2 ret(*this);
    ret.x *= f;
    ret.y *= f;
    
    return ret;
}

Vec2& Vec2::operator*=(float f)
{
    x *= f;
    y *= f;
    
    return *this;
}

const Vec2 Vec2::operator/(float f) const
{
    Vec2 ret(*this);
    ret.x /= f;
    ret.y /= f;
    
    return ret;
}

Vec2 Vec2::operator/=(float f)
{
    x /= f;
    y /= f;
    
    return *this;
}

const Vec2 Vec2::operator-() const
{
    Vec2 ret(*this);
    ret.negate();
    
    return ret;
}

bool Vec2::operator<(const Vec2& v) const
{
    if (x == v.x) {
        return y < v.y;
    }
    
    return x < v.x;
}

bool Vec2::operator==(const Vec2& v) const
{
    return x == v.x && y == v.y;
}

bool Vec2::operator!=(const Vec2& v) const
{
    return x != v.x || y != v.y;
}
