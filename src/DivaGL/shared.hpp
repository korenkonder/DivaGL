/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "wrap.hpp"

enum blur_filter_mode {
    BLUR_FILTER_4   = 0x00,
    BLUR_FILTER_9   = 0x01,
    BLUR_FILTER_16  = 0x02,
    BLUR_FILTER_32  = 0x03,
    BLUR_FILTER_MAX = 0x04,
};

extern void fbo_blit(GLuint src_fbo, GLuint dst_fbo,
    GLint src_x, GLint src_y, GLint src_width, GLint src_height,
    GLint dst_x, GLint dst_y, GLint dst_width, GLint dst_height, GLbitfield mask, GLenum filter);
