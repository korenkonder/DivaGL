/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"
#include "vec.hpp"

struct quat {
    float_t x;
    float_t y;
    float_t z;
    float_t w;

    quat();
    quat(float_t value);
    quat(float_t x, float_t y, float_t z, float_t w);
    quat(const vec3& axis, const float_t angle);
    quat(float_t m00, float_t m01, float_t m02, float_t m10,
        float_t m11, float_t m12, float_t m20, float_t m21, float_t m22);

    static __m128 load_xmm(const float_t data);
    static __m128 load_xmm(const quat& data);
    static __m128 load_xmm(const quat&& data);
    static quat store_xmm(const __m128& data);
    static quat store_xmm(const __m128&& data);

    static quat mul(const quat& in_q1, const quat& in_q2);

    static float_t dot(const quat& left, const quat& right);
    static float_t length(const quat& left);
    static float_t length_squared(const quat& left);
    static float_t distance(const quat& left, const quat& right);
    static float_t distance_squared(const quat& left, const quat& right);
    static quat abs(const quat& left);
    static quat lerp(const quat& left, const quat& right, const float_t blend);
    static quat slerp(const quat& left, const quat& right, const float_t blend);
    static quat normalize(const quat& left);
    static quat rcp(const quat& left);
    static quat min(const quat& min, const quat& max);
    static quat max(const quat& min, const quat& max);
    static quat clamp(const quat& left, const quat& min, const quat& max);
    static quat clamp(const quat& left, const float_t min, const float_t max);
    static quat mult_min_max(const quat& left, const quat& min, const quat& max);
    static quat mult_min_max(const quat& left, const float_t min, const float_t max);
    static quat div_min_max(const quat& left, const quat& min, const quat& max);
    static quat div_min_max(const quat& left, const float_t min, const float_t max);
};

extern const quat quat_identity;

inline quat::quat() : x(), y(), z(), w() {

}

inline quat::quat(float_t value) : x(value), y(value), z(value), w(value) {

}

inline quat::quat(float_t x, float_t y, float_t z, float_t w) : x(x), y(y), z(z), w(w) {

}

inline quat::quat(const vec3& axis, const float_t angle) {
    vec3 _axis = vec3::normalize(axis) * sinf(angle * 0.5f);
    x = _axis.x;
    y = _axis.y;
    z = _axis.z;
    w = cosf(angle * 0.5f);
}

inline quat::quat(float_t m00, float_t m01, float_t m02, float_t m10,
    float_t m11, float_t m12, float_t m20, float_t m21, float_t m22) {
    if (m00 + m11 + m22 >= 0.0f) {
        float_t sq = sqrtf(m00 + m11 + m22 + 1.0f);
        w = sq * 0.5f;
        sq = 0.5f / sq;
        x = (m21 - m12) * sq;
        y = (m02 - m20) * sq;
        z = (m10 - m01) * sq;
        return;
    }

    float_t max = max_def(m22, max_def(m11, m00));
    if (max == m00) {
        float_t sq = sqrtf(m00 - (m11 + m22) + 1.0f);
        x = sq * 0.5f;
        sq = 0.5f / sq;
        y = (m01 + m10) * sq;
        z = (m02 + m20) * sq;
        w = (m21 - m12) * sq;
    }
    else if (max == m11) {
        float_t sq = sqrtf(m11 - (m00 + m22) + 1.0f);
        y = sq * 0.5f;
        sq = 0.5f / sq;
        x = (m01 + m10) * sq;
        z = (m12 + m21) * sq;
        w = (m02 - m20) * sq;
    }
    else {
        float_t sq = sqrtf(m22 - (m00 + m11) + 1.0f);
        z = sq * 0.5f;
        sq = 0.5f / sq;
        x = (m02 + m20) * sq;
        y = (m12 + m21) * sq;
        w = (m10 - m01) * sq;
    }
}

inline quat operator +(const quat& left, const quat& right) {
    __m128 yt;
    quat z;
    *(quat*)&yt = right;
    _mm_storeu_ps((float*)&z, _mm_add_ps(_mm_loadu_ps((const float*)&left), yt));
    return z;
}

inline quat operator +(const quat& left, const float_t right) {
    __m128 yt;
    quat z;
    yt = _mm_set_ss(right);
    _mm_storeu_ps((float*)&z, _mm_add_ps(_mm_loadu_ps((const float*)&left), _mm_shuffle_ps(yt, yt, 0)));
    return z;
}

