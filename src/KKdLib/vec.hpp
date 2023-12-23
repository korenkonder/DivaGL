/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"
#include "half_t.hpp"
#include <pmmintrin.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <emmintrin.h>

struct vec2i8 {
    int8_t x;
    int8_t y;

    inline vec2i8() : x(), y() {

    }

    inline vec2i8(int8_t value) : x(value), y(value) {

    }

    inline vec2i8(int8_t x, int8_t y) : x(x), y(y) {

    }
};

struct vec3i8 {
    int8_t x;
    int8_t y;
    int8_t z;

    inline vec3i8() : x(), y(), z() {

    }

    inline vec3i8(int8_t value) : x(value), y(value), z(value) {

    }

    inline vec3i8(int8_t x, int8_t y, int8_t z) : x(x), y(y), z(z) {

    }
};

struct vec4i8 {
    int8_t x;
    int8_t y;
    int8_t z;
    int8_t w;

    inline vec4i8() : x(), y(), z(), w() {

    }

    inline vec4i8(int8_t value) : x(value), y(value), z(value), w(value) {

    }

    inline vec4i8(int8_t x, int8_t y, int8_t z, int8_t w) : x(x), y(y), z(z), w(w) {

    }
};

struct vec2u8 {
    uint8_t x;
    uint8_t y;

    inline vec2u8() : x(), y() {

    }

    inline vec2u8(uint8_t value) : x(value), y(value) {

    }

    inline vec2u8(uint8_t x, uint8_t y) : x(x), y(y) {

    }
};

struct vec3u8 {
    uint8_t x;
    uint8_t y;
    uint8_t z;

    inline vec3u8() : x(), y(), z() {

    }

    inline vec3u8(uint8_t value) : x(value), y(value), z(value) {

    }

    inline vec3u8(uint8_t x, uint8_t y, uint8_t z) : x(x), y(y), z(z) {

    }
};

struct vec4u8 {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t w;

    inline vec4u8() : x(), y(), z(), w() {

    }

    inline vec4u8(uint8_t value) : x(value), y(value), z(value), w(value) {

    }

    inline vec4u8(uint8_t x, uint8_t y, uint8_t z, uint8_t w) : x(x), y(y), z(z), w(w) {

    }
};

struct vec2i16 {
    int16_t x;
    int16_t y;

    inline vec2i16() : x(), y() {

    }

    inline vec2i16(int16_t value) : x(value), y(value) {

    }

    inline vec2i16(int16_t x, int16_t y) : x(x), y(y) {

    }
};

struct vec3i16 {
    int16_t x;
    int16_t y;
    int16_t z;

    inline vec3i16() : x(), y(), z() {

    }

    inline vec3i16(int16_t value) : x(value), y(value), z(value) {

    }

    inline vec3i16(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {

    }
};

struct vec4i16 {
    int16_t x;
    int16_t y;
    int16_t z;
    int16_t w;

    inline vec4i16() : x(), y(), z(), w() {

    }

    inline vec4i16(int16_t value) : x(value), y(value), z(value), w(value) {

    }

    inline vec4i16(int16_t x, int16_t y, int16_t z, int16_t w) : x(x), y(y), z(z), w(w) {

    }
};

struct vec2u16 {
    uint16_t x;
    uint16_t y;

    inline vec2u16() : x(), y() {

    }

    inline vec2u16(uint16_t value) : x(value), y(value) {

    }

    inline vec2u16(uint16_t x, uint16_t y) : x(x), y(y) {

    }
};

struct vec3u16 {
    uint16_t x;
    uint16_t y;
    uint16_t z;

    inline vec3u16() : x(), y(), z() {

    }

    inline vec3u16(uint16_t value) : x(value), y(value), z(value) {

    }

    inline vec3u16(uint16_t x, uint16_t y, uint16_t z) : x(x), y(y), z(z) {

    }
};

struct vec4u16 {
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint16_t w;

    inline vec4u16() : x(), y(), z(), w() {

    }

    inline vec4u16(uint16_t value) : x(value), y(value), z(value), w(value) {

    }

    inline vec4u16(uint16_t x, uint16_t y, uint16_t z, uint16_t w) : x(x), y(y), z(z), w(w) {

    }
};

struct vec2h {
    half_t x;
    half_t y;

    inline vec2h() : x(), y() {

    }

    inline vec2h(half_t value) : x(value), y(value) {

    }

    inline vec2h(half_t x, half_t y) : x(x), y(y) {

    }
};

struct vec3h {
    half_t x;
    half_t y;
    half_t z;

    inline vec3h() : x(), y(), z() {

    }

    inline vec3h(half_t value) : x(value), y(value), z(value) {

    }

    inline vec3h(half_t x, half_t y, half_t z) : x(x), y(y), z(z) {

    }
};

struct vec4h {
    half_t x;
    half_t y;
    half_t z;
    half_t w;

    inline vec4h() : x(), y(), z(), w() {

    }

    inline vec4h(half_t value) : x(value), y(value), z(value), w(value) {

    }

    inline vec4h(half_t x, half_t y, half_t z, half_t w) : x(x), y(y), z(z), w(w) {

    }
};

struct vec2 {
    float_t x;
    float_t y;

    vec2();
    vec2(float_t value);
    vec2(float_t x, float_t y);

    static __m128 load_xmm(const float_t data);
    static __m128 load_xmm(const vec2& data);
    static __m128 load_xmm(const vec2&& data);
    static vec2 store_xmm(const __m128& data);
    static vec2 store_xmm(const __m128&& data);

    static float_t angle(const vec2& left, const vec2& right);
    static float_t dot(const vec2& left, const vec2& right);
    static float_t length(const vec2& left);
    static float_t length_squared(const vec2& left);
    static float_t distance(const vec2& left, const vec2& right);
    static float_t distance_squared(const vec2& left, const vec2& right);
    static vec2 abs(const vec2& left);
    static vec2 lerp(const vec2& left, const vec2& right, const vec2& blend);
    static vec2 lerp(const vec2& left, const vec2& right, const float_t blend);
    static vec2 normalize(const vec2& left);
    static vec2 normalize_rcp(const vec2& left);
    static vec2 rcp(const vec2& left);
    static vec2 min(const vec2& left, const vec2& right);
    static vec2 max(const vec2& left, const vec2& right);
    static vec2 clamp(const vec2& left, const vec2& min, const vec2& max);
    static vec2 clamp(const vec2& left, const float_t min, const float_t max);
    static vec2 mult_min_max(const vec2& left, const vec2& min, const vec2& max);
    static vec2 mult_min_max(const vec2& left, const float_t min, const float_t max);
    static vec2 div_min_max(const vec2& left, const vec2& min, const vec2& max);
    static vec2 div_min_max(const vec2& left, const float_t min, const float_t max);
};

struct vec3 {
    float_t x;
    float_t y;
    float_t z;

    vec3();
    vec3(float_t value);
    vec3(float_t x, float_t y, float_t z);

    static __m128 load_xmm(const float_t data);
    static __m128 load_xmm(const vec3& data);
    static __m128 load_xmm(const vec3&& data);
    static vec3 store_xmm(const __m128& data);
    static vec3 store_xmm(const __m128&& data);

    static float_t angle(const vec3& left, const vec3& right);
    static float_t dot(const vec3& left, const vec3& right);
    static float_t length(const vec3& left);
    static float_t length_squared(const vec3& left);
    static float_t distance(const vec3& left, const vec3& right);
    static float_t distance_squared(const vec3& left, const vec3& right);
    static vec3 abs(const vec3& left);
    static vec3 lerp(const vec3& left, const vec3& right, const vec3& blend);
    static vec3 lerp(const vec3& left, const vec3& right, const float_t blend);
    static vec3 normalize(const vec3& left);
    static vec3 normalize_rcp(const vec3& left);
    static vec3 rcp(const vec3& left);
    static vec3 min(const vec3& left, const vec3& right);
    static vec3 max(const vec3& left, const vec3& right);
    static vec3 clamp(const vec3& left, const vec3& min, const vec3& max);
    static vec3 clamp(const vec3& left, const float_t min, const float_t max);
    static vec3 mult_min_max(const vec3& left, const vec3& min, const vec3& max);
    static vec3 mult_min_max(const vec3& left, const float_t min, const float_t max);
    static vec3 div_min_max(const vec3& left, const vec3& min, const vec3& max);
    static vec3 div_min_max(const vec3& left, const float_t min, const float_t max);
    static vec3 cross(const vec3& left, const vec3& right);
};

struct vec4 {
    float_t x;
    float_t y;
    float_t z;
    float_t w;

