/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_context.hpp"
#include "render_manager.hpp"
#include "uniform.hpp"

draw_state_struct& draw_state = *(draw_state_struct*)0x00000001411A32B0;

sss_data& _sss_data = *(sss_data*)0x000000014CC924E0;

render_context* rctx;

void draw_state_stats::reset() {
    object_draw_count = 0;
    object_translucent_draw_count = 0;
    object_reflect_draw_count = 0;
    field_C = 0;
    field_10 = 0;
    draw_count = 0;
    draw_triangle_count = 0;
    field_1C = 0;
}

void sss_data::free() {
    if (init_data)
        for (RenderTexture& i : textures)
            i.Free();
}

void sss_data::init() {
    if (init_data)
        return;

    textures[0].Init(640, 360, 0, GL_RGBA16F, GL_ZERO /*GL_DEPTH_COMPONENT32F*/);
    textures[1].Init(320, 180, 0, GL_RGBA16F, GL_ZERO);
    textures[2].Init(320, 180, 0, GL_RGBA16F, GL_ZERO);
    textures[3].Init(320, 180, 0, GL_RGBA16F, GL_ZERO);

    param = { 0.0f, 0.0f, 0.0f, 1.0f };
    init_data = true;
}

void sss_data::set_texture(int32_t texture_index) {
    gl_state_active_bind_texture_2d(16, textures[texture_index].GetColorTex());
    gl_state_active_texture(0);
}

void draw_state_struct::set_fog_height(bool value) {
    fog_height = value;
}

