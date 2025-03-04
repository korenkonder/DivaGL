/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"
#include "../object.hpp"
#include "../texture.hpp"

namespace mdl {
    enum ObjFlags : uint32_t {
        OBJ_SHADOW                     = 0x00000001,
        OBJ_2                          = 0x00000002,
        OBJ_4                          = 0x00000004,
        OBJ_8                          = 0x00000008,
        OBJ_10                         = 0x00000010,
        OBJ_20                         = 0x00000020,
        OBJ_40                         = 0x00000040,
        OBJ_SHADOW_OBJECT              = 0x00000080,
        OBJ_CHARA_REFLECT              = 0x00000100,
        OBJ_REFLECT                    = 0x00000200,
        OBJ_REFRACT                    = 0x00000400,
        OBJ_800                        = 0x00000800,
        OBJ_TRANSLUCENT_SORT_BY_RADIUS = 0x00001000,
        OBJ_SSS                        = 0x00002000,
        OBJ_4000                       = 0x00004000,
        OBJ_8000                       = 0x00008000,
        OBJ_ALPHA_ORDER_1              = 0x00010000,
        OBJ_ALPHA_ORDER_2              = 0x00020000,
        OBJ_ALPHA_ORDER_3              = 0x00040000,
        OBJ_80000                      = 0x00080000,
        OBJ_100000                     = 0x00100000,
        OBJ_200000                     = 0x00200000,
        OBJ_400000                     = 0x00400000,
        OBJ_800000                     = 0x00800000,
        OBJ_USER                       = 0x01000000,
        OBJ_2000000                    = 0x02000000,
        OBJ_4000000                    = 0x04000000,
        OBJ_8000000                    = 0x08000000,
        OBJ_10000000                   = 0x10000000,
        OBJ_20000000                   = 0x20000000,
        OBJ_40000000                   = 0x40000000,
        OBJ_NO_TRANSLUCENCY            = 0x80000000,
    };
}

struct morph_struct {
    object_info object;
    float_t weight;

    inline morph_struct() : weight() {

    }

    inline morph_struct(object_info object, float_t weight) : object(object), weight(weight) {

    }
};

static_assert(sizeof(morph_struct) == 0x08, "\"morph_struct\" struct should have a size of 0x08");

struct texture_pattern_struct {
    texture_id src;
    texture_id dst;

    inline texture_pattern_struct() {

    }

    inline texture_pattern_struct(texture_id src, texture_id dst) : src(src), dst(dst) {

    }
};

static_assert(sizeof(texture_pattern_struct) == 0x08, "\"texture_pattern_struct\" struct should have a size of 0x08");

struct texture_transform_struct {
    uint32_t id;
    mat4 mat;

    inline texture_transform_struct() {
        id = (uint32_t)-1;
        mat = mat4_identity;
    }

    inline texture_transform_struct(uint32_t id, const mat4& mat) : id(id), mat(mat) {

    }
};

static_assert(sizeof(texture_transform_struct) == 0x44, "\"texture_pattern_struct\" struct should have a size of 0x44");

struct texture_data_struct {
    int32_t field_0;
    vec3 texture_color_coefficients;
    vec3 texture_color_offset;
    vec3 texture_specular_coefficients;
    vec3 texture_specular_offset;

    inline texture_data_struct() : field_0(), texture_color_coefficients(),
        texture_color_offset(), texture_specular_coefficients(), texture_specular_offset() {

    }
};

static_assert(sizeof(texture_data_struct) == 0x34, "\"texture_pattern_struct\" struct should have a size of 0x34");
