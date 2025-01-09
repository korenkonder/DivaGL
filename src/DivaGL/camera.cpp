/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "camera.hpp"
#include "render.hpp"
#include "resolution_mode.hpp"
#include <Helpers.h>

camera_struct& camera_data = *(camera_struct*)0x0000000140FBC2C0;

static void (FASTCALL* camera_struct__update_view)(camera_struct& cam)
    = (void (FASTCALL*)(camera_struct & cam))0x00000001401F7690;

HOOK(void, FASTCALL, camera_data_update_projection, 0x00000001401F8E90) {
    resolution_struct* res_wind = res_window_get();
    resolution_struct* res_wind_int = res_window_internal_get();

    float_t sprite_half_width = (float_t)res_wind->width * 0.5f;
    float_t sprite_half_height = (float_t)res_wind->height * 0.5f;
    float_t render_half_width = (float_t)res_wind_int->width * 0.5f;
    float_t render_half_height = (float_t)res_wind_int->height * 0.5f;

    float_t aet_depth = camera_data.aet_depth;

    float_t spr_2d_range = camera_data.min_distance / aet_depth;
    float_t spr_2d_range_x = spr_2d_range * sprite_half_width;
    float_t spr_2d_range_y = spr_2d_range * sprite_half_height;

    mat4 spr_2d_proj;
    mat4_frustrum(-spr_2d_range_x, spr_2d_range_x, spr_2d_range_y, -spr_2d_range_y,
        camera_data.min_distance, 3000.0f, &spr_2d_proj);

    vec3 spr_2d_viewpoint = { sprite_half_width, sprite_half_height, aet_depth };
    vec3 spr_2d_interest = { sprite_half_width, sprite_half_height, 0.0f };
    vec3 spr_2d_up = { 0.0f, 1.0f, 0.0f };
    mat4 spr_2d_view;
    mat4_look_at(&spr_2d_viewpoint, &spr_2d_interest, &spr_2d_up, &spr_2d_view);

    mat4_mul(&spr_2d_view, &spr_2d_proj, &spr_2d_proj);
    mat4_transpose(&spr_2d_proj, &camera_data.view_projection_aet_2d);

    float_t aet_3d_depth = render_half_height / tanf(camera_data.aet_fov * 0.5f * DEG_TO_RAD_FLOAT);

    float_t spr_3d_range = camera_data.min_distance / aet_3d_depth;
    float_t spr_3d_range_x = spr_3d_range * render_half_width;
    float_t spr_3d_range_y = spr_3d_range * render_half_height;

    mat4 spr_3d_proj;
    mat4_frustrum(-spr_3d_range_x, spr_3d_range_x, spr_3d_range_y, -spr_3d_range_y,
        camera_data.min_distance, 3000.0f, &spr_3d_proj);

    vec3 spr_3d_viewpoint = { render_half_width, render_half_height, aet_3d_depth };
    vec3 spr_3d_interest = { render_half_width, render_half_height, 0.0f };
    vec3 spr_3d_up = { 0.0f, 1.0f, 0.0f };
    mat4 spr_3d_view;
    mat4_look_at(&spr_3d_viewpoint, &spr_3d_interest, &spr_3d_up, &spr_3d_view);

    mat4_mul(&spr_3d_view, &spr_3d_proj, &spr_3d_proj);
    mat4_transpose(&spr_3d_proj, &camera_data.view_projection_aet_3d);

    render_get()->calc_projection_matrix(&camera_data.projection, camera_data.fov,
        (float_t)camera_data.aspect, camera_data.min_distance, camera_data.max_distance,
        camera_data.proj_left_offset, camera_data.proj_right_offset,
        camera_data.proj_bottom_offset, camera_data.proj_top_offset);
    camera_data.fov_horizontal_rad = tanf(camera_data.fov * 0.5f * DEG_TO_RAD_FLOAT);
}

HOOK(void, FASTCALL, camera_update, 0x00000001401F8970) {
    implOfcamera_data_update_projection();
    camera_struct__update_view(camera_data);
    camera_data.fast_change_hist1 = camera_data.fast_change_hist0;
    camera_data.fast_change_hist0 = camera_data.fast_change;
    camera_data.fast_change = false;
}

void camera_struct::get_view_point(vec3& value) {
    value = view_point;
}

void camera_struct::get_view_point(vec4& value) {
    *(vec3*)&value = view_point;
    value.w = 0.0f;
}

void camera_patch() {
    INSTALL_HOOK(camera_data_update_projection);
    INSTALL_HOOK(camera_update);
}
