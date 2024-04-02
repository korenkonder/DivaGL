/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"
#include "vec.hpp"

struct color_tone {
    vec3 blend;
    vec3 offset;
    float_t hue;
    float_t saturation;
    float_t value;
    float_t contrast;
    bool inverse;

    color_tone();
};

struct rgb565 {
    uint16_t b : 5;
    uint16_t g : 6;
    uint16_t r : 5;
};

struct dxt1_block {
    rgb565 color0;
    rgb565 color1;
    uint16_t indices[2];
};

struct dxt5_block {
    uint8_t alpha0;
    uint8_t alpha1;
    uint8_t alpha_indices[6];
    rgb565 color0;
    rgb565 color1;
    uint16_t color_indices[2];
};

extern void dxt1_image_apply_color_tone(int32_t width, int32_t height,
    int32_t size, dxt1_block* data, const color_tone* col_tone);
extern void dxt5_image_apply_color_tone(int32_t width, int32_t height,
    int32_t size, dxt5_block* data, const color_tone* col_tone);
extern void rgb565_image_apply_color_tone(int32_t width, int32_t height,
    int32_t size, rgb565* data, const color_tone* col_tone);