void obj_shader_shader_data::set_shader_flags(int32_t* shader_flags) {
    g_shader_flags.x.m.alpha_mask    = shader_flags[U_ALPHA_MASK];
    g_shader_flags.x.m.alpha_test    = shader_flags[U_ALPHA_TEST];
    g_shader_flags.x.m.aniso         = shader_flags[U_ANISO];
    g_shader_flags.x.m.aet_back      = shader_flags[U_AET_BACK];
    g_shader_flags.x.m.texture_blend = shader_flags[U_TEXTURE_BLEND];
    g_shader_flags.x.m.unk           = 0; //shader_flags[U_UNK_NEW_IN_MM_IDK_WHAT_TO_DO_WITH_IT];
    g_shader_flags.x.m.chara_color   = shader_flags[U_CHARA_COLOR];
    g_shader_flags.x.m.clip_plane    = shader_flags[U_CLIP_PLANE];
    g_shader_flags.x.m.u08           = shader_flags[U08];
    g_shader_flags.x.m.depth_peel    = shader_flags[U_DEPTH_PEEL];
    g_shader_flags.x.m.depth         = shader_flags[U_DEPTH];
    g_shader_flags.x.m.u0b           = shader_flags[U0B];
    g_shader_flags.x.m.alpha_blend   = shader_flags[U_ALPHA_BLEND];
    g_shader_flags.x.m.ripple_emit   = shader_flags[U_RIPPLE_EMIT];
    g_shader_flags.x.m.esm_filter    = shader_flags[U_ESM_FILTER];
    g_shader_flags.x.m.exposure      = shader_flags[U_EXPOSURE];
    g_shader_flags.x.m.scene_fade    = shader_flags[U_SCENE_FADE];
    g_shader_flags.x.m.fade          = shader_flags[U_FADE];
    g_shader_flags.x.m.stage_ambient = shader_flags[U_STAGE_AMBIENT];
    g_shader_flags.x.m.flare         = shader_flags[U_FLARE];
    g_shader_flags.y.m.fog_stage     = shader_flags[U_FOG_STAGE];
    g_shader_flags.y.m.fog_chara     = shader_flags[U_FOG_CHARA];
    g_shader_flags.y.m.u16           = shader_flags[U16];
    g_shader_flags.y.m.gauss         = shader_flags[U_GAUSS];
    g_shader_flags.y.m.eyeball       = shader_flags[U_EYEBALL];
    g_shader_flags.y.m.image_filter  = shader_flags[U_IMAGE_FILTER];
    g_shader_flags.y.m.instance      = shader_flags[U_INSTANCE];
    g_shader_flags.y.m.tone_curve    = shader_flags[U_TONE_CURVE];
    g_shader_flags.y.m.light_proj    = shader_flags[U_LIGHT_PROJ];
    g_shader_flags.y.m.magnify       = shader_flags[U_MAGNIFY];
    g_shader_flags.y.m.membrane      = shader_flags[U_MEMBRANE];
    g_shader_flags.y.m.mlaa          = shader_flags[U_MLAA];
    g_shader_flags.y.m.mlaa_search   = shader_flags[U_MLAA_SEARCH];
    g_shader_flags.y.m.morph_color   = shader_flags[U_MORPH_COLOR];
    g_shader_flags.y.m.morph         = shader_flags[U_MORPH];
    g_shader_flags.y.m.movie         = shader_flags[U_MOVIE];
    g_shader_flags.y.m.u24           = shader_flags[U24];
    g_shader_flags.y.m.u25           = shader_flags[U25];
    g_shader_flags.y.m.npr_normal    = shader_flags[U_NPR_NORMAL];
    g_shader_flags.z.m.npr           = shader_flags[U_NPR];
    g_shader_flags.z.m.stage_shadow2 = shader_flags[U_STAGE_SHADOW2];
    g_shader_flags.z.m.reflect       = shader_flags[U_REFLECT];
    g_shader_flags.z.m.reduce        = shader_flags[U_REDUCE];
    g_shader_flags.z.m.chara_shadow  = shader_flags[U_CHARA_SHADOW];
    g_shader_flags.z.m.chara_shadow2 = shader_flags[U_CHARA_SHADOW2];
    g_shader_flags.z.m.u2d           = shader_flags[U2D];
    g_shader_flags.z.m.u2e           = shader_flags[U2E];
    g_shader_flags.z.m.show_vector   = shader_flags[U_SHOW_VECTOR];
    g_shader_flags.z.m.skinning      = shader_flags[U_SKINNING];
    g_shader_flags.z.m.snow_particle = shader_flags[U_SNOW_PARTICLE];
    g_shader_flags.z.m.specular_ibl  = shader_flags[U_SPECULAR_IBL];
    g_shader_flags.z.m.combiner      = shader_flags[U_COMBINER];
    g_shader_flags.z.m.tex_0_type    = shader_flags[U_TEX_0_TYPE];
    g_shader_flags.z.m.tex_1_type    = shader_flags[U_TEX_1_TYPE];
    g_shader_flags.z.m.sss_filter    = shader_flags[U_SSS_FILTER];
    g_shader_flags.w.m.sss_chara     = shader_flags[U_SSS_CHARA];
    g_shader_flags.w.m.star          = shader_flags[U_STAR];
    g_shader_flags.w.m.texture_count = shader_flags[U_TEXTURE_COUNT];
    g_shader_flags.w.m.env_map       = shader_flags[U_ENV_MAP];
    g_shader_flags.w.m.ripple        = shader_flags[U_RIPPLE];
    g_shader_flags.w.m.translucency  = shader_flags[U_TRANSLUCENCY];
    g_shader_flags.w.m.normal        = shader_flags[U_NORMAL];
    g_shader_flags.w.m.transparency  = shader_flags[U_TRANSPARENCY];
    g_shader_flags.w.m.water_reflect = shader_flags[U_WATER_REFLECT];
    g_shader_flags.w.m.u40           = shader_flags[U40];
    g_shader_flags.w.m.u41           = shader_flags[U41];
    g_shader_flags.w.m.stage_shadow  = shader_flags[U_STAGE_SHADOW];
    g_shader_flags.w.m.specular      = shader_flags[U_SPECULAR];
    g_shader_flags.w.m.tone_map      = shader_flags[U_TONE_MAP];
    g_shader_flags.w.m.u45           = shader_flags[U45];
    g_shader_flags.w.m.dof           = 0; //shader_flags[U_DOF]; // Added
    g_shader_flags.w.m.dof_stage     = 0; //shader_flags[U_DOF_STAGE]; // Added
}

