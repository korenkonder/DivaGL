/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "gl_state.hpp"
#include "wrap.hpp"

gl_state_struct gl_state;

void gl_state_active_bind_texture_2d(int32_t index, GLuint texture, bool force) {
    if (force || gl_state.texture_binding_2d[index] != texture) {
        if (force || gl_state.active_texture_index != index) {
            glActiveTexture((GLenum)(GL_TEXTURE0 + index));
            gl_state.active_texture = (GLenum)(GL_TEXTURE0 + index);
            gl_state.active_texture_index = (GLuint)index;
        }
        glBindTextureDLL(GL_TEXTURE_2D, texture);
        gl_state.texture_binding_2d[index] = texture;
    }
}

void gl_state_active_bind_texture_cube_map(int32_t index, GLuint texture, bool force) {
    if (force || gl_state.texture_binding_cube_map[index] != texture) {
        if (force || gl_state.active_texture_index != index) {
            glActiveTexture((GLenum)(GL_TEXTURE0 + index));
            gl_state.active_texture = (GLenum)(GL_TEXTURE0 + index);
            gl_state.active_texture_index = (GLuint)index;
        }
        glBindTextureDLL(GL_TEXTURE_CUBE_MAP, texture);
        gl_state.texture_binding_cube_map[index] = texture;
    }
}

void gl_state_active_texture(size_t index, bool force) {
    if (force || gl_state.active_texture_index != index) {
        glActiveTexture((GLenum)(GL_TEXTURE0 + index));
        gl_state.active_texture = (GLenum)(GL_TEXTURE0 + index);
        gl_state.active_texture_index = (GLuint)index;
    }
}

void gl_state_bind_framebuffer(GLuint framebuffer, bool force) {
    if (force || gl_state.read_framebuffer_binding != framebuffer
        || gl_state.draw_framebuffer_binding != framebuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        gl_state.read_framebuffer_binding = framebuffer;
        gl_state.draw_framebuffer_binding = framebuffer;
    }
}

void gl_state_bind_read_framebuffer(GLuint framebuffer, bool force) {
    if (force || gl_state.read_framebuffer_binding != framebuffer) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        gl_state.read_framebuffer_binding = framebuffer;
    }
}

void gl_state_bind_draw_framebuffer(GLuint framebuffer, bool force) {
    if (force || gl_state.draw_framebuffer_binding != framebuffer) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
        gl_state.draw_framebuffer_binding = framebuffer;
    }
}

void gl_state_bind_vertex_array(GLuint array, bool force) {
    if (force || gl_state.vertex_array_binding != array) {
        glBindVertexArray(array);
        gl_state.vertex_array_binding = array;
    }
}

void gl_state_bind_array_buffer(GLuint buffer, bool force) {
    if (force || gl_state.array_buffer_binding != buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        gl_state.array_buffer_binding = buffer;
    }
}

void gl_state_bind_element_array_buffer(GLuint buffer, bool force) {
    if (force || gl_state.element_array_buffer_binding != buffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        gl_state.element_array_buffer_binding = buffer;
    }
}

void gl_state_bind_uniform_buffer(GLuint buffer, bool force) {
    if (force || gl_state.uniform_buffer_binding != buffer) {
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        gl_state.uniform_buffer_binding = buffer;
    }
}

void gl_state_bind_uniform_buffer_base(GLuint index, GLuint buffer, bool force) {
    if (force || gl_state.uniform_buffer_bindings[index] != buffer) {
        glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer);
        gl_state.uniform_buffer_bindings[index] = buffer;
        gl_state.uniform_buffer_offsets[index] = 0;
        gl_state.uniform_buffer_sizes[index] = -1;
    }
}

void gl_state_bind_uniform_buffer_range(GLuint index,
    GLuint buffer, GLintptr offset, GLsizeiptr size, bool force) {
    if (force || gl_state.uniform_buffer_bindings[index] != buffer
        || gl_state.uniform_buffer_offsets[index] != offset
        || gl_state.uniform_buffer_sizes[index] != size) {
        glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, offset, size);
        gl_state.uniform_buffer_bindings[index] = buffer;
        gl_state.uniform_buffer_offsets[index] = offset;
        gl_state.uniform_buffer_sizes[index] = size;
    }
}

