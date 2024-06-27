/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/vec.hpp"
#include "../render_texture.hpp"
#include "fbo.hpp"

namespace renderer {
    struct Transparency {
    private:
        FBO fbo;
        GLuint program; // Unused
        GLuint sampler;
        GLuint vao;

    public:
        Transparency(GLuint color_texture, GLuint depth_texture, int32_t width, int32_t height);
        ~Transparency();

        void combine(RenderTexture* rt, float_t alpha);
        void copy(GLuint texture);

        void resize(GLuint color_texture, GLuint depth_texture, int32_t width, int32_t height);
    };

    static_assert(sizeof(renderer::Transparency) == 0x30, "\"renderer::Transparency\" struct should have a size of 0x30");
};
