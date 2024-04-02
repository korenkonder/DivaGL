/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "transparency.hpp"
#include "../../KKdLib/str_utils.hpp"
#include "../render_context.hpp"

extern render_context* rctx;

namespace renderer {
    Transparency::Transparency(GLuint color_texture, GLuint depth_texture,
        int32_t width, int32_t height) : sampler(), program(), vao() {
        GLuint color_textures[1];
        color_textures[0] = color_texture;
        fbo.init(width, height, color_textures, 1, depth_texture);

        load_shaders();

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
        vec4 g_opacity = { alpha, 0.0f, 0.0f, 0.0f };

        gl_state_disable_blend();
        gl_state_disable_depth_test();
        rctx->render_buffer.Bind();
        program.use_program();
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
        fbo.bind_buffer();
        glViewportDLL(0, 0, fbo.width, fbo.height);
    }

    void Transparency::resize(GLuint color_texture, GLuint depth_texture, int32_t width, int32_t height) {
        GLuint color_textures[1];
        color_textures[0] = color_texture;
        fbo.init(width, height, color_textures, 1, depth_texture);
    }

    void renderer::Transparency::load_shaders() {
        GLProgram::ShaderData shader_data[2];

        shader_data[0].type = GL_VERTEX_SHADER;
        shader_data[0].string =
            u8"#version 430\n"
            u8"out VertexData{\n"
            u8"    vec2 texcoord;\n"
            u8"}result;\n"
            u8"void main(){\n"
            u8"    gl_Position.x = -1.f + (gl_VertexID / 2) * 4;\n"
            u8"    gl_Position.y = 1.f - (gl_VertexID % 2) * 4;\n"
            u8"    gl_Position.z = 0.f;\n"
            u8"    gl_Position.w = 1.f;\n"
            u8"    result.texcoord = gl_Position.xy * 0.5f + 0.5f;\n"
            u8"}\n";
        shader_data[0].length = 273;

        shader_data[1].type = GL_FRAGMENT_SHADER;
        shader_data[1].string =
            u8"#version 430\n"
            u8"layout(binding = 0) uniform sampler2D g_layerd_color;\n"
            u8"layout(binding = 1) uniform sampler2D g_base_color;\n"
            u8"layout(location = 0) uniform vec4 g_opacity;//x=opacity, yzw=unused\n"
            u8"in VertexData{\n"
            u8"    vec2 texcoord;\n"
            u8"}frg;\n"
            u8"layout(location = 0) out vec4 result;\n"
            u8"void main(){\n"
            u8"    vec4 cl = textureLod(g_layerd_color, frg.texcoord, 0.f);\n"
            u8"    vec4 cb = textureLod(g_base_color, frg.texcoord, 0.f);\n"
            u8"    result.rgba = mix(cb, cl, g_opacity.x);\n"
            u8"};\n";
        shader_data[1].length = 445;
        program.compile_program(shader_data, 2);
    }
}