void gl_state_bind_shader_storage_buffer(GLuint buffer, bool force) {
    if (force || gl_state.shader_storage_buffer_binding != buffer) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
        gl_state.shader_storage_buffer_binding = buffer;
    }
}

void gl_state_bind_shader_storage_buffer_base(GLuint index, GLuint buffer, bool force) {
    if (force || gl_state.shader_storage_buffer_bindings[index] != buffer) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buffer);
        gl_state.shader_storage_buffer_bindings[index] = buffer;
        gl_state.shader_storage_buffer_offsets[index] = 0;
        gl_state.shader_storage_buffer_sizes[index] = -1;
    }
}

void gl_state_bind_shader_storage_buffer_range(GLuint index,
    GLuint buffer, GLintptr offset, GLsizeiptr size, bool force) {
    if (force || gl_state.shader_storage_buffer_bindings[index] != buffer
        || gl_state.shader_storage_buffer_offsets[index] != offset
        || gl_state.shader_storage_buffer_sizes[index] != size) {
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, buffer, offset, size);
        gl_state.shader_storage_buffer_bindings[index] = buffer;
        gl_state.shader_storage_buffer_offsets[index] = offset;
        gl_state.shader_storage_buffer_sizes[index] = size;
    }
}

void gl_state_bind_texture_2d(GLuint texture, bool force) {
    if (gl_state.texture_binding_2d[gl_state.active_texture_index] != texture) {
        glBindTextureDLL(GL_TEXTURE_2D, texture);
        gl_state.texture_binding_2d[gl_state.active_texture_index] = texture;
    }
}

void gl_state_bind_texture_cube_map(GLuint texture, bool force) {
    if (force || gl_state.texture_binding_cube_map[gl_state.active_texture_index] != texture) {
        glBindTextureDLL(GL_TEXTURE_CUBE_MAP, texture);
        gl_state.texture_binding_cube_map[gl_state.active_texture_index] = texture;
    }
}

void gl_state_bind_sampler(int32_t index, GLuint sampler, bool force) {
    if (force || gl_state.sampler_binding[index] != sampler) {
        glBindSampler(index, sampler);
        gl_state.sampler_binding[index] = sampler;
    }
}

bool gl_state_check_uniform_buffer_binding() {
    return gl_state.uniform_buffer_binding != 0;
}

bool gl_state_check_uniform_buffer_binding_base(size_t index) {
    return gl_state.uniform_buffer_bindings[index] != 0;
}

bool gl_state_check_texture_binding_2d(size_t index) {
    return gl_state.texture_binding_2d[index] != 0;
}

bool gl_state_check_shader_storage_buffer_binding() {
    return gl_state.shader_storage_buffer_binding != 0;
}

bool gl_state_check_shader_storage_buffer_binding_base(size_t index) {
    return gl_state.shader_storage_buffer_bindings[index] != 0;
}

bool gl_state_check_texture_binding_cube_map(size_t index) {
    return gl_state.texture_binding_cube_map[index] != 0;
}

bool gl_state_check_sampler_binding(int32_t index, GLuint sampler) {
    return gl_state.sampler_binding[index] != 0;
}

void gl_state_disable_blend(bool force) {
    if (force || gl_state.blend) {
        glDisableDLL(GL_BLEND);
        gl_state.blend = GL_FALSE;
    }
}

void gl_state_disable_cull_face(bool force) {
    if (force || gl_state.cull_face) {
        glDisableDLL(GL_CULL_FACE);
        gl_state.cull_face = GL_FALSE;
    }
}

void gl_state_disable_depth_test(bool force) {
    if (force || gl_state.depth_test) {
        glDisableDLL(GL_DEPTH_TEST);
        gl_state.depth_test = GL_FALSE;
    }
}

