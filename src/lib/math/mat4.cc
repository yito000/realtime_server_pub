#include "mat4.h"

#include <math.h>
#include <string.h>
#include <float.h>

#ifdef __SSE__
#include "sse/matrix_sse.h"
#include "sse/matrix_sse_inl.h"
#endif

Mat4::Mat4()
{
    identity();
}

Mat4::Mat4(float m1, float m2, float m3, float m4,
    float m5, float m6, float m7, float m8,
    float m9, float m10, float m11, float m12,
    float m13, float m14, float m15, float m16)
{
    m[0] = m1;
    m[1] = m5;
    m[2] = m9;
    m[3] = m13;
    m[4] = m2;
    m[5] = m6;
    m[6] = m10;
    m[7] = m14;
    m[8] = m3;
    m[9] = m7;
    m[10] = m11;
    m[11] = m15;
    m[12] = m4;
    m[13] = m8;
    m[14] = m12;
    m[15] = m16;
}

Mat4::Mat4(float* f)
{
    for (int i = 0; i < 16; i++) {
        m[i] = f[i];
    }
}

Mat4::Mat4(const Mat4& matrix)
{
    for (int i = 0; i < 16; i++) {
        m[i] = matrix[i];
    }
}

void Mat4::identity()
{
    m[0] = 1.0;
    m[1] = 0.0;
    m[2] = 0.0;
    m[3] = 0.0;
    m[4] = 0.0;
    m[5] = 1.0;
    m[6] = 0.0;
    m[7] = 0.0;
    m[8] = 0.0;
    m[9] = 0.0;
    m[10] = 1.0;
    m[11] = 0.0;
    m[12] = 0.0;
    m[13] = 0.0;
    m[14] = 0.0;
    m[15] = 1.0;
}

void Mat4::add(float scalar)
{
#ifdef __SSE__
    MatrixSSE::add(this->col, scalar, this->col);
#else
    for (int i = 0; i < 16; i++) {
        m[i] += scalar;
    }
#endif
}

void Mat4::add(float scalar, Mat4* dst)
{
#ifdef __SSE__
    MatrixSSE::add(this->col, scalar, dst->col);
#else
    for (int i = 0; i < 16; i++) {
        dst->m[i] = m[i] + scalar;
    }
#endif
}

void Mat4::add(const Mat4& matrix)
{
#ifdef __SSE__
    MatrixSSE::add(this->col, matrix.col, this->col);
#else
    for (int i = 0; i < 16; i++) {
        m[i] = m[i] + matrix.m[i];
    }
#endif
}

void Mat4::add(const Mat4& matrix, Mat4* dst)
{
#ifdef __SSE__
    MatrixSSE::add(this->col, matrix.col, dst->col);
#else
    for (int i = 0; i < 16; i++) {
        dst->m[i] = m[i] + matrix.m[i];
    }
#endif
}

void Mat4::subtract(float scalar)
{
#ifdef __SSE__
    MatrixSSE::subtract(this->col, scalar, this->col);
#else
    for (int i = 0; i < 16; i++) {
        m[i] -= scalar;
    }
#endif
}

void Mat4::subtract(float scalar, Mat4* dst)
{
#ifdef __SSE__
    MatrixSSE::subtract(this->col, scalar, dst->col);
#else
    for (int i = 0; i < 16; i++) {
        dst->m[i] = m[i] - scalar;
    }
#endif
}

void Mat4::subtract(const Mat4& matrix)
{
#ifdef __SSE__
    MatrixSSE::subtract(this->col, matrix.col, this->col);
#else
    for (int i = 0; i < 16; i++) {
        m[i] = m[i] - matrix.m[i];
    }
#endif
}

void Mat4::subtract(const Mat4& matrix, Mat4* dst)
{
#ifdef __SSE__
    MatrixSSE::subtract(this->col, matrix.col, dst->col);
#else
    for (int i = 0; i < 16; i++) {
        dst->m[i] = m[i] - matrix.m[i];
    }
#endif
}

