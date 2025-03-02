/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "light_param.hpp"
#include "../KKdLib/hash.hpp"
#include "../KKdLib/str_utils.hpp"
#include "../AFTModsShared/file_handler.hpp"
#include "render_context.hpp"
#include <Helpers.h>

struct light_param_data_light_data {
    bool has_type;
    light_type type;
    bool has_ambient;
    vec4 ambient;
    bool has_diffuse;
    vec4 diffuse;
    bool has_specular;
    vec4 specular;
    bool has_position;
    vec3 position;
    bool has_spot_direction;
    bool has_spot_exponent;
    bool has_spot_cutoff;
    bool has_attenuation;
    vec3 spot_direction;
    float_t spot_exponent;
    float_t spot_cutoff;
    vec3 attenuation;
    char has_clip_plane;
    bool clip_plane[4];
    bool has_tone_curve;
    vec3 tone_curve;
};

static_assert(sizeof(light_param_data_light_data) == 0x8C, "\"light_param_data_light_data\" struct should have a size of 0x8C");

struct light_param_data_light {
    prj::map<light_set_id, prj::map<light_id, light_param_data_light_data>> data;
    bool has_data;
};

static_assert(sizeof(light_param_data_light) == 0x18, "\"light_param_data_light\" struct should have a size of 0x18");

struct light_param_data_fog_data {
    bool has_type;
    fog_type type;
    bool has_density;
    float_t density;
    bool has_linear;
    float_t linear_start;
    float_t linear_end;
    bool has_color;
    vec4 color;
};

static_assert(sizeof(light_param_data_fog_data) == 0x30, "\"light_param_data_fog_data\" struct should have a size of 0x30");

struct light_param_data_fog {
    prj::map<fog_id, light_param_data_fog_data> data;
    bool has_data;
};

static_assert(sizeof(light_param_data_fog) == 0x18, "\"light_param_data_fog\" struct should have a size of 0x18");

struct light_param_data_glow {
    bool has_exposure;
    float_t exposure;
    bool has_gamma;
    float_t gamma;
    bool has_saturate_power;
    int32_t saturate_power;
    bool has_saturate_coef;
    float_t saturate_coef;
    bool has_flare;
    vec3 flare;
    bool has_sigma;
    vec3 sigma;
    bool has_intensity;
    vec3 intensity;
    bool has_auto_exposure;
    bool auto_exposure;
    bool has_tone_map_method;
    int32_t tone_map_method;
    bool has_fade_color;
    vec4 fade_color;
    int32_t fade_color_blend_func;
    bool has_tone_transform;
    vec3 tone_transform_start;
    vec3 tone_transform_end;
    bool has_data;
};

static_assert(sizeof(light_param_data_glow) == 0x90, "\"light_param_data_glow\" struct should have a size of 0x90");

struct light_param_data_ibl_diffuse {
    prj::vector<uint8_t> data;
    GLenum type;
    int32_t size;
    int32_t level;
    GLenum internal_format;
};

static_assert(sizeof(light_param_data_ibl_diffuse) == 0x28, "\"light_param_data_ibl_diffuse\" struct should have a size of 0x28");

struct light_param_data_ibl_specular_byte_data {
    prj::vector<uint8_t> data;
    size_t size;
};

static_assert(sizeof(light_param_data_ibl_specular_byte_data) == 0x20, "\"light_param_data_ibl_specular_byte_data\" struct should have a size of 0x20");

struct light_param_data_ibl_specular_half_data {
    prj::vector<half_t> data;
    size_t size;
};

static_assert(sizeof(light_param_data_ibl_specular_half_data) == 0x20, "\"light_param_data_ibl_specular_half_data\" struct should have a size of 0x20");

struct light_param_data_ibl_specular {
    prj::vector<light_param_data_ibl_specular_byte_data> uint8_t_data;
    prj::vector<light_param_data_ibl_specular_half_data> half_t_data;
};

static_assert(sizeof(light_param_data_ibl_specular) == 0x30, "\"light_param_data_ibl_specular\" struct should have a size of 0x30");