void gl_state_disable_multisample(bool force) {
    if (force || gl_state.multisample) {
        glDisableDLL(GL_MULTISAMPLE);
        gl_state.multisample = GL_FALSE;
    }
}

void gl_state_disable_primitive_restart(bool force) {
    if (force || gl_state.primitive_restart) {
        glDisableDLL(GL_PRIMITIVE_RESTART);
        gl_state.primitive_restart = GL_FALSE;
    }
}

void gl_state_disable_scissor_test(bool force) {
    if (force || gl_state.scissor_test) {
        glDisableDLL(GL_SCISSOR_TEST);
        gl_state.scissor_test = GL_FALSE;
    }
}

void gl_state_disable_stencil_test(bool force) {
    if (force || gl_state.stencil_test) {
        glDisableDLL(GL_STENCIL_TEST);
        gl_state.stencil_test = GL_FALSE;
    }
}

void gl_state_enable_blend(bool force) {
    if (force || !gl_state.blend) {
        glEnableDLL(GL_BLEND);
        gl_state.blend = GL_TRUE;
    }
}

void gl_state_enable_cull_face(bool force) {
    if (force || !gl_state.cull_face) {
        glEnableDLL(GL_CULL_FACE);
        gl_state.cull_face = GL_TRUE;
    }
}

void gl_state_enable_depth_test(bool force) {
    if (force || !gl_state.depth_test) {
        glEnableDLL(GL_DEPTH_TEST);
        gl_state.depth_test = GL_TRUE;
    }
}

void gl_state_enable_multisample(bool force) {
    if (force || !gl_state.multisample) {
        glEnableDLL(GL_MULTISAMPLE);
        gl_state.multisample = GL_TRUE;
    }
}

void gl_state_enable_primitive_restart(bool force) {
    if (force || !gl_state.primitive_restart) {
        glEnableDLL(GL_PRIMITIVE_RESTART);
        gl_state.primitive_restart = GL_TRUE;
    }
}

void gl_state_enable_scissor_test(bool force) {
    if (force || !gl_state.scissor_test) {
        glEnableDLL(GL_SCISSOR_TEST);
        gl_state.scissor_test = GL_TRUE;
    }
}

void gl_state_enable_stencil_test(bool force) {
    if (force || !gl_state.stencil_test) {
        glEnableDLL(GL_STENCIL_TEST);
        gl_state.stencil_test = GL_TRUE;
    }
}