void Mat4::multiply(float scalar)
{
#ifdef __SSE__
    MatrixSSE::multiply(this->col, scalar, this->col);
#else
    for (int i = 0; i < 16; i++) {
        m[i] *= scalar;
    }
#endif
}

void Mat4::multiply(float scalar, Mat4* dst)
{
#ifdef __SSE__
    MatrixSSE::multiply(this->col, scalar, dst->col);
#else
    for (int i = 0; i < 16; i++) {
        dst->m[i] = m[i] * scalar;
    }
#endif
}

static void MultiplyMatrix(const Mat4& m1, const Mat4& m2, Mat4* dst)
{
    Mat4 tmp_mat;
    const Mat4& this_inst = m1;
    const Mat4& matrix = m2;

    tmp_mat.set(0, this_inst[0] * matrix[0] + this_inst[4] * matrix[1] + this_inst[8] * matrix[2] + this_inst[12] * matrix[3]);
    tmp_mat.set(4, this_inst[0] * matrix[4] + this_inst[4] * matrix[5] + this_inst[8] * matrix[6] + this_inst[12] * matrix[7]);
    tmp_mat.set(8, this_inst[0] * matrix[8] + this_inst[4] * matrix[9] + this_inst[8] * matrix[10] + this_inst[12] * matrix[11]);
    tmp_mat.set(12, this_inst[0] * matrix[12] + this_inst[4] * matrix[13] + this_inst[8] * matrix[14] + this_inst[12] * matrix[15]);

    tmp_mat.set(1, this_inst[1] * matrix[0] + this_inst[5] * matrix[1] + this_inst[9] * matrix[2] + this_inst[13] * matrix[3]);
    tmp_mat.set(5, this_inst[1] * matrix[4] + this_inst[5] * matrix[5] + this_inst[9] * matrix[6] + this_inst[13] * matrix[7]);
    tmp_mat.set(9, this_inst[1] * matrix[8] + this_inst[5] * matrix[9] + this_inst[9] * matrix[10] + this_inst[13] * matrix[11]);
    tmp_mat.set(13, this_inst[1] * matrix[12] + this_inst[5] * matrix[13] + this_inst[9] * matrix[14] + this_inst[13] * matrix[15]);

    tmp_mat.set(2, this_inst[2] * matrix[0] + this_inst[6] * matrix[1] + this_inst[10] * matrix[2] + this_inst[14] * matrix[3]);
    tmp_mat.set(6, this_inst[2] * matrix[4] + this_inst[6] * matrix[5] + this_inst[10] * matrix[6] + this_inst[14] * matrix[7]);
    tmp_mat.set(10, this_inst[2] * matrix[8] + this_inst[6] * matrix[9] + this_inst[10] * matrix[10] + this_inst[14] * matrix[11]);
    tmp_mat.set(14, this_inst[2] * matrix[12] + this_inst[6] * matrix[13] + this_inst[10] * matrix[14] + this_inst[14] * matrix[15]);

    tmp_mat.set(3, this_inst[3] * matrix[0] + this_inst[7] * matrix[1] + this_inst[11] * matrix[2] + this_inst[15] * matrix[3]);
    tmp_mat.set(7, this_inst[3] * matrix[4] + this_inst[7] * matrix[5] + this_inst[11] * matrix[6] + this_inst[15] * matrix[7]);
    tmp_mat.set(11, this_inst[3] * matrix[8] + this_inst[7] * matrix[9] + this_inst[11] * matrix[10] + this_inst[15] * matrix[11]);
    tmp_mat.set(15, this_inst[3] * matrix[12] + this_inst[7] * matrix[13] + this_inst[11] * matrix[14] + this_inst[15] * matrix[15]);
    
    memcpy(dst->m, tmp_mat.m, sizeof(float) * 16);
}

void Mat4::multiply(const Mat4& matrix)
{
#ifdef __SSE__
    MatrixSSE::multiply(this->col, matrix.col, this->col);
#else
    MultiplyMatrix(*this, matrix, this);
#endif
}

