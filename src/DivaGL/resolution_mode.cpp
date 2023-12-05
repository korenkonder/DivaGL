/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "resolution_mode.hpp"

const resolution_table_struct resolution_table[] = {
    {  320,  240, 0, 0,   0,  320,  225 }, // QVGA
    {  640,  480, 0, 0,  60,  640,  360 }, // VGA
    {  800,  600, 0, 0,  75,  800,  450 }, // SVGA
    { 1024,  768, 0, 0,  96, 1024,  576 }, // XGA
    { 1280, 1024, 1, 0, 152, 1280,  720 }, // SXGA
    { 1400, 1050, 0, 0, 131, 1400,  788 }, // SXGA+
    { 1600, 1200, 0, 0, 150, 1600,  900 }, // UXGA
    {  800,  480, 2, 0,  15,  800,  450 }, // WVGA
    { 1024,  600, 2, 0,  12, 1024,  576 }, // WSVGA
    { 1280,  768, 2, 0,  24, 1280,  720 }, // WXGA
    { 1360,  768, 3, 0,   2, 1360,  765 }, // FWXGA
    { 1920, 1200, 2, 0,  60, 1920, 1200 }, // WUXGA
    { 2560, 1536, 2, 0,  48, 2560, 1440 }, // WQXGA
    { 1280,  720, 3, 0,   0, 1280,  720 }, // HD
    { 1920, 1080, 3, 0,   0, 1920, 1080 }, // FHD
    { 2560, 1440, 3, 0,   0, 2560, 1440 }, // QHD
    {  480,  272, 3, 0,   0,  480,  272 }, // WQVGA
    {  960,  544, 3, 0,   0,  960,  544 }, // qHD

    // MM+
    /*{  320,  240, 0, 0,   0,  320,  225 }, // QVGA
    {  640,  480, 0, 0,  60,  640,  360 }, // VGA
    {  800,  600, 0, 0,  75,  800,  450 }, // SVGA
    { 1024,  768, 0, 0,  96, 1024,  576 }, // XGA
    { 1280, 1024, 1, 0, 152, 1280,  720 }, // SXGA
    { 1400, 1050, 0, 0, 131, 1400,  788 }, // SXGA+
    { 1600, 1200, 0, 0, 150, 1600,  900 }, // UXGA
    {  800,  480, 2, 0,  15,  800,  450 }, // WVGA
    { 1024,  600, 2, 0,  12, 1024,  576 }, // WSVGA
    { 1280,  768, 2, 0,  24, 1280,  720 }, // WXGA
    { 1360,  768, 3, 0,   2, 1360,  765 }, // FWXGA
    { 1920, 1200, 2, 0,  60, 1920, 1200 }, // WUXGA
    { 2560, 1536, 2, 0,  48, 2560, 1440 }, // WQXGA
    { 1280,  720, 3, 0,   0, 1280,  720 }, // HD
    { 1920, 1080, 3, 0,   0, 1920, 1080 }, // FHD
    { 3840, 2160, 3, 0,   0, 3840, 2160 }, // UHD
    { 3840, 2160, 3, 0,   0, 2880, 1620 }, // 3K at UHD
    { 2880, 1620, 3, 0,   0, 2880, 1620 }, // 3K
    { 2560, 1440, 3, 0,   0, 2560, 1440 }, // QHD
    {  480,  272, 3, 0,   0,  480,  272 }, // WQVGA
    {  960,  544, 3, 0,   0,  960,  544 }, // qHD
    { 1152,  864, 0, 0,   0, 1152,  864 }, // XGA+
    { 1176,  664, 3, 0,   0, 1176,  664 }, // 1176x664
    { 1200,  960, 1, 0,   0, 1200,  960 }, // 1200x960
    { 1280,  800, 2, 0,   0, 1280,  800 }, // WXGA1280x900
    { 1280,  960, 0, 0,   0, 1280,  960 }, // SXGA-
    { 1366,  768, 3, 0,   0, 1366,  768 }, // FWXGA1366x766
    { 1440,  900, 2, 0,   0, 1440,  900 }, // WXGA+
    { 1600,  900, 0, 0,   0, 1600,  900 }, // HD+
    { 1600, 1024, 2, 0,   0, 1600, 1024 }, // WSXGA
    { 1680, 1050, 2, 0,   0, 1680, 1050 }, // WSXGA+
    { 1920, 1440, 0, 0,   0, 1920, 1440 }, // 1920x1440
    { 2048, 1152, 0, 0,   0, 2048, 1152 }, // QWXGA*/
};

resolution_struct* res_window = (resolution_struct*)0x0000000140EDA8B0;
resolution_struct* res_window_internal = (resolution_struct*)0x0000000140EDA8D8;

