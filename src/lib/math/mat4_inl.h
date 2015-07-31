#ifndef MAT4_INL_H
#define MAT4_INL_H

#include "mat4.h"

inline void Mat4::set(int index, float val)
{
    m[index] = val;
}

inline float Mat4::operator[](int index) const
{
    return m[index];
}

inline Mat4 Mat4::operator+(const Mat4& matrix) const
{
    Mat4 tmp_mat;
    const Mat4& this_inst = *this;
    
    tmp_mat.set(0, this_inst[0] + matrix[0]);
    tmp_mat.set(1, this_inst[1] + matrix[1]);
    tmp_mat.set(2, this_inst[2] + matrix[2]);
    tmp_mat.set(3, this_inst[3] + matrix[3]);
    
    tmp_mat.set(4, this_inst[4] + matrix[4]);
    tmp_mat.set(5, this_inst[5] + matrix[5]);
    tmp_mat.set(6, this_inst[6] + matrix[6]);
    tmp_mat.set(7, this_inst[7] + matrix[7]);
    
    tmp_mat.set(8, this_inst[8] + matrix[8]);
    tmp_mat.set(9, this_inst[9] + matrix[9]);
    tmp_mat.set(10, this_inst[10] + matrix[10]);
    tmp_mat.set(11, this_inst[11] + matrix[11]);
    
    tmp_mat.set(12, this_inst[12] + matrix[12]);
    tmp_mat.set(13, this_inst[13] + matrix[13]);
    tmp_mat.set(14, this_inst[14] + matrix[14]);
    tmp_mat.set(15, this_inst[15] + matrix[15]);
    
    return tmp_mat;
}

inline Mat4& Mat4::operator+=(const Mat4& matrix)
{
    Mat4 tmp_mat;
    Mat4& this_inst = *this;
    
    tmp_mat.set(0, this_inst[0] + matrix[0]);
    tmp_mat.set(1, this_inst[1] + matrix[1]);
    tmp_mat.set(2, this_inst[2] + matrix[2]);
    tmp_mat.set(3, this_inst[3] + matrix[3]);
    
    tmp_mat.set(4, this_inst[4] + matrix[4]);
    tmp_mat.set(5, this_inst[5] + matrix[5]);
    tmp_mat.set(6, this_inst[6] + matrix[6]);
    tmp_mat.set(7, this_inst[7] + matrix[7]);
    
    tmp_mat.set(8, this_inst[8] + matrix[8]);
    tmp_mat.set(9, this_inst[9] + matrix[9]);
    tmp_mat.set(10, this_inst[10] + matrix[10]);
    tmp_mat.set(11, this_inst[11] + matrix[11]);
    
    tmp_mat.set(12, this_inst[12] + matrix[12]);
    tmp_mat.set(13, this_inst[13] + matrix[13]);
    tmp_mat.set(14, this_inst[14] + matrix[14]);
    tmp_mat.set(15, this_inst[15] + matrix[15]);
    
    for (int i = 0; i < 16; i++) {
        this_inst.set(i, tmp_mat[i]);
    }

    return this_inst;
}

inline Mat4 Mat4::operator-(const Mat4& matrix) const
{
    Mat4 tmp_mat;
    const Mat4& this_inst = *this;
    
    tmp_mat.set(0, this_inst[0] - matrix[0]);
    tmp_mat.set(1, this_inst[1] - matrix[1]);
    tmp_mat.set(2, this_inst[2] - matrix[2]);
    tmp_mat.set(3, this_inst[3] - matrix[3]);
    
    tmp_mat.set(4, this_inst[4] - matrix[4]);
    tmp_mat.set(5, this_inst[5] - matrix[5]);
    tmp_mat.set(6, this_inst[6] - matrix[6]);
    tmp_mat.set(7, this_inst[7] - matrix[7]);
    
    tmp_mat.set(8, this_inst[8] - matrix[8]);
    tmp_mat.set(9, this_inst[9] - matrix[9]);
    tmp_mat.set(10, this_inst[10] - matrix[10]);
    tmp_mat.set(11, this_inst[11] - matrix[11]);
    
    tmp_mat.set(12, this_inst[12] - matrix[12]);
    tmp_mat.set(13, this_inst[13] - matrix[13]);
    tmp_mat.set(14, this_inst[14] - matrix[14]);
    tmp_mat.set(15, this_inst[15] - matrix[15]);
    
    return tmp_mat;
}

inline Mat4& Mat4::operator-=(const Mat4& matrix)
{
    Mat4 tmp_mat;
    Mat4& this_inst = *this;
    
    tmp_mat.set(0, this_inst[0] - matrix[0]);
    tmp_mat.set(1, this_inst[1] - matrix[1]);
    tmp_mat.set(2, this_inst[2] - matrix[2]);
    tmp_mat.set(3, this_inst[3] - matrix[3]);
    
    tmp_mat.set(4, this_inst[4] - matrix[4]);
    tmp_mat.set(5, this_inst[5] - matrix[5]);
    tmp_mat.set(6, this_inst[6] - matrix[6]);
    tmp_mat.set(7, this_inst[7] - matrix[7]);
    
    tmp_mat.set(8, this_inst[8] - matrix[8]);
    tmp_mat.set(9, this_inst[9] - matrix[9]);
    tmp_mat.set(10, this_inst[10] - matrix[10]);
    tmp_mat.set(11, this_inst[11] - matrix[11]);
    
    tmp_mat.set(12, this_inst[12] - matrix[12]);
    tmp_mat.set(13, this_inst[13] - matrix[13]);
    tmp_mat.set(14, this_inst[14] - matrix[14]);
    tmp_mat.set(15, this_inst[15] - matrix[15]);
    
    for (int i = 0; i < 16; i++) {
        this_inst.set(i, tmp_mat[i]);
    }

    return this_inst;
}

inline Mat4 Mat4::operator*(const Mat4& matrix) const
{
    Mat4 tmp_mat;
    const Mat4& this_inst = *this;

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

    return tmp_mat;
}

inline Mat4& Mat4::operator*=(const Mat4& matrix)
{
    Mat4 tmp_mat;
    Mat4& this_inst = *this;

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

    for (int i = 0; i < 16; i++) {
        this_inst.set(i, tmp_mat[i]);
    }

    return this_inst;
}

// inline global function
inline Vec3& operator*=(Vec3& v, const Mat4& m)
{
    m.multiplyVector(&v);
    
    return v;
}

inline const Vec3 operator*(const Mat4& m, const Vec3& v)
{
    Vec3 ret;
    m.multiplyVector(v, &ret);
    
    return ret;
}

inline Vec4& operator*=(Vec4& v, const Mat4& m)
{
    m.multiplyVector(&v);
    
    return v;
}

inline const Vec4 operator*(const Mat4& m, const Vec4& v)
{
    Vec4 ret;
    m.multiplyVector(v, &ret);
    
    return ret;
}

#endif