    vec4();
    vec4(float_t value);
    vec4(float_t x, float_t y, float_t z, float_t w);

    static __m128 load_xmm(const float_t data);
    static __m128 load_xmm(const vec4& data);
    static __m128 load_xmm(const vec4&& data);
    static vec4 store_xmm(const __m128& data);
    static vec4 store_xmm(const __m128&& data);

    static float_t angle(const vec4& left, const vec4& right);
    static float_t dot(const vec4& left, const vec4& right);
    static float_t length(const vec4& left);
    static float_t length_squared(const vec4& left);
    static float_t distance(const vec4& left, const vec4& right);
    static float_t distance_squared(const vec4& left, const vec4& right);
    static vec4 abs(const vec4& left);
    static vec4 lerp(const vec4& left, const vec4& right, const vec4& blend);
    static vec4 lerp(const vec4& left, const vec4& right, const float_t blend);
    static vec4 normalize(const vec4& left);
    static vec4 normalize_rcp(const vec4& left);
    static vec4 rcp(const vec4& left);
    static vec4 min(const vec4& min, const vec4& max);
    static vec4 max(const vec4& min, const vec4& max);
    static vec4 clamp(const vec4& left, const vec4& min, const vec4& max);
    static vec4 clamp(const vec4& left, const float_t min, const float_t max);
    static vec4 mult_min_max(const vec4& left, const vec4& min, const vec4& max);
    static vec4 mult_min_max(const vec4& left, const float_t min, const float_t max);
    static vec4 div_min_max(const vec4& left, const vec4& min, const vec4& max);
    static vec4 div_min_max(const vec4& left, const float_t min, const float_t max);
};

struct vec2i {
    int32_t x;
    int32_t y;

    vec2i();
    vec2i(int32_t value);
    vec2i(int32_t x, int32_t y);

    static __m128i load_xmm(const int32_t data);
    static __m128i load_xmm(const vec2i& data);
    static __m128i load_xmm(const vec2i&& data);
    static vec2i store_xmm(const __m128i& data);
    static vec2i store_xmm(const __m128i&& data);

    static vec2i min(const vec2i& left, const vec2i& right);
    static vec2i max(const vec2i& left, const vec2i& right);
    static vec2i clamp(const vec2i& left, const vec2i& min, const vec2i& max);
    static vec2i clamp(const vec2i& left, const int32_t min, const int32_t max);
};

struct vec3i {
    int32_t x;
    int32_t y;
    int32_t z;

    vec3i();
    vec3i(int32_t value);
    vec3i(int32_t x, int32_t y, int32_t z);

    static __m128i load_xmm(const int32_t data);
    static __m128i load_xmm(const vec3i& data);
    static __m128i load_xmm(const vec3i&& data);
    static vec3i store_xmm(const __m128i& data);
    static vec3i store_xmm(const __m128i&& data);

    static vec3i min(const vec3i& left, const vec3i& right);
    static vec3i max(const vec3i& left, const vec3i& right);
    static vec3i clamp(const vec3i& left, const vec3i& min, const vec3i& max);
    static vec3i clamp(const vec3i& left, const int32_t min, const int32_t max);
};

struct vec4i {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;

    vec4i();
    vec4i(int32_t value);
    vec4i(int32_t x, int32_t y, int32_t z, int32_t w);

    static __m128i load_xmm(const int32_t data);
    static __m128i load_xmm(const vec4i& data);
    static __m128i load_xmm(const vec4i&& data);
    static vec4i store_xmm(const __m128i& data);
    static vec4i store_xmm(const __m128i&& data);

    static vec4i min(const vec4i& left, const vec4i& right);
    static vec4i max(const vec4i& left, const vec4i& right);
    static vec4i clamp(const vec4i& left, const vec4i& min, const vec4i& max);
    static vec4i clamp(const vec4i& left, const int32_t min, const int32_t max);
};

extern const __m128 vec2_neg;
extern const __m128 vec3_neg;
extern const __m128 vec4_neg;

extern const __m128i vec2i_abs;
extern const __m128i vec3i_abs;
extern const __m128i vec4i_abs;

inline vec2::vec2() : x(), y() {

}

inline vec2::vec2(float_t value) : x(value), y(value) {

}

inline vec2::vec2(float_t x, float_t y) : x(x), y(y) {

}

inline __m128 vec2::load_xmm(const float_t data) {
    __m128 _data = _mm_set_ss(data);
    return _mm_shuffle_ps(_data, _data, 0x50);
}

inline __m128 vec2::load_xmm(const vec2& data) {
    return _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*) & data));
}

inline __m128 vec2::load_xmm(const vec2&& data) {
    return _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*) & data));
}

inline vec2 vec2::store_xmm(const __m128& data) {
    vec2 _data;
    _mm_storel_epi64((__m128i*) & _data, _mm_castps_si128(data));
    return _data;
}

inline vec2 vec2::store_xmm(const __m128&& data) {
    vec2 _data;
    _mm_storel_epi64((__m128i*) & _data, _mm_castps_si128(data));
    return _data;
}

