/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"
#include "render.hpp"
#include "shadow.hpp"
#include "types.hpp"
#include "wrap.hpp"

enum draw_pass_3d_type {
    DRAW_PASS_3D_OPAQUE = 0,
    DRAW_PASS_3D_TRANSLUCENT,
    DRAW_PASS_3D_TRANSPARENT,
    DRAW_PASS_3D_MAX,
};

enum reflect_refract_resolution_mode {
    REFLECT_REFRACT_RESOLUTION_256x256 = 0,
    REFLECT_REFRACT_RESOLUTION_512x256,
    REFLECT_REFRACT_RESOLUTION_512x512,
    REFLECT_REFRACT_RESOLUTION_MAX,
};

enum stage_data_reflect_type {
    STAGE_DATA_REFLECT_DISABLE     = 0x00,
    STAGE_DATA_REFLECT_NORMAL      = 0x01,
    STAGE_DATA_REFLECT_REFLECT_MAP = 0x02,
};

struct draw_pre_process {
    int32_t type;
    void(FASTCALL* func)(void*);
    void* data;
};

namespace rndr {
    enum RenderPassID {
        RND_PASSID_SHADOW = 0,
        RND_PASSID_SS_SSS,
        RND_PASSID_2,
        RND_PASSID_REFLECT,
        RND_PASSID_REFRACT,
        RND_PASSID_PRE_PROCESS,
        RND_PASSID_CLEAR,
        RND_PASSID_PRE_SPRITE,
        RND_PASSID_3D,
        RND_PASSID_SHOW_VECTOR,
        RND_PASSID_POST_PROCESS,
        RND_PASSID_SPRITE,
        RND_PASSID_12,
        RND_PASSID_NUM,
    };

    struct RenderManager {
        bool pass_sw[RND_PASSID_NUM];
        struct Shadow* shadow_ptr;
        bool reflect;
        bool refract;
        int32_t reflect_blur_num;
        blur_filter_mode reflect_blur_filter;
        rndr::Render* render;
        bool sync_gpu;
        double_t cpu_time[RND_PASSID_NUM];
        double_t gpu_time[RND_PASSID_NUM];
        time_struct time;
        bool shadow;
        bool opaque_z_sort;
        bool alpha_z_sort;
        bool draw_pass_3d[DRAW_PASS_3D_MAX];
        bool field_11E;
        bool field_11F;
        bool field_120;
        bool show_ref_map;
        stage_data_reflect_type reflect_type;
        bool clear;
        int32_t tex_index[11];
        RenderTexture render_textures[8];
        int32_t width;
        int32_t height;
        GLuint multisample_framebuffer;
        GLuint multisample_renderbuffer;
        bool multisample;
        bool check_state;
        int32_t show_vector_flags;
        float_t show_vector_length;
        float_t show_vector_z_offset;
        bool show_stage_shadow;
        prj::list<draw_pre_process> pre_process;
        texture* effect_texture;
        int32_t npr_param;
        bool field_31C;
        bool field_31D;
        bool field_31E;
        bool field_31F;
        bool field_320;
        bool npr;

        void add_pre_process(int32_t type, void(*func)(void*), void* data);
        void clear_pre_process(int32_t type);
        RenderTexture& get_render_texture(int32_t index);
        void resize(int32_t width, int32_t height);
        void set_clear(bool value);
        void set_effect_texture(texture* value);
        void set_multisample(bool value);
        void set_npr_param(int32_t value);
        void set_pass_sw(rndr::RenderPassID id, bool value);
        void set_reflect(bool value);
        void set_reflect_blur(int32_t reflect_blur_num, blur_filter_mode reflect_blur_filter);
        void set_reflect_resolution_mode(reflect_refract_resolution_mode mode);
        void set_reflect_type(stage_data_reflect_type type);
        void set_refract(bool value);
        void set_refract_resolution_mode(reflect_refract_resolution_mode mode);
        void set_shadow_false();
        void set_shadow_true();

        void render_all();

        void render_single_pass(rndr::RenderPassID id);

        void render_pass_begin();
        void render_pass_end(rndr::RenderPassID id);

        void pass_shadow();
        void pass_ss_sss();
        void pass_reflect();
        void pass_refract();
        void pass_pre_process();
        void pass_clear();
        void pass_pre_sprite();
        void pass_3d();
        void pass_show_vector();
        void pass_post_process();
        void pass_sprite();

        void pass_3d_contour();
        void pass_sprite_surf();
    };

    static_assert(sizeof(rndr::RenderManager) == 0x328, "\"rndr::RenderManager\" struct should have a size of 0x328");
}

extern RenderTexture* litproj_shadow;
extern RenderTexture* litproj_texture;
extern rndr::RenderManager* render_manager;

extern void image_filter_scale(GLuint dst, GLuint src, const vec4 scale);

extern void draw_pass_set_camera();

extern void render_manager_patch();
