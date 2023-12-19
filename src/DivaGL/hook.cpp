/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "hook.hpp"
#include "Glitter/glitter.hpp"
#include "mdl/disp_manager.hpp"
#include "rob/rob.hpp"
#include "auth_3d.hpp"
#include "camera.hpp"
#include "effect.hpp"
#include "print.hpp"
#include "resource.h"
#include "shader_ft.hpp"
#include "sprite.hpp"
#include "stage.hpp"
#include "texture.hpp"
#include <Helpers.h>

#ifdef DEBUG
static void APIENTRY gl_debug_output(GLenum source, GLenum type, uint32_t id,
    GLenum severity, GLsizei length, const char* message, const void* userParam);
#endif

HOOK(int32_t, FASTCALL, data_init, 0x0000000140192FF0) {
#ifdef DEBUG
    typedef void (APIENTRY* GLDEBUGPROC)(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

    typedef void (GLAPIENTRY* PFNGLDEBUGMESSAGECALLBACKPROC)(GLDEBUGPROC callback, const void* userParam);
    PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback
        = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddressDLL("glDebugMessageCallback");
    typedef void (GLAPIENTRY* PFNGLDEBUGMESSAGECONTROLPROC)(GLenum source,
        GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
    PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl
        = (PFNGLDEBUGMESSAGECONTROLPROC)wglGetProcAddressDLL("glDebugMessageControl");

    glEnableDLL(GL_DEBUG_OUTPUT);
    glEnableDLL(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
#endif

    wrap_addresses();

    auth_3d_patch();
    camera_patch();
    mdl::disp_manager_patch();
    render_manager_patch();
    rob_patch();
    sprite_patch();
    stage_patch();
    effect_patch();
    Glitter::Patch();

    rctx = new render_context;
    int32_t ret = originaldata_init();

    extern size_t diva_handle;
    diva_handle = (size_t)WindowFromDC(wglGetCurrentDC());

    rctx->init();
    sprite_manager_init();
    return ret;
}

HOOK(int32_t, FASTCALL, data_free, 0x000000140192490) {
    sprite_manager_free();
    rctx->free();

    int32_t ret = originaldata_free();
    delete rctx;
    return ret;
}

HOOK(void, FASTCALL, sub_140194CD0, 0x000000140194CD0) {
    rctx->ctrl();
    originalsub_140194CD0();
}

HOOK(void, FASTCALL, _gl_state_get, 0x0000000140442DF0) {
    gl_state_get();
}

HOOK(bool, FASTCALL, obj_mesh_vertex_buffer__load, 0x0000000140458280, obj_mesh_vertex_buffer* obj_vb, obj_mesh* mesh) {
    return obj_vb->load(mesh);
}

HOOK(void, FASTCALL, rndr__Render__update_res, 0x00000001404A9480, rndr::Render* rend, bool set, int32_t base_downsample) {
    rend->update_res(set, base_downsample);
}

HOOK(void, FASTCALL, rndr__Render__init_tone_map_buffers, 0x00000001404A9FF0, rndr::Render* rend) {
    rend->init_tone_map_buffers();
}

HOOK(void, FASTCALL, rndr__Render__init_render_buffers, 0x00000001404AB0C0, rndr::Render* rend,
    int32_t width, int32_t height, int32_t ssaa, int32_t hd_res, int32_t ss_alpha_mask) {
    rend->init_render_buffers(width, height, ssaa, hd_res, ss_alpha_mask);
}

HOOK(void, FASTCALL, rndr__Render__free, 0x00000001404AB900, rndr::Render* rend) {
    rend->free();
}

HOOK(void, FASTCALL, rndr__RenderManager__render_ctrl, 0x0000000140502C90) {
    render_manager->render->ctrl();
}

HOOK(void, FASTCALL, rndr__RenderManager__render_all, 0x0000000140502CA0) {
    render_manager->render_all();
}

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

HOOK(void, FASTCALL, ScreenShotImpl__get_data, 0x0000000140557F50, ScreenShotImpl* impl) {
    gl_state_active_bind_texture_2d(0, impl->tex->tex);
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

HOOK(void, FASTCALL, shader_free, 0x00000001405E4DB0) {
    shaders_ft.unload();
}

HOOK(void, FASTCALL, shader_load_all_shaders, 0x00000001405E4FC0) {
    extern size_t dll_handle;
    HRSRC res = FindResourceA((HMODULE)dll_handle, MAKEINTRESOURCEA(IDR_FT_SHADERS_FARC1), "ft_shaders_farc");
    if (res) {
        DWORD size = SizeofResource((HMODULE)dll_handle, res);
        HGLOBAL data = LoadResource((HMODULE)dll_handle, res);

        farc f;
        f.read(data, size);
        shaders_ft.load(&f, false, "ft", shader_ft_table, shader_ft_table_size,
            shader_ft_bind_func_table, shader_ft_bind_func_table_size,
            shader_ft_get_index_by_name);
    }
}

HOOK(int32_t, FASTCALL, shader_get_index_by_name, 0x00000001405E4ED0, const char* name) {
    return shaders_ft.get_index_by_name(name);
}

HOOK(void, FASTCALL, env_set_blend_color, 0x00000001405E5600, float_t r, float_t g, float_t b, float_t a) {
    rctx->obj_batch.g_blend_color = { r, g, b, a };
}

HOOK(void, FASTCALL, env_set_offset_color, 0x00000001405E5630, float_t r, float_t g, float_t b, float_t a) {
    rctx->obj_batch.g_offset_color = { r, g, b, a };
}

void hook_funcs() {
    INSTALL_HOOK(data_init);
    INSTALL_HOOK(data_free);

    INSTALL_HOOK(sub_140194CD0);

    INSTALL_HOOK(_gl_state_get);

    INSTALL_HOOK(obj_mesh_vertex_buffer__load);

    INSTALL_HOOK(rndr__Render__update_res);
    INSTALL_HOOK(rndr__Render__init_tone_map_buffers);
    INSTALL_HOOK(rndr__Render__init_render_buffers);
    INSTALL_HOOK(rndr__Render__free);

    INSTALL_HOOK(rndr__RenderManager__render_ctrl);
    INSTALL_HOOK(rndr__RenderManager__render_all);

    INSTALL_HOOK(ScreenShotImpl__get_data);
    INSTALL_HOOK(ScreenShotData__get_data);

    INSTALL_HOOK(shader_free);
    INSTALL_HOOK(shader_load_all_shaders);
    INSTALL_HOOK(shader_get_index_by_name);

    INSTALL_HOOK(env_set_blend_color);
    INSTALL_HOOK(env_set_offset_color);
}

#ifdef DEBUG
static void APIENTRY gl_debug_output(GLenum source, GLenum type, uint32_t id,
    GLenum severity, GLsizei length, const char* message, const void* userParam) {
    if (!id && severity == GL_DEBUG_SEVERITY_NOTIFICATION
        || id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    printf_divagl("########################################\n");
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        printf_divagl("Type: Error;                ");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        printf_divagl("Type: Deprecated Behaviour; ");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        printf_divagl("Type: Undefined Behaviour;  ");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        printf_divagl("Type: Portability;          ");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        printf_divagl("Type: Performance;          ");
        break;
    case GL_DEBUG_TYPE_MARKER:
        printf_divagl("Type: Marker;               ");
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        printf_divagl("Type: Push Group;           ");
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        printf_divagl("Type: Pop Group;            ");
        break;
    case GL_DEBUG_TYPE_OTHER:
        printf_divagl("Type: Other;                ");
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        printf_divagl("Severity: high;   ");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        printf_divagl("Severity: medium; ");
        break;
    case GL_DEBUG_SEVERITY_LOW:
        printf_divagl("Severity: low;    ");
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        printf_divagl("Severity: notif;  ");
        break;
    }

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        printf_divagl("Source: API\n");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        printf_divagl("Source: Window System\n");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        printf_divagl("Source: Shader Compiler\n");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        printf_divagl("Source: Third Party\n");
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        printf_divagl("Source: Application\n");
        break;
    case GL_DEBUG_SOURCE_OTHER:
        printf_divagl("Source: Other\n");
        break;
    }

    printf_divagl("Debug message (%d): %s\n", id, message);
    printf_divagl("########################################\n\n");
}
#endif
