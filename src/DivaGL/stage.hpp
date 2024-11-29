/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"

enum reflect_refract_resolution_mode {
    REFLECT_REFRACT_RESOLUTION_256x256 = 0,
    REFLECT_REFRACT_RESOLUTION_512x256,
    REFLECT_REFRACT_RESOLUTION_512x512,
    REFLECT_REFRACT_RESOLUTION_MAX,
};

enum stage_data_reflect_type {
    STAGE_DATA_REFLECT_DISABLE     = 0x00,
    STAGE_DATA_REFLECT_NORMAL      = 0x01,
    STAGE_DATA_REFLECT_REFLECT_MAP = 0x02,
};

enum stage_data_blur_filter_mode {
    STAGE_DATA_BLUR_FILTER_4   = 0x00,
    STAGE_DATA_BLUR_FILTER_9   = 0x01,
    STAGE_DATA_BLUR_FILTER_16  = 0x02,
    STAGE_DATA_BLUR_FILTER_32  = 0x03,
    STAGE_DATA_BLUR_FILTER_MAX = 0x03,
};

struct stage_data_reflect {
    reflect_refract_resolution_mode mode;
    int32_t blur_num;
    stage_data_blur_filter_mode blur_filter;
};

extern size_t(FASTCALL*stage_data_handler_get_stage_data_count)();

extern void stage_patch();
