/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/spr.hpp"
#include "../KKdLib/vec.hpp"

struct resolution_table_struct {
    int32_t width_full;
    int32_t height_full;
    int32_t field_8;
    int32_t x_offset;
    int32_t y_offset;
    int32_t width;
    int32_t height;
};

struct resolution_struct {
    resolution_mode resolution_mode;
    int32_t x_offset;
    int32_t y_offset;
    int32_t width;
    int32_t height;
    double_t aspect;
    int32_t field_20;

    inline resolution_struct() : resolution_struct(RESOLUTION_MODE_MAX) {

    }

    resolution_struct(::resolution_mode mode);
};

struct resolution_mode_scale_data {
    vec2 scale;
    vec2 src_res;
    vec2 dst_res;
    resolution_mode src_mode;
    resolution_mode dst_mode;

    resolution_mode_scale_data(resolution_mode src_mode, resolution_mode dst_mode);
};

extern const resolution_table_struct resolution_table[];

extern resolution_struct* res_window_get();
extern resolution_struct* res_window_internal_get();
extern void res_window_set(resolution_mode mode);
extern vec2 resolution_mode_get_scale(const resolution_mode dst_mode, const resolution_mode src_mode);
extern void resolution_mode_scale_pos(vec2& dst_pos, const resolution_mode dst_mode,
    const vec2& src_pos, const resolution_mode src_mode);
