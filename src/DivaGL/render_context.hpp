/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "GL/array_buffer.hpp"
#include "GL/uniform_buffer.hpp"
#include "light_param/fog.hpp"
#include "light_param/light.hpp"
#include "mdl/disp_manager.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "render_manager.hpp"
#include "render_texture.hpp"

struct draw_state_stats {
    int32_t object_draw_count;
    int32_t object_translucent_draw_count;
    int32_t object_reflect_draw_count;
    int32_t field_C;
    int32_t field_10;
    int32_t draw_count;
    int32_t draw_triangle_count;
    int32_t field_1C;

    void reset();
};

static_assert(sizeof(draw_state_stats) == 0x20, "\"draw_state_stats\" struct should have a size of 0x20");

struct sss_data {
    bool init_data;
    bool enable;
    bool npr_contour;
    RenderTexture textures[4];
    vec4 param;

    void free();
    void init();
    void set_texture(int32_t);
};

static_assert(sizeof(sss_data) == 0xD8, "\"sss_data\" struct should have a size of 0xD8");

struct draw_state_struct {
    draw_state_stats stats;
    draw_state_stats stats_prev;
    bool vertex_buffer;
    bool wireframe;
    bool wireframe_overlay;
    bool shadow;
    bool self_shadow;
    bool shader_debug_flag;
    bool use_global_material;
    bool fog_height;
    bool back_display;
    int32_t shader_index;
    int32_t show;
    float_t bump_depth;
    float_t intensity;
    float_t reflectivity;
    float_t reflect_uv_scale;
    float_t refract_uv_scale;
    int32_t field_68;
    void* curr_obj_sub_mesh_args;
    float_t fresnel;
    GLuint program_buffer_index;
    void* program_buffer_matrix_data;

    void set_fog_height(bool value);
};

static_assert(sizeof(draw_state_struct) == 0x88, "\"draw_state_struct\" struct should have a size of 0x88");

struct camera_blur_shader_data {
    vec4 g_transform[4];
};

struct contour_coef_shader_data {
    vec4 g_contour;
    vec4 g_near_far;
};

struct contour_params_shader_data {
    vec4 g_near_far;
};

struct filter_scene_shader_data {
    vec4 g_transform;
    vec4 g_texcoord;
};

struct exposure_measure_shader_data {
    vec4 g_spot_weight;
    vec4 g_spot_coefficients[32];
};

struct gaussian_coef_shader_data {
    vec4 g_coef[8];
};

struct esm_filter_batch_shader_data {
    vec4 g_params;
    vec4 g_gauss[2];
};

struct imgfilter_batch_shader_data {
    vec4 g_texture_lod;
    vec4 g_color_scale;
    vec4 g_color_offset;
};

struct glass_eye_batch_shader_data {
    vec4 g_ellipsoid_radius;
    vec4 g_ellipsoid_scale;
    vec4 g_tex_model_param;
    vec4 g_tex_offset;
    vec4 g_eb_radius;
    vec4 g_eb_tex_model_param;
    vec4 g_fresnel;
    vec4 g_refract1;
    vec4 g_refract2;
    vec4 g_iris_radius;
    vec4 g_cornea_radius;
    vec4 g_pupil_radius;
    vec4 g_tex_scale;
};

struct glitter_batch_shader_data {
    vec4 g_mvp[4];
    vec4 g_glitter_blend_color;
    vec4 g_state_material_diffuse;
    vec4 g_state_material_emission;
};

