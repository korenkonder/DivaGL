/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "shared.hpp"
#include "texture.hpp"

struct RenderTexture {
    void* __vftable;
    texture* color_texture;
    texture* depth_texture;
    int32_t binding;
    int32_t max_level;
    GLuint* fbos;
    GLuint rbo;
    GLuint field_2C;

    int32_t Bind(int32_t index = 0);
    void Free();
    int32_t Init(int32_t width, int32_t height,
        int32_t max_level, GLenum color_format, GLenum depth_format);
    int32_t SetColorDepthTextures(GLuint color_texture,
        int32_t max_level = 0, GLuint depth_texture = 0);

    inline GLuint GetColorTex() {
        return color_texture->tex;
    };

    inline GLuint GetDepthTex() {
        return depth_texture->tex;
    };

    inline int32_t GetHeight() {
        return color_texture->height;
    };

    inline int32_t GetWidth() {
        return color_texture->width;
    };

    inline void SetViewport() {
        glViewportDLL(0, 0, GetWidth(), GetHeight());
    };
};

static_assert(sizeof(RenderTexture) == 0x30, "\"RenderTexture\" struct should have a size of 0x30");
