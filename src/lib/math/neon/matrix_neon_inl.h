#ifdef __ARM_NEON__

#ifndef MATRIX_NEON_INL_H
#define MATRIX_NEON_INL_H

void inline MatrixNeon::multiply(const float32x4x4_t& v1, const float32x4x4_t& v2, float32x4x4_t& dst)
{
    float32x4_t m1 = vmulq_n_f32(v2.val[3], vgetq_lane_f32(v1.val[0], 3));
    float32x4_t m2 = vmulq_n_f32(v2.val[3], vgetq_lane_f32(v1.val[1], 3));
    float32x4_t m3 = vmulq_n_f32(v2.val[3], vgetq_lane_f32(v1.val[2], 3));
    float32x4_t m4 = vmulq_n_f32(v2.val[3], vgetq_lane_f32(v1.val[3], 3));
    
    float32x4_t m11 = vmlaq_n_f32(m1, v2.val[2], vgetq_lane_f32(v1.val[0], 2));
    float32x4_t m12 = vmlaq_n_f32(m2, v2.val[2], vgetq_lane_f32(v1.val[1], 2));
    float32x4_t m13 = vmlaq_n_f32(m3, v2.val[2], vgetq_lane_f32(v1.val[2], 2));
    float32x4_t m14 = vmlaq_n_f32(m4, v2.val[2], vgetq_lane_f32(v1.val[3], 2));
    
    float32x4_t m21 = vmlaq_n_f32(m11, v2.val[1], vgetq_lane_f32(v1.val[0], 1));
    float32x4_t m22 = vmlaq_n_f32(m12, v2.val[1], vgetq_lane_f32(v1.val[1], 1));
    float32x4_t m23 = vmlaq_n_f32(m13, v2.val[1], vgetq_lane_f32(v1.val[2], 1));
    float32x4_t m24 = vmlaq_n_f32(m14, v2.val[1], vgetq_lane_f32(v1.val[3], 1));
    
    dst.val[0] = vmlaq_n_f32(m21, v2.val[0], vgetq_lane_f32(v1.val[0], 0));
    dst.val[1] = vmlaq_n_f32(m22, v2.val[0], vgetq_lane_f32(v1.val[1], 0));
    dst.val[2] = vmlaq_n_f32(m23, v2.val[0], vgetq_lane_f32(v1.val[2], 0));
    dst.val[3] = vmlaq_n_f32(m24, v2.val[0], vgetq_lane_f32(v1.val[3], 0));
}

void inline MatrixNeon::transformVector4(const float32x4x4_t& m, const float32x4_t& v, float32x4_t& dst)
{
    dst = vmulq_n_f32(m.val[0], v[0]);
    
    dst = vmlaq_n_f32(dst, m.val[1], v[1]);
    dst = vmlaq_n_f32(dst, m.val[2], v[2]);
    dst = vmlaq_n_f32(dst, m.val[3], v[3]);
}

#endif

#endif
