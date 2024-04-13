/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "screen_shot.hpp"
#include "gl_state.hpp"
#include "render_context.hpp"
#include "shader_ft.hpp"
#include "texture.hpp"
#include <Helpers.h>

struct ScreenShotData {
    prj::string path;
    int32_t format;
    bool screen_shot_4x;
    int32_t width;
    int32_t height;
    GLuint buffer;
    const void* tex_data;
};

struct ScreenShotImpl {
    void* __vftable;
    prj::string path;
    int32_t format;
    bool screen_shot_4x;
    int32_t width;
    int32_t height;
    int32_t curr_width;
    int32_t curr_height;
    bool capture;
    texture* tex;
    GLuint data_buffers[2];
    ScreenShotData ss_data[2];
    int32_t data_buffer_index;
    void* thread; // Thrd_t
};

HOOK(bool, FASTCALL, sub_1401898E0, 0x00000001401898E0, __int64 a1, texture* tex) {
    struct struc_21 {
        int32_t width;
        int32_t height;
        int32_t pix_fmt;
        void* field_10;
    };

    if (!tex || !tex->width || !tex->height || !tex->glid)
        return false;

    static struc_21* (FASTCALL* sub_140188010)(struc_21*, int32_t width, int32_t height, int32_t pix_fmt)
        = (struc_21 * (FASTCALL*)(struc_21*, int32_t width, int32_t height, int32_t pix_fmt))0x0000000140188010;
    static void (FASTCALL* sub_1401880E0)(struc_21*)
        = (void (FASTCALL*)(struc_21*))0x00000001401880E0;
    static struc_21* (FASTCALL* sub_140188100)(struc_21*, struc_21*)
        = (struc_21 * (FASTCALL*)(struc_21*, struc_21*))0x0000000140188100;
    static void* (FASTCALL* sub_140188410)(struc_21*)
        = (void* (FASTCALL*)(struc_21*))0x0000000140188410;
    static struc_21* (FASTCALL* sub_140189B10)(struc_21*, struc_21*, bool, bool)
        = (struc_21 * (FASTCALL*)(struc_21*, struc_21*, bool, bool))0x0000000140189B10;

    struc_21 v9 = {};
    sub_140188010(&v9, tex->width, tex->height, 1);

    gl_state_bind_texture_2d(tex->glid);
    glGetTexImageDLL(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, sub_140188410(&v9));
    gl_state_bind_texture_2d(0);

    struc_21 v10 = {};
    sub_140188100(&v9, sub_140189B10(&v9, &v10, false, true));
    sub_1401880E0(&v10);
    sub_140188100((struc_21*)a1, &v9);
    sub_1401880E0(&v9);
    return true;
}

HOOK(void, FASTCALL, ScreenShotImpl__get_data, 0x0000000140557F50, ScreenShotImpl* impl) {
    gl_state_active_bind_texture_2d(0, impl->tex->glid);
    glCopyTexSubImage2DDLL(GL_TEXTURE_2D, 0, 0, 0, 0, 0, impl->width, impl->height);

    float_t ratio = (float_t)(impl->curr_height * impl->curr_width) / (float_t)(impl->width * impl->height);
    float_t scale = 1.0f;
    if (ratio < 0.1f)
        uniform->arr[U_REDUCE] = 1;
    else if (ratio < 0.6f) {
        scale = 0.75f;
        uniform->arr[U_REDUCE] = 1;
    }
    else
        uniform->arr[U_REDUCE] = 0;

    quad_shader_data quad = {};
    quad.g_texcoord_modifier = { 0.5f, 0.5f, 0.5f, 0.5f };
    quad.g_texel_size = { scale / (float_t)impl->width, scale / (float_t)impl->height, 0.0f, 0.0f };
    quad.g_color = 0.0f;
    quad.g_texture_lod = 0.0f;

    rctx->quad_ubo.WriteMemory(quad);
    rctx->quad_ubo.Bind(0);

    gl_state_disable_depth_test();
    gl_state_disable_blend();
    gl_state_disable_cull_face();
    glViewportDLL(0, 0, impl->curr_width, impl->curr_height);

    shaders_ft.set(SHADER_FT_REDUCE);
    gl_state_bind_vertex_array(rctx->common_vao);
    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
    gl_state_bind_vertex_array(0);
    shader::unbind();

    gl_state_enable_cull_face();
    gl_state_enable_blend();
    gl_state_enable_depth_test();
}

HOOK(void, FASTCALL, ScreenShotData__get_data, 0x0000000140557BE0, ScreenShotData* data) {
    if (!data->screen_shot_4x) {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, data->buffer);
        glReadPixelsDLL(0, 0, data->width, data->height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        glGetErrorDLL();
    }
}

void screen_shot_patch() {
    INSTALL_HOOK(sub_1401898E0);

    INSTALL_HOOK(ScreenShotImpl__get_data);
    INSTALL_HOOK(ScreenShotData__get_data);
}
