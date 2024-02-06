/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"

enum uniform_name {
    U_NONE          = 0x00,
    U_ALPHA_MASK    = 0x01,
    U_ALPHA_TEST    = 0x02,
    U_ANISO         = 0x03,
    U_AET_BACK      = 0x04,
    U_TEXTURE_BLEND = 0x05,
    U_CHARA_COLOR   = 0x06,
    U_CLIP_PLANE    = 0x07,
    U08             = 0x08,
    U_DEPTH_PEEL    = 0x09,
    U_DEPTH         = 0x0A,
    U0B             = 0x0B,
    U_ALPHA_BLEND   = 0x0C,
    U_RIPPLE_EMIT   = 0x0D,
    U_ESM_FILTER    = 0x0E,
    U_EXPOSURE      = 0x0F,
    U_SCENE_FADE    = 0x10,
    U_FADE          = 0x11,
    U12             = 0x12,
    U_FLARE         = 0x13,
    U_FOG_HEIGHT    = 0x14,
    U_FOG           = 0x15,
    U16             = 0x16,
    U_GAUSS         = 0x17,
    U18             = 0x18,
    U_IMAGE_FILTER  = 0x19,
    U_INSTANCE      = 0x1A,
    U_TONE_CURVE    = 0x1B,
    U_LIGHT_PROJ    = 0x1C,
    U_MAGNIFY       = 0x1D,
    U_MEMBRANE      = 0x1E,
    U_MLAA          = 0x1F,
    U_MLAA_SEARCH   = 0x20,
    U_MORPH_COLOR   = 0x21,
    U_MORPH         = 0x22,
    U_MOVIE         = 0x23,
    U24             = 0x24,
    U25             = 0x25,
    U26             = 0x26,
    U_NPR           = 0x27,
    U_LIGHT_1       = 0x28,
    U_REFLECT       = 0x29,
    U_REDUCE        = 0x2A,
    U_SELF_SHADOW   = 0x2B,
    U_SHADOW        = 0x2C,
    U2D             = 0x2D,
    U2E             = 0x2E,
    U_SHOW_VECTOR   = 0x2F,
    U_SKINNING      = 0x30,
    U_SNOW_PARTICLE = 0x31,
    U_SPECULAR_IBL  = 0x32,
    U_COMBINER      = 0x33,
    U_TEX_0_TYPE    = 0x34,
    U_TEX_1_TYPE    = 0x35,
    U_SSS_FILTER    = 0x36,
    U37             = 0x37,
    U_STAR          = 0x38,
    U_TEXTURE_COUNT = 0x39,
    U_ENV_MAP       = 0x3A,
    U_RIPPLE        = 0x3B,
    U_TRANSLUCENCY  = 0x3C,
    U_NORMAL        = 0x3D,
    U_TRANSPARENCY  = 0x3E,
    U_WATER_REFLECT = 0x3F,
    U40             = 0x40,
    U41             = 0x41,
    U_LIGHT_0       = 0x42,
    U_SPECULAR      = 0x43,
    U_TONE_MAP      = 0x44,
    U45             = 0x45,
    U_MAX           = 0x46,
    U_INVALID       = 0xFFFFFFFF,
};

struct uniform_value {
    int32_t arr[U_MAX];
};

extern uniform_value* uniform;

extern void(FASTCALL* uniform_value_reset)();
