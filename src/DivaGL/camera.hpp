/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"

struct camera_struct {
    vec3 view_point;
    vec3 interest;
    float_t roll;
    float_t fov;
    float_t aet_fov;
    int32_t field_24;
    double_t aspect;
    float_t min_distance;
    float_t max_distance;
    float_t proj_left_offset;
    float_t proj_right_offset;
    float_t proj_bottom_offset;
    float_t proj_top_offset;
    bool use_up;
    char field_49;
    char field_4A;
    char field_4B;
    vec3 up;
    bool ignore_fov;
    bool ignore_min_dist;
    char field_5A;
    char field_5B;
    mat4 view;
    mat4 inv_view;
    mat4 projection;
    mat4 view_projection;
    mat4 view_projection_aet_2d;
    mat4 view_projection_aet_3d;
    float_t depth;
    float_t aet_depth;
    vec3 field_1E4;
    vec3 field_1F0;
    vec3 field_1FC;
    vec3 field_208;
    float_t distance;
    vec3 rotation;
    float_t fov_horizontal_rad;
    char field_228;
    bool fast_change;
    bool fast_change_hist0;
    bool fast_change_hist1;
    int32_t field_22C;

    void get_view_point(vec3& value);
    void get_view_point(vec4& value);
};

extern camera_struct& camera_data;

extern void camera_patch();
