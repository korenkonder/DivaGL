/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "stage_param.hpp"

stage_param_data_litproj*& stage_param_data_litproj_current = *(stage_param_data_litproj**)0x0000000141194D80;

double_t stage_param_data_star_utc::to_julian_date() const {
    int64_t year = this->year;
    int64_t month = this->month;
    if (month == 1 || month == 2) {
        year--;
        month += 12;
    }

    return (double_t)(day + year / 400 + (int64_t)((double_t)year * 365.25)
        - (int32_t)((double_t)(month - 2) * -30.59) - year / 100)
        + 1721088.5
        + (double_t)hour * (1.0 / 24.0)
        + (double_t)minute * (1.0 / 1440.0)
        + (double_t)second * (1.0 / 86400.0);
}

void stage_param_data_star::get_mat(mat4& mat, const float_t observer_north_latitude,
    const float_t observer_east_longitude, const stage_param_data_star_utc& utc, const float_t rotation_y) {
    double_t v7 = (utc.to_julian_date() - 2440000.5) * 1.0027379094 + 0.671262;

    mat3 rot_y;
    mat3_rotate_y((float_t)(v7 - (double_t)(int64_t)v7) * (float_t)(M_PI * 2.0) + observer_east_longitude, &rot_y);

    float_t latitude_sin = sinf(observer_north_latitude);
    float_t latitude_cos = cosf(observer_north_latitude) * 0.99664718f;

    vec3 x_axis = { 0.0f, latitude_cos, -latitude_sin };
    mat3_transform_vector(&rot_y, &x_axis, &x_axis);

    vec3 y_axis = { 0.0f, latitude_sin, latitude_cos };
    mat3_transform_vector(&rot_y, &y_axis, &y_axis);

    vec3 z_axis = { 1.0f, 0.0f, 0.0f };
    mat3_transform_vector(&rot_y, &z_axis, &z_axis);

    x_axis = vec3::normalize(x_axis);
    y_axis = vec3::normalize(y_axis);

    float_t rot_y_sin = sinf(rotation_y);
    float_t rot_y_cos = cosf(rotation_y);

    mat4 temp = mat4_identity;
    *(vec3*)&temp.row0 = x_axis * -rot_y_sin + z_axis * rot_y_cos;
    *(vec3*)&temp.row1 = y_axis;
    *(vec3*)&temp.row2 = x_axis * -rot_y_cos + z_axis * -rot_y_sin;
    mat4_transpose(&temp, &mat);
}