inline quat operator -(const quat& left, const quat& right) {
    __m128 yt;
    quat z;
    *(quat*)&yt = right;
    _mm_storeu_ps((float*)&z, _mm_sub_ps(_mm_loadu_ps((const float*)&left), yt));
    return z;
}

inline quat operator -(const quat& left, const float_t right) {
    __m128 yt;
    quat z;
    yt = _mm_set_ss(right);
    _mm_storeu_ps((float*)&z, _mm_sub_ps(_mm_loadu_ps((const float*)&left), _mm_shuffle_ps(yt, yt, 0)));
    return z;
}

inline quat operator *(const quat& left, const quat& right) {
    __m128 yt;
    quat z;
    *(quat*)&yt = right;
    _mm_storeu_ps((float*)&z, _mm_mul_ps(_mm_loadu_ps((const float*)&left), yt));
    return z;
}

inline quat operator *(const quat& left, const float_t right) {
    __m128 yt;
    quat z;
    yt = _mm_set_ss(right);
    _mm_storeu_ps((float*)&z, _mm_mul_ps(_mm_loadu_ps((const float*)&left), _mm_shuffle_ps(yt, yt, 0)));
    return z;
}

inline quat operator /(const quat& left, const quat& right) {
    __m128 yt;
    quat z;
    *(quat*)&yt = right;
    _mm_storeu_ps((float*)&z, _mm_div_ps(_mm_loadu_ps((const float*)&left), yt));
    return z;
}

inline quat operator /(const quat& left, const float_t right) {
    __m128 yt;
    quat z;
    yt = _mm_set_ss(right);
    _mm_storeu_ps((float*)&z, _mm_div_ps(_mm_loadu_ps((const float*)&left), _mm_shuffle_ps(yt, yt, 0)));
    return z;
}

inline quat operator &(const quat& left, const quat& right) {
    __m128 yt;
    quat z;
    *(quat*)&yt = right;
    _mm_storeu_ps((float*)&z, _mm_and_ps(_mm_loadu_ps((const float*)&left), yt));
    return z;
}

inline quat operator &(const quat& left, const float_t right) {
    __m128 yt;
    quat z;
    yt = _mm_set_ss(right);
    _mm_storeu_ps((float*)&z, _mm_and_ps(_mm_loadu_ps((const float*)&left), _mm_shuffle_ps(yt, yt, 0)));
    return z;
}

inline quat operator ^(const quat& left, const quat& right) {
    __m128 yt;
    quat z;
    *(quat*)&yt = right;
    _mm_storeu_ps((float*)&z, _mm_xor_ps(_mm_loadu_ps((const float*)&left), yt));
    return z;
}

inline quat operator ^(const quat& left, const float_t right) {
    __m128 yt;
    quat z;
    yt = _mm_set_ss(right);
    _mm_storeu_ps((float*)&z, _mm_xor_ps(_mm_loadu_ps((const float*)&left), _mm_shuffle_ps(yt, yt, 0)));
    return z;
}

inline quat operator -(const quat& left) {
    quat z;
    _mm_storeu_ps((float*)&z, _mm_xor_ps(_mm_loadu_ps((const float*)&left), vec4_neg));
    return z;
}

inline __m128 quat::load_xmm(const float_t data) {
    __m128 _data = _mm_set_ss(data);
    return _mm_shuffle_ps(_data, _data, 0);
}

inline __m128 quat::load_xmm(const quat& data) {
    return _mm_loadu_ps((const float*)&data);
}

inline __m128 quat::load_xmm(const quat&& data) {
    return _mm_loadu_ps((const float*)&data);
}

inline quat quat::store_xmm(const __m128& data) {
    quat _data;
    _mm_storeu_ps((float*)&_data, data);
    return _data;
}

inline quat quat::store_xmm(const __m128&& data) {
    quat _data;
    _mm_storeu_ps((float*)&_data, data);
    return _data;
}

