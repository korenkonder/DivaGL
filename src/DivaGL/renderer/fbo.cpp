/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "fbo.hpp"
#include "../../AFTModsShared/types.hpp"

namespace renderer {
    FBO::FBO() : flags(), width(), height(), buffer(), count(), textures() {

    }

    FBO::~FBO() {
        free();
    }

#pragma warning(push)
#pragma warning(disable: 6385)
#pragma warning(disable: 6386)
    void FBO::init(int32_t width, int32_t height,
        GLuint* color_textures, int32_t count, GLuint depth_texture) {
        free();

        count = min_def(count, 8);

        this->width = width;
        this->height = height;
        this->count = count;
        flags = (Flags)0;
        if (depth_texture) {
            this->flags = DEPTH_ATTACHMENT;
            this->count++;
        }

        textures = new (_operator_new(sizeof(GLuint) * this->count)) GLuint[this->count];
        for (int32_t i = 0; i < count; i++)
            textures[i] = color_textures[i];

        if (flags & DEPTH_ATTACHMENT)
            textures[count] = depth_texture;

        glGenFramebuffers(1, &buffer);
        gl_state_bind_framebuffer(buffer, true);
        for (int32_t i = 0; i < count; i++)
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textures[i], 0);

        if (flags & DEPTH_ATTACHMENT)
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[count], 0);

        GLenum color_attachments[8];
        for (int32_t i = 0; i < count; i++)
            color_attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        glDrawBuffers(count, color_attachments);
        glCheckFramebufferStatus(GL_FRAMEBUFFER);
        gl_state_bind_framebuffer(0);
    }
#pragma warning(pop)

    void FBO::free() {
        if (buffer) {
            glDeleteFramebuffers(1, &buffer);
            buffer = 0;
        }

        if (textures) {
            _operator_delete(textures);
            textures = 0;
        }
    }
}
