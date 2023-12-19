/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"
#include "kf.hpp"
#include "vec.hpp"
#include <vector>

inline float_t interpolate_linear_value(const float_t p1, const float_t p2,
    const float_t f1, const float_t f2, const float_t f) {
    if (p1 == p2)
        return p1;

    float_t t = (f - f1) / (f2 - f1);
    return (1.0f - t) * p1 + t * p2;
}

inline vec2 interpolate_linear_value(const vec2 p1, const vec2 p2,
    const vec2 f1, const vec2 f2, const vec2 f) {
    if (p1 == p2)
        return p1;

    __m128 _p1 = vec2::load_xmm(p1);
    __m128 _p2 = vec2::load_xmm(p2);
    __m128 _f1 = vec2::load_xmm(f1);
    __m128 _f2 = vec2::load_xmm(f2);
    __m128 _f = vec2::load_xmm(f);

    const __m128 _1 = vec4::load_xmm(1.0f);

    __m128 t = _mm_div_ps(_mm_sub_ps(_f, _f1), _mm_sub_ps(_f2, _f1));
    return vec2::store_xmm(_mm_add_ps(_mm_mul_ps(_p1, _mm_sub_ps(_1, t)), _mm_mul_ps(_p2, t)));
}

inline vec3 interpolate_linear_value(const vec3 p1, const vec3 p2,
    const vec3 f1, const vec3 f2, const vec3 f) {
    if (p1 == p2)
        return p1;

    __m128 _p1 = vec3::load_xmm(p1);
    __m128 _p2 = vec3::load_xmm(p2);
    __m128 _f1 = vec3::load_xmm(f1);
    __m128 _f2 = vec3::load_xmm(f2);
    __m128 _f = vec3::load_xmm(f);

    const __m128 _1 = vec4::load_xmm(1.0f);

    __m128 t = _mm_div_ps(_mm_sub_ps(_f, _f1), _mm_sub_ps(_f2, _f1));
    return vec3::store_xmm(_mm_add_ps(_mm_mul_ps(_p1, _mm_sub_ps(_1, t)), _mm_mul_ps(_p2, t)));
}

inline vec4 interpolate_linear_value(const vec4 p1, const vec4 p2,
    const vec4 f1, const vec4 f2, const vec4 f) {
    if (p1 == p2)
        return p1;

    __m128 _p1 = vec4::load_xmm(p1);
    __m128 _p2 = vec4::load_xmm(p2);
    __m128 _f1 = vec4::load_xmm(f1);
    __m128 _f2 = vec4::load_xmm(f2);
    __m128 _f = vec4::load_xmm(f);

    const __m128 _1 = vec4::load_xmm(1.0f);

    __m128 t = _mm_div_ps(_mm_sub_ps(_f, _f1), _mm_sub_ps(_f2, _f1));
    return vec4::store_xmm(_mm_add_ps(_mm_mul_ps(_p1, _mm_sub_ps(_1, t)), _mm_mul_ps(_p2, t)));
}

inline std::vector<float_t> interpolate_linear(float_t p1, float_t p2, size_t f1, size_t f2) {
    size_t length = f2 - f1 + 1;
    if (p1 == p2)
        return std::vector<float_t>(length, p1);

    std::vector<float_t> arr(length);
    float_t* a = arr.data();
    for (size_t i = 0, j = length; j; i++, j--, a++)
        *a = interpolate_linear_value(p1, p2,
            (float_t)f1, (float_t)f2, (float_t)(f1 + i));
    return arr;
}

inline float_t interpolate_chs_value(const float_t p1, const float_t p2,
    const float_t t1, const float_t t2, const float_t f1, const float_t f2, const float_t f) {
    if (p1 == p2 && fabsf(t1) == 0.0f && fabsf(t2) == 0.0f)
        return p1;

    float_t df = f2 - f1;
    float_t t = (f - f1) / df;
    float_t t_2 = t * t;
    float_t t_3 = t_2 * t;
    float_t t_23 = 3.0f * t_2;
    float_t t_32 = 2.0f * t_3;

    float_t h00 = t_32 - t_23 + 1.0f;
    float_t h01 = t_23 - t_32;
    float_t h10 = t_3 - 2.0f * t_2 + t;
    float_t h11 = t_3 - t_2;

    return h00 * p1 + h01 * p2 + h10 * (t1 * df) + h11 * (t2 * df);
}