struct light_param_data_ibl {
    vec4 lit_dir[4];
    vec4 lit_col[4];
    mat4 diff_coef[4][3];
    int32_t widths[6];
    int32_t heights[6];
    bool has_data;
    light_param_data_ibl_diffuse diffuse[2];
    light_param_data_ibl_specular specular[4];
};

static_assert(sizeof(light_param_data_ibl) == 0x4C8, "\"light_param_data_ibl\" struct should have a size of 0x4C8");

struct light_param_data_wind_spc {
    float_t cos;
    float_t sin;
};

static_assert(sizeof(light_param_data_wind_spc) == 0x08, "\"light_param_data_wind_spc\" struct should have a size of 0x08");

struct light_param_data_wind {
    bool has_scale;
    float_t scale;
    bool has_cycle;
    float_t cycle;
    bool has_rot;
    vec2 rot;
    bool has_bias;
    float_t bias;
    bool has_spc[16];
    light_param_data_wind_spc spc[16];
    bool has_data;
};

static_assert(sizeof(light_param_data_wind) == 0xB8, "\"light_param_data_wind\" struct should have a size of 0xB8");

struct light_param_data_face_data {
    float_t offset;
    float_t scale;
    vec3 position;
    vec3 direction;
};

static_assert(sizeof(light_param_data_face_data) == 0x20, "\"light_param_data_face_data\" struct should have a size of 0x20");

struct __declspec(align(8)) light_param_data_face {
    light_param_data_face_data data;
    bool has_data;
};

static_assert(sizeof(light_param_data_face) == 0x28, "\"light_param_data_face\" struct should have a size of 0x28");

struct light_param_data_storage;

struct light_param_data {
    prj::string name;
    prj::string files[6];
    bool pv;
    light_param_data_light light;
    light_param_data_fog fog;
    light_param_data_glow glow;
    light_param_data_ibl ibl;
    light_param_data_wind wind;
    light_param_data_face face;

    static void set_ibl(const light_param_data_ibl* ibl, const light_param_data_storage* storage);
    static void set_ibl_diffuse(const light_param_data_ibl_diffuse* diffuse);
    static void set_ibl_specular(const light_param_data_ibl_specular* specular);
    static void set_ibl_specular_byte(
        const light_param_data_ibl_specular_byte_data* data, const int32_t level);
    static void set_ibl_specular_half(
        const light_param_data_ibl_specular_half_data* data, const int32_t level);
};

static_assert(sizeof(light_param_data) == 0x750, "\"light_param_data\" struct should have a size of 0x750");

struct light_param_data_storage {
    GLuint textures[5];
    int32_t state;
    prj::string name;
    int32_t stage_index;
    light_param_data default_light_param;
    prj::map<int32_t, light_param_data> stage;
    prj::map<int32_t, light_param_data>::iterator current_light_param;
    prj::map<int32_t, light_param_data> pv_cut;
    p_file_handler file_handlers[6];
    p_file_handler farc_file_handler;
    int32_t pv_id;
};

static_assert(sizeof(light_param_data_storage) == 0x7F8, "\"light_param_data_storage\" struct should have a size of 0x7F8");

light_param_data_storage* light_param_data_storage_data = (light_param_data_storage*)0x00000001411A06A0;

static uint8_t(FASTCALL* sub_1403EAB10)() = (uint8_t(FASTCALL*)())0x00000001403EAB10;

HOOK(void, FASTCALL, light_param_data__set_ibl, 0x00000001403E7210,
    const light_param_data_ibl* ibl, const light_param_data_storage* storage) {
    light_param_data::set_ibl(ibl, storage);
}

HOOK(void, FASTCALL, light_param_data_storage__gen_textures, 0x00000001403E8C50, light_param_data_storage* storage) {
    glGenTextures(5, storage->textures);
}

HOOK(void, FASTCALL, light_param_data_storage__delete_textures, 0x00000001403E8D80, light_param_data_storage* storage) {
    glDeleteTextures(5, storage->textures);
}

void light_param_patch() {
    INSTALL_HOOK(light_param_data__set_ibl);
    INSTALL_HOOK(light_param_data_storage__gen_textures);
    INSTALL_HOOK(light_param_data_storage__delete_textures);
}

