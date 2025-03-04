/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "light.hpp"
#include "../../KKdLib/mat.hpp"

light_set* light_set_data = (light_set*)0x00000001411A00A0;

light_type light_data::get_type() const {
    return type;
}

void light_data::set_type(light_type value) {
    type = value;
    if (value < LIGHT_POINT)
        position.w = 0.0f;
    else
        position.w = 1.0f;
}

void light_data::get_ambient(vec4& value) const {
    value = ambient;
}

void light_data::set_ambient(const vec4& value) {
    ambient = value;
}

void light_data::set_ambient(const vec4&& value) {
    ambient = value;
}

void light_data::set_ambient(const vec4& value, bool set[4]) {
    if (set[0])
        ambient.x = value.x;
    if (set[1])
        ambient.y = value.y;
    if (set[2])
        ambient.z = value.z;
    if (set[3])
        ambient.w = value.w;
}

void light_data::set_ambient(const vec4&& value, bool set[4]) {
    if (set[0])
        ambient.x = value.x;
    if (set[1])
        ambient.y = value.y;
    if (set[2])
        ambient.z = value.z;
    if (set[3])
        ambient.w = value.w;
}

void light_data::get_diffuse(vec4& value) const {
    value = diffuse;
}

void light_data::set_diffuse(const vec4& value) {
    diffuse = value;
}

void light_data::set_diffuse(const vec4&& value) {
    diffuse = value;
}

void light_data::set_diffuse(const vec4& value, bool set[4]) {
    if (set[0])
        diffuse.x = value.x;
    if (set[1])
        diffuse.y = value.y;
    if (set[2])
        diffuse.z = value.z;
    if (set[3])
        diffuse.w = value.w;
}

void light_data::set_diffuse(const vec4&& value, bool set[4]) {
    if (set[0])
        diffuse.x = value.x;
    if (set[1])
        diffuse.y = value.y;
    if (set[2])
        diffuse.z = value.z;
    if (set[3])
        diffuse.w = value.w;
}

void light_data::get_specular(vec4& value) const {
    value = specular;
}

void light_data::set_specular(const vec4& value) {
    specular = value;
}

void light_data::set_specular(const vec4&& value) {
    specular = value;
}

void light_data::set_specular(const vec4& value, bool set[4]) {
    if (set[0])
        specular.x = value.x;
    if (set[1])
        specular.y = value.y;
    if (set[2])
        specular.z = value.z;
    if (set[3])
        specular.w = value.w;
}

void light_data::set_specular(const vec4&& value, bool set[4]) {
    if (set[0])
        specular.x = value.x;
    if (set[1])
        specular.y = value.y;
    if (set[2])
        specular.z = value.z;
    if (set[3])
        specular.w = value.w;
}

void light_data::get_position(vec3& value) const {
    value = *(vec3*)&position;
}

void light_data::set_position(const vec3& value) {
    *(vec3*)&position = value;
}

void light_data::set_position(const vec3&& value) {
    *(vec3*)&position = value;
}

void light_data::get_position(vec4& value) const {
    value = position;
}

void light_data::set_position(const vec4& value) {
    position = value;
}

void light_data::set_position(const vec4&& value) {
    position = value;
}

void light_data::get_spot_direction(vec3& value) const {
    value = spot_direction;
}

void light_data::set_spot_direction(const vec3& value) {
    spot_direction = value;
}

void light_data::set_spot_direction(const vec3&& value) {
    spot_direction = value;
}

float_t light_data::get_spot_exponent() const {
    return spot_exponent;
}

void light_data::set_spot_exponent(float_t value) {
    spot_exponent = value;
}

float_t light_data::get_spot_cutoff() const {
    return spot_cutoff;
}

void light_data::set_spot_cutoff(float_t value) {
    spot_cutoff = value;
}

float_t light_data::get_constant() const {
    return attenuation.constant;
}

void light_data::set_constant(float_t value) {
    attenuation.constant = value;
}

float_t light_data::get_linear() const {
    return attenuation.linear;
}

void light_data::set_linear(float_t value) {
    attenuation.linear = value;
}

float_t light_data::get_quadratic() const {
    return attenuation.quadratic;
}

void light_data::set_quadratic(float_t value) {
    attenuation.quadratic = value;
}

void light_data::get_attenuation(light_attenuation& value) const {
    value = attenuation;
}

void light_data::set_attenuation(const light_attenuation& value) {
    attenuation = value;
}

void light_data::set_attenuation(const light_attenuation&& value) {
    attenuation = value;
}

void light_data::get_ibl_color0(vec4& value) const {
    value = ibl_color0;
}

void light_data::set_ibl_color0(const vec4& value) {
    ibl_color0 = value;
}

void light_data::set_ibl_color0(const vec4&& value) {
    ibl_color0 = value;
}

void light_data::get_ibl_color1(vec4& value) const {
    value = ibl_color1;
}

void light_data::set_ibl_color1(const vec4& value) {
    ibl_color1 = value;
}

void light_data::set_ibl_color1(const vec4&& value) {
    ibl_color1 = value;
}

void light_data::get_ibl_direction(vec4& value) const {
    value = ibl_direction;
}

void light_data::set_ibl_direction(const vec4& value) {
    ibl_direction = value;
}

void light_data::set_ibl_direction(const vec4&& value) {
    ibl_direction = value;
}

void light_data::get_tone_curve(light_tone_curve& value) const {
    value = tone_curve;
}

void light_data::set_tone_curve(const light_tone_curve& value) {
    tone_curve = value;
}

void light_data::set_tone_curve(const light_tone_curve&& value) {
    tone_curve = value;
}

void light_data::get_clip_plane(light_clip_plane& value) const {
    value = clip_plane;
}

void light_data::set_clip_plane(const light_clip_plane& value) {
    clip_plane = value;
}

void light_data::set_clip_plane(const light_clip_plane&& value) {
    clip_plane = value;
}

void light_set::get_ambient_intensity(vec4& value) const {
    value = ambient_intensity;
}

void light_set::set_ambient_intensity(const vec4& value) {
    ambient_intensity = value;
}

void light_set::set_ambient_intensity(const vec4&& value) {
    ambient_intensity = value;
}

void light_set::get_irradiance(mat4& r, mat4& g, mat4& b) const {
    r = irradiance_r;
    g = irradiance_g;
    b = irradiance_b;
}

void light_set::set_irradiance(const mat4& r, const mat4& g, const mat4& b) {
    irradiance_r = r;
    irradiance_g = g;
    irradiance_b = b;
}

void light_set::set_irradiance(const mat4&& r, const mat4&& g, const mat4&& b) {
    irradiance_r = r;
    irradiance_g = g;
    irradiance_b = b;
}