inline vec2 interpolate_chs_value(const vec2 p1, const vec2 p2,
    const vec2 t1, const vec2 t2, const vec2 f1, const vec2 f2, const vec2 f) {
    if (p1 == p2 && vec2::abs(t1) == 0.0f && vec2::abs(t2) == 0.0f)
        return p1;

    __m128 _p1 = vec2::load_xmm(p1);
    __m128 _p2 = vec2::load_xmm(p2);
    __m128 _t1 = vec2::load_xmm(t1);
    __m128 _t2 = vec2::load_xmm(t2);
    __m128 _f1 = vec2::load_xmm(f1);
    __m128 _f2 = vec2::load_xmm(f2);
    __m128 _f = vec2::load_xmm(f);

    const __m128 _1 = vec4::load_xmm(1.0f);
    const __m128 _2 = vec4::load_xmm(2.0f);
    const __m128 _3 = vec4::load_xmm(3.0f);

    __m128 df = _mm_sub_ps(_f2, _f1);
    __m128 t = _mm_div_ps(_mm_sub_ps(_f, _f1), df);
    __m128 t_2 = _mm_mul_ps(t, t);
    __m128 t_3 = _mm_mul_ps(t_2, t);
    __m128 t_23 = _mm_mul_ps(_3, t_2);
    __m128 t_32 = _mm_mul_ps(_2, t_3);

    __m128 h00 = _mm_add_ps(_mm_sub_ps(t_32, t_23), _1);
    __m128 h01 = _mm_sub_ps(t_23, t_32);
    __m128 h10 = _mm_add_ps(_mm_sub_ps(t_3, _mm_mul_ps(_2, t_2)), t);
    __m128 h11 = _mm_sub_ps(t_3, t_2);

    _p1 = _mm_mul_ps(h00, _p1);
    _p2 = _mm_mul_ps(h01, _p2);
    _t1 = _mm_mul_ps(h10, _mm_mul_ps(_t1, df));
    _t2 = _mm_mul_ps(h11, _mm_mul_ps(_t2, df));
    return vec2::store_xmm(_mm_add_ps(_mm_add_ps(_p1, _p2), _mm_add_ps(_t1, _t2)));
}

inline vec3 interpolate_chs_value(const vec3 p1, const vec3 p2,
    const vec3 t1, const vec3 t2, const vec3 f1, const vec3 f2, const vec3 f) {
    if (p1 == p2 && vec3::abs(t1) == 0.0f && vec3::abs(t2) == 0.0f)
        return p1;

    __m128 _p1 = vec3::load_xmm(p1);
    __m128 _p2 = vec3::load_xmm(p2);
    __m128 _t1 = vec3::load_xmm(t1);
    __m128 _t2 = vec3::load_xmm(t2);
    __m128 _f1 = vec3::load_xmm(f1);
    __m128 _f2 = vec3::load_xmm(f2);
    __m128 _f = vec3::load_xmm(f);

    const __m128 _1 = vec4::load_xmm(1.0f);
    const __m128 _2 = vec4::load_xmm(2.0f);
    const __m128 _3 = vec4::load_xmm(3.0f);

    __m128 df = _mm_sub_ps(_f2, _f1);
    __m128 t = _mm_div_ps(_mm_sub_ps(_f, _f1), df);
    __m128 t_2 = _mm_mul_ps(t, t);
    __m128 t_3 = _mm_mul_ps(t_2, t);
    __m128 t_23 = _mm_mul_ps(_3, t_2);
    __m128 t_32 = _mm_mul_ps(_2, t_3);

    __m128 h00 = _mm_add_ps(_mm_sub_ps(t_32, t_23), _1);
    __m128 h01 = _mm_sub_ps(t_23, t_32);
    __m128 h10 = _mm_add_ps(_mm_sub_ps(t_3, _mm_mul_ps(_2, t_2)), t);
    __m128 h11 = _mm_sub_ps(t_3, t_2);

    _p1 = _mm_mul_ps(h00, _p1);
    _p2 = _mm_mul_ps(h01, _p2);
    _t1 = _mm_mul_ps(h10, _mm_mul_ps(_t1, df));
    _t2 = _mm_mul_ps(h11, _mm_mul_ps(_t2, df));
    return vec3::store_xmm(_mm_add_ps(_mm_add_ps(_p1, _p2), _mm_add_ps(_t1, _t2)));
}

