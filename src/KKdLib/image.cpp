/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "image.hpp"

static rgb565 rgb565_apply_color_tone(rgb565 col, const color_tone* col_tone);
static void rgb_to_ycc(vec3& rgb, vec3& ycc);
static void ycc_apply_col_tone_hue(vec3& ycc, const color_tone* col_tone);
static void ycc_to_rgb(vec3& ycc, vec3& rgb);

color_tone::color_tone() : hue(), saturation(), value(), contrast(), inverse() {

}

void dxt1_image_apply_color_tone(int32_t width, int32_t height,
    int32_t size, dxt1_block* data, const color_tone* col_tone) {
    if (size <= 0)
        return;

    for (size_t i = (size - 1ULL) / sizeof(dxt1_block) + 1; i; i--, data++) {
        data->color0 = rgb565_apply_color_tone(data->color0, col_tone);
        data->color1 = rgb565_apply_color_tone(data->color1, col_tone);
        if (*(uint16_t*)&data->color0 <= *(uint16_t*)&data->color1) {
            data->indices[0] &= ~((data->indices[0] >> 1) & 0x5555);
            data->indices[1] &= ~((data->indices[1] >> 1) & 0x5555);
        }
    }
}

void dxt5_image_apply_color_tone(int32_t width, int32_t height,
    int32_t size, dxt5_block* data, const color_tone* col_tone) {
    if (size <= 0)
        return;

    for (size_t i = (size - 1ULL) / sizeof(dxt5_block) + 1; i; i--, data++) {
        data->color0 = rgb565_apply_color_tone(data->color0, col_tone);
        data->color1 = rgb565_apply_color_tone(data->color1, col_tone);
        if (*(uint16_t*)&data->color0 <= *(uint16_t*)&data->color1) {
            data->color_indices[0] &= ~((data->color_indices[0] >> 1) & 0x5555);
            data->color_indices[1] &= ~((data->color_indices[1] >> 1) & 0x5555);
        }
    }
}

void rgb565_image_apply_color_tone(int32_t width, int32_t height,
    int32_t size, rgb565* data, const color_tone* col_tone) {
    for (size_t i = (size - 1ULL) / sizeof(uint16_t) + 1; i; i--, data++)
        *data = rgb565_apply_color_tone(*data, col_tone);
}

static rgb565 rgb565_apply_color_tone(rgb565 col, const color_tone* col_tone) {
    const vec3 scale = {
        (float_t)((1 << 5) - 1),
        (float_t)((1 << 6) - 1),
        (float_t)((1 << 5) - 1),
    };

    const vec3 inv_scale = {
        (float_t)(1.0 / (double_t)((1 << 5) - 1)),
        (float_t)(1.0 / (double_t)((1 << 6) - 1)),
        (float_t)(1.0 / (double_t)((1 << 5) - 1)),
    };

    vec3 rgb;
    if (col_tone->inverse) {
        rgb.x = (float_t)((1 << 5) - 1 - col.r);
        rgb.y = (float_t)((1 << 5) - 1 - col.g);
        rgb.z = (float_t)((1 << 5) - 1 - col.b);
    }
    else {
        rgb.x = (float_t)col.r;
        rgb.y = (float_t)col.g;
        rgb.z = (float_t)col.b;
    }

    rgb = vec3::clamp(rgb * inv_scale * col_tone->blend + col_tone->offset, 0.0f, 1.0f);

    vec3 ycc;
    rgb_to_ycc(rgb, ycc);

    *(vec2*)&ycc.y = *(vec2*)&ycc.y * col_tone->saturation;
    ycc_apply_col_tone_hue(ycc, col_tone);
    ycc.x = (ycc.x + col_tone->value - 0.5f) * col_tone->contrast + 0.5f;
    ycc.x = clamp_def(ycc.x, 0.0f, 1.0f);
    *(vec2*)&ycc.y = vec2::clamp(*(vec2*)&ycc.y, -0.5f, 0.5f);

    ycc_to_rgb(ycc, rgb);

    rgb = vec3::clamp(rgb, 0.0f, 1.0f) * scale;

    col.r = (uint8_t)rgb.x;
    col.g = (uint8_t)rgb.y;
    col.b = (uint8_t)rgb.z;
    return col;
}

inline static void rgb_to_ycc(vec3& rgb, vec3& ycc) {
    const vec3 _y_coef_601 = { 0.29891f, 0.58661f, 0.11448f };
    const vec3 _cb_coef_601 = { -0.16874f, -0.33126f, 0.5f };
    const vec3 _cr_coef_601 = { 0.5f, -0.41869f, -0.08131f };

    ycc.x = vec3::dot(rgb, _y_coef_601);
    ycc.y = vec3::dot(rgb, _cb_coef_601);
    ycc.z = vec3::dot(rgb, _cr_coef_601);
}

inline static void ycc_apply_col_tone_hue(vec3& ycc, const color_tone* col_tone) {
    float_t hue = col_tone->hue * DEG_TO_RAD_FLOAT;
    float_t cos = cosf(hue);
    float_t sin = sinf(hue);
    ycc.y = ycc.y * cos + ycc.z * sin;
    ycc.z = ycc.z * cos - ycc.y * sin;
}

inline static void ycc_to_rgb(vec3& ycc, vec3& rgb) {
    const vec3 _red_coef_601 = { 1.0f, 0.0f, 1.402f };
    const vec3 _grn_coef_601 = { 1.0f, -0.34414f, -0.71414f };
    const vec3 _blu_coef_601 = { 1.0f, 1.772f, 0.0f };

    rgb.x = vec3::dot(ycc, _red_coef_601);
    rgb.y = vec3::dot(ycc, _grn_coef_601);
    rgb.z = vec3::dot(ycc, _blu_coef_601);
}