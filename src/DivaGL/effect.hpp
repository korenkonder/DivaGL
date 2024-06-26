/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"

enum EffectType {
    EFFECT_INVALID  = -1,
    EFFECT_AUTH_3D  = 0x00,
    EFFECT_TYPE_1   = 0x01,
    EFFECT_LEAF     = 0x02,
    EFFECT_TYPE_3   = 0x03,
    EFFECT_SNOW     = 0x04,
    EFFECT_TYPE_5   = 0x05,
    EFFECT_RIPPLE   = 0x06,
    EFFECT_RAIN     = 0x07,
    EFFECT_TYPE_8   = 0x08,
    EFFECT_TYPE_9   = 0x09,
    EFFECT_TYPE_10  = 0x0A,
    EFFECT_TYPE_11  = 0x0B,
    EFFECT_SPLASH   = 0x0C,
    EFFECT_TYPE_13  = 0x0D,
    EFFECT_FOG_ANIM = 0x0E,
    EFFECT_TYPE_15  = 0x0F,
    EFFECT_FOG_RING = 0x10,
    EFFECT_TYPE_17  = 0x11,
    EFFECT_PARTICLE = 0x12,
    EFFECT_LITPROJ  = 0x13,
    EFFECT_STAR     = 0x14,
    EFFECT_MAX      = 0x15,
};

struct particle_init_data {
    float_t field_0;
    float_t field_4;
    float_t field_8;
    vec3 trans;
    float_t scale_y;
};

extern void leaf_particle_draw();
extern void rain_particle_draw();
extern void particle_draw();
extern void snow_particle_draw();
extern void star_catalog_draw();

extern void effect_patch();
