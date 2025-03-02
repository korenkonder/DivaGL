/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "fog.hpp"

fog* fog_data = (fog*)0x00000001411974D0;

fog_type fog::get_type() const {
    return type;
}

void fog::set_type(fog_type value) {
    type = value;
}

float_t fog::get_density() const {
    return density;
}

void fog::set_density(float_t value) {
    density = value;
}

float_t fog::get_start() const {
    return start;
}

void fog::set_start(float_t value) {
    start = value;
}

float_t fog::get_end() const {
    return end;
}

void fog::set_end(float_t value) {
    end = value;
}

int32_t fog::get_index() const {
    return index;
}

void fog::set_index(int32_t value) {
    index = value;
}

void fog::get_color(vec4& value) const {
    value = color;
}

void fog::set_color(const vec4& value) {
    color = value;
}

void fog::set_color(const vec4&& value) {
    color = value;
}