void gl_state_get() {
    glGetIntegervDLL(GL_CURRENT_PROGRAM, &gl_state.program);
    glGetIntegervDLL(GL_ACTIVE_TEXTURE, (GLint*)&gl_state.active_texture);
    gl_state.active_texture_index = gl_state.active_texture - GL_TEXTURE0;
    for (GLuint i = 0; i < 32; i++) {
        glActiveTexture((GLenum)(GL_TEXTURE0 + i));
        glGetIntegervDLL(GL_TEXTURE_BINDING_2D, &gl_state.texture_binding_2d[i]);
        glGetIntegervDLL(GL_TEXTURE_BINDING_CUBE_MAP, &gl_state.texture_binding_cube_map[i]);
        glGetIntegeri_v(GL_SAMPLER_BINDING, i, &gl_state.sampler_binding[i]);
    }
    glActiveTexture(gl_state.active_texture);

    glGetBooleanvDLL(GL_BLEND, &gl_state.blend);
    glGetIntegervDLL(GL_BLEND_SRC_RGB, (GLint*)&gl_state.blend_src_rgb);
    glGetIntegervDLL(GL_BLEND_SRC_ALPHA, (GLint*)&gl_state.blend_src_alpha);
    glGetIntegervDLL(GL_BLEND_DST_RGB, (GLint*)&gl_state.blend_dst_rgb);
    glGetIntegervDLL(GL_BLEND_DST_ALPHA, (GLint*)&gl_state.blend_dst_alpha);
    glGetIntegervDLL(GL_BLEND_EQUATION_RGB, (GLint*)&gl_state.blend_mode_rgb);
    glGetIntegervDLL(GL_BLEND_EQUATION_ALPHA, (GLint*)&gl_state.blend_mode_alpha);

    glGetIntegervDLL(GL_READ_FRAMEBUFFER_BINDING, (GLint*)&gl_state.read_framebuffer_binding);
    glGetIntegervDLL(GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)&gl_state.draw_framebuffer_binding);
    glGetIntegervDLL(GL_VERTEX_ARRAY_BINDING, (GLint*)&gl_state.vertex_array_binding);
    glGetIntegervDLL(GL_ARRAY_BUFFER_BINDING, (GLint*)&gl_state.array_buffer_binding);
    glGetIntegervDLL(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&gl_state.element_array_buffer_binding);

    glGetIntegervDLL(GL_UNIFORM_BUFFER_BINDING, (GLint*)&gl_state.uniform_buffer_binding);
    for (GLuint i = 0; i < 14; i++) {
        glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, i, (GLint*)&gl_state.uniform_buffer_bindings[i]);
        glGetIntegeri_v(GL_UNIFORM_BUFFER_START, i, (GLint*)&gl_state.uniform_buffer_offsets[i]);
        glGetIntegeri_v(GL_UNIFORM_BUFFER_SIZE, i, (GLint*)&gl_state.uniform_buffer_sizes[i]);
    }

    glGetIntegervDLL(GL_SHADER_STORAGE_BUFFER_BINDING, (GLint*)&gl_state.shader_storage_buffer_binding);
    for (GLuint i = 0; i < 14; i++) {
        glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_BINDING, i, (GLint*)&gl_state.shader_storage_buffer_bindings[i]);
        glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_START, i, (GLint*)&gl_state.shader_storage_buffer_offsets[i]);
        glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_SIZE, i, (GLint*)&gl_state.shader_storage_buffer_sizes[i]);
    }

    glGetBooleanvDLL(GL_COLOR_WRITEMASK, gl_state.color_mask);
    glGetBooleanvDLL(GL_CULL_FACE, &gl_state.cull_face);
    glGetIntegervDLL(GL_CULL_FACE_MODE, (GLint*)&gl_state.cull_face_mode);
    glGetBooleanvDLL(GL_DEPTH_TEST, &gl_state.depth_test);
    glGetIntegervDLL(GL_DEPTH_FUNC, (GLint*)&gl_state.depth_func);
    glGetBooleanvDLL(GL_DEPTH_WRITEMASK, &gl_state.depth_mask);
    glGetBooleanvDLL(GL_MULTISAMPLE, &gl_state.multisample);
    glGetBooleanvDLL(GL_PRIMITIVE_RESTART, &gl_state.primitive_restart);
    glGetIntegervDLL(GL_PRIMITIVE_RESTART_INDEX, (GLint*)&gl_state.primitive_restart_index);
    glGetIntegervDLL(GL_SCISSOR_BOX, (GLint*)&gl_state.scissor_box);
    glGetBooleanvDLL(GL_SCISSOR_TEST, &gl_state.scissor_test);
    glGetBooleanvDLL(GL_STENCIL_TEST, &gl_state.stencil_test);
    glGetIntegervDLL(GL_STENCIL_WRITEMASK, (GLint*)&gl_state.stencil_mask);
    glGetIntegervDLL(GL_VIEWPORT, (GLint*)&gl_state.viewport);

    glPolygonModeDLL(GL_FRONT_AND_BACK, GL_FILL);
    gl_state.polygon_front_face_mode = GL_FILL;
    gl_state.polygon_back_face_mode = GL_FILL;
}

void gl_state_get_all_gl_errors() {
    GLenum error;
    while (error = glGetErrorDLL())
        printf_debug("GL Error: 0x%04X\n", error);
}

