/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/vec.hpp"

enum fog_id {
    FOG_DEPTH = 0x00,
    FOG_HEIGHT = 0x01,
    FOG_BUMP = 0x02,
    FOG_MAX = 0x03,
};

enum fog_type {
    FOG_NONE = 0x00,
    FOG_LINEAR = 0x01,
    FOG_EXP = 0x02,
    FOG_EXP2 = 0x03,
};

struct fog {
    fog_type type;
    float_t density;
    float_t start;
    float_t end;
    int32_t index;
    vec4 color;

    fog_type get_type() const;
    void set_type(fog_type value);
    float_t get_density() const;
    void set_density(float_t value);
    float_t get_start() const;
    void set_start(float_t value);
    float_t get_end() const;
    void set_end(float_t value);
    int32_t get_index() const;
    void set_index(int32_t value);
    void get_color(vec4& value) const;
    void set_color(const vec4& value);
    void set_color(const vec4&& value);
};

static_assert(sizeof(fog) == 0x24, "\"fog\" struct should have a size of 0x24");

extern fog* fog_data;
