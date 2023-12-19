/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../default.hpp"
#include <xmmintrin.h>

namespace prj {
    inline int32_t extract_sign(float_t x) {
        return _mm_movemask_ps(_mm_set_ss(x)) & 0x01;
    }

    inline float_t ceilf(float_t x) {
        int32_t x_int = (int32_t)x;
        if (x_int != 0x80000000 && (float_t)x_int != x)
            x = (float_t)(x_int + !extract_sign(x));
        return x;
    }

    inline float_t floorf(float_t x) {
        int32_t x_int = (int32_t)x;
        if (x_int != 0x80000000 && (float_t)x_int != x)
            x = (float_t)(x_int - extract_sign(x));
        return x;
    }

    inline float_t roundf(float_t x) {
        if (x >= 0.0f)
            return floorf(x + 0.5f);
        else
            return ceilf(x - 0.5f);
    }

    inline float_t truncf(float_t x) {
        if (x >= 0.0f)
            return floorf(x);
        else
            return ceilf(x);
    }
}