struct obj_shader_shader_data {
    struct {
        union {
            struct {
                uint32_t alpha_mask    : 1; // bit 0
                uint32_t alpha_test    : 1; // bit 1
                uint32_t aniso         : 2; // bit 2:3
                uint32_t aet_back      : 1; // bit 4
                uint32_t texture_blend : 3; // bit 5:7
                uint32_t unk           : 2; // bit 8:9
                uint32_t chara_color   : 1; // bit 10
                uint32_t clip_plane    : 1; // bit 11
                uint32_t u08           : 1; // bit 12
                uint32_t depth_peel    : 1; // bit 13
                uint32_t depth         : 1; // bit 14
                uint32_t u0b           : 1; // bit 15
                uint32_t alpha_blend   : 3; // bit 16:18
                uint32_t ripple_emit   : 1; // bit 19
                uint32_t esm_filter    : 2; // bit 20:21
                uint32_t exposure      : 2; // bit 22:23
                uint32_t scene_fade    : 1; // bit 24
                uint32_t fade          : 3; // bit 25:27
                uint32_t stage_ambient : 1; // bit 28
                uint32_t flare         : 2; // bit 29:30
            } m;
            uint32_t w;
        } x;
        union {
            struct {
                uint32_t fog_stage     : 2; // bit 32+0:1
                uint32_t fog_chara     : 2; // bit 32+2:3
                uint32_t u16           : 1; // bit 32+4
                uint32_t gauss         : 2; // bit 32+5:6
                uint32_t eyeball       : 1; // bit 32+7
                uint32_t image_filter  : 3; // bit 32+8:10
                uint32_t instance      : 1; // bit 32+11
                uint32_t tone_curve    : 1; // bit 32+12
                uint32_t light_proj    : 1; // bit 32+13
                uint32_t magnify       : 4; // bit 32+14:17
                uint32_t membrane      : 2; // bit 32+18:19
                uint32_t mlaa          : 2; // bit 32+20:21
                uint32_t mlaa_search   : 2; // bit 32+22:23
                uint32_t morph_color   : 1; // bit 32+24
                uint32_t morph         : 1; // bit 32+25
                uint32_t movie         : 2; // bit 32+26:27
                uint32_t u24           : 2; // bit 32+28:29
                uint32_t u25           : 1; // bit 32+30
                uint32_t npr_normal    : 1; // bit 32+31
            } m;
            uint32_t w;
        } y;
        union {
            struct {
                uint32_t npr           : 1; // bit 64+0
                uint32_t stage_shadow2 : 2; // bit 64+1:2
                uint32_t reflect       : 2; // bit 64+3:4
                uint32_t reduce        : 4; // bit 64+5:8
                uint32_t chara_shadow  : 1; // bit 64+9
                uint32_t chara_shadow2 : 2; // bit 64+10:11
                uint32_t u2d           : 3; // bit 64+12:14
                uint32_t u2e           : 2; // bit 64+15:16
                uint32_t show_vector   : 2; // bit 64+17:18
                uint32_t skinning      : 1; // bit 64+19
                uint32_t snow_particle : 2; // bit 64+20:21
                uint32_t specular_ibl  : 2; // bit 64+22:23
                uint32_t combiner      : 1; // bit 64+24
                uint32_t tex_0_type    : 2; // bit 64+25:26
                uint32_t tex_1_type    : 2; // bit 64+27:28
                uint32_t sss_filter    : 2; // bit 64+29:30
            } m;
            uint32_t w;
        } z;
        union {
            struct {
                uint32_t sss_chara     : 1; // bit 96+0
                uint32_t star          : 1; // bit 96+1
                uint32_t texture_count : 2; // bit 96+2:3
                uint32_t env_map       : 1; // bit 96+4
                uint32_t ripple        : 2; // bit 96+5:6
                uint32_t translucency  : 1; // bit 96+7
                uint32_t normal        : 1; // bit 96+8
                uint32_t transparency  : 1; // bit 96+9
                uint32_t water_reflect : 1; // bit 96+10
                uint32_t u40           : 1; // bit 96+11
                uint32_t u41           : 1; // bit 96+12
                uint32_t stage_shadow  : 1; // bit 96+13
                uint32_t specular      : 1; // bit 96+14
                uint32_t tone_map      : 2; // bit 96+15
                uint32_t u45           : 1; // bit 96+16
            } m;
            uint32_t w;
        } w;
    } g_shader_flags;

    void set_shader_flags(int32_t* shader_flags);
};

struct obj_scene_shader_data {
    vec4 g_irradiance_r_transforms[4];
    vec4 g_irradiance_g_transforms[4];
    vec4 g_irradiance_b_transforms[4];
    vec4 g_light_env_stage_diffuse;
    vec4 g_light_env_stage_specular;
    vec4 g_light_env_chara_diffuse;
    vec4 g_light_env_chara_ambient;
    vec4 g_light_env_chara_specular;
    vec4 g_light_env_reflect_diffuse;
    vec4 g_light_env_reflect_ambient;
    vec4 g_light_env_proj_diffuse;
    vec4 g_light_env_proj_specular;
    vec4 g_light_env_proj_position;
    vec4 g_light_stage_dir;
    vec4 g_light_stage_diff;
    vec4 g_light_stage_spec;
    vec4 g_light_chara_dir;
    vec4 g_light_chara_spec;
    vec4 g_light_chara_luce;
    vec4 g_light_chara_back;
    vec4 g_light_face_diff;
    vec4 g_chara_color_rim;
    vec4 g_chara_color0;
    vec4 g_chara_color1;
    vec4 g_chara_f_dir;
    vec4 g_chara_f_ambient;
    vec4 g_chara_f_diffuse;
    vec4 g_chara_tc_param;
    vec4 g_fog_depth_color;
    vec4 g_fog_height_params; //x=density, y=start, z=end, w=1/(end-start)
    vec4 g_fog_height_color;
    vec4 g_fog_bump_params; //x=density, y=start, z=end, w=1/(end-start)
    vec4 g_fog_state_params; //x=density, y=start, z=end, w=1/(end-start)
    vec4 g_normal_tangent_transforms[3];
    vec4 g_esm_param;
    vec4 g_self_shadow_receivers[6];
    vec4 g_shadow_ambient;
    vec4 g_shadow_ambient1;
    vec4 g_framebuffer_size;
    vec4 g_light_reflect_dir;
    vec4 g_clip_plane;
    vec4 g_npr_cloth_spec_color;
    vec4 g_view[3];
    vec4 g_view_inverse[3];
    vec4 g_projection_view[4];
    vec4 g_view_position;
    vec4 g_light_projection[4];

