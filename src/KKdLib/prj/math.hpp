/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../default.hpp"
#include <xmmintrin.h>
#include <emmintrin.h>

namespace prj {
    inline int32_t extract_sign(const float_t x) {
        return _mm_movemask_ps(_mm_load_ss(&x)) & 0x01;
    }

    inline float_t ceilf(const float_t x) {
        int32_t x_int = (int32_t)x;
        if (x_int != 0x80000000 && (float_t)x_int != x)
            return (float_t)(x_int + !extract_sign(x));
        return x;
    }

    inline float_t floorf(const float_t x) {
        int32_t x_int = (int32_t)x;
        if (x_int != 0x80000000 && (float_t)x_int != x)
            return (float_t)(x_int - extract_sign(x));
        return x;
    }

    inline float_t roundf(const float_t x) {
        if (x >= 0.0f)
            return floorf(x + 0.5f);
        else
            return ceilf(x - 0.5f);
    }

    inline float_t truncf(const float_t x) {
        if (x >= 0.0f)
            return floorf(x);
        else
            return ceilf(x);
    }

    inline int32_t extract_sign(const double_t x) {
        return _mm_movemask_pd(_mm_load_sd(&x)) & 0x01;
    }

    inline double_t ceil(const double_t x) {
        int64_t x_int = (int64_t)x;
        if (x_int != 0x8000000000000000 && (float_t)x_int != x)
            return (float_t)(x_int + !extract_sign(x));
        return x;
    }

    inline double_t floor(const double_t x) {
        int64_t x_int = (int64_t)x;
        if (x_int != 0x8000000000000000 && (float_t)x_int != x)
            return (float_t)(x_int - extract_sign(x));
        return x;
    }

    inline double_t roundf(const double_t x) {
        if (x >= 0.0f)
            return floor(x + 0.5f);
        else
            return ceil(x - 0.5f);
    }

    inline double_t truncf(const double_t x) {
        if (x >= 0.0f)
            return floor(x);
        else
            return ceil(x);
    }
}
