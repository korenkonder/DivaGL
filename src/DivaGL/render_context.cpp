/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_context.hpp"
#include "render_manager.hpp"

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

draw_state_struct* draw_state = (draw_state_struct*)0x00000001411A32B0;

render_context* rctx;

sss_data* (FASTCALL* sss_data_get)()
    = (sss_data * (FASTCALL*)())0x0000000140641B70;

void draw_state_struct::set_fog_height(bool value) {
    fog_height = value;
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

render_context::render_context() : box_vao(), lens_ghost_vao(), common_vao(),
reflect_buffer(), render_buffer(), screen_buffer(), shadow_buffer(), empty_texture_2d(),
empty_texture_cube_map(), samplers(), render_samplers(), sprite_samplers(), sprite_width(),
sprite_height(), screen_x_offset(), screen_y_offset(), screen_width(), screen_height() {
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

    obj_scene = {};
    obj_batch = {};
    obj_skinning = {};
    obj_scene_ubo.Create(sizeof(obj_scene_shader_data));
    obj_batch_ubo.Create(sizeof(obj_batch_shader_data));
    obj_skinning_ubo.Create(sizeof(obj_skinning_shader_data));

    static const uint8_t empty_texture_data[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    static const GLenum target_cube_map_array[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    glGenTextures(1, &empty_texture_2d);
    gl_state_bind_texture_2d(empty_texture_2d);
    texture_set_params(GL_TEXTURE_2D, 0, false);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0,
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 4, 4, 0, 8, empty_texture_data);
    gl_state_bind_texture_2d(0);

    glGenTextures(1, &empty_texture_cube_map);
    gl_state_bind_texture_cube_map(empty_texture_cube_map);
    texture_set_params(GL_TEXTURE_CUBE_MAP, 0, false);
    for (int32_t side = 0; side < 6; side++)
        glCompressedTexImage2D(target_cube_map_array[side], 0,
            GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 4, 4, 0, 8, empty_texture_data);
    gl_state_bind_texture_cube_map(0);

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

    glDeleteTextures(1, &empty_texture_cube_map);
    glDeleteTextures(1, &empty_texture_2d);

    obj_skinning_ubo.Destroy();
    obj_batch_ubo.Destroy();
    obj_scene_ubo.Destroy();

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
    if (screen_width == width && screen_height == height)
        return;

    static const double_t aspect = 16.0 / 9.0;

    double_t res_width = (double_t)width;
    double_t res_height = (double_t)height;
    double_t view_aspect = res_width / res_height;
    if (view_aspect < aspect)
        res_height = round(res_width / aspect);
    else if (view_aspect > aspect)
        res_width = round(res_height * aspect);

    int32_t render_width = (int32_t)res_width;
    int32_t render_height = (int32_t)res_height;
    int32_t sprite_width = (int32_t)res_width;
    int32_t sprite_height = (int32_t)res_height;
    int32_t screen_width = width;
    int32_t screen_height = height;

    //bool render_res_change = render->inner_width != render_width || render->inner_height != render_height;
    bool sprite_res_change = this->sprite_width != sprite_width || this->sprite_height != sprite_height;
    bool screen_res_change = this->screen_width != screen_width || this->screen_height != screen_height;

    this->sprite_width = sprite_width;
    this->sprite_height = sprite_height;

    screen_x_offset = (screen_width - sprite_width) / 2 + (screen_width - sprite_width) % 2;
    screen_y_offset = (screen_height - sprite_height) / 2 + (screen_height - sprite_height) % 2;

    this->screen_width = screen_width;
    this->screen_height = screen_height;

    if (!change_res || /*!render_res_change && */!sprite_res_change && !screen_res_change)
        return;

    /*if (render_res_change)
        render.resize(render_width, render_height);*/

    if (sprite_res_change)
        render_manager->resize(sprite_width, sprite_height);

    /*if (render_res_change)
          litproj_texture->Init(render_width, render_height, 0, GL_RGBA8, GL_DEPTH_COMPONENT32F);*/

    /*sprite_manager_set_res((double_t)sprite_width / (double_t)sprite_height,
        sprite_width, sprite_height);*/

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

    if (sprite_res_change)
        screen_buffer.Init(sprite_width, sprite_height, 0, GL_RGBA8, 0);
}

void render_context::free() {
    shadow_buffer.Free();
    screen_buffer.Free();
    render_buffer.Free();
    reflect_buffer.Free();
}

void render_context::init() {
    ctrl(false);

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

    screen_buffer.Init(sprite_width, sprite_height, 0, GL_RGBA8, 0);
}

void fbo_blit(GLuint src_fbo, GLuint dst_fbo,
    GLint src_x, GLint src_y, GLint src_width, GLint src_height,
    GLint dst_x, GLint dst_y, GLint dst_width, GLint dst_height, GLbitfield mask, GLenum filter) {
    gl_state_bind_read_framebuffer(src_fbo);
    gl_state_bind_draw_framebuffer(dst_fbo);
    glBlitFramebuffer(src_x, src_y, src_x + src_width, src_y + src_height,
        dst_x, dst_y, dst_x + dst_width, dst_y + dst_height, mask, filter);
}
