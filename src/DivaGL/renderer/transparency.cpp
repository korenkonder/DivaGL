/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "transparency.hpp"
#include "../../KKdLib/str_utils.hpp"
#include "../render_context.hpp"
#include "../gl_state.hpp"
#include "../shader_ft.hpp"

extern render_context* rctx;

namespace renderer {
    Transparency::Transparency(GLuint color_texture, GLuint depth_texture,
        int32_t width, int32_t height) : sampler(), program(), vao() {
        GLuint color_textures[1];
        color_textures[0] = color_texture;
        fbo.init(width, height, color_textures, 1, depth_texture);

        glGenSamplers(1, &sampler);
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenVertexArrays(1, &vao);
    }

    Transparency::~Transparency() {
        if (vao) {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }

        if (sampler) {
            glDeleteSamplers(1, &sampler);
            sampler = 0;
        }
    }

    void Transparency::combine(RenderTexture* rt, float_t alpha) {
        transparency_batch_shader_data shader_data = {};
        shader_data.g_opacity = { alpha, 0.0f, 0.0f, 0.0f };
        rctx->transparency_batch_ubo.WriteMemory(shader_data);

        gl_state_disable_blend();
        gl_state_disable_depth_test();
        rctx->render_buffer.Bind();
        shaders_ft.set(SHADER_FT_TRANSPARENCY);
        rctx->transparency_batch_ubo.Bind(0);
        gl_state_active_bind_texture_2d(0, fbo.textures[0]);
        gl_state_active_bind_texture_2d(1, rt->GetColorTex());
        gl_state_bind_vertex_array(vao);
        shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
        gl_state_enable_depth_test();

        glCopyImageSubData(rctx->render_buffer.GetColorTex(), GL_TEXTURE_2D, 0, 0, 0, 0,
            rt->GetColorTex(), GL_TEXTURE_2D, 0, 0, 0, 0, fbo.width, fbo.height, 1);
    }

    void Transparency::copy(GLuint texture) {
        glCopyImageSubData(texture, GL_TEXTURE_2D, 0, 0, 0, 0,
            fbo.textures[0], GL_TEXTURE_2D, 0, 0, 0, 0, fbo.width, fbo.height, 1);
        fbo.bind_buffer();
        gl_state_set_viewport(0, 0, fbo.width, fbo.height);
    }

    void Transparency::resize(GLuint color_texture, GLuint depth_texture, int32_t width, int32_t height) {
        GLuint color_textures[1];
        color_textures[0] = color_texture;
        fbo.init(width, height, color_textures, 1, depth_texture);
    }
}
