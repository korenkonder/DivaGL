/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "camera.hpp"
#include "resolution_mode.hpp"

camera_struct& camera_data = *(camera_struct*)0x0000000140FBC2C0;

void camera_struct::get_view_point(vec3& value) {
    value = view_point;
}

void camera_struct::get_view_point(vec4& value) {
    *(vec3*)&value = view_point;
    value.w = 0.0f;
}
