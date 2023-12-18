/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "texture.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/txp.hpp"
#include "gl_state.hpp"
#include <Helpers.h>

static void texture_bind(GLenum target, GLuint texture);

texture* (FASTCALL* _texture_init)(texture_id id)
    = (texture * (FASTCALL*)(texture_id id))0x000000014069A390;
texture* (FASTCALL* _texture_copy)(texture_id id, texture* org_tex)
    = (texture * (FASTCALL*)(texture_id id, texture * org_tex))0x000000014069B550;
texture* (FASTCALL* texture_load_tex_cubemap)(uint32_t id, GLenum internal_format,
    int32_t width, int32_t height, int32_t max_mipmap_level, void** data_ptr)
    = (texture * (FASTCALL*)(uint32_t id, GLenum internal_format, int32_t width,
        int32_t height, int32_t max_mipmap_level, void** data_ptr))0x000000014069B860;
texture* (FASTCALL* texture_load_tex_2d)(texture_id id, GLenum internal_format,
    int32_t width, int32_t height, int32_t max_mipmap_level, void** data_ptr, bool use_high_anisotropy)
    = (texture * (FASTCALL*)(texture_id id, GLenum internal_format, int32_t width, int32_t height,
        int32_t max_mipmap_level, void** data_ptr, bool use_high_anisotropy))0x000000014069B8E0;
void(FASTCALL* texture_free)(texture* tex)
    = (void(FASTCALL*)(texture * tex))0x000000014069DA70;
int32_t(FASTCALL* texture_info_get_id)(const char* name)
    = (int32_t(FASTCALL*)(const char* name))0x000000014069CBD0;
texture* (FASTCALL* texture_handler_get_texture)(uint32_t id)
    = (texture * (FASTCALL*)(uint32_t id))0x000000014069CD70;

uint32_t texture::get_height_align_mip_level(uint8_t mip_level) {
    if (flags & TEXTURE_BLOCK_COMPRESSION)
        return max_def((uint32_t)height >> mip_level, 4u);
    else
        return max_def((uint32_t)height >> mip_level, 1u);
}

uint32_t texture::get_width_align_mip_level(uint8_t mip_level) {
    if (flags & TEXTURE_BLOCK_COMPRESSION)
        return max_def((uint32_t)width >> mip_level, 4u);
    else
        return max_def((uint32_t)width >> mip_level, 1u);
}

void texture_set_params(GLenum target, int32_t max_mipmap_level, bool use_high_anisotropy) {
    glTexParameteriDLL(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteriDLL(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteriDLL(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    static const vec4 border_color = 0.0f;
    glTexParameterfvDLL(target, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);
    glTexParameteriDLL(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteriDLL(target, GL_TEXTURE_MIN_FILTER,
        max_mipmap_level > 0 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteriDLL(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteriDLL(target, GL_TEXTURE_MAX_LEVEL, max_mipmap_level);

    float_t max_anisotropy;
    if (use_high_anisotropy)
        max_anisotropy = 16.0f;
    else
        max_anisotropy = 1.0f;
    glTexParameterfDLL(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
}

void texture_params_get(GLuint tex_0, texture_param* tex_0_param,
    GLuint tex_1, texture_param* tex_1_param, GLuint tex_2, texture_param* tex_2_param) {
    gl_state_disable_depth_test();
    if (tex_0_param) {
        tex_0_param->width = 0;
        tex_0_param->height = 0;
        if (tex_0) {
            gl_state_bind_texture_2d(tex_0);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_0_param->width);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_0_param->height);
        }
    }

    if (tex_1_param) {
        tex_1_param->width = 0;
        tex_1_param->height = 0;
        if (tex_1) {
            gl_state_bind_texture_2d(tex_1);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_1_param->width);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_1_param->height);
        }
    }

    if (tex_2_param) {
        tex_2_param->width = 0;
        tex_2_param->height = 0;
        if (tex_2) {
            gl_state_bind_texture_2d(tex_2);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_2_param->width);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_2_param->height);
        }
    }
    gl_state_bind_texture_2d(0);

    if (tex_0_param)
        glViewportDLL(0, 0, tex_0_param->width, tex_0_param->height);
}

void texture_params_restore(texture_param* tex_0_param,
    texture_param* tex_1_param, texture_param* tex_2_param) {
    for (int32_t i = 0; i < 4; i++)
        gl_state_active_bind_texture_2d(i, 0);
}

inline static void texture_bind(GLenum target, GLuint texture) {
    switch (target) {
    case GL_TEXTURE_2D:
        gl_state_bind_texture_2d(texture, true);
        break;
    case GL_TEXTURE_CUBE_MAP:
        gl_state_bind_texture_cube_map(texture, true);
        break;
    }
}