void obj_scene_shader_data::set_g_irradiance_r_transforms(const mat4& mat) {
    mat4 temp;
    mat4_transpose(&mat, &temp);
    g_irradiance_r_transforms[0] = temp.row0;
    g_irradiance_r_transforms[1] = temp.row1;
    g_irradiance_r_transforms[2] = temp.row2;
    g_irradiance_r_transforms[3] = temp.row3;
}

void obj_scene_shader_data::set_g_irradiance_g_transforms(const mat4& mat) {
    mat4 temp;
    mat4_transpose(&mat, &temp);
    g_irradiance_g_transforms[0] = temp.row0;
    g_irradiance_g_transforms[1] = temp.row1;
    g_irradiance_g_transforms[2] = temp.row2;
    g_irradiance_g_transforms[3] = temp.row3;
}

void obj_scene_shader_data::set_g_irradiance_b_transforms(const mat4& mat) {
    mat4 temp;
    mat4_transpose(&mat, &temp);
    g_irradiance_b_transforms[0] = temp.row0;
    g_irradiance_b_transforms[1] = temp.row1;
    g_irradiance_b_transforms[2] = temp.row2;
    g_irradiance_b_transforms[3] = temp.row3;
}

void obj_scene_shader_data::set_g_normal_tangent_transforms(const mat4& mat) {
    mat4 temp;
    mat4_transpose(&mat, &temp);
    g_normal_tangent_transforms[0] = temp.row0;
    g_normal_tangent_transforms[1] = temp.row1;
    g_normal_tangent_transforms[2] = temp.row2;
}

void obj_scene_shader_data::set_g_self_shadow_receivers(int32_t index, const mat4& mat) {
    size_t _index = index * 3LL;

    mat4 temp;
    mat4_transpose(&mat, &temp);
    g_self_shadow_receivers[_index + 0] = temp.row0;
    g_self_shadow_receivers[_index + 1] = temp.row1;
    g_self_shadow_receivers[_index + 2] = temp.row2;
}

void obj_scene_shader_data::set_g_light_projection(const mat4& mat) {
    mat4 temp;
    mat4_transpose(&mat, &temp);
    g_light_projection[0] = temp.row0;
    g_light_projection[1] = temp.row1;
    g_light_projection[2] = temp.row2;
    g_light_projection[3] = temp.row3;
}

void obj_scene_shader_data::set_projection_view(const mat4& view, const mat4& proj) {
    mat4 temp;
    mat4_transpose(&view, &temp);
    g_view[0] = temp.row0;
    g_view[1] = temp.row1;
    g_view[2] = temp.row2;

    mat4_invert(&view, &temp);
    mat4_transpose(&temp, &temp);
    g_view_inverse[0] = temp.row0;
    g_view_inverse[1] = temp.row1;
    g_view_inverse[2] = temp.row2;

    mat4_mul(&view, &proj, &temp);
    mat4_transpose(&temp, &temp);
    g_projection_view[0] = temp.row0;
    g_projection_view[1] = temp.row1;
    g_projection_view[2] = temp.row2;
    g_projection_view[3] = temp.row3;
}

void obj_batch_shader_data::set_g_joint(const mat4& mat) {
    mat4 temp;
    mat4_transpose(&mat, &temp);
    g_joint[0] = temp.row0;
    g_joint[1] = temp.row1;
    g_joint[2] = temp.row2;

    mat4_invert(&mat, &temp);
    mat4_transpose(&temp, &temp);
    g_joint_inverse[0] = temp.row0;
    g_joint_inverse[1] = temp.row1;
    g_joint_inverse[2] = temp.row2;
}

void obj_batch_shader_data::set_g_texcoord_transforms(int32_t index, const mat4& mat) {
    size_t _index = index * 2LL;

    mat4 temp;
    mat4_transpose(&mat, &temp);
    g_texcoord_transforms[_index + 0] = temp.row0;
    g_texcoord_transforms[_index + 1] = temp.row1;
}

