/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"

enum resolution_mode {
    RESOLUTION_MODE_QVGA     = 0x00,
    RESOLUTION_MODE_VGA      = 0x01,
    RESOLUTION_MODE_SVGA     = 0x02,
    RESOLUTION_MODE_XGA      = 0x03,
    RESOLUTION_MODE_SXGA     = 0x04,
    RESOLUTION_MODE_SXGAPlus = 0x05,
    RESOLUTION_MODE_UXGA     = 0x06,
    RESOLUTION_MODE_WVGA     = 0x07,
    RESOLUTION_MODE_WSVGA    = 0x08,
    RESOLUTION_MODE_WXGA     = 0x09,
    RESOLUTION_MODE_FWXGA    = 0x0A,
    RESOLUTION_MODE_WUXGA    = 0x0B,
    RESOLUTION_MODE_WQXGA    = 0x0C,
    RESOLUTION_MODE_HD       = 0x0D,
    RESOLUTION_MODE_FHD      = 0x0E,
    RESOLUTION_MODE_QHD      = 0x0F,
    RESOLUTION_MODE_WQVGA    = 0x10,
    RESOLUTION_MODE_qHD      = 0x11,
    RESOLUTION_MODE_MAX      = 0x12,

    // MM+
    /*RESOLUTION_MODE_QVGA          = 0x00,
    RESOLUTION_MODE_VGA           = 0x01,
    RESOLUTION_MODE_SVGA          = 0x02,
    RESOLUTION_MODE_XGA           = 0x03,
    RESOLUTION_MODE_SXGA          = 0x04,
    RESOLUTION_MODE_SXGAPlus      = 0x05,
    RESOLUTION_MODE_UXGA          = 0x06,
    RESOLUTION_MODE_WVGA          = 0x07,
    RESOLUTION_MODE_WSVGA         = 0x08,
    RESOLUTION_MODE_WXGA          = 0x09,
    RESOLUTION_MODE_FWXGA         = 0x0A,
    RESOLUTION_MODE_WUXGA         = 0x0B,
    RESOLUTION_MODE_WQXGA         = 0x0C,
    RESOLUTION_MODE_HD            = 0x0D,
    RESOLUTION_MODE_FHD           = 0x0E,
    RESOLUTION_MODE_UHD           = 0x0F,
    RESOLUTION_MODE_3KatUHD       = 0x10,
    RESOLUTION_MODE_3K            = 0x11,
    RESOLUTION_MODE_QHD           = 0x12,
    RESOLUTION_MODE_WQVGA         = 0x13,
    RESOLUTION_MODE_qHD           = 0x14,
    RESOLUTION_MODE_XGAPlus       = 0x15,
    RESOLUTION_MODE_1176x664      = 0x16,
    RESOLUTION_MODE_1200x960      = 0x17,
    RESOLUTION_MODE_WXGA1280x900  = 0x18,
    RESOLUTION_MODE_SXGAMinus     = 0x19,
    RESOLUTION_MODE_FWXGA1366x768 = 0x1A,
    RESOLUTION_MODE_WXGAPlus      = 0x1B,
    RESOLUTION_MODE_HDPlus        = 0x1C,
    RESOLUTION_MODE_WSXGA         = 0x1D,
    RESOLUTION_MODE_WSXGAPlus     = 0x1E,
    RESOLUTION_MODE_1920x1440     = 0x1F,
    RESOLUTION_MODE_QWXGA         = 0x20,
    RESOLUTION_MODE_MAX           = 0x21,*/
};

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
