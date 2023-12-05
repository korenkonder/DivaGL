/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../gl_uniform_buffer.hpp"
#include "../render_texture.hpp"
#include "fbo.hpp"

enum dof_debug_flags {
    DOF_DEBUG_USE_UI_PARAMS   = 0x01,
    DOF_DEBUG_ENABLE_DOF      = 0x02,
    DOF_DEBUG_ENABLE_PHYS_DOF = 0x04,
    DOF_DEBUG_AUTO_FOCUS      = 0x08,
};

struct dof_f2 {
    float_t focus;
    float_t focus_range;
    float_t fuzzing_range;
    float_t ratio;
};

static_assert(sizeof(dof_f2) == 0x10, "\"dof_f2\" struct should have a size of 0x10");

struct dof_debug {
    dof_debug_flags flags;
    float_t focus;
    float_t focal_length;
    float_t f_number;
    dof_f2 f2;
};

static_assert(sizeof(dof_debug) == 0x20, "\"dof_debug\" struct should have a size of 0x14");

struct dof_pv {
    bool enable;
    dof_f2 f2;
};

static_assert(sizeof(dof_pv) == 0x14, "\"dof_pv\" struct should have a size of 0x14");

namespace renderer {
    struct DOF3 {
        int32_t width;
        int32_t height;
        GLuint textures[6];
        FBO fbo[4];
        GLuint samplers[2];
        GLuint vao;
        GLuint program[9];
        GLUniformBuffer common_ubo;
        GLUniformBuffer texcoords_ubo;

        void apply(RenderTexture* rt);
        void resize(int32_t width, int32_t height);

    private:
        void apply_f2(RenderTexture* rt, GLuint color_texture,
            GLuint depth_texture, float_t min_distance, float_t max_distance, float_t fov,
            float_t focus, float_t focus_range, float_t fuzzing_range, float_t ratio);
        void apply_physical(RenderTexture* rt, GLuint color_texture,
            GLuint depth_texture, float_t min_distance, float_t max_distance,
            float_t focus, float_t focal_length, float_t fov, float_t f_number);

        void render_tiles(GLuint depth_texture, bool f2);
        void downsample(GLuint color_texture, GLuint depth_texture, bool f2);
        void main_filter(bool f2);
        void upsample(RenderTexture* rt, GLuint color_texture, GLuint depth_texture, bool f2);

        void update_data(float_t min_dist, float_t max_dist, float_t fov, float_t dist, float_t focal_length,
            float_t f_number, float_t focus_range, float_t fuzzing_range, float_t ratio);
    };

    static_assert(sizeof(renderer::DOF3) == 0xD8, "\"renderer::DOF3\" struct should have a size of 0xD8");
}

extern dof_debug* dof_debug_data;
extern dof_pv* dof_pv_data;