void obj_batch_shader_data::set_transforms(const mat4& model, const mat4& view, const mat4& proj) {
    mat4 temp;
    mat4_transpose(&model, &temp);
    g_worlds[0] = temp.row0;
    g_worlds[1] = temp.row1;
    g_worlds[2] = temp.row2;

    mat4_invert(&model, &temp);
    g_worlds_invtrans[0] = temp.row0;
    g_worlds_invtrans[1] = temp.row1;
    g_worlds_invtrans[2] = temp.row2;

    mat4 mv;
    mat4_mul(&model, &view, &mv);

    mat4_transpose(&mv, &temp);
    g_worldview[0] = temp.row0;
    g_worldview[1] = temp.row1;
    g_worldview[2] = temp.row2;

    mat4_invert(&mv, &temp);
    mat4_transpose(&temp, &temp);
    g_worldview_inverse[0] = temp.row0;
    g_worldview_inverse[1] = temp.row1;
    g_worldview_inverse[2] = temp.row2;

    mat4_mul(&mv, &proj, &temp);
    mat4_transpose(&temp, &temp);
    g_transforms[0] = temp.row0;
    g_transforms[1] = temp.row1;
    g_transforms[2] = temp.row2;
    g_transforms[3] = temp.row3;
}

render_context::render_context() : box_vao(), lens_ghost_vao(), common_vao(), reflect_buffer(),
render_buffer(), screen_buffer(), shadow_buffer(), empty_texture_2d(), empty_texture_cube_map(),
samplers(), render_samplers(), sprite_samplers(), screen_width(), screen_height() {
    static const float_t box_texcoords[] = {
         1.0f,  0.0f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f, -1.5f, -1.5f,  0.5f,  1.5f, -0.5f, -0.5f,  1.5f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f, -1.5f, -1.5f,  0.5f,  1.5f, -0.5f, -0.5f,  1.5f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f, -1.5f, -1.5f,  0.5f,  1.5f, -0.5f, -0.5f,  1.5f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f, -1.5f, -1.5f,  0.5f,  1.5f, -0.5f, -0.5f,  1.5f,
         0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         3.5f, -3.5f, -1.5f, -3.5f,  1.5f, -1.5f, -3.5f, -1.5f,
         3.5f,  1.5f, -1.5f,  1.5f,  1.5f,  3.5f, -3.5f,  3.5f,
         3.5f, -3.5f, -1.5f, -3.5f,  1.5f, -1.5f, -3.5f, -1.5f,
         3.5f,  1.5f, -1.5f,  1.5f,  1.5f,  3.5f, -3.5f,  3.5f,
         3.5f, -3.5f, -1.5f, -3.5f,  1.5f, -1.5f, -3.5f, -1.5f,
         3.5f,  1.5f, -1.5f,  1.5f,  1.5f,  3.5f, -3.5f,  3.5f,
         3.5f, -3.5f, -1.5f, -3.5f,  1.5f, -1.5f, -3.5f, -1.5f,
         3.5f,  1.5f, -1.5f,  1.5f,  1.5f,  3.5f, -3.5f,  3.5f,
    };

    glGenVertexArrays(1, &box_vao);
    gl_state_bind_vertex_array(box_vao);

    box_vbo.Create(sizeof(box_texcoords), box_texcoords);
    box_vbo.Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float_t) * 16, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float_t) * 16, (void*)(sizeof(float_t) * 4));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float_t) * 16, (void*)(sizeof(float_t) * 8));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float_t) * 16, (void*)(sizeof(float_t) * 12));

    glGenVertexArrays(1, &lens_ghost_vao);
    gl_state_bind_vertex_array(lens_ghost_vao);

    lens_ghost_vbo.Create(sizeof(float_t) * 5 * (6 * 16));
    lens_ghost_vbo.Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float_t) * 5, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float_t) * 5, (void*)(sizeof(float_t) * 2));
    gl_state_bind_array_buffer(0);
    gl_state_bind_vertex_array(0);

    glGenVertexArrays(1, &common_vao);

    camera_blur_ubo.Create(sizeof(camera_blur_shader_data));
    contour_coef_ubo.Create(sizeof(contour_coef_shader_data));
    contour_params_ubo.Create(sizeof(contour_params_shader_data));
    filter_scene_ubo.Create(sizeof(filter_scene_shader_data));
    esm_filter_batch_ubo.Create(sizeof(esm_filter_batch_shader_data));
    imgfilter_batch_ubo.Create(sizeof(imgfilter_batch_shader_data));
    exposure_measure_ubo.Create(sizeof(exposure_measure_shader_data));
    gaussian_coef_ubo.Create(sizeof(gaussian_coef_shader_data));
    glass_eye_batch_ubo.Create(sizeof(glass_eye_batch_shader_data));
    glitter_batch_ubo.Create(sizeof(glitter_batch_shader_data));
    quad_ubo.Create(sizeof(quad_shader_data));
    sprite_scene_ubo.Create(sizeof(sprite_scene_shader_data));
    sss_filter_gaussian_coef_ubo.Create(sizeof(sss_filter_gaussian_coef_shader_data));
    sun_quad_ubo.Create(sizeof(sun_quad_shader_data));
    tone_map_ubo.Create(sizeof(tone_map_shader_data));

    obj_shader = {};
    obj_scene = {};
    obj_batch = {};
    obj_skinning = {};
    obj_shader_ubo.Create(sizeof(obj_shader_shader_data));
    obj_scene_ubo.Create(sizeof(obj_scene_shader_data));
    obj_batch_ubo.Create(sizeof(obj_batch_shader_data));
    obj_skinning_ubo.Create(sizeof(obj_skinning_shader_data));

    static const vec4 border_color = 0.0f;

    glGenSamplers(18, samplers);
    for (int32_t i = 0; i < 18; i++) {
        GLuint sampler = samplers[i];

        glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER,
            i % 2 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

        GLuint wrap_s;
        switch (i / 2 % 3) {
        case 0:
            wrap_s = GL_CLAMP_TO_EDGE;
            break;
        case 1:
            wrap_s = GL_REPEAT;
            break;
        case 2:
            wrap_s = GL_MIRRORED_REPEAT;
            break;
        }
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, wrap_s);

        GLenum wrap_t;
        switch (i / 6 % 3) {
        case 0:
            wrap_t = GL_CLAMP_TO_EDGE;
            break;
        case 1:
            wrap_t = GL_REPEAT;
            break;
        case 2:
            wrap_t = GL_MIRRORED_REPEAT;
            break;
        }
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, wrap_t);
    }

    GLuint sampler;
    glGenSamplers(4, render_samplers);
    sampler = render_samplers[0];
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    sampler = render_samplers[1];
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    sampler = render_samplers[2];
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);

    sampler = render_samplers[3];
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);

    glGenSamplers(3, sprite_samplers);
    sampler = sprite_samplers[0];
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    sampler = sprite_samplers[1];
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    sampler = sprite_samplers[2];
    glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
}

