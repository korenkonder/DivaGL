/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"

struct color4u8 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    inline color4u8() : r(), g(), b(), a() {

    }

    inline color4u8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {

    }

    inline color4u8(uint32_t value) {
        r = (uint8_t)(value & 0xFF);
        g = (uint8_t)((value >> 8) & 0xFF);
        b = (uint8_t)((value >> 16) & 0xFF);
        a = (uint8_t)((value >> 24) & 0xFF);
    }

    inline color4u8(const vec4& value) {
        vec4i _value = vec4i::store_xmm(_mm_cvtps_epi32(vec4::load_xmm(vec4::clamp(value, 0.0f, 1.0f) * 255.0f)));
        r = (uint8_t)_value.x;
        g = (uint8_t)_value.y;
        b = (uint8_t)_value.z;
        a = (uint8_t)_value.w;
    }

    inline operator vec4() const {
        return vec4::store_xmm(_mm_cvtepi32_ps(vec4i::load_xmm(vec4i(r, g, b, a)))) * (1.0f / 255.0f);
    }

    inline color4u8(const struct color4u8_bgra& value);
};

struct color4u8_bgra {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;

    inline color4u8_bgra() : r(), g(), b(), a() {

    }

    inline color4u8_bgra(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {

    }

    inline color4u8_bgra(uint32_t value) {
        b = (uint8_t)(value & 0xFF);
        g = (uint8_t)((value >> 8) & 0xFF);
        r = (uint8_t)((value >> 16) & 0xFF);
        a = (uint8_t)((value >> 24) & 0xFF);
    }

    inline color4u8_bgra(const vec4& value) {
        vec4i _value = vec4i::store_xmm(_mm_cvtps_epi32(vec4::load_xmm(vec4::clamp(value, 0.0f, 1.0f) * 255.0f)));
        r = (uint8_t)_value.x;
        g = (uint8_t)_value.y;
        b = (uint8_t)_value.z;
        a = (uint8_t)_value.w;
    }

    inline operator vec4() const {
        return vec4::store_xmm(_mm_cvtepi32_ps(vec4i::load_xmm(vec4i(r, g, b, a)))) * (1.0f / 255.0f);
    }

    color4u8_bgra(const struct color4u8& value);
};

inline color4u8::color4u8(const color4u8_bgra& value) {
    r = value.r;
    g = value.g;
    b = value.b;
    a = value.a;
}

inline color4u8_bgra::color4u8_bgra(const color4u8& value) {
    r = value.r;
    g = value.g;
    b = value.b;
    a = value.a;
}

extern const color4u8 color_black;
extern const color4u8 color_grey;
extern const color4u8 color_white;
extern const color4u8 color_red;
extern const color4u8 color_green;
extern const color4u8 color_blue;
extern const color4u8 color_cyan;
extern const color4u8 color_magenta;
extern const color4u8 color_yellow;
extern const color4u8 color_dark_red;
extern const color4u8 color_dark_green;
extern const color4u8 color_dark_blue;
extern const color4u8 color_dark_cyan;
extern const color4u8 color_dark_magenta;
extern const color4u8 color_dark_yellow;
