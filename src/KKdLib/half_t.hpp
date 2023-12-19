/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"
#include <immintrin.h>

#define FLOAT16_NAN           ((half_t)0x7FFF)
#define FLOAT16_POSITIVE_NAN  ((half_t)0x7FFF)
#define FLOAT16_NEGATIVE_NAN  ((half_t)0xFFFF)
#define FLOAT16_POSITIVE_ZERO ((half_t)0x0000)
#define FLOAT16_NEGATIVE_ZERO ((half_t)0x8000)
#define FLOAT16_POSITIVE_INF  ((half_t)0x7C00)
#define FLOAT16_NEGATIVE_INF  ((half_t)0xFC00)

#define HALF_MAX 65504
#define HALF_MIN 0.00006103515625

typedef unsigned short half_t;

inline half_t load_reverse_endianness_half_t(void* ptr) {
    return (half_t)_byteswap_ushort(*(uint16_t*)ptr);
}

inline void store_reverse_endianness_half_t(void* ptr, half_t value) {
    *(half_t*)ptr = (half_t)_byteswap_ushort((uint16_t)value);
}

inline half_t reverse_endianness_half_t(half_t value) {
    return (half_t)_byteswap_ushort((uint16_t)value);
}

extern float_t half_to_float_convert(half_t h);
extern half_t float_to_half_convert(float_t val);
extern double_t half_to_double_convert(half_t h);
extern half_t double_to_half_convert(double_t val);

inline float_t half_to_float(half_t h) {
    extern bool f16c;
    if (f16c)
        return _mm_cvtss_f32(_mm_cvtph_ps(_mm_cvtsi32_si128((uint16_t)h)));
    return half_to_float_convert(h);
}

inline half_t float_to_half(float_t val) {
    extern bool f16c;
    if (f16c)
        return (half_t)_mm_cvtsi128_si32(_mm_cvtps_ph(_mm_set_ss(val), _MM_FROUND_CUR_DIRECTION));
    return float_to_half_convert(val);
}

inline double_t half_to_double(half_t h) {
    extern bool f16c;
    if (f16c)
        return _mm_cvtss_f32(_mm_cvtph_ps(_mm_cvtsi32_si128((uint16_t)h)));
    return half_to_double_convert(h);
}

inline half_t double_to_half(double_t val) {
    extern bool f16c;
    if (f16c)
        return (half_t)_mm_cvtsi128_si32(_mm_cvtps_ph(_mm_cvtpd_ps(_mm_set_sd(val)), _MM_FROUND_CUR_DIRECTION));
    return double_to_half_convert(val);
}
