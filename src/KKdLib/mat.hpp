/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"
#include "vec.hpp"
#include "quat.hpp"

struct mat3 {
    vec3 row0;
    vec3 row1;
    vec3 row2;

    inline mat3() : row0(), row1(), row2() {

    }

    inline mat3(vec3 row0, vec3 row1, vec3 row2) :
        row0(row0), row1(row1), row2(row2) {

    }
};

struct mat4 {
    vec4 row0;
    vec4 row1;
    vec4 row2;
    vec4 row3;

    inline mat4() : row0(), row1(), row2(), row3() {

    }

    inline mat4(vec4 row0, vec4 row1, vec4 row2, vec4 row3) :
        row0(row0), row1(row1), row2(row2), row3(row3) {

    }
};

extern const mat3 mat3_identity;
extern const mat3 mat3_null;
extern const mat4 mat4_identity;
extern const mat4 mat4_null;

extern void mat3_set(const quat* in_q1, mat3* out_m);
extern void mat3_set(const vec3* in_axis, const float_t in_angle, mat3* out_m);
extern void mat3_set(const vec3* in_axis, const float_t s, const float_t c, mat3* out_m);
extern void mat3_add(const mat3* in_m1, const float_t value, mat3* out_m);
extern void mat3_add(const mat3* in_m1, const mat3* in_m2, mat3* out_m);
extern void mat3_sub(const mat3* in_m1, const float_t value, mat3* out_m);
extern void mat3_sub(const mat3* in_m1, const mat3* in_m2, mat3* out_m);
extern void mat3_mul(const mat3* in_m1, const float_t value, mat3* out_m);
extern void mat3_mul(const mat3* in_m1, const mat3* in_m2, mat3* out_m);
extern void mat3_mul(const mat3* in_m1, const vec3* in_axis, const float_t in_angle, mat3* out_m);
extern void mat3_transform_vector(const mat3* in_m1, const vec2* normal, vec2* normalOut);
extern void mat3_transform_vector(const mat3* in_m1, const vec3* normal, vec3* normalOut);
extern void mat3_inverse_transform_vector(const mat3* in_m1, const vec2* normal, vec2* normalOut);
extern void mat3_inverse_transform_vector(const mat3* in_m1, const vec3* normal, vec3* normalOut);
extern void mat3_transpose(const mat3* in_m1, mat3* out_m);
extern void mat3_invert(const mat3* in_m1, mat3* out_m);
extern void mat3_invert_fast(const mat3* in_m1, mat3* out_m);
extern void mat3_normalize(const mat3* in_m1, mat3* out_m);
extern void mat3_normalize_rotation(const mat3* in_m1, mat3* out_m);
extern float_t mat3_determinant(const mat3* in_m1);
extern void mat3_rotate_x(float_t rad, mat3* out_m);
extern void mat3_rotate_y(float_t rad, mat3* out_m);
extern void mat3_rotate_z(float_t rad, mat3* out_m);
extern void mat3_rotate_x(float_t s, float_t c, mat3* out_m);
extern void mat3_rotate_y(float_t s, float_t c, mat3* out_m);
extern void mat3_rotate_z(float_t s, float_t c, mat3* out_m);
extern void mat3_rotate_xyz(float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_rotate_xzy(float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_rotate_yxz(float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_rotate_yzx(float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_rotate_zxy(float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_rotate_zyx(float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_mul_rotate_x(const mat3* in_m1, float_t rad, mat3* out_m);
extern void mat3_mul_rotate_y(const mat3* in_m1, float_t rad, mat3* out_m);
extern void mat3_mul_rotate_z(const mat3* in_m1, float_t rad, mat3* out_m);
extern void mat3_mul_rotate_x(const mat3* in_m1, float_t s, float_t c, mat3* out_m);
extern void mat3_mul_rotate_y(const mat3* in_m1, float_t s, float_t c, mat3* out_m);
extern void mat3_mul_rotate_z(const mat3* in_m1, float_t s, float_t c, mat3* out_m);
extern void mat3_mul_rotate_xyz(const mat3* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_mul_rotate_xzy(const mat3* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_mul_rotate_yxz(const mat3* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_mul_rotate_yzx(const mat3* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_mul_rotate_zxy(const mat3* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_mul_rotate_zyx(const mat3* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat3* out_m);
extern void mat3_scale(float_t sx, float_t sy, float_t sz, mat3* out_m);
extern void mat3_scale_x(float_t s, mat3* out_m);
extern void mat3_scale_y(float_t s, mat3* out_m);
extern void mat3_scale_z(float_t s, mat3* out_m);
extern void mat3_mul_scale(const mat3* in_m1, float_t sx, float_t sy, float_t sz, mat3* out_m);
extern void mat3_mul_scale_x(const mat3* in_m1, float_t s, mat3* out_m);
extern void mat3_mul_scale_y(const mat3* in_m1, float_t s, mat3* out_m);
extern void mat3_mul_scale_z(const mat3* in_m1, float_t s, mat3* out_m);
extern void mat3_get_rotation(const mat3* in_m1, vec3* out_rad);
extern void mat3_get_scale(const mat3* in_m1, vec3* out_s);
extern float_t mat3_get_max_scale(const mat3* in_m1);

extern void mat4_set(const quat* in_q1, mat4* out_m);
extern void mat4_set(const vec3* in_v1, const vec3* in_v2, mat4* out_m);
extern void mat4_set(const vec3* in_axis, const float_t in_angle, mat4* out_m);
extern void mat4_set(const vec3* in_axis, const float_t s, const float_t c, mat4* out_m);
extern void mat4_set_rotation(mat4* in_m1, const quat* in_q1);
extern void mat4_set_rotation(mat4* in_m1, const vec3* in_axis, const float_t in_angle);
extern void mat4_set_rotation(mat4* in_m1, const vec3* in_axis, const float_t s, const float_t c);
extern void mat4_add(const mat4* in_m1, const float_t value, mat4* out_m);
extern void mat4_add(const mat4* in_m1, const mat4* in_m2, mat4* out_m);
extern void mat4_sub(const mat4* in_m1, const float_t value, mat4* out_m);
extern void mat4_sub(const mat4* in_m1, const mat4* in_m2, mat4* out_m);
extern void mat4_mul(const mat4* in_m1, const float_t value, mat4* out_m);
extern void mat4_mul(const mat4* in_m1, const mat4* in_m2, mat4* out_m);
extern void mat4_mul_rotation(const mat4* in_m1, const vec3* in_axis, const float_t angle, mat4* out_m);
extern void mat4_transform_vector(const mat4* in_m1, const vec2* normal, vec2* normalOut);
extern void mat4_transform_vector(const mat4* in_m1, const vec3* normal, vec3* normalOut);
extern void mat4_transform_vector(const mat4* in_m1, const vec4* normal, vec4* normalOut);
extern void mat4_transform_point(const mat4* in_m1, const vec2* point, vec2* pointOut);
extern void mat4_transform_point(const mat4* in_m1, const vec3* point, vec3* pointOut);
extern void mat4_inverse_transform_vector(const mat4* in_m1, const vec2* normal, vec2* normalOut);
extern void mat4_inverse_transform_vector(const mat4* in_m1, const vec3* normal, vec3* normalOut);
extern void mat4_inverse_transform_vector(const mat4* in_m1, const vec4* normal, vec4* normalOut);
extern void mat4_inverse_transform_point(const mat4* in_m1, const vec2* point, vec2* pointOut);
extern void mat4_inverse_transform_point(const mat4* in_m1, const vec3* point, vec3* pointOut);
extern void mat4_transpose(const mat4* in_m1, mat4* out_m);
extern void mat4_invert(const mat4* in_m1, mat4* out_m);
extern void mat4_invert_rotation(const mat4* in_m1, mat4* out_m);
extern void mat4_invert_fast(const mat4* in_m1, mat4* out_m);
extern void mat4_invert_rotation_fast(const mat4* in_m1, mat4* out_m);
extern void mat4_normalize(const mat4* in_m1, mat4* out_m);
extern void mat4_normalize_rotation(const mat4* in_m1, mat4* out_m);
extern float_t mat4_determinant(const mat4* in_m1);
extern void mat4_rotate_x(float_t rad, mat4* out_m);
extern void mat4_rotate_y(float_t rad, mat4* out_m);
extern void mat4_rotate_z(float_t rad, mat4* out_m);
extern void mat4_rotate_x(float_t s, float_t c, mat4* out_m);
extern void mat4_rotate_y(float_t s, float_t c, mat4* out_m);
extern void mat4_rotate_z(float_t s, float_t c, mat4* out_m);
extern void mat4_rotate_xyz(float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_rotate_xzy(float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_rotate_yxz(float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_rotate_yzx(float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_rotate_zxy(float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_rotate_zyx(float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_mul_rotate_x(const mat4* in_m1, float_t rad, mat4* out_m);
extern void mat4_mul_rotate_y(const mat4* in_m1, float_t rad, mat4* out_m);
extern void mat4_mul_rotate_z(const mat4* in_m1, float_t rad, mat4* out_m);
extern void mat4_mul_rotate_x(const mat4* in_m1, float_t s, float_t c, mat4* out_m);
extern void mat4_mul_rotate_y(const mat4* in_m1, float_t s, float_t c, mat4* out_m);
extern void mat4_mul_rotate_z(const mat4* in_m1, float_t s, float_t c, mat4* out_m);
extern void mat4_mul_rotate_xyz(const mat4* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_mul_rotate_xzy(const mat4* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_mul_rotate_yxz(const mat4* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_mul_rotate_yzx(const mat4* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_mul_rotate_zxy(const mat4* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_mul_rotate_zyx(const mat4* in_m1, float_t rad_x, float_t rad_y, float_t rad_z, mat4* out_m);
extern void mat4_scale(float_t sx, float_t sy, float_t sz, mat4* out_m);
extern void mat4_scale_x(float_t s, mat4* out_m);
extern void mat4_scale_y(float_t s, mat4* out_m);
extern void mat4_scale_z(float_t s, mat4* out_m);
extern void mat4_mul_scale(const mat4* in_m1, float_t sx, float_t sy, float_t sz, float_t sw, mat4* out_m);
extern void mat4_mul_scale_x(const mat4* in_m1, float_t s, mat4* out_m);
extern void mat4_mul_scale_y(const mat4* in_m1, float_t s, mat4* out_m);
extern void mat4_mul_scale_z(const mat4* in_m1, float_t s, mat4* out_m);
extern void mat4_scale_w_mult(const mat4* in_m1, float_t s, mat4* out_m);
extern void mat4_scale_rot(const mat4* in_m1, float_t sx, float_t sy, float_t sz, mat4* out_m);
extern void mat4_scale_x_rot(const mat4* in_m1, float_t s, mat4* out_m);
extern void mat4_scale_y_rot(const mat4* in_m1, float_t s, mat4* out_m);
extern void mat4_scale_z_rot(const mat4* in_m1, float_t s, mat4* out_m);
extern void mat4_translate(float_t tx, float_t ty, float_t tz, mat4* out_m);
extern void mat4_translate_x(float_t t, mat4* out_m);
extern void mat4_translate_y(float_t t, mat4* out_m);
extern void mat4_translate_z(float_t t, mat4* out_m);
extern void mat4_mul_translate(const mat4* in_m1, float_t tx, float_t ty, float_t tz, mat4* out_m);
extern void mat4_mul_translate_x(const mat4* in_m1, float_t t, mat4* out_m);
extern void mat4_mul_translate_y(const mat4* in_m1, float_t t, mat4* out_m);
extern void mat4_mul_translate_z(const mat4* in_m1, float_t t, mat4* out_m);
extern void mat4_add_translate(const mat4* in_m1, float_t tx, float_t ty, float_t tz, mat4* out_m);
extern void mat4_add_translate_x(const mat4* in_m1, float_t t, mat4* out_m);
extern void mat4_add_translate_y(const mat4* in_m1, float_t t, mat4* out_m);
extern void mat4_add_translate_z(const mat4* in_m1, float_t t, mat4* out_m);
extern void mat4_to_mat3(const mat4* in_m1, mat3* out_m);
extern void mat4_to_mat3_inverse(const mat4* in_m1, mat3* out_m);
extern void mat4_from_mat3(const mat3* in_m1, mat4* out_m);
extern void mat4_from_mat3_inverse(const mat3* in_m1, mat4* out_m);
extern void mat4_clear_rot(const mat4* in_m1, mat4* out_m);
extern void mat4_clear_trans(const mat4* in_m1, mat4* out_m);
extern void mat4_get_scale(const mat4* in_m1, vec3* out_s);
extern void mat4_get_rotation(const mat4* in_m1, vec3* out_rad);
extern void mat4_get_translation(const mat4* in_m1, vec3* out_t);
extern void mat4_set_translation(mat4* in_m1, const vec3* in_t);
extern float_t mat4_get_max_scale(const mat4* in_m1);
extern void mat4_blend(const mat4* in_m1, const mat4* in_m2, mat4* out_m, float_t blend);
extern void mat4_blend_rotation(const mat4* in_m1, const mat4* in_m2, mat4* out_m, float_t blend);
extern void mat4_lerp_rotation(const mat4* in_m1, const mat4* in_m2, mat4* out_m, float_t blend);
extern void mat4_frustrum(double_t left, double_t right,
    double_t bottom, double_t top, double_t z_near, double_t z_far, mat4* out_m);
extern void mat4_ortho(double_t left, double_t right,
    double_t bottom, double_t top, double_t z_near, double_t z_far, mat4* out_m);
extern void mat4_persp(double_t fov_y, double_t aspect, double_t z_near, double_t z_far, mat4* out_m);
extern void mat4_look_at(const vec3* eye, const vec3* target, const vec3* up, mat4* out_m);
extern void mat4_look_at(const vec3* eye, const vec3* target, mat4* out_m);

inline void mat3_rotate_xyz(const vec3* rad, mat3* out_m) {
    mat3_rotate_xyz(rad->x, rad->y, rad->z, out_m);
}

inline void mat3_rotate_xzy(const vec3* rad, mat3* out_m) {
    mat3_rotate_xzy(rad->x, rad->y, rad->z, out_m);
}

inline void mat3_rotate_yxz(const vec3* rad, mat3* out_m) {
    mat3_rotate_yxz(rad->x, rad->y, rad->z, out_m);
}

inline void mat3_rotate_yzx(const vec3* rad, mat3* out_m) {
    mat3_rotate_yzx(rad->x, rad->y, rad->z, out_m);
}

inline void mat3_rotate_zxy(const vec3* rad, mat3* out_m) {
    mat3_rotate_zxy(rad->x, rad->y, rad->z, out_m);
}

inline void mat3_rotate_zyx(const vec3* rad, mat3* out_m) {
    mat3_rotate_zyx(rad->x, rad->y, rad->z, out_m);
}

inline void mat3_mul_rotate_xyz(const mat3* in_m1, const vec3* rad, mat3* out_m) {
    mat3_mul_rotate_xyz(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat3_mul_rotate_xzy(const mat3* in_m1, const vec3* rad, mat3* out_m) {
    mat3_mul_rotate_xzy(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat3_mul_rotate_yxz(const mat3* in_m1, const vec3* rad, mat3* out_m) {
    mat3_mul_rotate_yxz(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat3_mul_rotate_yzx(const mat3* in_m1, const vec3* rad, mat3* out_m) {
    mat3_mul_rotate_yzx(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat3_mul_rotate_zxy(const mat3* in_m1, const vec3* rad, mat3* out_m) {
    mat3_mul_rotate_zxy(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat3_mul_rotate_zyx(const mat3* in_m1, const vec3* rad, mat3* out_m) {
    mat3_mul_rotate_zyx(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat3_scale(const vec3* s, mat3* out_m) {
    mat3_scale(s->x, s->y, s->z, out_m);
}

inline void mat3_mul_scale(const mat3* in_m1, float_t s, mat3* out_m) {
    mat3_mul_scale(in_m1, s, s, s, out_m);
}

inline void mat3_mul_scale(const mat3* in_m1, const vec3* s, mat3* out_m) {
    mat3_mul_scale(in_m1, s->x, s->y, s->z, out_m);
}

inline void mat4_rotate_xyz(const vec3* rad, mat4* out_m) {
    mat4_rotate_xyz(rad->x, rad->y, rad->z, out_m);
}

inline void mat4_rotate_xzy(const vec3* rad, mat4* out_m) {
    mat4_rotate_xzy(rad->x, rad->y, rad->z, out_m);
}

inline void mat4_rotate_yxz(const vec3* rad, mat4* out_m) {
    mat4_rotate_yxz(rad->x, rad->y, rad->z, out_m);
}

inline void mat4_rotate_yzx(const vec3* rad, mat4* out_m) {
    mat4_rotate_yzx(rad->x, rad->y, rad->z, out_m);
}

inline void mat4_rotate_zxy(const vec3* rad, mat4* out_m) {
    mat4_rotate_zxy(rad->x, rad->y, rad->z, out_m);
}

inline void mat4_rotate_zyx(const vec3* rad, mat4* out_m) {
    mat4_rotate_zyx(rad->x, rad->y, rad->z, out_m);
}

inline void mat4_mul_rotate_xyz(const mat4* in_m1, const vec3* rad, mat4* out_m) {
    mat4_mul_rotate_xyz(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat4_mul_rotate_xzy(const mat4* in_m1, const vec3* rad, mat4* out_m) {
    mat4_mul_rotate_xzy(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat4_mul_rotate_yxz(const mat4* in_m1, const vec3* rad, mat4* out_m) {
    mat4_mul_rotate_yxz(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat4_mul_rotate_yzx(const mat4* in_m1, const vec3* rad, mat4* out_m) {
    mat4_mul_rotate_yzx(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat4_mul_rotate_zxy(const mat4* in_m1, const vec3* rad, mat4* out_m) {
    mat4_mul_rotate_zxy(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat4_mul_rotate_zyx(const mat4* in_m1, const vec3* rad, mat4* out_m) {
    mat4_mul_rotate_zyx(in_m1, rad->x, rad->y, rad->z, out_m);
}

inline void mat4_scale(const vec3* s, mat4* out_m) {
    mat4_scale(s->x, s->y, s->z, out_m);
}

inline void mat4_mul_scale(const mat4* in_m1, float_t s, mat4* out_m) {
    mat4_mul_scale(in_m1, s, s, s, s, out_m);
}

inline void mat4_mul_scale(const mat4* in_m1, vec4* s, mat4* out_m) {
    mat4_mul_scale(in_m1, s->x, s->y, s->z, s->w, out_m);
}

inline void mat4_scale_rot(const mat4* in_m1, const float_t s, mat4* out_m) {
    mat4_scale_rot(in_m1, s, s, s, out_m);
}

inline void mat4_scale_rot(const mat4* in_m1, const vec3* s, mat4* out_m) {
    mat4_scale_rot(in_m1, s->x, s->y, s->z, out_m);
}

inline void mat4_translate(const vec3* s, mat4* out_m) {
    mat4_translate(s->x, s->y, s->z, out_m);
}

inline void mat4_mul_translate(const mat4* in_m1, const vec3* t, mat4* out_m) {
    mat4_mul_translate(in_m1, t->x, t->y, t->z, out_m);
}

inline void mat4_add_translate(const mat4* in_m1, const vec3* t, mat4* out_m) {
    mat4_add_translate(in_m1, t->x, t->y, t->z, out_m);
}
