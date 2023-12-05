/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "fog.hpp"
#include "../render_context.hpp"

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

void fog::data_set(fog_id id) {
    fog_type type = get_type();
    if (type == FOG_NONE)
        return;

    float_t density = get_density();
    float_t start = get_start();
    float_t end = get_end();
    if (start >= end)
        start = end - 0.01f;

    vec4 params;
    params.x = density;
    params.y = start;
    params.z = end;
    params.w = 1.0f / (end - start);

    extern render_context* rctx;

    switch (id) {
    case FOG_DEPTH:
        get_color(rctx->obj_scene.g_fog_depth_color);
        rctx->obj_scene.g_fog_state_params = params;
        break;
    case FOG_HEIGHT:
        get_color(rctx->obj_scene.g_fog_height_color);
        rctx->obj_scene.g_fog_height_params = params;
        break;
    case FOG_BUMP:
        rctx->obj_scene.g_fog_bump_params = params;
        break;
    }
}
