/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "hook.hpp"
#include "Glitter/glitter.hpp"
#include "data_test/rob_osage_test.hpp"
#include "graphics/color_change_dw.hpp"
#include "mdl/disp_manager.hpp"
#include "pv_game/pv_game.hpp"
#include "rob/rob.hpp"
#include "auth_3d.hpp"
#include "camera.hpp"
#include "effect.hpp"
#include "light_param.hpp"
#include "object.hpp"
#include "print.hpp"
#include "resource.h"
#include "render_manager.hpp"
#include "render_texture.hpp"
#include "shader_ft.hpp"
#include "sprite.hpp"
#include "stage.hpp"
#include "texture.hpp"
#include <Helpers.h>

static HGLRC FASTCALL glut_create_context(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int32_t a5);

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

#ifdef USE_STUB
    wrap_addresses(true);
#else
    wrap_addresses();
#endif

    auth_3d_patch();
    camera_patch();
    color_change_dw_patch();
    mdl::disp_manager_patch();
    effect_patch();
    light_param_patch();
    object_patch();
    pv_game_patch();
    render_manager_patch();
    render_texture_patch();
    rob_patch();
    shadow_patch();
    sprite_patch();
    stage_patch();
    texture_patch();
    Glitter::Patch();

    rctx = new render_context;
    int32_t ret = originaldata_init();

    extern size_t diva_handle;
    diva_handle = (size_t)WindowFromDC(wglGetCurrentDC());

    rctx->init();
    sprite_manager_init();
    rob_osage_test_init();
    return ret;
}

HOOK(int32_t, FASTCALL, data_free, 0x000000140192490) {
    sprite_manager_free();
    rctx->free();

    int32_t ret = originaldata_free();
    delete rctx;
    rctx = 0;
    return ret;
}

HOOK(void, FASTCALL, sub_140194CD0, 0x000000140194CD0) {
    rctx->ctrl();
    originalsub_140194CD0();
}

HOOK(void, FASTCALL, _gl_state_get, 0x0000000140442DF0) {
    gl_state_get();
}

HOOK(void, FASTCALL, rndr__Render__update_res, 0x00000001404A9480, rndr::Render* rend, bool set, int32_t base_downsample) {
    rend->update_res(set, base_downsample);
}

HOOK(void, FASTCALL, rndr__Render__take_ss, 0x00000001404A9CD0,
    rndr::Render* rend, texture* tex, bool vertical, float_t horizontal_offset) {
    rend->take_ss(tex, vertical, horizontal_offset);
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
        f.read(data, size, true);
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

HOOK(void, FASTCALL, set_render_defaults, 0x00000001401948B0) {
    glClearDepthDLL(1.0);
    glClearStencilDLL(0);
    //glShadeModelDLL(GL_SMOOTH);
    glPolygonModeDLL(GL_FRONT_AND_BACK, GL_FILL);
    glDisableDLL(GL_MULTISAMPLE);
    glDisableDLL(GL_DITHER);
    glEnableDLL(GL_DEPTH_TEST);
    glDepthFuncDLL(GL_LEQUAL);
    glEnableDLL(GL_CULL_FACE);
    glFrontFaceDLL(GL_CCW);
    glCullFaceDLL(GL_BACK);
    //glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_ZERO);
    //glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FIXED_ONLY_ARB);
    //glClampColorARB(GL_CLAMP_READ_COLOR_ARB, GL_FIXED_ONLY_ARB);
    glPixelStoreiDLL(GL_UNPACK_ALIGNMENT, 1);
}

HOOK(GLenum, FASTCALL, gl_check_framebuffer_status, 0x0000000140503240) {
    GLenum ret = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
    if (ret != GL_FRAMEBUFFER_COMPLETE)
        return ret;

    return GL_FRAMEBUFFER_COMPLETE;
}

void hook_funcs() {
    WRITE_NOP_5(0x0000000140441290);
    WRITE_JUMP(0x00000001404412E2, 0x00000001404413BD);
    WRITE_NOP(0x00000001404412E7, 0xD6);

    INSTALL_HOOK(data_init);
    INSTALL_HOOK(data_free);

    INSTALL_HOOK(sub_140194CD0);

    INSTALL_HOOK(_gl_state_get);

    INSTALL_HOOK(rndr__Render__update_res);
    INSTALL_HOOK(rndr__Render__take_ss);
    INSTALL_HOOK(rndr__Render__init_tone_map_buffers);
    INSTALL_HOOK(rndr__Render__init_render_buffers);
    INSTALL_HOOK(rndr__Render__free);

    INSTALL_HOOK(rndr__RenderManager__render_ctrl);
    INSTALL_HOOK(rndr__RenderManager__render_all);

    INSTALL_HOOK(shader_free);
    INSTALL_HOOK(shader_load_all_shaders);
    INSTALL_HOOK(shader_get_index_by_name);

    INSTALL_HOOK(env_set_blend_color);
    INSTALL_HOOK(env_set_offset_color);

    INSTALL_HOOK(set_render_defaults);

    INSTALL_HOOK(gl_check_framebuffer_status);

    uint8_t buf[0x0C] = {};
    buf[0x00] = 0x48; // mov rax, 0
    buf[0x01] = 0xB8;
    buf[0x0A] = 0xFF; // jmp rax
    buf[0x0B] = 0xE0;

    *(uint64_t*)&buf[0x02] = (uint64_t)&printf_proxy;
    WRITE_MEMORY_STRING(0x00000001401D3860, buf, 0x0C);

    *(uint64_t*)&buf[0x02] = (uint64_t)&printf_proxy;
    WRITE_MEMORY_STRING(0x00000001400DE640, buf, 0x0C);

    *(uint64_t*)&buf[0x02] = (uint64_t)&gl_state_get;
    WRITE_MEMORY_STRING(0x0000000140442FB4, buf, 0x0C);

    extern size_t glut_handle;
    *(uint64_t*)&buf[0x02] = (uint64_t)&glut_create_context;
    WRITE_MEMORY_STRING(glut_handle + 0x0000A970, buf, 0x0C);

    // Patch font shader index
    WRITE_MEMORY(0x00000001401982D8, uint32_t, SHADER_FT_FONT);
}

static HGLRC FASTCALL glut_create_context(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int32_t a5) {
    extern size_t glut_handle;
    HDC& XHDC = *(HDC*)(glut_handle + 0x55F20);

    wrap_addresses();

    if (true/*a5*/) {
        typedef HGLRC(GLAPIENTRY* PFNWGLCREATECONTEXTGLUTPROC)(HDC hDc);

        HGLRC tmp_ctx = wglCreateContextGLUT(XHDC);;
        wglMakeCurrentGLUT(XHDC, tmp_ctx);
        PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB
            = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddressGLUT("wglCreateContextAttribsARB");

        int32_t minor = 6;
        HGLRC ctx = 0;
        while (!ctx || minor < 3) {
            const int32_t attrib_list[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, minor,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
                0,
            };
            ctx = _wglCreateContextAttribsARB(XHDC, 0, attrib_list);;
            minor--;
        }
        wglMakeCurrentGLUT(XHDC, ctx);
        wglDeleteContextGLUT(tmp_ctx);
        return ctx;
    }
    else
        return wglCreateContextGLUT(XHDC);
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
