/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "light_param/fog.hpp"
#include "light_param/light.hpp"
#include "mdl/disp_manager.hpp"
#include "camera.hpp"
#include "gl_array_buffer.hpp"
#include "gl_element_array_buffer.hpp"
#include "gl_uniform_buffer.hpp"
#include "render.hpp"
#include "render_manager.hpp"
#include "render_texture.hpp"

#define TEXTURE_PATTERN_COUNT 24
#define TEXTURE_TRANSFORM_COUNT 24

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

struct sss_data {
    bool init;
    bool enable;
    bool npr_contour;
    RenderTexture textures[4];
    vec4 param;
};

struct draw_state_struct {
    draw_state_stats stats;
    draw_state_stats stats_prev;
    bool vertex_buffer;
    bool wireframe;
    bool wireframe_overlay;
    bool light;
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
    vec4 g_texture_blend;

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
    vec4 g_flare_coef;
    vec4 g_fade_color;
    vec4 g_tone_scale; //xyz=tone_scale, w=fade_func
    vec4 g_tone_offset; //xyz=tone_offset, w=inv_tone
    vec4 g_texcoord_transforms[8];
};

struct render_context {
    mat4 view_mat;
    mat4 proj_mat;
    mat4 vp_mat;
    vec4 g_near_far;

    GLuint box_vao;
    GLArrayBuffer box_vbo;

    GLuint lens_ghost_vao;
    GLArrayBuffer lens_ghost_vbo;

    GLuint common_vao;

    RenderTexture reflect_buffer;
    RenderTexture render_buffer;
    RenderTexture screen_buffer;
    RenderTexture shadow_buffer;

    GLUniformBuffer contour_coef_ubo;
    GLUniformBuffer contour_params_ubo;
    GLUniformBuffer filter_scene_ubo;
    GLUniformBuffer esm_filter_batch_ubo;
    GLUniformBuffer imgfilter_batch_ubo;
    GLUniformBuffer exposure_measure_ubo;
    GLUniformBuffer gaussian_coef_ubo;
    GLUniformBuffer glass_eye_batch_ubo;
    GLUniformBuffer glitter_batch_ubo;
    GLUniformBuffer quad_ubo;
    GLUniformBuffer sprite_scene_ubo;
    GLUniformBuffer sss_filter_gaussian_coef_ubo;
    GLUniformBuffer sun_quad_ubo;
    GLUniformBuffer transparency_batch_ubo;
    GLUniformBuffer tone_map_ubo;

    obj_scene_shader_data obj_scene;
    obj_batch_shader_data obj_batch;
    obj_skinning_shader_data obj_skinning;
    GLUniformBuffer obj_scene_ubo;
    GLUniformBuffer obj_batch_ubo;
    GLUniformBuffer obj_skinning_ubo;

    GLuint empty_texture_2d;
    GLuint empty_texture_cube_map;

    GLuint samplers[18];
    GLuint render_samplers[3];
    GLuint sprite_samplers[3];

    int32_t sprite_width;
    int32_t sprite_height;
    int32_t screen_x_offset;
    int32_t screen_y_offset;
    int32_t screen_width;
    int32_t screen_height;

    render_context();
    ~render_context();

    void ctrl(bool change_res = true);
    void free();
    void init();
};

extern draw_state_struct* draw_state;

extern render_context* rctx;

extern sss_data* (FASTCALL* sss_data_get)();

void fbo_blit(GLuint src_fbo, GLuint dst_fbo,
    GLint src_x, GLint src_y, GLint src_width, GLint src_height,
    GLint dst_x, GLint dst_y, GLint dst_width, GLint dst_height, GLbitfield mask, GLenum filter);
