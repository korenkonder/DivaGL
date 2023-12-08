/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "gl_shader_storage_buffer.hpp"
#include "gl_uniform_buffer.hpp"
#include "rob/rob.hpp"
#include "render_texture.hpp"
#include "stage.hpp"
#include "stage_param.hpp"

enum TaskEffectType {
    TASK_EFFECT_INVALID  = -1,
    TASK_EFFECT_AUTH_3D  = 0x00,
    TASK_EFFECT_TYPE_1   = 0x01,
    TASK_EFFECT_LEAF     = 0x02,
    TASK_EFFECT_TYPE_3   = 0x03,
    TASK_EFFECT_SNOW     = 0x04,
    TASK_EFFECT_TYPE_5   = 0x05,
    TASK_EFFECT_RIPPLE   = 0x06,
    TASK_EFFECT_RAIN     = 0x07,
    TASK_EFFECT_TYPE_8   = 0x08,
    TASK_EFFECT_TYPE_9   = 0x09,
    TASK_EFFECT_TYPE_10  = 0x0A,
    TASK_EFFECT_TYPE_11  = 0x0B,
    TASK_EFFECT_SPLASH   = 0x0C,
    TASK_EFFECT_TYPE_13  = 0x0D,
    TASK_EFFECT_FOG_ANIM = 0x0E,
    TASK_EFFECT_TYPE_15  = 0x0F,
    TASK_EFFECT_FOG_RING = 0x10,
    TASK_EFFECT_TYPE_17  = 0x11,
    TASK_EFFECT_PARTICLE = 0x12,
    TASK_EFFECT_LITPROJ  = 0x13,
    TASK_EFFECT_STAR     = 0x14,
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

extern void task_effect_patch();