void Mat4::multiply(const Mat4& matrix, Mat4* dst)
{
#ifdef __SSE__
    MatrixSSE::multiply(this->col, matrix.col, dst->col);
#else
    MultiplyMatrix(*this, matrix, dst);
#endif
}

void Mat4::translate(float x, float y, float z)
{
    translate(x, y, z, this);
}

void Mat4::translate(float x, float y, float z, Mat4* matrix)
{
    Mat4 t;
    Mat4::createTranslation(x, y, z, &t);
    
    (*this) *= t;
}

void Mat4::translate(const Vec3& t)
{
    translate(t.x, t.y, t.z, this);
}

void Mat4::translate(const Vec3& t, Mat4* matrix)
{
    translate(t.x, t.y, t.z, matrix);
}

void Mat4::rotate(const Quaternion& q)
{
    rotate(q, this);
}

void Mat4::rotate(const Quaternion& q, Mat4* matrix)
{
    Mat4 r;
    Mat4::createRotation(q, &r);
    
    (*this) *= r;
}

void Mat4::rotate(const Vec3& axis, float angle)
{
    rotate(axis, angle, this);
}

void Mat4::rotate(const Vec3& axis, float angle, Mat4* matrix)
{
    Mat4 r;
    Mat4::createRotation(axis, angle, &r);
    
    (*this) *= r;
}

void Mat4::rotateX(float angle)
{
    rotateX(angle, this);
}

void Mat4::rotateX(float angle, Mat4* matrix)
{
    Mat4 r;
    Mat4::createRotationX(angle, &r);
    
    (*this) *= r;
}

void Mat4::rotateY(float angle)
{
    rotateY(angle, this);
}

void Mat4::rotateY(float angle, Mat4* matrix)
{
    Mat4 r;
    Mat4::createRotationY(angle, &r);
    
    (*this) *= r;
}

void Mat4::rotateZ(float angle)
{
    rotateZ(angle, this);
}

void Mat4::rotateZ(float angle, Mat4* matrix)
{
    Mat4 r;
    Mat4::createRotationZ(angle, &r);
    
    (*this) *= r;
}

void Mat4::scale(float f)
{
    scale(f, this);
}

void Mat4::scale(float f, Mat4* matrix)
{
    scale(f, f, f, matrix);
}

void Mat4::scale(float scale_x, float scale_y, float scale_z)
{
    scale(scale_x, scale_y, scale_z, this);
}

void Mat4::scale(float scale_x, float scale_y, float scale_z, Mat4* matrix)
{
    Mat4 s;
    Mat4::createScale(scale_x, scale_y, scale_z, &s);
    
    (*this) *= s;
}

void Mat4::scale(const Vec3& s)
{
    scale(s.x, s.y, s.z, this);
}

void Mat4::scale(const Vec3& s, Mat4* matrix)
{
    scale(s.x, s.y, s.z, matrix);
}

