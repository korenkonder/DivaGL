/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "transparency.hpp"
#include "../../KKdLib/str_utils.hpp"
#include "../render_context.hpp"
#include "../gl_state.hpp"

extern render_context* rctx;

namespace renderer {
    void Transparency::combine(RenderTexture* rt, float_t alpha) {
        vec4 g_opacity = { alpha, 0.0f, 0.0f, 0.0f };

        gl_state_disable_blend();
        gl_state_disable_depth_test();
        rctx->render_buffer.Bind();
        gl_state_use_program(program);
        glUniform4fv(0, 1, (GLfloat*)&g_opacity.x);
        gl_state_active_bind_texture_2d(0, fbo.textures[0]);
        gl_state_active_bind_texture_2d(1, rt->GetColorTex());
        gl_state_bind_vertex_array(vao);
        glDrawArraysDLL(GL_TRIANGLE_STRIP, 0, 4);
        gl_state_enable_depth_test();

        glCopyImageSubData(
            rctx->render_buffer.GetColorTex(), GL_TEXTURE_2D, 0, 0, 0, 0,
            rt->GetColorTex(), GL_TEXTURE_2D, 0, 0, 0, 0, fbo.width, fbo.height, 1);
    }

    void Transparency::copy(GLuint texture) {
        glCopyImageSubData(
            texture, GL_TEXTURE_2D, 0, 0, 0, 0,
            fbo.textures[0], GL_TEXTURE_2D, 0, 0, 0, 0, fbo.width, fbo.height, 1);
        gl_state_bind_framebuffer(fbo.buffer);
        glViewportDLL(0, 0, fbo.width, fbo.height);
    }

    void Transparency::resize(GLuint color_texture, GLuint depth_texture, int32_t width, int32_t height) {
        GLuint color_textures[1];
        color_textures[0] = color_texture;
        fbo.init(width, height, color_textures, 1, depth_texture);
    }
}
