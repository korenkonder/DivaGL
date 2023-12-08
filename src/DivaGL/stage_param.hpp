/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"
#include "object.hpp"
#include "types.hpp"

/*struct stage_param_data_coli {
    int32_t pv_id;
    prj::map<int32_t, osage_ring_data> stage_ring_data;
    prj::map<int32_t, osage_ring_data> mhd_id_ring_data;
    prj::map<int32_t, prj::string> stage_wind_file;
};

static_assert(sizeof(stage_param_data_coli) == 0x38, "\"stage_param_data_coli\" struct should have a size of 0x38");*/

struct stage_param_data_fog_ring {
    int32_t num_ptcls;
    float_t ring_size;
    vec3 wind_dir;
    int32_t tex_id;
    vec4 color;
    float_t ptcl_size;
    float_t density;
    float_t density_offset;
};

static_assert(sizeof(stage_param_data_fog_ring) == 0x34, "\"stage_param_data_fog_ring\" struct should have a size of 0x34");

struct stage_param_data_leaf_lie_plane_xz {
    float_t min_x;
    float_t max_x;
    float_t min_z;
    float_t max_z;
};

static_assert(sizeof(stage_param_data_leaf_lie_plane_xz) == 0x10, "\"stage_param_data_fog_ring\" struct should have a size of 0x10");

struct stage_param_data_leaf {
    int32_t tex_id;
    vec4 color;
    float_t psize;
    int32_t num_initial_ptcls;
    float_t frame_speed_coef;
    float_t emit_interval;
    vec3 wind;
    vec3 range;
    vec3 offset;
    stage_param_data_leaf_lie_plane_xz lie_plane_xz;
    bool split_tex;
};

static_assert(sizeof(stage_param_data_leaf) == 0x5C, "\"stage_param_data_leaf\" struct should have a size of 0x5C");

struct stage_param_data_litproj {
    int32_t tex_id;
};

static_assert(sizeof(stage_param_data_litproj) == 0x04, "\"stage_param_data_litproj\" struct should have a size of 0x04");

struct stage_param_data_rain {
    int32_t tex_id;
    int32_t num_rain;
    vec4 color;
    vec3 velocity;
    vec3 vel_range;
    vec2 psize;
    vec3 range;
    vec3 offset;
};

static_assert(sizeof(stage_param_data_rain) == 0x50, "\"stage_param_data_rain\" struct should have a size of 0x50");

struct stage_param_data_ripple {
    size_t rain_ripple_num;
    float_t rain_ripple_min_value;
    float_t rain_ripple_max_value;
    float_t ground_y;
    float_t emit_pos_scale;
    float_t emit_pos_ofs_x;
    float_t emit_pos_ofs_z;
    float_t wake_attn;
    float_t speed;
    int32_t ripple_tex_id;
    bool use_float_ripplemap;
    float_t rob_emitter_size;
    vec3* emitter_list;
    size_t emitter_num;
    float_t emitter_size;
};

static_assert(sizeof(stage_param_data_ripple) == 0x50, "\"stage_param_data_ripple\" struct should have a size of 0x50");

struct stage_param_data_snow_colli_ground {
    float_t min_x;
    float_t max_x;
    float_t min_z;
    float_t max_z;
    float_t y;
};

static_assert(sizeof(stage_param_data_snow_colli_ground) == 0x14, "\"stage_param_data_snow_colli_ground\" struct should have a size of 0x14");

struct stage_param_data_snow {
    int32_t tex_id;
    int32_t num_snow;
    int32_t num_snow_gpu;
    vec4 color;
    vec3 velocity;
    vec3 vel_range;
    vec3 range;
    vec3 range_gpu;
    vec3 offset;
    vec3 offset_gpu;
    stage_param_data_snow_colli_ground colli_ground;
};

static_assert(sizeof(stage_param_data_snow) == 0x78, "\"stage_param_data_snow\" struct should have a size of 0x78");

struct stage_param_data_splash {
    int32_t splash_tex_id;
    object_info splash_obj_id;
    float_t particle_size;
    int32_t emit_num;
    float_t emission_ratio_attn;
    float_t emission_velocity_scale;
    float_t ripple_emission;
    vec4 color;
    bool in_water;
    bool blink;
};

static_assert(sizeof(stage_param_data_splash) == 0x30, "\"stage_param_data_splash\" struct should have a size of 0x30");

struct stage_param_data_star_utc {
    int32_t year;
    int32_t month;
    int32_t day;
    int32_t hour;
    int32_t minute;
    float_t second;

    double_t to_julian_date() const;
};

static_assert(sizeof(stage_param_data_star_utc) == 0x18, "\"stage_param_data_star_utc\" struct should have a size of 0x18");

struct stage_param_data_star_modifiers {
    float_t color_scale;
    float_t pos_scale;
    float_t offset_scale;
    float_t threshold;
    float_t size_max;
};

static_assert(sizeof(stage_param_data_star_modifiers) == 0x14, "\"stage_param_data_star_modifiers\" struct should have a size of 0x14");

struct stage_param_data_star {
    int32_t tex_id;
    stage_param_data_star_utc utc;
    float_t rotation_y_deg;
    float_t observer_north_latitude_deg;
    float_t observer_east_longitude_deg;
    stage_param_data_star_modifiers modifiers[2];

    static void get_mat(mat4& mat, const float_t observer_north_latitude,
        const float_t observer_east_longitude, const stage_param_data_star_utc& utc, const float_t rotation_y);
};

static_assert(sizeof(stage_param_data_star) == 0x50, "\"stage_param_data_star\" struct should have a size of 0x50");

extern stage_param_data_litproj*& stage_param_data_litproj_current;