    void set_g_irradiance_r_transforms(const mat4& mat);
    void set_g_irradiance_g_transforms(const mat4& mat);
    void set_g_irradiance_b_transforms(const mat4& mat);
    void set_g_normal_tangent_transforms(const mat4& mat);
    void set_g_self_shadow_receivers(int32_t index, const mat4& mat);
    void set_g_light_projection(const mat4& mat);

    void set_projection_view(const mat4& view, const mat4& proj);
};

struct obj_batch_shader_data {
    vec4 g_transforms[4];
    vec4 g_worlds[3];
    vec4 g_worlds_invtrans[3];
    vec4 g_worldview[3];
    vec4 g_worldview_inverse[3];
    vec4 g_joint[3];
    vec4 g_joint_inverse[3];
    vec4 g_texcoord_transforms[4];
    vec4 g_blend_color;
    vec4 g_offset_color;
    vec4 g_material_state_diffuse;
    vec4 g_material_state_ambient;
    vec4 g_material_state_emission;
    vec4 g_material_state_shininess;
    vec4 g_material_state_specular;
    vec4 g_fresnel_coefficients;
    vec4 g_texture_color_coefficients;
    vec4 g_texture_color_offset;
    vec4 g_texture_specular_coefficients;
    vec4 g_texture_specular_offset;
    vec4 g_shininess;
    vec4 g_max_alpha;
    vec4 g_morph_weight;
    vec4 g_sss_param;
    vec4 g_bump_depth;
    vec4 g_intensity;
    vec4 g_reflect_uv_scale;

    void set_g_joint(const mat4& mat);
    void set_g_texcoord_transforms(int32_t index, const mat4& mat);

    void set_transforms(const mat4& model, const mat4& view, const mat4& proj);
};

struct obj_skinning_shader_data {
    vec4 g_joint_transforms[768];
};

struct quad_shader_data {
    vec4 g_texcoord_modifier;
    vec4 g_texel_size;
    vec4 g_color;
    vec4 g_texture_lod;
};

struct sprite_scene_shader_data {
    mat4 g_transform;
    vec4 g_framebuffer_size;
};

struct sss_filter_gaussian_coef_shader_data {
    vec4 g_param;
    vec4 g_coef[64];
};

struct sun_quad_shader_data {
    vec4 g_transform[4];
    vec4 g_emission;
};

struct tone_map_shader_data {
    vec4 g_exposure;
    vec4 g_flare_coef; //xy=flare_coef, z=light_proj
    vec4 g_fade_color;
    vec4 g_tone_scale; //xyz=tone_scale, w=fade_func
    vec4 g_tone_offset; //xyz=tone_offset, w=inv_tone
    vec4 g_texcoord_transforms[8];
};

struct transparency_batch_shader_data {
    vec4 g_opacity;
};

struct render_context {
    mat4 view_mat;
    mat4 proj_mat;
    mat4 vp_mat;
    vec4 g_near_far;

    GLuint box_vao;
    GL::ArrayBuffer box_vbo;

    GLuint lens_ghost_vao;
    GL::ArrayBuffer lens_ghost_vbo;

    GLuint common_vao;

    RenderTexture reflect_buffer;
    RenderTexture render_buffer;
    RenderTexture screen_buffer;
    RenderTexture shadow_buffer;

    GL::UniformBuffer camera_blur_ubo;
    GL::UniformBuffer contour_coef_ubo;
    GL::UniformBuffer contour_params_ubo;
    GL::UniformBuffer filter_scene_ubo;
    GL::UniformBuffer esm_filter_batch_ubo;
    GL::UniformBuffer imgfilter_batch_ubo;
    GL::UniformBuffer exposure_measure_ubo;
    GL::UniformBuffer gaussian_coef_ubo;
    GL::UniformBuffer glass_eye_batch_ubo;
    GL::UniformBuffer glitter_batch_ubo;
    GL::UniformBuffer quad_ubo;
    GL::UniformBuffer sprite_scene_ubo;
    GL::UniformBuffer sss_filter_gaussian_coef_ubo;
    GL::UniformBuffer sun_quad_ubo;
    GL::UniformBuffer tone_map_ubo;
    GL::UniformBuffer transparency_batch_ubo;

    obj_shader_shader_data obj_shader;
    obj_scene_shader_data obj_scene;
    obj_batch_shader_data obj_batch;
    obj_skinning_shader_data obj_skinning;
    GL::UniformBuffer obj_shader_ubo;
    GL::UniformBuffer obj_scene_ubo;
    GL::UniformBuffer obj_batch_ubo;
    GL::UniformBuffer obj_skinning_ubo;

    texture* empty_texture_2d;
    texture* empty_texture_cube_map;

    GLuint samplers[18];
    GLuint render_samplers[4];
    GLuint sprite_samplers[3];

    int32_t screen_width;
    int32_t screen_height;

    mdl::ObjList obj_local[mdl::OBJ_TYPE_LOCAL_MAX - mdl::OBJ_TYPE_MAX];

    render_context();
    ~render_context();

    void ctrl(bool change_res = true);
    void free();
    void init();
};

extern draw_state_struct& draw_state;

extern render_context* rctx;

extern sss_data* sss_data_get();