void light_param_data_storage_data_set_ibl() {
    static const int32_t ibl_texture_index[] = {
        9, 10, 11, 12, 13
    };

    for (int32_t i = 0; i < 5; i++)
        gl_state_active_bind_texture_cube_map(ibl_texture_index[i],
            light_param_data_storage_data->textures[i]);
}

void light_param_data::set_ibl(const light_param_data_ibl* ibl, const light_param_data_storage* storage) {
    for (int32_t i = 0, j = -1; i < 5; i++, j++) {
        gl_state_bind_texture_cube_map(storage->textures[i]);
        glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        if (!i) {
            glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 1);
            glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            light_param_data::set_ibl_diffuse(&ibl->diffuse[0]);
            light_param_data::set_ibl_diffuse(&ibl->diffuse[1]);
        }
        else {
            glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 2);
            if (sub_1403EAB10()) {
                glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameterfDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, 0.0f);
            }
            else {
                glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteriDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameterfDLL(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, -1.5f);
            }
            light_param_data::set_ibl_specular(&ibl->specular[j]);
        }
    }
    gl_state_bind_texture_cube_map(0);

    ::light_set* set = &light_set_data[LIGHT_SET_MAIN];
    set->set_irradiance(ibl->diff_coef[1][0], ibl->diff_coef[1][1], ibl->diff_coef[1][2]);

    float_t len;
    vec3 pos;

    light_data* l = &set->lights[LIGHT_CHARA];
    l->get_position(pos);
    len = vec3::length(pos);
    if (fabsf(len - 1.0f) < 0.02f)
        l->set_position(ibl->lit_dir[0]);

    l->set_ibl_color0(ibl->lit_col[0]);
    l->set_ibl_color1(ibl->lit_col[2]);
    l->set_ibl_direction(ibl->lit_dir[0]);

    l = &set->lights[LIGHT_STAGE];
    l->get_position(pos);
    len = vec3::length(pos);
    if (fabsf(len - 1.0f) < 0.02f)
        l->set_position(ibl->lit_dir[1]);

    l->set_ibl_color0(ibl->lit_col[1]);
    l->set_ibl_direction(ibl->lit_dir[1]);
}

void light_param_data::set_ibl_diffuse(const light_param_data_ibl_diffuse* diffuse) {
    if (!diffuse->data.size())
        return;

    int32_t size = diffuse->size;
    const int32_t level = diffuse->level;
    const uint8_t* _data = diffuse->data.data();
    size_t data_size = diffuse->data.size() / 6;
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 0]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 1]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 2]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 3]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 4]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 5]);
}

void light_param_data::set_ibl_specular(const light_param_data_ibl_specular* specular) {
    int32_t level = 0;
    if (sub_1403EAB10())
        for (const light_param_data_ibl_specular_half_data& i : specular->half_t_data)
            light_param_data::set_ibl_specular_half(&i, level++);
    else
        for (const light_param_data_ibl_specular_byte_data& i : specular->uint8_t_data)
            light_param_data::set_ibl_specular_byte(&i, level++);
}

void light_param_data::set_ibl_specular_byte(
    const light_param_data_ibl_specular_byte_data* data, const int32_t level) {
    int32_t size = (int32_t)data->size;
    const uint8_t* _data = data->data.data();
    size_t data_size = data->data.size() / 6;
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, GL_RGBA8,
        size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &_data[data_size * 0]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, GL_RGBA8,
        size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &_data[data_size * 1]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, GL_RGBA8,
        size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &_data[data_size * 2]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, GL_RGBA8,
        size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &_data[data_size * 3]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, GL_RGBA8,
        size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &_data[data_size * 4]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, GL_RGBA8,
        size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &_data[data_size * 5]);
}

void light_param_data::set_ibl_specular_half(
    const light_param_data_ibl_specular_half_data* data, const int32_t level) {
    int32_t size = (int32_t)data->size;
    const half_t* _data = data->data.data();
    size_t data_size = data->data.size() / 6;
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 0]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 1]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 2]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 3]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 4]);
    glTexImage2DDLL(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, GL_RGBA16F,
        size, size, 0, GL_RGBA, GL_HALF_FLOAT, &_data[data_size * 5]);
}
