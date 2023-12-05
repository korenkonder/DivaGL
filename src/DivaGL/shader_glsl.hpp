/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <string>
#include <vector>
#include "../KKdLib/default.hpp"
#include "../KKdLib/farc.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "gl.hpp"
#include "uniform.hpp"

#define SHADER_MAX_LIGHTS 8
#define SHADER_MAX_PALETTE_MATRICES 32
#define SHADER_MAX_PROGRAM_ENV_PARAMETERS 256
#define SHADER_MAX_PROGRAM_LOCAL_PARAMETERS 64
#define SHADER_MAX_PROGRAM_BUFFER_PARAMETERS 1024
#define SHADER_MAX_PROGRAM_MATRICES 8
#define SHADER_MAX_TEXTURE_COORDS 8
#define SHADER_MAX_UNIFORM_BLOCK_SIZE 65536
#define SHADER_MAX_UNIFORM_VALUES 16

struct shader_glsl_sub_table {
    uint32_t sub_index;
    const int32_t* vp_unival_max;
    const int32_t* fp_unival_max;
    const char* vp;
    const char* fp;
};

struct shader_glsl_table {
    const char* name;
    uint32_t name_index;
    int32_t num_sub;
    const shader_glsl_sub_table* sub;
    int32_t num_uniform;
    const uniform_name* use_uniform;
    const bool* use_permut;
};

struct shader_glsl;
struct shader_glsl_set_data;

typedef int32_t (*PFNSHADERGLSLGETINDEXFUNCPROC)(const char* name);

typedef void (*PFNSHADERGLSLBINDFUNCPROC)(shader_glsl_set_data* set, shader_glsl* shad);

struct shader_glsl_bind_func {
    uint32_t name_index;
    PFNSHADERGLSLBINDFUNCPROC bind_func;
};

struct shader_glsl_sub_shader {
    GLuint program;
    int32_t uniform_val[SHADER_MAX_UNIFORM_VALUES];
    bool uniform_val_update;
};

struct shader_glsl_sub {
    uint32_t sub_index;
    const int32_t* vp_unival_max;
    const int32_t* fp_unival_max;
    shader_glsl_sub_shader* shaders;
};

struct shader_glsl {
    const char* name;
    uint32_t name_index;
    int32_t num_sub;
    shader_glsl_sub* sub;
    int32_t num_uniform;
    const uniform_name* use_uniform;
    const bool* use_permut;
    PFNSHADERGLSLBINDFUNCPROC bind_func;

    int32_t bind(shader_glsl_set_data* set, uint32_t sub_index);

    static bool parse_define(const char* data, char** temp, size_t* temp_size);
    static bool parse_define(const char* data, int32_t num_uniform,
        int32_t* uniform_value, char** temp, size_t* temp_size);
    static char* parse_include(char* data, farc* f);
    static void unbind();
};

struct shader_glsl_set_data {
    size_t size;
    shader_glsl* shaders;
    shader_glsl_sub_shader* curr_shader;
    GLboolean primitive_restart;
    GLuint primitive_restart_index;

    PFNSHADERGLSLGETINDEXFUNCPROC get_index_by_name_func;

    shader_glsl_set_data();

    void disable_primitive_restart();
    void draw_arrays(GLenum mode, GLint first, GLsizei count);
    void draw_elements(GLenum mode,
        GLsizei count, GLenum type, const void* indices);
    void draw_range_elements(GLenum mode,
        GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
    void enable_primitive_restart();
    int32_t get_index_by_name(const char* name);
    void load(farc* f, bool ignore_cache, const char* name,
        const shader_glsl_table* shaders_table, const size_t size,
        const shader_glsl_bind_func* bind_func_table, const size_t bind_func_table_size,
        PFNSHADERGLSLGETINDEXFUNCPROC get_index_by_name);
    void set(uint32_t index);
    void unload();
};
