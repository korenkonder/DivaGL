/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_texture.hpp"
#include "gl_state.hpp"

int32_t RenderTexture::Bind(int32_t index) {
    if (index < 0 || index > max_level)
        return -1;

    gl_state_bind_framebuffer(fbos[index]);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return -1;

    gl_state_get_error();
    return 0;
}

void RenderTexture::Free() {
    static void (FASTCALL * RenderTexture__Free)(RenderTexture * rt)
        = (void (FASTCALL*)(RenderTexture * rt))0x00000001405035C0;
    RenderTexture__Free(this);
}

int32_t RenderTexture::Init(int32_t width, int32_t height,
    int32_t max_level, GLenum color_format, GLenum depth_format) {
    static int32_t(FASTCALL * RenderTexture__Init)(RenderTexture * rt, int32_t width, int32_t height,
        int32_t max_level, GLenum color_format, GLenum depth_format)
        = (int32_t(FASTCALL*)(RenderTexture * rt, int32_t width, int32_t height,
            int32_t max_level, GLenum color_format, GLenum depth_format))0x0000000140503420;
    return RenderTexture__Init(this, width, height, max_level, color_format, depth_format);
}

int32_t RenderTexture::SetColorDepthTextures(GLuint color_texture,
    int32_t max_level, GLuint depth_texture) {
    int32_t(FASTCALL * RenderTexture__SetColorDepthTextures)(RenderTexture * rt,
        GLuint color_texture, int32_t level, GLuint depth_texture)
        = (int32_t(FASTCALL*)(RenderTexture * rt,
            GLuint color_texture, int32_t level, GLuint depth_texture))0x0000000140503130;
    return RenderTexture__SetColorDepthTextures(this, color_texture, max_level, depth_texture);
}
