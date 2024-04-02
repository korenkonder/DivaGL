/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"
#include "renderer/dof.hpp"
#include "renderer/transparency.hpp"
#include "camera.hpp"
#include "gl_uniform_buffer.hpp"

enum tone_map_method {
    TONE_MAP_YCC_EXPONENT = 0,
    TONE_MAP_RGB_LINEAR   = 1,
    TONE_MAP_RGB_LINEAR2  = 2,
    TONE_MAP_MAX          = 3,
};

namespace rndr {
    struct Render {
        enum MagFilterType {
            MAG_FILTER_NEAREST = 0,
            MAG_FILTER_BILINEAR,
            MAG_FILTER_SHARPEN_5_TAP,
            MAG_FILTER_SHARPEN_4_TAP,
            MAG_FILTER_CONE_4_TAP,
            MAG_FILTER_CONE_2_TAP,
            MAG_FILTER_MAX,
        };

        enum FrameTextureType {
            FRAME_TEXTURE_PRE_PP = 0,
            FRAME_TEXTURE_POST_PP,
            FRAME_TEXTURE_FB,
            FRAME_TEXTURE_MAX,
        };

        struct ExposureCharaData {
            vec4 spot_coefficients[8];
            float_t spot_weight;
            GLuint query[3];
            GLuint query_data[3];

            void reset();
        };

        struct FrameTextureData {
            texture* texture;
            RenderTexture render_texture;
            FrameTextureType type;
        };

        struct FrameTexture {
            FrameTextureData data[4];
            bool capture;
        };

        int32_t downsample_count;
        int32_t downsample_max_count;
        int32_t ssaa;
        int32_t taa;
        int32_t mlaa;
        int32_t cam_blur;
        int32_t hd_res;
        int32_t base_downsample;
        int32_t ss_alpha_mask;
        RenderTexture rend_texture[5];
        RenderTexture taa_buffer[3];
        texture* taa_tex[3];
        RenderTexture aet_back_texture;
        texture* aet_back_tex;
        RenderTexture downsample_texture;
        RenderTexture reduce_texture[5];
        texture* reduce_tex[5];
        RenderTexture field_340;
        GLuint reduce_tex_draw;
        RenderTexture exposure_texture;
        GLuint exposure_tex;
        texture* exposure_history;
        RenderTexture field_3B8;
        RenderTexture field_3E8;
        GLuint tonemap_lut_texture;
        RenderTexture mlaa_buffer;
        RenderTexture temp_buffer;
        GLuint mlaa_area_texture;
        RenderTexture* sss_contour_texture;
        int32_t texture_counter;
        GLuint lens_shaft_query[3];
        GLuint lens_flare_query[3];
        GLuint lens_shaft_query_data[3];
        GLuint lens_flare_query_data[3];
        int32_t lens_flare_query_index;
        GLuint lens_flare_texture;
        GLuint lens_shaft_texture;
        GLuint lens_ghost_texture;
        int32_t lens_ghost_count;
        int32_t width;
        int32_t height;
        int32_t inner_width;
        int32_t inner_height;
        int32_t render_width[5];
        int32_t render_height[5];
        int32_t render_post_width[5];
        int32_t render_post_height[5];
        float_t render_post_width_scale;
        float_t render_post_height_scale;
        int32_t reduce_width[5];
        int32_t reduce_height[5];
        int32_t taa_texture_selector;
        int32_t taa_texture;
        float_t taa_blend;
        vec3 view_point;
        vec3 interest;
        vec3 view_point_prev;
        vec3 interest_prev;
        mat4 cam_view_projection;
        mat4 cam_view_projection_prev;
        int32_t stage_index;
        int32_t stage_index_prev;
        bool reset_exposure;
        int32_t screen_x_offset;
        int32_t screen_y_offset;
        int32_t screen_width;
        int32_t screen_height;
        int32_t update_lut;
        int32_t saturate_lock;
        ExposureCharaData exposure_chara_data[6];
        int32_t exposure_history_counter;
        int32_t exposure_query_index;
        vec3 lens_flare_pos;
        float_t lens_shaft_scale;
        float_t lens_shaft_inv_scale;
        float_t lens_flare_power;
        float_t field_A10;
        float_t lens_flare_appear_power;
        texture* render_textures_data[16];
        RenderTexture render_textures[16];
        texture* movie_textures_data[1];
        RenderTexture movie_textures[1];
        int32_t aet_back;
        renderer::DOF3* dof;
        texture* transparency_tex[1];
        renderer::Transparency* transparency;
        int32_t saturate_index;
        int32_t scene_fade_index;
        int32_t tone_trans_index;
        float_t saturate_coeff[2];
        vec3 scene_fade_color[2];
        float_t scene_fade_alpha[2];
        int32_t scene_fade_blend_func[2];
        vec3 tone_trans_scale[2];
        vec3 tone_trans_offset[2];
        vec3 tone_trans_start[2];
        vec3 tone_trans_end[2];
        tone_map_method tone_map;
        float_t exposure;
        float_t exposure_rate;
        int32_t auto_exposure;
        float_t gamma;
        float_t gamma_rate;
        int32_t saturate_power;
        MagFilterType mag_filter;
        float_t fade_alpha;
        int32_t fade;
        float_t lens_flare;
        float_t lens_shaft;
        float_t lens_ghost;
        vec3 radius;
        vec3 intensity;
        int32_t update;
        FrameTexture frame_texture[6];

        void apply_post_process();
        void bind_render_texture(bool aet_back = false);
        void calc_exposure_chara_data();
        void calc_projection_matrix(mat4* mat, float_t fov, float_t aspect, float_t z_near, float_t z_far,
            float_t left_scale, float_t right_scale, float_t bottom_scale, float_t top_scale);
        void ctrl();
        void draw_quad(int32_t width, int32_t height, float_t s0, float_t t0, float_t s1, float_t t1,
            float_t scale, float_t param_x, float_t param_y, float_t param_z, float_t param_w);
        void draw_lens_flare();
        void free();
        void init_render_buffers(int32_t width, int32_t height,
            int32_t ssaa, int32_t hd_res, int32_t ss_alpha_mask);
        void init_tone_map_buffers();
        int32_t movie_texture_set(texture* movie_texture);
        void movie_texture_free(texture* movie_texture);
        int32_t render_texture_set(texture* render_texture, bool task_photo);
        void render_texture_free(texture* render_texture, bool task_photo);
        void resize(int32_t width, int32_t height);
        void set_screen_res(int32_t x_offset, int32_t y_offset, int32_t width, int32_t height);
        void take_ss(texture* tex, bool vertical, float_t horizontal_offset);
        void transparency_combine(float_t alpha);
        void transparency_copy();
        void update_res(bool set, int32_t base_downsample);

    private:
        void apply_mlaa(int32_t destination, int32_t source, int32_t mlaa);
        void apply_tone_map();
        void calc_exposure();
        void calc_gaussian_blur(float_t start, float_t step,
            int32_t kernel_size, float_t radius_scale, float_t intensity_scale);
        void calc_taa_blend();
        void copy_to_frame_texture(GLuint pre_pp_tex, int32_t wight, int32_t height, GLuint post_pp_tex);
        void draw_lens_ghost();
        void downsample();
        void get_blur();
        void update_tone_map_lut();
    };

    static_assert(sizeof(rndr::Render) == 0x1510, "\"rndr::Render\" struct should have a size of 0x1510");
}

extern rndr::Render* (FASTCALL* render_get)();