render_context::~render_context() {
    glDeleteSamplers(3, sprite_samplers);
    glDeleteSamplers(4, render_samplers);
    glDeleteSamplers(18, samplers);

    obj_skinning_ubo.Destroy();
    obj_batch_ubo.Destroy();
    obj_scene_ubo.Destroy();
    obj_shader_ubo.Destroy();

    tone_map_ubo.Destroy();
    transparency_batch_ubo.Destroy();
    sun_quad_ubo.Destroy();
    sss_filter_gaussian_coef_ubo.Destroy();
    sprite_scene_ubo.Destroy();
    quad_ubo.Destroy();
    glitter_batch_ubo.Destroy();
    glass_eye_batch_ubo.Destroy();
    gaussian_coef_ubo.Destroy();
    exposure_measure_ubo.Destroy();
    imgfilter_batch_ubo.Destroy();
    esm_filter_batch_ubo.Destroy();
    filter_scene_ubo.Destroy();
    contour_params_ubo.Destroy();
    contour_coef_ubo.Destroy();
    camera_blur_ubo.Destroy();

    if (common_vao) {
        glDeleteVertexArrays(1, &common_vao);
        common_vao = 0;
    }

    if (lens_ghost_vao) {
        glDeleteVertexArrays(1, &lens_ghost_vao);
        lens_ghost_vao = 0;
    }

    lens_ghost_vbo.Destroy();

    if (box_vao) {
        glDeleteVertexArrays(1, &box_vao);
        box_vao = 0;
    }

    box_vbo.Destroy();
}

