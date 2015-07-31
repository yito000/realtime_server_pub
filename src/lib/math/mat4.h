#ifndef MAT4_H
#define MAT4_H

#include "vec3.h"
#include "vec4.h"
#include "quaternion.h"

#ifdef __SSE__
#include <xmmintrin.h>
#endif

class Mat4
{
public:
#ifdef __SSE__
    union {
        __m128 col[4];
        float m[16];
    };
#else
    float m[16];
#endif

    Mat4();
    Mat4(float m1, float m2, float m3, float m4,
        float m5, float m6, float m7, float m8,
        float m9, float m10, float m11, float m12,
        float m13, float m14, float m15, float m16);
    Mat4(float* f);
    Mat4(const Mat4& matrix);

    // inline function
    inline void set(int index, float val);

    inline float operator[](int index) const;
    
    inline Mat4 operator+(const Mat4& matrix) const;
    inline Mat4& operator+=(const Mat4& matrix);
    
    inline Mat4 operator-(const Mat4& matrix) const;
    inline Mat4& operator-=(const Mat4& matrix);

    inline Mat4 operator*(const Mat4& matrix) const;
    inline Mat4& operator*=(const Mat4& matrix);

    // method
    void identity();
    
    void add(float scalar);
    void add(float scalar, Mat4* dst);
    void add(const Mat4& matrix);
    void add(const Mat4& matrix, Mat4* dst);
    
    void subtract(float scalar);
    void subtract(float scalar, Mat4* dst);
    void subtract(const Mat4& matrix);
    void subtract(const Mat4& matrix, Mat4* dst);
    
    void multiply(float scalar);
    void multiply(float scalar, Mat4* dst);
    void multiply(const Mat4& matrix);
    void multiply(const Mat4& matrix, Mat4* dst);
    
    void translate(float x, float y, float z);
    void translate(float x, float y, float z, Mat4* matrix);
    void translate(const Vec3& t);
    void translate(const Vec3& t, Mat4* matrix);
    
    void rotate(const Quaternion& q);
    void rotate(const Quaternion& q, Mat4* matrix);
    void rotate(const Vec3& axis, float angle);
    void rotate(const Vec3& axis, float angle, Mat4* matrix);
    void rotateX(float angle);
    void rotateX(float angle, Mat4* matrix);
    void rotateY(float angle);
    void rotateY(float angle, Mat4* matrix);
    void rotateZ(float angle);
    void rotateZ(float angle, Mat4* matrix);
    
    void scale(float f);
    void scale(float f, Mat4* matrix);
    void scale(float scale_x, float scale_y, float scale_z);
    void scale(float scale_x, float scale_y, float scale_z, Mat4* matrix);
    void scale(const Vec3& s);
    void scale(const Vec3& s, Mat4* matrix);
    
    bool inverse();
    Mat4 getInverse() const;
    
    void negate();
    Mat4 getNegate() const;

    void transpose();
    Mat4 getTranspose() const;
    
    bool decompose(Vec3* translation, Quaternion* rotation, Vec3* scale) const;
    float determinant() const;
    
    void getTranslation(Vec3* translation) const;
    void getRotation(Quaternion* rotation) const;
    void getScale(Vec3* scale) const;
    
    void multiplyVector(Vec3* v) const;
    void multiplyVector(const Vec3& v, Vec3* dst) const;
    void multiplyVector(float x, float y, float z, float w, Vec3* dst) const;
    void multiplyVector(Vec4* v) const;
    void multiplyVector(const Vec4& v, Vec4* dst) const;

    // static method
    static void createTranslation(const Vec3& translation, Mat4* m);
    static void createTranslation(float x, float y, float z, Mat4* m);
    
    static void createRotation(const Quaternion& q, Mat4* m);
    static void createRotation(const Vec3& axis, float angle, Mat4* m);
    static void createRotationX(float angle, Mat4* m);
    static void createRotationY(float angle, Mat4* m);
    static void createRotationZ(float angle, Mat4* m);
    
    static void createScale(const Vec3& scale, Mat4* m);
    static void createScale(float scale_x, float scale_y, float scale_z, Mat4* m);

    static void createPerspective(double fov, double aspect, double near, double far, Mat4* m);
    static void createOrthographic(float left, float right, float bottom, 
        float top, float near, float far, Mat4* m);
    
    static void createLookAt(const Vec3& eye, const Vec3& center, const Vec3& up, Mat4* m);
    static void createLookAt(float eye_x, float eye_y, float eye_z,
        float center_x, float center_y, float center_z,
        float up_x, float up_y, float up_z, Mat4* m);
};

inline Vec3& operator*=(Vec3& v, const Mat4& m);
inline const Vec3 operator*(const Mat4& m, const Vec3& v);

inline Vec4& operator*=(Vec4& v, const Mat4& m);
inline const Vec4 operator*(const Mat4& m, const Vec4& v);

#include "mat4_inl.h"

#endif