inline quat quat::mul(const quat& in_q1, const quat& in_q2) {
    __m128 xt;
    __m128 yt;
    __m128 zt0;
    __m128 zt1;
    __m128 zt2;
    __m128 zt3;

    xt = quat::load_xmm(in_q1);
    yt = quat::load_xmm(in_q2);
    zt0 = _mm_mul_ps(xt, _mm_shuffle_ps(yt, yt, 0x1B));
    zt1 = _mm_mul_ps(xt, _mm_shuffle_ps(yt, yt, 0x4E));
    zt2 = _mm_mul_ps(xt, _mm_shuffle_ps(yt, yt, 0xB1));
    zt3 = _mm_mul_ps(xt, _mm_shuffle_ps(yt, yt, 0xE4));
    zt0 = _mm_xor_ps(zt0, __m128({ 0.0f,  0.0f, -0.0f, 0.0f }));
    zt1 = _mm_xor_ps(zt1, __m128({ -0.0f,  0.0f,  0.0f, 0.0f }));
    zt2 = _mm_xor_ps(zt2, __m128({ 0.0f, -0.0f,  0.0f, 0.0f }));
    zt3 = _mm_xor_ps(zt3, __m128({ -0.0f, -0.0f, -0.0f, 0.0f }));
    zt0 = _mm_hadd_ps(zt0, zt0);
    zt1 = _mm_hadd_ps(zt1, zt1);
    zt2 = _mm_hadd_ps(zt2, zt2);
    zt3 = _mm_hadd_ps(zt3, zt3);

    quat out_q;
    out_q.x = _mm_cvtss_f32(_mm_hadd_ps(zt0, zt0));
    out_q.y = _mm_cvtss_f32(_mm_hadd_ps(zt1, zt1));
    out_q.z = _mm_cvtss_f32(_mm_hadd_ps(zt2, zt2));
    out_q.w = _mm_cvtss_f32(_mm_hadd_ps(zt3, zt3));
    return out_q;
}

inline float_t quat::dot(const quat& left, const quat& right) {
    __m128 zt;
    zt = _mm_mul_ps(_mm_loadu_ps((const float*)&(left)), _mm_loadu_ps((const float*)&(right)));
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline float_t quat::length(const quat& left) {
    __m128 xt;
    __m128 zt;
    xt = _mm_loadu_ps((const float*)&left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(zt, zt)));
}