inline vec2 operator +(const vec2& left, const vec2& right) {
    return vec2::store_xmm(_mm_add_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator +(const vec2& left, const float_t right) {
    return vec2::store_xmm(_mm_add_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator +(const float_t left, const vec2& right) {
    return vec2::store_xmm(_mm_add_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator +=(vec2& left, const vec2& right) {
    left = vec2::store_xmm(_mm_add_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator +=(vec2& left, const float_t right) {
    left = vec2::store_xmm(_mm_add_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator -(const vec2& left, const vec2& right) {
    return vec2::store_xmm(_mm_sub_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator -(const vec2& left, const float_t right) {
    return vec2::store_xmm(_mm_sub_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator -(const float_t left, const vec2& right) {
    return vec2::store_xmm(_mm_sub_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator -=(vec2& left, const vec2& right) {
    left = vec2::store_xmm(_mm_sub_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator -=(vec2& left, const float_t right) {
    left = vec2::store_xmm(_mm_sub_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator *(const vec2& left, const vec2& right) {
    return vec2::store_xmm(_mm_mul_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator *(const vec2& left, const float_t right) {
    return vec2::store_xmm(_mm_mul_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator *(const float_t left, const vec2& right) {
    return vec2::store_xmm(_mm_mul_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator *=(vec2& left, const vec2& right) {
    left = vec2::store_xmm(_mm_mul_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator *=(vec2& left, const float_t right) {
    left = vec2::store_xmm(_mm_mul_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator /(const vec2& left, const vec2& right) {
    return vec2::store_xmm(_mm_div_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator /(const vec2& left, const float_t right) {
    return vec2::store_xmm(_mm_div_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator /(const float_t left, const vec2& right) {
    return vec2::store_xmm(_mm_div_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator /=(vec2& left, const vec2& right) {
    left = vec2::store_xmm(_mm_div_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator /=(vec2& left, const float_t right) {
    left = vec2::store_xmm(_mm_div_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator &(const vec2& left, const vec2& right) {
    return vec2::store_xmm(_mm_and_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator &(const vec2& left, const float_t right) {
    return vec2::store_xmm(_mm_and_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator &(const float_t left, const vec2& right) {
    return vec2::store_xmm(_mm_and_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator &=(vec2& left, const vec2& right) {
    left = vec2::store_xmm(_mm_and_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator &=(vec2& left, const float_t right) {
    left = vec2::store_xmm(_mm_and_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator ^(const vec2& left, const vec2& right) {
    return vec2::store_xmm(_mm_xor_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator ^(const vec2& left, const float_t right) {
    return vec2::store_xmm(_mm_xor_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator ^(const float_t left, const vec2& right) {
    return vec2::store_xmm(_mm_xor_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator ^=(vec2& left, const vec2& right) {
    left = vec2::store_xmm(_mm_xor_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline void operator ^=(vec2& left, const float_t right) {
    left = vec2::store_xmm(_mm_xor_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 operator -(const vec2& left) {
    return vec2::store_xmm(_mm_xor_ps(vec2::load_xmm(left), vec2_neg));
}

inline bool operator ==(const vec2& left, const vec2& right) {
    return !memcmp(&left, &right, sizeof(vec2));
}

inline bool operator !=(const vec2& left, const vec2& right) {
    return !!memcmp(&left, &right, sizeof(vec2));
}

inline float_t vec2::angle(const vec2& left, const vec2& right) {
    return acosf(vec2::dot(left, right) / (vec2::length(left) * vec2::length(right)));
}

inline float_t vec2::dot(const vec2& left, const vec2& right) {
    __m128 zt;
    zt = _mm_mul_ps(vec2::load_xmm(left), vec2::load_xmm(right));
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline float_t vec2::length(const vec2& left) {
    __m128 xt;
    __m128 zt;
    xt = vec2::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(zt, zt)));
}

inline float_t vec2::length_squared(const vec2& left) {
    __m128 xt;
    __m128 zt;
    xt = vec2::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline float_t vec2::distance(const vec2& left, const vec2& right) {
    __m128 zt;
    zt = _mm_sub_ps(vec2::load_xmm(left), vec2::load_xmm(right));
    zt = _mm_mul_ps(zt, zt);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(zt, zt)));
}

inline float_t vec2::distance_squared(const vec2& left, const vec2& right) {
    __m128 zt;
    zt = _mm_sub_ps(vec2::load_xmm(left), vec2::load_xmm(right));
    zt = _mm_mul_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline vec2 vec2::abs(const vec2& left) {
    return vec2::store_xmm(_mm_castsi128_ps(_mm_and_si128(_mm_castps_si128(vec2::load_xmm(left)), vec2i_abs)));
}

inline vec2 vec2::lerp(const vec2& left, const vec2& right, const vec2& blend) {
    __m128 b1;
    __m128 b2;
    b1 = vec2::load_xmm(blend);
    b2 = _mm_sub_ps(vec2::load_xmm(1.0f), b1);
    return vec2::store_xmm(_mm_add_ps(_mm_mul_ps(vec2::load_xmm(left), b2),
        _mm_mul_ps(vec2::load_xmm(right), b1)));
}

inline vec2 vec2::lerp(const vec2& left, const vec2& right, const float_t blend) {
    __m128 b1;
    __m128 b2;
    b1 = vec2::load_xmm(blend);
    b2 = _mm_sub_ps(vec2::load_xmm(1.0f), b1);
    return vec2::store_xmm(_mm_add_ps(_mm_mul_ps(vec2::load_xmm(left), b2),
        _mm_mul_ps(vec2::load_xmm(right), b1)));
}

inline vec2 vec2::normalize(const vec2& left) {
    __m128 xt;
    __m128 zt;
    xt = vec2::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_sqrt_ss(_mm_hadd_ps(zt, zt));
    if (_mm_cvtss_f32(zt) != 0.0f)
        return vec2::store_xmm(_mm_div_ps(xt, _mm_shuffle_ps(zt, zt, 0)));
    return vec2::store_xmm(xt);
}

inline vec2 vec2::normalize_rcp(const vec2& left) {
    __m128 xt;
    __m128 zt;
    xt = vec2::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_sqrt_ss(_mm_hadd_ps(zt, zt));
    if (_mm_cvtss_f32(zt) != 0.0f)
        zt = _mm_div_ss(_mm_set_ss(1.0f), zt);
    return vec2::store_xmm(_mm_mul_ps(xt, _mm_shuffle_ps(zt, zt, 0)));
}

inline vec2 vec2::rcp(const vec2& left) {
    return vec2::store_xmm(_mm_div_ps(vec2::load_xmm(1.0f), vec2::load_xmm(left)));
}

inline vec2 vec2::min(const vec2& left, const vec2& right) {
    return vec2::store_xmm(_mm_min_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 vec2::max(const vec2& left, const vec2& right) {
    return vec2::store_xmm(_mm_max_ps(vec2::load_xmm(left), vec2::load_xmm(right)));
}

inline vec2 vec2::clamp(const vec2& left, const vec2& min, const vec2& max) {
    return vec2::store_xmm(_mm_min_ps(_mm_max_ps(vec2::load_xmm(left),
        vec2::load_xmm(min)), vec2::load_xmm(max)));
}

inline vec2 vec2::clamp(const vec2& left, const float_t min, const float_t max) {
    return vec2::store_xmm(_mm_min_ps(_mm_max_ps(vec2::load_xmm(left),
        vec2::load_xmm(min)), vec2::load_xmm(max)));
}

inline vec2 vec2::mult_min_max(const vec2& left, const vec2& min, const vec2& max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec2::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec2::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec2::load_xmm(max));
    return vec2::store_xmm(_mm_mul_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec2 vec2::mult_min_max(const vec2& left, const float_t min, const float_t max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec2::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec2::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec2::load_xmm(max));
    return vec2::store_xmm(_mm_mul_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec2 vec2::div_min_max(const vec2& left, const vec2& min, const vec2& max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec2::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec2::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec2::load_xmm(max));
    return vec2::store_xmm(_mm_div_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec2 vec2::div_min_max(const vec2& left, const float_t min, const float_t max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec2::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec2::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec2::load_xmm(max));
    return vec2::store_xmm(_mm_div_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec3::vec3() : x(), y(), z() {

}

inline vec3::vec3(float_t value) : x(value), y(value), z(value) {

}

inline vec3::vec3(float_t x, float_t y, float_t z) : x(x), y(y), z(z) {

}

inline vec3 operator +(const vec3& left, const vec3& right) {
    return vec3::store_xmm(_mm_add_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator +(const vec3& left, const float_t right) {
    return vec3::store_xmm(_mm_add_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator +(const float_t left, const vec3& right) {
    return vec3::store_xmm(_mm_add_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator +=(vec3& left, const vec3& right) {
    left = vec3::store_xmm(_mm_add_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator +=(vec3& left, const float_t right) {
    left = vec3::store_xmm(_mm_add_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator -(const vec3& left, const vec3& right) {
    return vec3::store_xmm(_mm_sub_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator -(const vec3& left, const float_t right) {
    return vec3::store_xmm(_mm_sub_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator -(const float_t left, const vec3& right) {
    return vec3::store_xmm(_mm_sub_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator -=(vec3& left, const vec3& right) {
    left = vec3::store_xmm(_mm_sub_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator -=(vec3& left, const float_t right) {
    left = vec3::store_xmm(_mm_sub_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator *(const vec3& left, const vec3& right) {
    return vec3::store_xmm(_mm_mul_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator *(const vec3& left, const float_t right) {
    return vec3::store_xmm(_mm_mul_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator *(const float_t left, const vec3& right) {
    return vec3::store_xmm(_mm_mul_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator *=(vec3& left, const vec3& right) {
    left = vec3::store_xmm(_mm_mul_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator *=(vec3& left, const float_t right) {
    left = vec3::store_xmm(_mm_mul_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator /(const vec3& left, const vec3& right) {
    return vec3::store_xmm(_mm_div_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator /(const vec3& left, const float_t right) {
    return vec3::store_xmm(_mm_div_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator /(const float_t left, const vec3& right) {
    return vec3::store_xmm(_mm_div_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator /=(vec3& left, const vec3& right) {
    left = vec3::store_xmm(_mm_div_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator /=(vec3& left, const float_t right) {
    left = vec3::store_xmm(_mm_div_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator &(const vec3& left, const vec3& right) {
    return vec3::store_xmm(_mm_and_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator &(const vec3& left, const float_t right) {
    return vec3::store_xmm(_mm_and_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator &(const float_t left, const vec3& right) {
    return vec3::store_xmm(_mm_and_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator &=(vec3& left, const vec3& right) {
    left = vec3::store_xmm(_mm_and_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator &=(vec3& left, const float_t right) {
    left = vec3::store_xmm(_mm_and_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator ^(const vec3& left, const vec3& right) {
    return vec3::store_xmm(_mm_xor_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator ^(const vec3& left, const float_t right) {
    return vec3::store_xmm(_mm_xor_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator ^(const float_t left, const vec3& right) {
    return vec3::store_xmm(_mm_xor_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator ^=(vec3& left, const vec3& right) {
    left = vec3::store_xmm(_mm_xor_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline void operator ^=(vec3& left, const float_t right) {
    left = vec3::store_xmm(_mm_xor_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 operator -(const vec3& left) {
    return vec3::store_xmm(_mm_xor_ps(vec3::load_xmm(left), vec3_neg));
}

inline bool operator ==(const vec3& left, const vec3& right) {
    return !memcmp(&left, &right, sizeof(vec3));
}

inline bool operator !=(const vec3& left, const vec3& right) {
    return !!memcmp(&left, &right, sizeof(vec3));
}

inline __m128 vec3::load_xmm(const float_t data) {
    __m128 _data = _mm_set_ss(data);
    return _mm_shuffle_ps(_data, _data, 0x40);
}

inline __m128 vec3::load_xmm(const vec3& data) {
    return _mm_movelh_ps(_mm_castsi128_ps(_mm_loadl_epi64((const __m128i*) & data)), _mm_load_ss(&data.z));
}

inline __m128 vec3::load_xmm(const vec3&& data) {
    return _mm_movelh_ps(_mm_castsi128_ps(_mm_loadl_epi64((const __m128i*) & data)), _mm_load_ss(&data.z));
}

inline vec3 vec3::store_xmm(const __m128& data) {
    vec3 _data;
    _mm_storel_epi64((__m128i*) & _data, _mm_castps_si128(data));
    _mm_store_ss(&_data.z, _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(data), 8)));
    return _data;
}

inline vec3 vec3::store_xmm(const __m128&& data) {
    vec3 _data;
    _mm_storel_epi64((__m128i*) & _data, _mm_castps_si128(data));
    _mm_store_ss(&_data.z, _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(data), 8)));
    return _data;
}

inline float_t vec3::angle(const vec3& left, const vec3& right) {
    return acosf(vec3::dot(left, right) / (vec3::length(left) * vec3::length(right)));
}

inline float_t vec3::dot(const vec3& left, const vec3& right) {
    __m128 zt;
    zt = _mm_mul_ps(vec3::load_xmm(left), vec3::load_xmm(right));
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline float_t vec3::length(const vec3& left) {
    __m128 xt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(zt, zt)));
}

inline float_t vec3::length_squared(const vec3& left) {
    __m128 xt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline float_t vec3::distance(const vec3& left, const vec3& right) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    yt = vec3::load_xmm(right);
    zt = _mm_sub_ps(xt, yt);
    zt = _mm_mul_ps(zt, zt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(zt, zt)));
}

inline float_t vec3::distance_squared(const vec3& left, const vec3& right) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    yt = vec3::load_xmm(right);
    zt = _mm_sub_ps(xt, yt);
    zt = _mm_mul_ps(zt, zt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline vec3 vec3::abs(const vec3& left) {
    return vec3::store_xmm(_mm_castsi128_ps(_mm_and_si128(_mm_castps_si128(vec3::load_xmm(left)), vec3i_abs)));
}

inline vec3 vec3::lerp(const vec3& left, const vec3& right, const vec3& blend) {
    __m128 b1;
    __m128 b2;
    b1 = vec3::load_xmm(blend);
    b2 = _mm_sub_ps(vec3::load_xmm(1.0f), b1);
    return vec3::store_xmm(_mm_add_ps(_mm_mul_ps(vec3::load_xmm(left), b2),
        _mm_mul_ps(vec3::load_xmm(right), b1)));
}

inline vec3 vec3::lerp(const vec3& left, const vec3& right, const float_t blend) {
    __m128 b1;
    __m128 b2;
    b1 = vec3::load_xmm(blend);
    b2 = _mm_sub_ps(vec3::load_xmm(1.0f), b1);
    return vec3::store_xmm(_mm_add_ps(_mm_mul_ps(vec3::load_xmm(left), b2),
        _mm_mul_ps(vec3::load_xmm(right), b1)));
}

inline vec3 vec3::normalize(const vec3& left) {
    __m128 xt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    zt = _mm_sqrt_ss(_mm_hadd_ps(zt, zt));
    if (_mm_cvtss_f32(zt) != 0.0f)
        return vec3::store_xmm(_mm_div_ps(xt, _mm_shuffle_ps(zt, zt, 0)));
    return vec3::store_xmm(xt);
}

inline vec3 vec3::normalize_rcp(const vec3& left) {
    __m128 xt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    zt = _mm_sqrt_ss(_mm_hadd_ps(zt, zt));
    if (_mm_cvtss_f32(zt) != 0.0f)
        zt = _mm_div_ss(_mm_set_ss(1.0f), zt);
    return vec3::store_xmm(_mm_mul_ps(xt, _mm_shuffle_ps(zt, zt, 0)));
}

inline vec3 vec3::rcp(const vec3& left) {
    return vec3::store_xmm(_mm_div_ps(vec3::load_xmm(1.0f), vec3::load_xmm(left)));
}

inline vec3 vec3::min(const vec3& left, const vec3& right) {
    return vec3::store_xmm(_mm_min_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 vec3::max(const vec3& left, const vec3& right) {
    return vec3::store_xmm(_mm_max_ps(vec3::load_xmm(left), vec3::load_xmm(right)));
}

inline vec3 vec3::clamp(const vec3& left, const vec3& min, const vec3& max) {
    return vec3::store_xmm(_mm_min_ps(_mm_max_ps(vec3::load_xmm(left),
        vec3::load_xmm(min)), vec3::load_xmm(max)));
}

inline vec3 vec3::clamp(const vec3& left, const float_t min, const float_t max) {
    return vec3::store_xmm(_mm_min_ps(_mm_max_ps(vec3::load_xmm(left),
        vec3::load_xmm(min)), vec3::load_xmm(max)));
}

inline vec3 vec3::mult_min_max(const vec3& left, const vec3& min, const vec3& max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec3::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec3::load_xmm(max));
    return vec3::store_xmm(_mm_mul_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec3 vec3::mult_min_max(const vec3& left, const float_t min, const float_t max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec3::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec3::load_xmm(max));
    return vec3::store_xmm(_mm_mul_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec3 vec3::div_min_max(const vec3& left, const vec3& min, const vec3& max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec3::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec3::load_xmm(max));
    return vec3::store_xmm(_mm_div_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec3 vec3::div_min_max(const vec3& left, const float_t min, const float_t max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec3::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec3::load_xmm(max));
    return vec3::store_xmm(_mm_div_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec3 vec3::cross(const vec3& left, const vec3& right) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec3::load_xmm(left);
    yt = vec3::load_xmm(right);
    zt = _mm_sub_ps(
        _mm_mul_ps(xt, _mm_shuffle_ps(yt, yt, 0x09)),
        _mm_mul_ps(yt, _mm_shuffle_ps(xt, xt, 0x09))
    );
    return vec3::store_xmm(_mm_shuffle_ps(zt, zt, 0x09));
}

inline vec4::vec4() : x(), y(), z(), w() {

}

inline vec4::vec4(float_t value) : x(value), y(value), z(value), w(value) {

}

inline vec4::vec4(float_t x, float_t y, float_t z, float_t w) : x(x), y(y), z(z), w(w) {

}

inline vec4 operator +(const vec4& left, const vec4& right) {
    return vec4::store_xmm(_mm_add_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator +(const vec4& left, const float_t right) {
    return vec4::store_xmm(_mm_add_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator +(const float_t left, const vec4& right) {
    return vec4::store_xmm(_mm_add_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator +=(vec4& left, const vec4& right) {
    left = vec4::store_xmm(_mm_add_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator +=(vec4& left, const float_t right) {
    left = vec4::store_xmm(_mm_add_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator -(const vec4& left, const vec4& right) {
    return vec4::store_xmm(_mm_sub_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator -(const vec4& left, const float_t right) {
    return vec4::store_xmm(_mm_sub_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator -(const float_t left, const vec4& right) {
    return vec4::store_xmm(_mm_sub_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator -=(vec4& left, const vec4& right) {
    left = vec4::store_xmm(_mm_sub_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator -=(vec4& left, const float_t right) {
    left = vec4::store_xmm(_mm_sub_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator *(const vec4& left, const vec4& right) {
    return vec4::store_xmm(_mm_mul_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator *(const vec4& left, const float_t right) {
    return vec4::store_xmm(_mm_mul_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator *(const float_t left, const vec4& right) {
    return vec4::store_xmm(_mm_mul_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator *=(vec4& left, const vec4& right) {
    left = vec4::store_xmm(_mm_mul_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator *=(vec4& left, const float_t right) {
    left = vec4::store_xmm(_mm_mul_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator /(const vec4& left, const vec4& right) {
    return vec4::store_xmm(_mm_div_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator /(const vec4& left, const float_t right) {
    return vec4::store_xmm(_mm_div_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator /(const float_t left, const vec4& right) {
    return vec4::store_xmm(_mm_div_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator /=(vec4& left, const vec4& right) {
    left = vec4::store_xmm(_mm_div_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator /=(vec4& left, const float_t right) {
    left = vec4::store_xmm(_mm_div_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator &(const vec4& left, const vec4& right) {
    return vec4::store_xmm(_mm_and_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator &(const vec4& left, const float_t right) {
    return vec4::store_xmm(_mm_and_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator &(const float_t left, const vec4& right) {
    return vec4::store_xmm(_mm_and_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator &=(vec4& left, const vec4& right) {
    left = vec4::store_xmm(_mm_and_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator &=(vec4& left, const float_t right) {
    left = vec4::store_xmm(_mm_and_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator ^(const vec4& left, const vec4& right) {
    return vec4::store_xmm(_mm_xor_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator ^(const vec4& left, const float_t right) {
    return vec4::store_xmm(_mm_xor_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator ^(const float_t left, const vec4& right) {
    return vec4::store_xmm(_mm_xor_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator ^=(vec4& left, const vec4& right) {
    left = vec4::store_xmm(_mm_xor_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline void operator ^=(vec4& left, const float_t right) {
    left = vec4::store_xmm(_mm_xor_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 operator -(const vec4& left) {
    return vec4::store_xmm(_mm_xor_ps(vec4::load_xmm(left), vec4_neg));
}

inline bool operator ==(const vec4& left, const vec4& right) {
    return !memcmp(&left, &right, sizeof(vec4));
}

inline bool operator !=(const vec4& left, const vec4& right) {
    return !!memcmp(&left, &right, sizeof(vec4));
}

inline __m128 vec4::load_xmm(const float_t data) {
    __m128 _data = _mm_set_ss(data);
    return _mm_shuffle_ps(_data, _data, 0);
}

inline __m128 vec4::load_xmm(const vec4& data) {
    return _mm_loadu_ps((const float*)&data);
}

inline __m128 vec4::load_xmm(const vec4&& data) {
    return _mm_loadu_ps((const float*)&data);
}

inline vec4 vec4::store_xmm(const __m128& data) {
    vec4 _data;
    _mm_storeu_ps((float*)&_data, data);
    return _data;
}

inline vec4 vec4::store_xmm(const __m128&& data) {
    vec4 _data;
    _mm_storeu_ps((float*)&_data, data);
    return _data;
}

inline float_t vec4::angle(const vec4& left, const vec4& right) {
    return acosf(vec4::dot(left, right) / (vec4::length(left) * vec4::length(right)));
}

inline float_t vec4::dot(const vec4& left, const vec4& right) {
    __m128 zt;
    zt = _mm_mul_ps(vec4::load_xmm(left), vec4::load_xmm(right));
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline float_t vec4::length(const vec4& left) {
    __m128 xt;
    __m128 zt;
    xt = vec4::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(zt, zt)));
}

inline float_t vec4::length_squared(const vec4& left) {
    __m128 xt;
    __m128 zt;
    xt = vec4::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline float_t vec4::distance(const vec4& left, const vec4& right) {
    __m128 zt;
    zt = _mm_sub_ps(vec4::load_xmm(left), vec4::load_xmm(right));
    zt = _mm_mul_ps(zt, zt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(zt, zt)));
}

inline float_t vec4::distance_squared(const vec4& left, const vec4& right) {
    __m128 zt;
    zt = _mm_sub_ps(vec4::load_xmm(left), vec4::load_xmm(right));
    zt = _mm_mul_ps(zt, zt);
    zt = _mm_hadd_ps(zt, zt);
    return _mm_cvtss_f32(_mm_hadd_ps(zt, zt));
}

inline vec4 vec4::abs(const vec4& left) {
    return vec4::store_xmm(_mm_castsi128_ps(_mm_and_si128(_mm_castps_si128(vec4::load_xmm(left)), vec4i_abs)));
}

inline vec4 vec4::lerp(const vec4& left, const vec4& right, const vec4& blend) {
    __m128 b1;
    __m128 b2;
    b1 = vec4::load_xmm(blend);
    b2 = _mm_sub_ps(vec4::load_xmm(1.0f), b1);
    return vec4::store_xmm(_mm_add_ps(_mm_mul_ps(vec4::load_xmm(left), b2),
        _mm_mul_ps(vec4::load_xmm(right), b1)));
}

inline vec4 vec4::lerp(const vec4& left, const vec4& right, const float_t blend) {
    __m128 b1;
    __m128 b2;
    b1 = vec4::load_xmm(blend);
    b2 = _mm_sub_ps(vec4::load_xmm(1.0f), b1);
    return vec4::store_xmm(_mm_add_ps(_mm_mul_ps(vec4::load_xmm(left), b2),
        _mm_mul_ps(vec4::load_xmm(right), b1)));
}

inline vec4 vec4::normalize(const vec4& left) {
    __m128 xt;
    __m128 zt;
    xt = vec4::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    zt = _mm_sqrt_ss(_mm_hadd_ps(zt, zt));
    if (_mm_cvtss_f32(zt) != 0.0f)
        return vec4::store_xmm(_mm_div_ps(xt, _mm_shuffle_ps(zt, zt, 0)));
    return vec4::store_xmm(xt);
}

inline vec4 vec4::normalize_rcp(const vec4& left) {
    __m128 xt;
    __m128 zt;
    xt = vec4::load_xmm(left);
    zt = _mm_mul_ps(xt, xt);
    zt = _mm_hadd_ps(zt, zt);
    zt = _mm_sqrt_ss(_mm_hadd_ps(zt, zt));
    if (_mm_cvtss_f32(zt) != 0.0f)
        zt = _mm_div_ss(_mm_set_ss(1.0f), zt);
    return vec4::store_xmm(_mm_mul_ps(xt, _mm_shuffle_ps(zt, zt, 0)));
}

inline vec4 vec4::rcp(const vec4& left) {
    return vec4::store_xmm(_mm_div_ps(vec4::load_xmm(1.0f), vec4::load_xmm(left)));
}

inline vec4 vec4::min(const vec4& left, const vec4& right) {
    return vec4::store_xmm(_mm_min_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 vec4::max(const vec4& left, const vec4& right) {
    return vec4::store_xmm(_mm_max_ps(vec4::load_xmm(left), vec4::load_xmm(right)));
}

inline vec4 vec4::clamp(const vec4& left, const vec4& min, const vec4& max) {
    return vec4::store_xmm(_mm_min_ps(_mm_max_ps(vec4::load_xmm(left),
        vec4::load_xmm(min)), vec4::load_xmm(max)));
}

inline vec4 vec4::clamp(const vec4& left, const float_t min, const float_t max) {
    return vec4::store_xmm(_mm_min_ps(_mm_max_ps(vec4::load_xmm(left),
        vec4::load_xmm(min)), vec4::load_xmm(max)));
}

inline vec4 vec4::mult_min_max(const vec4& left, const vec4& min, const vec4& max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec4::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec4::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec4::load_xmm(max));
    return vec4::store_xmm(_mm_mul_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec4 vec4::mult_min_max(const vec4& left, const float_t min, const float_t max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec4::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec4::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec4::load_xmm(max));
    return vec4::store_xmm(_mm_mul_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec4 vec4::div_min_max(const vec4& left, const vec4& min, const vec4& max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec4::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec4::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec4::load_xmm(max));
    return vec4::store_xmm(_mm_div_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec4 vec4::div_min_max(const vec4& left, const float_t min, const float_t max) {
    __m128 xt;
    __m128 yt;
    __m128 zt;
    xt = vec4::load_xmm(left);
    yt = _mm_and_ps(_mm_cmplt_ps(xt, vec4::load_xmm(0.0f)), vec4::load_xmm(-min));
    zt = _mm_and_ps(_mm_cmpge_ps(xt, vec4::load_xmm(0.0f)), vec4::load_xmm(max));
    return vec4::store_xmm(_mm_div_ps(xt, _mm_or_ps(yt, zt)));
}

inline vec2i::vec2i() : x(), y() {

}

inline vec2i::vec2i(int32_t value) : x(value), y(value) {

}

inline vec2i::vec2i(int32_t x, int32_t y) : x(x), y(y) {

}

inline vec2i operator +(const vec2i& left, const vec2i& right) {
    return vec2i::store_xmm(_mm_add_epi32(vec2i::load_xmm(left), vec2i::load_xmm(right)));
}

inline vec2i operator +(const vec2i& left, const int32_t right) {
    return vec2i::store_xmm(_mm_add_epi32(vec2i::load_xmm(left), vec2i::load_xmm(right)));
}

inline vec2i operator +(const int32_t left, const vec2i& right) {
    return vec2i::store_xmm(_mm_add_epi32(vec2i::load_xmm(left), vec2i::load_xmm(right)));
}

inline vec2i operator -(const vec2i& left, const vec2i& right) {
    return vec2i::store_xmm(_mm_sub_epi32(vec2i::load_xmm(left), vec2i::load_xmm(right)));
}

inline vec2i operator -(const vec2i& left, const int32_t right) {
    return vec2i::store_xmm(_mm_sub_epi32(vec2i::load_xmm(left), vec2i::load_xmm(right)));
}

inline vec2i operator -(const int32_t left, const vec2i& right) {
    return vec2i::store_xmm(_mm_sub_epi32(vec2i::load_xmm(left), vec2i::load_xmm(right)));
}

inline __m128i vec2i::load_xmm(const int32_t data) {
    __m128i _data = _mm_cvtsi32_si128(data);
    return _mm_shuffle_epi32(_data, 0);
}

inline __m128i vec2i::load_xmm(const vec2i& data) {
    return _mm_loadl_epi64((const __m128i*) & data);
}

inline __m128i vec2i::load_xmm(const vec2i&& data) {
    return _mm_loadl_epi64((const __m128i*) & data);
}

inline vec2i vec2i::store_xmm(const __m128i& data) {
    vec2i _data;
    _mm_storel_epi64((__m128i*) & _data, data);
    return _data;
}

inline vec2i vec2i::store_xmm(const __m128i&& data) {
    vec2i _data;
    _mm_storel_epi64((__m128i*) & _data, data);
    return _data;
}

inline vec2i vec2i::min(const vec2i& left, const vec2i& right) {
    return vec2i::store_xmm(_mm_min_epi32(vec2i::load_xmm(left), vec2i::load_xmm(right)));
}

inline vec2i vec2i::max(const vec2i& left, const vec2i& right) {
    return vec2i::store_xmm(_mm_max_epi32(vec2i::load_xmm(left), vec2i::load_xmm(right)));
}

inline vec2i vec2i::clamp(const vec2i& left, const vec2i& min, const vec2i& max) {
    return vec2i::store_xmm(_mm_min_epi32(_mm_max_epi32(vec2i::load_xmm(left),
        vec2i::load_xmm(min)), vec2i::load_xmm(max)));
}

inline vec2i vec2i::clamp(const vec2i& left, const int32_t min, const int32_t max) {
    return vec2i::store_xmm(_mm_min_epi32(_mm_max_epi32(vec2i::load_xmm(left),
        vec2i::load_xmm(min)), vec2i::load_xmm(max)));
}

inline vec3i::vec3i() : x(), y(), z() {

}

inline vec3i::vec3i(int32_t value) : x(value), y(value), z(value) {

}

inline vec3i::vec3i(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) {

}

inline vec3i operator +(const vec3i& left, const vec3i& right) {
    return vec3i::store_xmm(_mm_add_epi32(vec3i::load_xmm(left), vec3i::load_xmm(right)));
}

inline vec3i operator +(const vec3i& left, const int32_t right) {
    return vec3i::store_xmm(_mm_add_epi32(vec3i::load_xmm(left), vec3i::load_xmm(right)));
}

inline vec3i operator +(const int32_t left, const vec3i& right) {
    return vec3i::store_xmm(_mm_add_epi32(vec3i::load_xmm(left), vec3i::load_xmm(right)));
}

inline vec3i operator -(const vec3i& left, const vec3i& right) {
    return vec3i::store_xmm(_mm_sub_epi32(vec3i::load_xmm(left), vec3i::load_xmm(right)));
}

inline vec3i operator -(const vec3i& left, const int32_t right) {
    return vec3i::store_xmm(_mm_sub_epi32(vec3i::load_xmm(left), vec3i::load_xmm(right)));
}

inline vec3i operator -(const int32_t left, const vec3i& right) {
    return vec3i::store_xmm(_mm_sub_epi32(vec3i::load_xmm(left), vec3i::load_xmm(right)));
}

inline __m128i vec3i::load_xmm(const int32_t data) {
    __m128i _data = _mm_cvtsi32_si128(data);
    return _mm_shuffle_epi32(_data, 0);
}

inline __m128i vec3i::load_xmm(const vec3i& data) {
    return _mm_unpacklo_epi64(_mm_loadl_epi64((const __m128i*) & data), _mm_cvtsi32_si128(data.z));
}

inline __m128i vec3i::load_xmm(const vec3i&& data) {
    return _mm_unpacklo_epi64(_mm_loadl_epi64((const __m128i*) & data), _mm_cvtsi32_si128(data.z));
}

inline vec3i vec3i::store_xmm(const __m128i& data) {
    vec3i _data;
    _mm_storel_epi64((__m128i*) & _data, data);
    _data.z = _mm_cvtsi128_si32(_mm_srli_si128(data, 8));
    return _data;
}

inline vec3i vec3i::store_xmm(const __m128i&& data) {
    vec3i _data;
    _mm_storel_epi64((__m128i*) & _data, data);
    _data.z = _mm_cvtsi128_si32(_mm_srli_si128(data, 8));
    return _data;
}

inline vec3i vec3i::min(const vec3i& left, const vec3i& right) {
    return vec3i::store_xmm(_mm_min_epi32(vec3i::load_xmm(left), vec3i::load_xmm(right)));
}

inline vec3i vec3i::max(const vec3i& left, const vec3i& right) {
    return vec3i::store_xmm(_mm_max_epi32(vec3i::load_xmm(left), vec3i::load_xmm(right)));
}

inline vec3i vec3i::clamp(const vec3i& left, const vec3i& min, const vec3i& max) {
    return vec3i::store_xmm(_mm_min_epi32(_mm_max_epi32(vec3i::load_xmm(left),
        vec3i::load_xmm(min)), vec3i::load_xmm(max)));
}

inline vec3i vec3i::clamp(const vec3i& left, const int32_t min, const int32_t max) {
    return vec3i::store_xmm(_mm_min_epi32(_mm_max_epi32(vec3i::load_xmm(left),
        vec3i::load_xmm(min)), vec3i::load_xmm(max)));
}

inline vec4i::vec4i() : x(), y(), z(), w() {

}

inline vec4i::vec4i(int32_t value) : x(value), y(value), z(value), w(value) {

}

inline vec4i::vec4i(int32_t x, int32_t y, int32_t z, int32_t w) : x(x), y(y), z(z), w(w) {

}

inline vec4i operator +(const vec4i& left, const vec4i& right) {
    return vec4i::store_xmm(_mm_add_epi32(vec4i::load_xmm(left), vec4i::load_xmm(right)));
}

inline vec4i operator +(const vec4i& left, const int32_t right) {
    return vec4i::store_xmm(_mm_add_epi32(vec4i::load_xmm(left), vec4i::load_xmm(right)));
}

inline vec4i operator +(const int32_t left, const vec4i& right) {
    return vec4i::store_xmm(_mm_add_epi32(vec4i::load_xmm(left), vec4i::load_xmm(right)));
}

inline vec4i operator -(const vec4i& left, const vec4i& right) {
    return vec4i::store_xmm(_mm_sub_epi32(vec4i::load_xmm(left), vec4i::load_xmm(right)));
}

inline vec4i operator -(const vec4i& left, const int32_t right) {
    return vec4i::store_xmm(_mm_sub_epi32(vec4i::load_xmm(left), vec4i::load_xmm(right)));
}

inline vec4i operator -(const int32_t left, const vec4i& right) {
    return vec4i::store_xmm(_mm_sub_epi32(vec4i::load_xmm(left), vec4i::load_xmm(right)));
}

inline __m128i vec4i::load_xmm(const int32_t data) {
    __m128i _data = _mm_cvtsi32_si128(data);
    return _mm_shuffle_epi32(_data, 0);
}

inline __m128i vec4i::load_xmm(const vec4i& data) {
    return _mm_loadu_si128((const __m128i*) & data);
}

inline __m128i vec4i::load_xmm(const vec4i&& data) {
    return _mm_loadu_si128((const __m128i*) & data);
}

inline vec4i vec4i::store_xmm(const __m128i& data) {
    vec4i _data;
    _mm_storeu_si128((__m128i*) & _data, data);
    return _data;
}

inline vec4i vec4i::store_xmm(const __m128i&& data) {
    vec4i _data;
    _mm_storeu_si128((__m128i*) & _data, data);
    return _data;
}

inline vec4i vec4i::min(const vec4i& left, const vec4i& right) {
    return vec4i::store_xmm(_mm_min_epi32(vec4i::load_xmm(left), vec4i::load_xmm(right)));
}

inline vec4i vec4i::max(const vec4i& left, const vec4i& right) {
    return vec4i::store_xmm(_mm_max_epi32(vec4i::load_xmm(left), vec4i::load_xmm(right)));
}

inline vec4i vec4i::clamp(const vec4i& left, const vec4i& min, const vec4i& max) {
    return vec4i::store_xmm(_mm_min_epi32(_mm_max_epi32(vec4i::load_xmm(left),
        vec4i::load_xmm(min)), vec4i::load_xmm(max)));
}

inline vec4i vec4i::clamp(const vec4i& left, const int32_t min, const int32_t max) {
    return vec4i::store_xmm(_mm_min_epi32(_mm_max_epi32(vec4i::load_xmm(left),
        vec4i::load_xmm(min)), vec4i::load_xmm(max)));
}

inline void vec2i8_to_vec2(const vec2i8& src, vec2& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
}

inline void vec3i8_to_vec3(const vec3i8& src, vec3& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
    dst.z = (float_t)src.z;
}

inline void vec4i8_to_vec4(const vec4i8& src, vec4& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
    dst.z = (float_t)src.z;
    dst.w = (float_t)src.w;
}

inline void vec2_to_vec2i8(const vec2& src, vec2i8& dst) {
    dst.x = (int8_t)src.x;
    dst.y = (int8_t)src.y;
}

inline void vec3_to_vec3i8(const vec3& src, vec3i8& dst) {
    dst.x = (int8_t)src.x;
    dst.y = (int8_t)src.y;
    dst.z = (int8_t)src.z;
}

inline void vec4_to_vec4i8(const vec4& src, vec4i8& dst) {
    dst.x = (int8_t)src.x;
    dst.y = (int8_t)src.y;
    dst.z = (int8_t)src.z;
    dst.w = (int8_t)src.w;
}

inline void vec2u8_to_vec2(const vec2u8& src, vec2& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
}

inline void vec3u8_to_vec3(const vec3u8& src, vec3& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
    dst.z = (float_t)src.z;
}

inline void vec4u8_to_vec4(const vec4u8& src, vec4& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
    dst.z = (float_t)src.z;
    dst.w = (float_t)src.w;
}

inline void vec2_to_vec2u8(const vec2& src, vec2u8& dst) {
    dst.x = (uint8_t)src.x;
    dst.y = (uint8_t)src.y;
}

inline void vec3_to_vec3u8(const vec3& src, vec3u8& dst) {
    dst.x = (uint8_t)src.x;
    dst.y = (uint8_t)src.y;
    dst.z = (uint8_t)src.z;
}

inline void vec4_to_vec4u8(const vec4& src, vec4u8& dst) {
    dst.x = (uint8_t)src.x;
    dst.y = (uint8_t)src.y;
    dst.z = (uint8_t)src.z;
    dst.w = (uint8_t)src.w;
}

inline void vec2i16_to_vec2(const vec2i16& src, vec2& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
}

inline void vec3i16_to_vec3(const vec3i16& src, vec3& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
    dst.z = (float_t)src.z;
}

inline void vec4i16_to_vec4(const vec4i16& src, vec4& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
    dst.z = (float_t)src.z;
    dst.w = (float_t)src.w;
}

inline void vec2_to_vec2i16(const vec2& src, vec2i16& dst) {
    dst.x = (int16_t)src.x;
    dst.y = (int16_t)src.y;
}

inline void vec3_to_vec3i16(const vec3& src, vec3i16& dst) {
    dst.x = (int16_t)src.x;
    dst.y = (int16_t)src.y;
    dst.z = (int16_t)src.z;
}

inline void vec4_to_vec4i16(const vec4& src, vec4i16& dst) {
    dst.x = (int16_t)src.x;
    dst.y = (int16_t)src.y;
    dst.z = (int16_t)src.z;
    dst.w = (int16_t)src.w;
}

inline void vec2u16_to_vec2(const vec2u16& src, vec2& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
}

inline void vec3u16_to_vec3(const vec3u16& src, vec3& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
    dst.z = (float_t)src.z;
}

inline void vec4u16_to_vec4(const vec4u16& src, vec4& dst) {
    dst.x = (float_t)src.x;
    dst.y = (float_t)src.y;
    dst.z = (float_t)src.z;
    dst.w = (float_t)src.w;
}

inline void vec2_to_vec2u16(const vec2& src, vec2u16& dst) {
    dst.x = (uint16_t)src.x;
    dst.y = (uint16_t)src.y;
}

inline void vec3_to_vec3u16(const vec3& src, vec3u16& dst) {
    dst.x = (uint16_t)src.x;
    dst.y = (uint16_t)src.y;
    dst.z = (uint16_t)src.z;
}

inline void vec4_to_vec4u16(const vec4& src, vec4u16& dst) {
    dst.x = (uint16_t)src.x;
    dst.y = (uint16_t)src.y;
    dst.z = (uint16_t)src.z;
    dst.w = (uint16_t)src.w;
}

inline void vec2h_to_vec2(const vec2h& src, vec2& dst) {
    extern bool f16c;
    if (f16c) {
        dst = vec2::store_xmm(_mm_cvtph_ps(_mm_cvtsi32_si128(*(int32_t*)&src)));
        return;
    }

    dst.x = half_to_float_convert(src.x);
    dst.y = half_to_float_convert(src.y);
}

inline void vec3h_to_vec3(const vec3h& src, vec3& dst) {
    dst.x = half_to_float_convert(src.x);
    dst.y = half_to_float_convert(src.y);
    dst.z = half_to_float_convert(src.z);
}

inline void vec4h_to_vec4(const vec4h& src, vec4& dst) {
    extern bool f16c;
    if (f16c) {
        dst = vec4::store_xmm(_mm_cvtph_ps(_mm_cvtsi64_si128(*(int64_t*)&src)));
        return;
    }

    dst.x = half_to_float_convert(src.x);
    dst.y = half_to_float_convert(src.y);
    dst.z = half_to_float_convert(src.z);
    dst.w = half_to_float_convert(src.w);
}

inline void vec2_to_vec2h(const vec2& src, vec2h& dst) {
    extern bool f16c;
    if (f16c) {
        *(int32_t*)&dst = _mm_cvtsi128_si32(_mm_cvtps_ph(vec2::load_xmm(src), _MM_FROUND_CUR_DIRECTION));
        return;
    }

    dst.x = float_to_half_convert(src.x);
    dst.y = float_to_half_convert(src.y);
}

inline void vec3_to_vec3h(const vec3& src, vec3h& dst) {
    dst.x = float_to_half_convert(src.x);
    dst.y = float_to_half_convert(src.y);
    dst.z = float_to_half_convert(src.z);
}

inline void vec4_to_vec4h(const vec4& src, vec4h& dst) {
    extern bool f16c;
    if (f16c) {
        *(int64_t*)&dst = _mm_cvtsi128_si64(_mm_cvtps_ph(vec4::load_xmm(src), _MM_FROUND_CUR_DIRECTION));
        return;
    }

    dst.x = float_to_half_convert(src.x);
    dst.y = float_to_half_convert(src.y);
    dst.z = float_to_half_convert(src.z);
    dst.w = float_to_half_convert(src.w);
}

inline void vec2i8_to_vec2i(const vec2i8& src, vec2i& dst) {
    dst.x = src.x;
    dst.y = src.y;
}

inline void vec3i8_to_vec3i(const vec3i8& src, vec3i& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

inline void vec4i8_to_vec4i(const vec4i8& src, vec4i& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}

inline void vec2i_to_vec2i8(const vec2i& src, vec2i8& dst) {
    dst.x = (int8_t)src.x;
    dst.y = (int8_t)src.y;
}

inline void vec3i_to_vec3i8(const vec3i& src, vec3i8& dst) {
    dst.x = (int8_t)src.x;
    dst.y = (int8_t)src.y;
    dst.z = (int8_t)src.z;
}

inline void vec4i_to_vec4i8(const vec4i& src, vec4i8& dst) {
    dst.x = (int8_t)src.x;
    dst.y = (int8_t)src.y;
    dst.z = (int8_t)src.z;
    dst.w = (int8_t)src.w;
}

inline void vec2u8_to_vec2i(const vec2u8& src, vec2i& dst) {
    dst.x = src.x;
    dst.y = src.y;
}

inline void vec3u8_to_vec4i(const vec3u8& src, vec3i& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

inline void vec4u8_to_vec4i(const vec4u8& src, vec4i& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}

inline void vec2i_to_vec2u8(const vec2i& src, vec2u8& dst) {
    dst.x = (uint8_t)src.x;
    dst.y = (uint8_t)src.y;
}

inline void vec3i_to_vec3u8(const vec3i& src, vec3u8& dst) {
    dst.x = (uint8_t)src.x;
    dst.y = (uint8_t)src.y;
    dst.z = (uint8_t)src.z;
}

inline void vec4i_to_vec4u8(const vec4i& src, vec4u8& dst) {
    dst.x = (uint8_t)src.x;
    dst.y = (uint8_t)src.y;
    dst.z = (uint8_t)src.z;
    dst.w = (uint8_t)src.w;
}

inline void vec2i16_to_vec2i(const vec2i16& src, vec2i& dst) {
    dst.x = src.x;
    dst.y = src.y;
}

inline void vec3i16_to_vec4i(const vec3i16& src, vec3i& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

inline void vec4i16_to_vec4i(const vec4i16& src, vec4i& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}

inline void vec2i_to_vec2i16(const vec2i& src, vec2i16& dst) {
    dst.x = (int16_t)src.x;
    dst.y = (int16_t)src.y;
}

inline void vec3i_to_vec3i16(const vec3i& src, vec3i16& dst) {
    dst.x = (int16_t)src.x;
    dst.y = (int16_t)src.y;
    dst.z = (int16_t)src.z;
}

inline void vec4i_to_vec4i16(const vec4i& src, vec4i16& dst) {
    dst.x = (int16_t)src.x;
    dst.y = (int16_t)src.y;
    dst.z = (int16_t)src.z;
    dst.w = (int16_t)src.w;
}

inline void vec2u16_to_vec2i(const vec2u16& src, vec2i& dst) {
    dst.x = src.x;
    dst.y = src.y;
}

inline void vec3u16_to_vec4i(const vec3u16& src, vec3i& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

inline void vec4u16_to_vec4i(const vec4u16& src, vec4i& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}

inline void vec2i_to_vec2u16(const vec2i& src, vec2u16& dst) {
    dst.x = (uint16_t)src.x;
    dst.y = (uint16_t)src.y;
}

inline void vec3i_to_vec3u16(const vec3i& src, vec3u16& dst) {
    dst.x = (uint16_t)src.x;
    dst.y = (uint16_t)src.y;
    dst.z = (uint16_t)src.z;
}

inline void vec4i_to_vec4u16(const vec4i& src, vec4u16& dst) {
    dst.x = (uint16_t)src.x;
    dst.y = (uint16_t)src.y;
    dst.z = (uint16_t)src.z;
    dst.w = (uint16_t)src.w;
}

inline void vec2_to_vec2i(const vec2& x, vec2i& z) {
    z = vec2i::store_xmm(_mm_cvtps_epi32(vec2::load_xmm(x)));
}

inline void vec2i_to_vec2(const vec2i& x, vec2& z) {
    z = vec2::store_xmm(_mm_cvtepi32_ps(vec2i::load_xmm(x)));
}

inline void vec3_to_vec3i(const vec3& x, vec3i& z) {
    z = vec3i::store_xmm(_mm_cvtps_epi32(vec3::load_xmm(x)));
}

inline void vec3i_to_vec3(const vec3i& x, vec3& z) {
    z = vec3::store_xmm(_mm_cvtepi32_ps(vec3i::load_xmm(x)));
}

inline void vec4_to_vec4i(const vec4& x, vec4i& z) {
    z = vec4i::store_xmm(_mm_cvtps_epi32(vec4::load_xmm(x)));
}

inline void vec4i_to_vec4(const vec4i& x, vec4& z) {
    z = vec4::store_xmm(_mm_cvtepi32_ps(vec4i::load_xmm(x)));
}
