/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shared.hpp"
#include "gl_state.hpp"

void fbo_blit(GLuint src_fbo, GLuint dst_fbo,
    GLint src_x, GLint src_y, GLint src_width, GLint src_height,
    GLint dst_x, GLint dst_y, GLint dst_width, GLint dst_height, GLbitfield mask, GLenum filter) {
    gl_state_bind_read_framebuffer(src_fbo);
    gl_state_bind_draw_framebuffer(dst_fbo);
    glBlitFramebuffer(src_x, src_y, src_x + src_width, src_y + src_height,
        dst_x, dst_y, dst_x + dst_width, dst_y + dst_height, mask, filter);
}