inline float_t quat::length_squared(const quat& left) {
    __m128 xt;
    __m128 zt;
    xt = _mm_loadu_ps((const float*)&left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline float_t quat::distance(const quat& left, const quat& right) {
    __m128 zt;
    zt = _mm_sub_ps(_mm_loadu_ps((const float*)&(left)), _mm_loadu_ps((const float*)&(right)));
    zt = _mm_mul_ps(zt, zt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(zt, zt)));
}

inline float_t quat::distance_squared(const quat& left, const quat& right) {
    __m128 zt;
    zt = _mm_sub_ps(_mm_loadu_ps((const float*)&(left)), _mm_loadu_ps((const float*)&(right)));
    zt = _mm_mul_ps(zt, zt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline quat quat::abs(const quat& left) {
    return quat::store_xmm(_mm_castsi128_ps(_mm_and_si128(_mm_castps_si128(quat::load_xmm(left)), vec4i_abs)));
}

inline quat quat::lerp(const quat& left, const quat& right, const float_t blend) {
    quat x_t;
    quat y_t;
    x_t = left;
    y_t = right;

    if (quat::dot(x_t, y_t) < 0.0f)
        x_t = -x_t;

    return quat::normalize(x_t * (1.0f - blend) + y_t * blend);
}

inline quat quat::slerp(const quat& left, const quat& right, const float_t blend) {
    quat x_t;
    quat y_t;
    x_t = left;
    y_t = right;

    float_t dot = quat::dot(x_t, y_t);
    if (dot < 0.0f) {
        dot = -dot;
        x_t = -x_t;
    }

    dot = min_def(dot, 1.0f);

    float_t theta = acosf(dot);
    if (theta == 0.0f)
        return x_t;

    float_t st = 1.0f / sinf(theta);
    float_t s0 = sinf((1.0f - blend) * theta) * st;
    float_t s1 = sinf(theta * blend) * st;
    return quat::normalize(x_t * s0 + y_t * s1);
}

inline quat quat::normalize(const quat& left) {
    __m128 xt;
    __m128 zt;
    quat z;
    xt = _mm_loadu_ps((const float*)&left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    zt = _mm_sqrt_ss(_mm_hadd_ps(zt, zt));
    if (zt.m128_f32[0] != 0.0f)
        zt.m128_f32[0] = 1.0f / zt.m128_f32[0];
    _mm_storeu_ps((float*)&z, _mm_mul_ps(xt, _mm_shuffle_ps(zt, zt, 0)));
    return z;
}

inline quat quat::rcp(const quat& left) {
    quat z;
    _mm_storeu_ps((float*)&z, _mm_div_ps(_mm_loadu_ps((const float*)&(quat_identity)), _mm_loadu_ps((const float*)&left)));
    return z;
}

inline quat quat::min(const quat& left, const quat& right) {
    quat z;
    _mm_storeu_ps((float*)&z, _mm_min_ps(_mm_loadu_ps((const float*)&(left)), _mm_loadu_ps((const float*)&(right))));
    return z;
}

inline quat quat::max(const quat& left, const quat& right) {
    quat z;
    _mm_storeu_ps((float*)&z, _mm_max_ps(_mm_loadu_ps((const float*)&(left)), _mm_loadu_ps((const float*)&(right))));
    return z;
}

inline quat quat::clamp(const quat& left, const quat& min, const quat& max) {
    quat w;
    _mm_storeu_ps((float*)&w, _mm_min_ps(_mm_max_ps(_mm_loadu_ps((const float*)&left),
        _mm_loadu_ps((const float*)&(min))), _mm_loadu_ps((const float*)&(max))));
    return w;
}

inline quat quat::clamp(const quat& left, const float_t min, const float_t max) {
    __m128 yt;
    __m128 zt;
    quat w;
    yt = _mm_set_ss(min);
    zt = _mm_set_ss(max);
    _mm_storeu_ps((float*)&w, _mm_min_ps(_mm_max_ps(_mm_loadu_ps((const float*)&left),
        _mm_shuffle_ps(yt, yt, 0)), _mm_shuffle_ps(zt, zt, 0)));
    return w;
}

inline quat quat::mult_min_max(const quat& left, const quat& min, const quat& max) {
    __m128 xt;
    __m128 yt;
    __m128 wt;
    quat w;
    xt = _mm_loadu_ps((const float*)&left);
    yt = _mm_xor_ps(_mm_loadu_ps((const float*)&(min)), vec4_neg);
    wt = _mm_or_ps(_mm_and_ps(yt, _mm_cmplt_ps(xt, vec4::load_xmm(0.0f))),
        _mm_and_ps(_mm_loadu_ps((const float*)&(max)), _mm_cmpge_ps(xt, vec4::load_xmm(0.0f))));
    _mm_storeu_ps((float*)&w, _mm_mul_ps(xt, wt));
    return w;
}

inline quat quat::mult_min_max(const quat& left, const float_t min, const float_t max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    __m128 wt;
    quat w;
    xt = _mm_loadu_ps((const float*)&left);
    yt = _mm_set_ss(min);
    yt = _mm_shuffle_ps(yt, yt, 0);
    zt = _mm_set_ss(max);
    zt = _mm_shuffle_ps(zt, zt, 0);
    yt = _mm_xor_ps(yt, vec4_neg);
    wt = _mm_or_ps(_mm_and_ps(yt, _mm_cmplt_ps(xt, vec4::load_xmm(0.0f))),
        _mm_and_ps(zt, _mm_cmpge_ps(xt, vec4::load_xmm(0.0f))));
    _mm_storeu_ps((float*)&w, _mm_mul_ps(xt, wt));
    return w;
}

inline quat quat::div_min_max(const quat& left, const quat& min, const quat& max) {
    __m128 xt;
    __m128 yt;
    __m128 wt;
    quat w;
    xt = _mm_loadu_ps((const float*)&left);
    yt = _mm_xor_ps(_mm_loadu_ps((const float*)&(min)), vec4_neg);
    wt = _mm_or_ps(_mm_and_ps(yt, _mm_cmplt_ps(xt, vec4::load_xmm(0.0f))),
        _mm_and_ps(_mm_loadu_ps((const float*)&(max)), _mm_cmpge_ps(xt, vec4::load_xmm(0.0f))));
    _mm_storeu_ps((float*)&w, _mm_div_ps(xt, wt));
    return w;
}

inline quat quat::div_min_max(const quat& left, const float_t min, const float_t max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    __m128 wt;
    quat w;
    xt = _mm_loadu_ps((const float*)&left);
    yt = _mm_set_ss(min);
    yt = _mm_shuffle_ps(yt, yt, 0);
    zt = _mm_set_ss(max);
    zt = _mm_shuffle_ps(zt, zt, 0);
    yt = _mm_xor_ps(yt, vec4_neg);
    wt = _mm_or_ps(_mm_and_ps(yt, _mm_cmplt_ps(xt, vec4::load_xmm(0.0f))),
        _mm_and_ps(zt, _mm_cmpge_ps(xt, vec4::load_xmm(0.0f))));
    _mm_storeu_ps((float*)&w, _mm_div_ps(xt, wt));
    return w;
}