bool Mat4::inverse()
{
    Mat4 inv;

    inv.m[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv.m[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv.m[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv.m[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv.m[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv.m[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv.m[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv.m[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv.m[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv.m[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv.m[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv.m[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv.m[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv.m[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv.m[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv.m[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    double det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det <= 2e-37f) {
        return false;
    }

    det = 1.0 / det;

    for (int i = 0; i < 16; i++) {
        m[i] = inv[i] * det;
    }

    return true;
}

Mat4 Mat4::getInverse() const
{
    Mat4 out(*this);
    out.inverse();

    return out;
}

void Mat4::negate()
{
#ifdef __SSE__
    MatrixSSE::negate(this->col, this->col);
#else
    m[0]  = -m[0];
    m[1]  = -m[1];
    m[2]  = -m[2];
    m[3]  = -m[3];
    m[4]  = -m[4];
    m[5]  = -m[5];
    m[6]  = -m[6];
    m[7]  = -m[7];
    m[8]  = -m[8];
    m[9]  = -m[9];
    m[10] = -m[10];
    m[11] = -m[11];
    m[12] = -m[12];
    m[13] = -m[13];
    m[14] = -m[14];
    m[15] = -m[15];
#endif
}

Mat4 Mat4::getNegate() const
{
    Mat4 out(*this);
    out.negate();
    
    return out;
}

void Mat4::transpose()
{
#ifdef __SSE__
    MatrixSSE::transpose(this->col, this->col);
#else
    Mat4 mm = *this;

    float t[16] = {
        mm[0], mm[4], mm[8], mm[12],
        mm[1], mm[5], mm[9], mm[13],
        mm[2], mm[6], mm[10], mm[14],
        mm[3], mm[7], mm[11], mm[15]
    };
    memcpy(this->m, t, sizeof(float) * 16);
#endif
}

Mat4 Mat4::getTranspose() const
{
    Mat4 out(*this);
    out.transpose();
    
    return out;
}

bool Mat4::decompose(Vec3* translation, Quaternion* rotation, Vec3* scale) const
{
    if (translation) {
        translation->x = m[12];
        translation->y = m[13];
        translation->z = m[14];
    }
    
    Vec3 xaxis(m[0], m[1], m[2]);
    float scale_x = xaxis.length();
    
    Vec3 yaxis(m[4], m[5], m[6]);
    float scale_y = yaxis.length();

    Vec3 zaxis(m[8], m[9], m[10]);
    float scale_z = zaxis.length();

    float det = determinant();
    if (det < 0.0) {
        scale_z = -scale_z;
    }

    if (scale) {
        scale->x = scale_x;
        scale->y = scale_y;
        scale->z = scale_z;
    }
    
    if (!rotation) {
        return true;
    }
    
    /*
    if (scale_x < 2e-37f || scale_y < 2e-37f || scale_z < 2e-37f) {
        return false;
    }
    */

    {
        float rn;

        rn = 1.0f / scale_x;
        xaxis.x *= rn;
        xaxis.y *= rn;
        xaxis.z *= rn;

        rn = 1.0f / scale_y;
        yaxis.x *= rn;
        yaxis.y *= rn;
        yaxis.z *= rn;

        rn = 1.0f / scale_z;
        zaxis.x *= rn;
        zaxis.y *= rn;
        zaxis.z *= rn;
    }
    
    float trace = xaxis.x + yaxis.y + zaxis.z;
    if (trace > 0.0f) {
        float s = 0.5f / sqrtf(trace + 1.0f);
        
        rotation->w = 0.25f / s;
        rotation->x = (yaxis.z - zaxis.y) * s;
        rotation->y = (zaxis.x - xaxis.z) * s;
        rotation->z = (xaxis.y - yaxis.x) * s;
    } else {
        if (xaxis.x > yaxis.y && xaxis.x > zaxis.z) {
            float s = 2.0 * sqrtf(1.0f + xaxis.x - yaxis.y - zaxis.z);
            
            rotation->w = (yaxis.z - zaxis.y) / s;
            rotation->x = 0.25f * s;
            rotation->y = (yaxis.x + xaxis.y) / s;
            rotation->z = (zaxis.x + xaxis.z) / s;
        } else if (yaxis.y > zaxis.z) {
            float s = 2.0 * sqrtf(1.0f + yaxis.y - xaxis.x - zaxis.z);
            
            rotation->w = (zaxis.x - xaxis.z) / s;
            rotation->x = (yaxis.x + xaxis.y) / s;
            rotation->y = 0.25f * s;
            rotation->z = (zaxis.y + yaxis.z) / s;
        } else {
            float s = 2.0 * sqrtf(1.0f + zaxis.z - xaxis.x - yaxis.y );
            
            rotation->w = (xaxis.y - yaxis.x) / s;
            rotation->x = (zaxis.x + xaxis.z) / s;
            rotation->y = (zaxis.y + yaxis.z) / s;
            rotation->z = 0.25f * s;
        }
    }
    
    return true;
}

float Mat4::determinant() const
{
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];

    return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

void Mat4::multiplyVector(Vec3* v) const
{
    multiplyVector(v->x, v->y, v->z, 1.0f, v);
}

void Mat4::multiplyVector(const Vec3& v, Vec3* dst) const
{
    multiplyVector(v.x, v.y, v.z, 1.0f, dst);
}

void Mat4::multiplyVector(float x, float y, float z, float w, Vec3* dst) const
{
    dst->x = x * m[0] + y * m[4] + z * m[8] + w * m[12];
    dst->y = x * m[1] + y * m[5] + z * m[9] + w * m[13];
    dst->z = x * m[2] + y * m[6] + z * m[10] + w * m[14];
}

void Mat4::multiplyVector(Vec4* v) const
{
    multiplyVector(*v, v);
}

void Mat4::multiplyVector(const Vec4& v, Vec4* dst) const
{
#ifdef __SSE__
    MatrixSSE::transformVector4(col, v.v, dst->v);
#else
    float x = v.x;
    float y = v.y;
    float z = v.z;
    float w = v.w;
    
    dst->x = x * m[0] + y * m[4] + z * m[8] + w * m[12];
    dst->y = x * m[1] + y * m[5] + z * m[9] + w * m[13];
    dst->z = x * m[2] + y * m[6] + z * m[10] + w * m[14];
    dst->w = x * m[3] + y * m[7] + z * m[11] + w * m[15];
#endif
}

void Mat4::getTranslation(Vec3* translation) const
{
    decompose(translation, nullptr, nullptr);
}

void Mat4::getRotation(Quaternion* rotation) const
{
    decompose(nullptr, rotation, nullptr);
}

void Mat4::getScale(Vec3* scale) const
{
    decompose(nullptr, nullptr, scale);
}

// static public method
void Mat4::createTranslation(const Vec3& translation, Mat4* m)
{
    m->identity();
    
    m->m[12] = translation.x;
    m->m[13] = translation.y;
    m->m[14] = translation.z;
}

void Mat4::createTranslation(float x, float y, float z, Mat4* m)
{
    m->identity();
    
    m->m[12] = x;
    m->m[13] = y;
    m->m[14] = z;
}

void Mat4::createRotation(const Quaternion& q, Mat4* m)
{
    float x2 = q.x + q.x;
    float y2 = q.y + q.y;
    float z2 = q.z + q.z;

    float xx2 = q.x * x2;
    float yy2 = q.y * y2;
    float zz2 = q.z * z2;
    float xy2 = q.x * y2;
    float xz2 = q.x * z2;
    float yz2 = q.y * z2;
    float wx2 = q.w * x2;
    float wy2 = q.w * y2;
    float wz2 = q.w * z2;

    m->m[0] = 1.0f - yy2 - zz2;
    m->m[1] = xy2 + wz2;
    m->m[2] = xz2 - wy2;
    m->m[3] = 0.0f;

    m->m[4] = xy2 - wz2;
    m->m[5] = 1.0f - xx2 - zz2;
    m->m[6] = yz2 + wx2;
    m->m[7] = 0.0f;

    m->m[8] = xz2 + wy2;
    m->m[9] = yz2 - wx2;
    m->m[10] = 1.0f - xx2 - yy2;
    m->m[11] = 0.0f;

    m->m[12] = 0.0f;
    m->m[13] = 0.0f;
    m->m[14] = 0.0f;
    m->m[15] = 1.0f;
}

void Mat4::createRotation(const Vec3& axis, float angle, Mat4* m)
{
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float n = x * x + y * y + z * z;
    if (n != 1.0f)
    {
        n = sqrtf(n);
        
        if (n > 0.000001f)
        {
            n = 1.0f / n;
            x *= n;
            y *= n;
            z *= n;
        }
    }

    float c = cos(angle);
    float s = sin(angle);

    float t = 1.0f - c;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float txy = tx * y;
    float txz = tx * z;
    float tyz = ty * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;

    m->m[0] = c + tx * x;
    m->m[1] = txy + sz;
    m->m[2] = txz - sy;
    m->m[3] = 0.0f;

    m->m[4] = txy - sz;
    m->m[5] = c + ty * y;
    m->m[6] = tyz + sx;
    m->m[7] = 0.0f;

    m->m[8] = txz + sy;
    m->m[9] = tyz - sx;
    m->m[10] = c + tz * z;
    m->m[11] = 0.0f;

    m->m[12] = 0.0f;
    m->m[13] = 0.0f;
    m->m[14] = 0.0f;
    m->m[15] = 1.0f;
}

void Mat4::createRotationX(float angle, Mat4* m)
{
    m->identity();
    
    float c = cos(angle);
    float s = sin(angle);
    
    m->m[5] = c;
    m->m[6] = s;
    m->m[9] = -s;
    m->m[10] = c;
}

void Mat4::createRotationY(float angle, Mat4* m)
{
    m->identity();
    
    float c = cos(angle);
    float s = sin(angle);
    
    m->m[0] = c;
    m->m[2] = -s;
    m->m[8] = s;
    m->m[10] = c;
}

void Mat4::createRotationZ(float angle, Mat4* m)
{
    m->identity();
    
    float c = cos(angle);
    float s = sin(angle);
    
    m->m[0] = c;
    m->m[1] = s;
    m->m[4] = -s;
    m->m[5] = c;
}

void Mat4::createScale(const Vec3& scale, Mat4* m)
{
    m->identity();
    
    m->m[0] = scale.x;
    m->m[5] = scale.y;
    m->m[10] = scale.z;
}

void Mat4::createScale(float scale_x, float scale_y, float scale_z, Mat4* m)
{
    m->identity();
    
    m->m[0] = scale_x;
    m->m[5] = scale_y;
    m->m[10] = scale_z;
}

void Mat4::createPerspective(double fov, double aspect, double near, double far, Mat4* m)
{
    const double DEGREES_TO_RADIAN = 3.141592 / 180.0;
    double theta = DEGREES_TO_RADIAN * fov * 0.5;
    double factor = 1.0 / tan(theta);
    double far_near = 1.0 / (far - near);
    
    m->identity();

    m->m[0] = (1.0 / aspect) * factor;
    m->m[5] = factor;
    m->m[10] = (-(far + near)) * far_near;
    m->m[11] = -1.0;
    m->m[14] = -2.0 * far * near * far_near;
}

void Mat4::createOrthographic(float left, float right, float bottom, float top, float near, float far, Mat4* m)
{
    m->identity();
    
    m->m[0] = 2.0 / (right - left);
    m->m[5] = 2.0 / (top - bottom);
    m->m[10] = -2.0 / (far - near);
    m->m[12] = -(right + left) / (right - left);
    m->m[13] = -(top + bottom) / (top - bottom);
    m->m[14] = -(far + near) / (far - near);
}

void Mat4::createLookAt(const Vec3& eye, const Vec3& center, const Vec3& up, Mat4* m)
{
    createLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z, m);
}

void Mat4::createLookAt(float eye_x, float eye_y, float eye_z,
    float center_x, float center_y, float center_z,
    float up_x, float up_y, float up_z, Mat4* m)
{
    Vec3 eye(eye_x, eye_y, eye_z);
    Vec3 center(center_x, center_y, center_z);
    Vec3 up(up_x, up_y, up_z);
    up.normalize();

    Vec3 zaxis;
    Vec3::subtract(eye, center, &zaxis);
    zaxis.normalize();

    Vec3 xaxis;
    Vec3::cross(up, zaxis, &xaxis);
    xaxis.normalize();

    Vec3 yaxis;
    Vec3::cross(zaxis, xaxis, &yaxis);
    yaxis.normalize();

    m->m[0] = xaxis.x;
    m->m[1] = yaxis.x;
    m->m[2] = zaxis.x;
    m->m[3] = 0.0;

    m->m[4] = xaxis.y;
    m->m[5] = yaxis.y;
    m->m[6] = zaxis.y;
    m->m[7] = 0.0;

    m->m[8] = xaxis.z;
    m->m[9] = yaxis.z;
    m->m[10] = zaxis.z;
    m->m[11] = 0.0;

    m->m[12] = -Vec3::dot(xaxis, eye);
    m->m[13] = -Vec3::dot(yaxis, eye);
    m->m[14] = -Vec3::dot(zaxis, eye);
    m->m[15] = 1.0;
}
