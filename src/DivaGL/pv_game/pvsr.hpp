/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../AFTModsShared/types.hpp"

enum pvsr_auth_3d_flags : uint8_t {
    PVSR_AUTH_3D_REPEAT         = 0x01,
    PVSR_AUTH_3D_MOVIE_TEXTURE  = 0x02,
    PVSR_AUTH_3D_RENDER_TEXTURE = 0x04,
    PVSR_AUTH_3D_MAIN           = 0x08,
};

enum pvsr_glitter_flags : uint8_t {
    PVSR_GLITTER_FORCE_DISP = 0x01,
};

struct pvsr_auth_2d {
    const char* name;
};

struct pvsr_auth_3d {
    const char* name;
    pvsr_auth_3d_flags flags;
};

struct pvsr_effect {
    const char* name;
    float_t emission;
};

struct pvsr_glitter {
    const char* name;
    int8_t fade_time;
    pvsr_glitter_flags flags;
};

struct pvsr_stage_change_effect {
    bool enable;
    int8_t bar_count;
    int8_t num_auth_3d;
    int8_t num_glitter;
    const pvsr_auth_3d* auth_3d_array;
    const pvsr_glitter* glitter_array;
};

struct pvsr_stage_effect {
    int8_t num_auth_3d;
    int8_t num_glitter;
    const pvsr_auth_3d* auth_3d_array;
    const pvsr_glitter* glitter_array;
};

struct pvsr_stage_effect_env {
    int8_t num_aet;
    pvsr_auth_2d* aet_array;
};

#define PVSR_STAGE_EFFECT_COUNT 0x10

struct pvsr {
    uint32_t signature;
    int8_t num_effect;
    int8_t num_stage_effect;
    int8_t num_stage_effect_env;
    const pvsr_effect* effect_array;
    const pvsr_stage_effect* stage_effect_array;
    const pvsr_stage_effect_env* stage_effect_env_array;
    const pvsr_stage_change_effect* stage_change_effect_array;

    static const pvsr* read(const void* data, size_t size);
};
