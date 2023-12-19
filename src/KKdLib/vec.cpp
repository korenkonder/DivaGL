/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "vec.hpp"

const __m128 vec2_neg = { -0.0f, -0.0f,  0.0f,  0.0f };
const __m128 vec3_neg = { -0.0f, -0.0f, -0.0f,  0.0f };
const __m128 vec4_neg = { -0.0f, -0.0f, -0.0f, -0.0f };

const __m128i vec2i_abs = {
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
    (char)0x00, (char)0x00, (char)0x00, (char)0x00,
    (char)0x00, (char)0x00, (char)0x00, (char)0x00,
};

const __m128i vec3i_abs = {
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
    (char)0x00, (char)0x00, (char)0x00, (char)0x00,
};

const __m128i vec4i_abs = {
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
    (char)0xFF, (char)0xFF, (char)0xFF, (char)0x7F,
};