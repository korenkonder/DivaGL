/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "half_t.hpp"

float_t half_to_float_convert(half_t h) {
    int32_t si32;
    uint16_t sign = (h >> 15) & 0x001;
    uint16_t exponent = (h >> 10) & 0x01F;
    uint16_t mantissa = h & 0x3FF;
    si32 = sign ? (int32_t)0x80000000 : 0x00000000;

    if (exponent == 0x1F)
        si32 |= 0x7F800000;
    else if (exponent != 0x00)
        si32 |= ((int32_t)exponent - 0x0F + 0x7F) << 23;
    si32 |= (int32_t)mantissa << 13;
    return *(float_t*)&si32;
}

half_t float_to_half_convert(float_t val) {
    int32_t si32 = *(int32_t*)&val;
    if (si32 == 0x00000000)
        return FLOAT16_POSITIVE_ZERO;
    else if ((uint64_t)si32 == 0x80000000)
        return FLOAT16_NEGATIVE_ZERO;

    int16_t sign = (int16_t)((si32 >> 31) & 0x001);
    int16_t exponent = (int16_t)((si32 >> 23) & 0x0FF);
    int16_t mantissa = (int16_t)((si32 >> 13) & 0x3FF);

    if (exponent == 0xFF)
        exponent = 0x1F;
    else if (exponent != 0x00) {
        exponent -= 0x7F - 0x0F;
        if (exponent < 0x00)
            exponent = mantissa = 0;
        else if (exponent >= 0x1F)
            exponent = 0x1F;
    }
    return (half_t)((sign << 15) | (exponent << 10) | mantissa);
}

double_t half_to_double_convert(half_t h) {
    int64_t si64;
    uint16_t sign = (h >> 15) & 0x001;
    uint16_t exponent = (h >> 10) & 0x01F;
    uint16_t mantissa = h & 0x3FF;
    si64 = sign ? (int64_t)0x8000000000000000 : 0x0000000000000000;

    if (exponent == 0x1F)
        si64 |= 0x7FF0000000000000;
    else if (exponent != 0x00)
        si64 |= ((int64_t)exponent - 0x0F + 0x3FF) << 52;
    si64 |= (int64_t)mantissa << 42;
    return *(double_t*)&si64;
}

half_t double_to_half_convert(double_t val) {
    int64_t si64 = *(int64_t*)&val;
    if (si64 == 0x0000000000000000)
        return FLOAT16_POSITIVE_ZERO;
    else if ((uint64_t)si64 == 0x8000000000000000)
        return FLOAT16_NEGATIVE_ZERO;

    int16_t sign = (int16_t)((si64 >> 63) & 0x001);
    int16_t exponent = (int16_t)((si64 >> 52) & 0x7FF);
    int16_t mantissa = (int16_t)((si64 >> 42) & 0x3FF);

    if (exponent == 0x7FF)
        exponent = 0x1F;
    else if (exponent != 0x00) {
        exponent -= 0x3FF - 0x0F;
        if (exponent < 0x00)
            exponent = mantissa = 0;
        else if (exponent >= 0x1F)
            exponent = 0x1F;
    }
    return (half_t)((sign << 15) | (exponent << 10) | mantissa);
}