resolution_struct::resolution_struct(::resolution_mode mode) {
    resolution_mode = mode;
    x_offset = 0;
    y_offset = 0;
    if (mode == RESOLUTION_MODE_MAX) {
        width = res_window->width;
        height = res_window->height;
        aspect = res_window->aspect;
        field_20 = res_window->field_20;
    }
    else {
        width = resolution_table[mode].width_full;
        height = resolution_table[mode].height_full;
        aspect = (double_t)width / (double_t)height;
        field_20 = resolution_table[mode].field_8;
    }
}

resolution_mode_scale_data::resolution_mode_scale_data(
    resolution_mode src_mode, resolution_mode dst_mode) {
    scale = 1.0f;
    src_res = 0.0f;
    dst_res = 0.0f;
    this->src_mode = src_mode;
    this->dst_mode = dst_mode;
    scale = resolution_mode_get_scale(dst_mode, src_mode);
    if (dst_mode != RESOLUTION_MODE_MAX && src_mode != RESOLUTION_MODE_MAX
        && dst_mode >= RESOLUTION_MODE_QVGA && src_mode >= RESOLUTION_MODE_MAX
        && dst_mode < RESOLUTION_MODE_MAX && src_mode < RESOLUTION_MODE_MAX) {
        src_res.x = (float_t)resolution_table[src_mode].width_full * 0.5f;
        src_res.y = (float_t)resolution_table[src_mode].height_full * 0.5f;
        dst_res.x = (float_t)resolution_table[dst_mode].width_full * 0.5f;
        dst_res.y = (float_t)resolution_table[dst_mode].height_full * 0.5f;
    }
}

resolution_struct* res_window_get() {
    return res_window;
}

resolution_struct* res_window_internal_get() {
    return res_window_internal;
}

void res_window_set(resolution_mode mode) {
    *res_window = resolution_struct(mode);

    int32_t width = resolution_table[mode].width;
    int32_t height = resolution_table[mode].height;

    res_window_internal->resolution_mode = RESOLUTION_MODE_MAX;
    res_window_internal->x_offset = resolution_table[mode].x_offset;
    res_window_internal->y_offset = resolution_table[mode].y_offset;
    res_window_internal->width = width;
    res_window_internal->height = height;
    res_window_internal->aspect = (double_t)width / (double_t)height;
    res_window_internal->field_20 = 4;

    for (int32_t i = 0; i < RESOLUTION_MODE_MAX; i++)
        if (resolution_table[i].width_full == width && resolution_table[i].height_full == height) {
            res_window_internal->resolution_mode = (resolution_mode)i;
            res_window_internal->field_20 = resolution_table[i].field_8;
        }
}

vec2 resolution_mode_get_scale(const resolution_mode dst_mode, const resolution_mode src_mode) {
    if (dst_mode == RESOLUTION_MODE_MAX || src_mode == RESOLUTION_MODE_MAX
        || dst_mode < RESOLUTION_MODE_QVGA || src_mode < RESOLUTION_MODE_QVGA
        || dst_mode >= RESOLUTION_MODE_MAX || src_mode >= RESOLUTION_MODE_MAX)
        return 1.0f;

    float_t scale_x = (float_t)resolution_table[dst_mode].width_full / (float_t)resolution_table[src_mode].width_full;
    float_t scale_y = (float_t)resolution_table[dst_mode].height_full / (float_t)resolution_table[src_mode].height_full;
    int32_t v9 = resolution_table[src_mode].field_8;
    int32_t v10 = resolution_table[dst_mode].field_8;
    if (v9 != v10) {
        if (!v9 && 10 == 1)
            return scale_x;

        if (v9 == 2 || v9 == 3)
            switch (v10) {
            case 0:
            case 1:
            case 2:
                return scale_x;
            }
    }

    if (src_mode == RESOLUTION_MODE_qHD && dst_mode != RESOLUTION_MODE_qHD && v10 == 3)
        return scale_x;
    else
        return scale_y;
}

void resolution_mode_scale_pos(vec2& dst_pos, const resolution_mode dst_mode,
    const vec2& src_pos, const resolution_mode src_mode) {
    if (dst_mode == RESOLUTION_MODE_MAX || src_mode == RESOLUTION_MODE_MAX
        || dst_mode < RESOLUTION_MODE_QVGA || src_mode < RESOLUTION_MODE_QVGA
        || dst_mode >= RESOLUTION_MODE_MAX || src_mode >= RESOLUTION_MODE_MAX) {
        dst_pos = src_pos;
        return;
    }

    const resolution_table_struct* src = &resolution_table[src_mode];
    const resolution_table_struct* dst = &resolution_table[dst_mode];

    vec2 src_size((float_t)src->width_full, (float_t)src->height_full);
    vec2 dst_size((float_t)dst->width_full, (float_t)dst->height_full);
    vec2 scale = resolution_mode_get_scale(dst_mode, src_mode);
    dst_pos = dst_size * 0.5f + (src_pos - src_size * 0.5f) * scale;
}