inline vec4 interpolate_chs_value(const vec4 p1, const vec4 p2,
    const vec4 t1, const vec4 t2, const vec4 f1, const vec4 f2, const vec4 f) {
    if (p1 == p2 && vec4::abs(t1) == 0.0f && vec4::abs(t2) == 0.0f)
        return p1;

    __m128 _p1 = vec4::load_xmm(p1);
    __m128 _p2 = vec4::load_xmm(p2);
    __m128 _t1 = vec4::load_xmm(t1);
    __m128 _t2 = vec4::load_xmm(t2);
    __m128 _f1 = vec4::load_xmm(f1);
    __m128 _f2 = vec4::load_xmm(f2);
    __m128 _f = vec4::load_xmm(f);

    const __m128 _1 = vec4::load_xmm(1.0f);
    const __m128 _2 = vec4::load_xmm(2.0f);
    const __m128 _3 = vec4::load_xmm(3.0f);

    __m128 df = _mm_sub_ps(_f2, _f1);
    __m128 t = _mm_div_ps(_mm_sub_ps(_f, _f1), df);
    __m128 t_2 = _mm_mul_ps(t, t);
    __m128 t_3 = _mm_mul_ps(t_2, t);
    __m128 t_23 = _mm_mul_ps(_3, t_2);
    __m128 t_32 = _mm_mul_ps(_2, t_3);

    __m128 h00 = _mm_add_ps(_mm_sub_ps(t_32, t_23), _1);
    __m128 h01 = _mm_sub_ps(t_23, t_32);
    __m128 h10 = _mm_add_ps(_mm_sub_ps(t_3, _mm_mul_ps(_2, t_2)), t);
    __m128 h11 = _mm_sub_ps(t_3, t_2);

    _p1 = _mm_mul_ps(h00, _p1);
    _p2 = _mm_mul_ps(h01, _p2);
    _t1 = _mm_mul_ps(h10, _mm_mul_ps(_t1, df));
    _t2 = _mm_mul_ps(h11, _mm_mul_ps(_t2, df));
    return vec4::store_xmm(_mm_add_ps(_mm_add_ps(_p1, _p2), _mm_add_ps(_t1, _t2)));
}

inline std::vector<float_t> interpolate_chs(const float_t p1, const float_t p2,
    const float_t t1, const float_t t2, const size_t f1, const size_t f2) {
    size_t length = f2 - f1 + 1;
    if (p1 == p2 && fabsf(t1) == 0.0f && fabsf(t2) == 0.0f)
        return std::vector<float_t>(length, p1);

    std::vector<float_t> arr(length);
    float_t* a = arr.data();
    for (size_t i = 0, j = length; j; i++, j--, a++)
        *a = interpolate_chs_value(p1, p2, t1, t2,
            (float_t)f1, (float_t)f2, (float_t)(f1 + i));
    return arr;
}

extern void interpolate_chs_reverse_value(float_t* arr, size_t length,
    float_t& t1, float_t& t2, size_t f1, size_t f2, size_t f);
extern void interpolate_chs_reverse(float_t* arr, size_t length,
    float_t& t1, float_t& t2, size_t f1, size_t f2);
extern int32_t interpolate_chs_reverse_sequence(
    std::vector<float_t>& values_src, std::vector<kft3>& values, bool fast = false);