GLenum gl_state_get_error() {
    GLenum error = glGetErrorDLL();
    if (error)
        printf_debug("GL Error: 0x%04X\n", error);
    return error;
}

GLuint gl_state_get_program() {
    return gl_state.program;
}

gl_state_rect gl_state_get_scissor() {
    return gl_state.scissor_box;
}

void gl_state_get_scissor(GLint& x, GLint& y, GLsizei& width, GLsizei& height) {
    x = gl_state.scissor_box.x;
    y = gl_state.scissor_box.y;
    width = gl_state.scissor_box.width;
    height = gl_state.scissor_box.height;
}

gl_state_rect gl_state_get_viewport() {
    return gl_state.viewport;
}

void gl_state_get_viewport(GLint& x, GLint& y, GLsizei& width, GLsizei& height) {
    x = gl_state.viewport.x;
    y = gl_state.viewport.y;
    width = gl_state.viewport.width;
    height = gl_state.viewport.height;
}

void gl_state_set_blend_func(GLenum src, GLenum dst, bool force) {
    if (force || gl_state.blend_src_rgb != src || gl_state.blend_dst_rgb != dst
        || gl_state.blend_src_alpha != GL_ONE || gl_state.blend_dst_alpha != GL_ONE_MINUS_SRC_ALPHA) {
        glBlendFuncSeparate(src, dst, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        gl_state.blend_src_rgb = src;
        gl_state.blend_dst_rgb = dst;
        gl_state.blend_src_alpha = GL_ONE;
        gl_state.blend_dst_alpha = GL_ONE_MINUS_SRC_ALPHA;
    }
}

void gl_state_set_blend_func_separate(GLenum src_rgb, GLenum dst_rgb,
    GLenum src_alpha, GLenum dst_alpha, bool force) {
    if (force || gl_state.blend_src_rgb != src_rgb || gl_state.blend_dst_rgb != dst_rgb
        || gl_state.blend_src_alpha != src_alpha || gl_state.blend_dst_alpha != dst_alpha) {
        glBlendFuncSeparate(src_rgb, dst_rgb, src_alpha, dst_alpha);
        gl_state.blend_src_rgb = src_rgb;
        gl_state.blend_dst_rgb = dst_rgb;
        gl_state.blend_src_alpha = src_alpha;
        gl_state.blend_dst_alpha = dst_alpha;
    }
}

void gl_state_set_blend_equation(GLenum mode, bool force) {
    if (force || gl_state.blend_mode_rgb != mode || gl_state.blend_mode_alpha != mode) {
        glBlendEquationSeparate(mode, mode);
        gl_state.blend_mode_rgb = mode;
        gl_state.blend_mode_alpha = mode;
    }
}

void gl_state_set_blend_equation_separate(GLenum mode_rgb, GLenum mode_alpha, bool force) {
    if (force || gl_state.blend_mode_rgb != mode_rgb || gl_state.blend_mode_alpha != mode_alpha) {
        glBlendEquationSeparate(mode_rgb, mode_alpha);
        gl_state.blend_mode_rgb = mode_rgb;
        gl_state.blend_mode_alpha = mode_alpha;
    }
}

void gl_state_set_color_mask(GLboolean red, GLboolean green,
    GLboolean blue, GLboolean alpha, bool force) {
    if (force || gl_state.color_mask[0] != red || gl_state.color_mask[1] != green
        || gl_state.color_mask[2] != blue || gl_state.color_mask[3] != alpha) {
        glColorMaskDLL(red, green, blue, alpha);
        gl_state.color_mask[0] = red;
        gl_state.color_mask[1] = green;
        gl_state.color_mask[2] = blue;
        gl_state.color_mask[3] = alpha;
    }
}

void gl_state_set_cull_face_mode(GLenum mode, bool force) {
    if (force || gl_state.cull_face_mode != mode) {
        glCullFaceDLL(mode);
        gl_state.cull_face_mode = mode;
    }
}

void gl_state_set_depth_func(GLenum func, bool force) {
    if (force || gl_state.depth_func != func) {
        glDepthFuncDLL(func);
        gl_state.depth_func = func;
    }
}

void gl_state_set_depth_mask(GLboolean flag, bool force) {
    if (force || gl_state.depth_mask != flag) {
        glDepthMaskDLL(flag);
        gl_state.depth_mask = flag;
    }
}

void gl_state_set_polygon_mode(GLenum face, GLenum mode, bool force) {
    switch (face) {
    case GL_FRONT:
        if (force || gl_state.polygon_front_face_mode != mode) {
            glPolygonModeDLL(GL_FRONT, mode);
            gl_state.polygon_front_face_mode = mode;
        }
        break;
    case GL_BACK:
        if (force || gl_state.polygon_back_face_mode != mode) {
            glPolygonModeDLL(GL_BACK, mode);
            gl_state.polygon_back_face_mode = mode;
        }
        break;
    case GL_FRONT_AND_BACK:
        if (force || gl_state.polygon_front_face_mode != mode
            && gl_state.polygon_back_face_mode != mode) {
            glPolygonModeDLL(GL_FRONT_AND_BACK, mode);
            gl_state.polygon_front_face_mode = mode;
            gl_state.polygon_back_face_mode = mode;
        }
        else if (gl_state.polygon_front_face_mode != mode) {
            glPolygonModeDLL(GL_FRONT, mode);
            gl_state.polygon_front_face_mode = mode;
        }
        else if (gl_state.polygon_back_face_mode != mode) {
            glPolygonModeDLL(GL_BACK, mode);
            gl_state.polygon_back_face_mode = mode;
        }
        break;
    }
}

void gl_state_set_primitive_restart_index(GLuint index, bool force) {
    if (force || gl_state.primitive_restart_index != index) {
        glPrimitiveRestartIndex(index);
        gl_state.primitive_restart_index = index;
    }
}

void gl_state_set_scissor(const gl_state_rect& rect, bool force) {
    if (force || gl_state.scissor_box.x != rect.x || gl_state.scissor_box.y != rect.y
        || gl_state.scissor_box.width != rect.width || gl_state.scissor_box.height != rect.height) {
        glScissorDLL(rect.x, rect.y, rect.width, rect.height);
        gl_state.scissor_box = rect;
    }
}

void gl_state_set_scissor(GLint x, GLint y, GLsizei width, GLsizei height, bool force) {
    if (force || gl_state.scissor_box.x != x || gl_state.scissor_box.y != y
        || gl_state.scissor_box.width != width || gl_state.scissor_box.height != height) {
        glScissorDLL(x, y, width, height);
        gl_state.scissor_box.x = x;
        gl_state.scissor_box.y = y;
        gl_state.scissor_box.width = width;
        gl_state.scissor_box.height = height;
    }
}

void gl_state_set_stencil_mask(GLuint mask, bool force) {
    if (force || gl_state.stencil_mask != mask) {
        glStencilMask(mask);
        gl_state.stencil_mask = mask;
    }
}

void gl_state_set_viewport(const gl_state_rect& rect, bool force) {
    if (force || gl_state.viewport.x != rect.x || gl_state.viewport.y != rect.y
        || gl_state.viewport.width != rect.width || gl_state.viewport.height != rect.height) {
        glViewportDLL(rect.x, rect.y, rect.width, rect.height);
        gl_state.viewport = rect;
    }
}

void gl_state_set_viewport(GLint x, GLint y, GLsizei width, GLsizei height, bool force) {
    if (force || gl_state.viewport.x != x || gl_state.viewport.y != y
        || gl_state.viewport.width != width || gl_state.viewport.height != height) {
        glViewportDLL(x, y, width, height);
        gl_state.viewport.x = x;
        gl_state.viewport.y = y;
        gl_state.viewport.width = width;
        gl_state.viewport.height = height;
    }
}

void gl_state_use_program(GLuint program, bool force) {
    if (force || gl_state.program != program) {
        glUseProgram(program);
        gl_state.program = program;
    }
}