void render_context::ctrl(bool change_res) {
    RECT rect;
    extern size_t diva_handle;
    GetClientRect((HWND)diva_handle, &rect);

    int32_t width = rect.right - rect.left;
    int32_t height = rect.bottom - rect.top;
    if (!width || !height || screen_width == width && screen_height == height)
        return;

    /*static const double_t aspect = 16.0 / 9.0;

    double_t res_width = (double_t)width;
    double_t res_height = (double_t)height;
    double_t view_aspect = res_width / res_height;
    if (view_aspect < aspect)
        res_height = round(res_width / aspect);
    else if (view_aspect > aspect)
        res_width = round(res_height * aspect);*/

    /*int32_t render_width = (int32_t)res_width;
    int32_t render_height = (int32_t)res_height;*/
    int32_t screen_width = width;
    int32_t screen_height = height;

    //bool render_res_change = render->inner_width != render_width || render->inner_height != render_height;
    bool screen_res_change = this->screen_width != screen_width || this->screen_height != screen_height;

    this->screen_width = screen_width;
    this->screen_height = screen_height;

    /*if (render_res_change)
    render.resize(render_width, render_height);*/
    
    if (!change_res || /*!render_res_change && */!screen_res_change)
        return;

    if (screen_res_change)
        render_manager->resize(screen_width, screen_height);

    /*if (render_res_change)
          litproj_texture->Init(render_width, render_height, 0, GL_RGBA8, GL_DEPTH_COMPONENT32F);*/

    /*sprite_manager_set_res((double_t)screen_width / (double_t)screen_height,
        screen_width, screen_height);*/

    /*if (render_res_change) {
        auto init_copy_buffer = [&](RenderTexture& src, RenderTexture& dst) {
            dst.Init(src.GetWidth(),
                src.GetHeight(), 0, src.color_texture->internal_format,
                src.depth_texture ? src.depth_texture->internal_format : GL_ZERO);
        };

        RenderTexture& render_buffer = render->rend_texture[0];
        RenderTexture& reflect_buffer = render_manager->get_render_texture(1);
        RenderTexture& shadow_buffer = shadow_ptr_get()->render_textures[1];
        init_copy_buffer(reflect_buffer, this->reflect_buffer);
        init_copy_buffer(render_buffer, this->render_buffer);
        init_copy_buffer(shadow_buffer, this->shadow_buffer);
    }*/

    if (screen_res_change)
        screen_buffer.Init(screen_width, screen_height, 0, GL_RGBA8, 0);
}

void render_context::free() {
    shadow_buffer.Free();
    screen_buffer.Free();
    render_buffer.Free();
    reflect_buffer.Free();

    texture_release(empty_texture_cube_map);
    texture_release(empty_texture_2d);
}

void render_context::init() {
    static const uint8_t empty_texture_data[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    static const void* empty_texture_2d_array[] = {
        empty_texture_data,
    };

    empty_texture_2d = texture_load_tex_2d(texture_id(0x2F, 0),
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 4, 4, 0, empty_texture_2d_array, false);

    static const void* empty_texture_cube_map_array[] = {
        empty_texture_data,
        empty_texture_data,
        empty_texture_data,
        empty_texture_data,
        empty_texture_data,
        empty_texture_data,
    };

    empty_texture_cube_map = texture_load_tex_cube_map(texture_id(0x2F, 1),
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 4, 4, 0, empty_texture_cube_map_array);

    ctrl(false);

    auto init_copy_buffer = [&](RenderTexture& src, RenderTexture& dst) {
        dst.Init(src.GetWidth(),
            src.GetHeight(), 0, src.color_texture->internal_format,
            src.depth_texture ? src.depth_texture->internal_format : GL_ZERO);
    };

    RenderTexture& render_buffer = render_get()->rend_texture[0];
    RenderTexture& reflect_buffer = render_manager->get_render_texture(1);
    RenderTexture& shadow_buffer = shadow_ptr_get()->render_textures[1];
    init_copy_buffer(reflect_buffer, this->reflect_buffer);
    init_copy_buffer(render_buffer, this->render_buffer);
    init_copy_buffer(shadow_buffer, this->shadow_buffer);

    screen_buffer.Init(screen_width, screen_height, 0, GL_RGBA8, 0);
}

sss_data* sss_data_get() {
    return &_sss_data;
}
