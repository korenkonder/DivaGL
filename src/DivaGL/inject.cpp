/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "inject.hpp"
#include "gl.hpp"
#include "print.hpp"
#include "shader_table.hpp"
#include "shader_glsl_ft.hpp"
#include "sprite.hpp"
#include <Windows.h>

struct patch_struct {
    void* address;
    size_t func_ptr;
};

void inject_data(void* address, const void* data, size_t count) {
    DWORD old_protect;
    VirtualProtect(address, count, PAGE_EXECUTE_READWRITE, &old_protect);
    memmove(address, (const void*)data, count);
    VirtualProtect(address, count, old_protect, &old_protect);
}

void inject_uint8_t(void* address, uint8_t data) {
    DWORD old_protect;
    VirtualProtect(address, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old_protect);
    *(uint8_t*)address = data;
    VirtualProtect(address, sizeof(uint8_t), old_protect, &old_protect);
}

void inject_uint16_t(void* address, uint16_t data) {
    DWORD old_protect;
    VirtualProtect(address, sizeof(uint16_t), PAGE_EXECUTE_READWRITE, &old_protect);
    *(uint16_t*)address = data;
    VirtualProtect(address, sizeof(uint16_t), old_protect, &old_protect);
}

void inject_uint32_t(void* address, uint32_t data) {
    DWORD old_protect;
    VirtualProtect(address, sizeof(uint32_t), PAGE_EXECUTE_READWRITE, &old_protect);
    *(uint32_t*)address = data;
    VirtualProtect(address, sizeof(uint32_t), old_protect, &old_protect);
}

void inject_uint64_t(void* address, uint64_t data) {
    DWORD old_protect;
    VirtualProtect(address, sizeof(uint64_t), PAGE_EXECUTE_READWRITE, &old_protect);
    *(uint64_t*)address = data;
    VirtualProtect(address, sizeof(uint64_t), old_protect, &old_protect);
}

extern size_t glut_handle;

static patch_struct patch_data[] = {
    { (void*)0x00000001405E4CE0, (uint64_t)&shader_set, },
    { (void*)0x00000001405E4FC0, (uint64_t)&shader_load_all_shaders, },
    { (void*)0x00000001405E4DB0, (uint64_t)&shader_free, },
    { (void*)0x00000001405E4B50, (uint64_t)&shader_bind, },
    { (void*)0x00000001401D3860, (uint64_t)&printf_proxy, },
    { (void*)0x00000001400DE640, (uint64_t)&printf_proxy, },
    { (void*)0x0000000140442FB4, (uint64_t)&gl_state_get, },
};

static HGLRC FASTCALL glut_create_context(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int32_t a5);

void inject_patches() {
    // Reorder gl_state_get end
    inject_data((void*)0x0000000140442F99,
        "\xFF\x15\x51\x2B\x52\x00\x48\x8B\x5C\x24\x30\x48\x8B\x74\x24\x38"
        "\x48\x8B\x7C\x24\x40\x48\x83\xC4\x20\x41\x5E\xC3", 28);

    uint8_t buf[0x0C];
    buf[0x00] = 0x48;
    buf[0x01] = 0xB8;
    buf[0x0A] = 0xFF;
    buf[0x0B] = 0xE0;

    for (int32_t i = 0; i < sizeof(patch_data) / sizeof(patch_struct); i++)
        if (patch_data[i].func_ptr) {
            *(uint64_t*)&buf[0x02] = patch_data[i].func_ptr;
            inject_data(patch_data[i].address, buf, 0x0C);
        }
    ;

    *(uint64_t*)&buf[0x02] = (uint64_t)&glut_create_context;
    inject_data((void*)(glut_handle + 0x0000A970), buf, 0x0C);

    inject_uint64_t((void*)&shader_name_bind_func_table[0].bind_func, (uint64_t)&shader_bind_blinn);
    inject_uint64_t((void*)&shader_name_bind_func_table[1].bind_func, (uint64_t)&shader_bind_cloth);
    inject_uint64_t((void*)&shader_name_bind_func_table[2].bind_func, (uint64_t)&shader_bind_hair);
    inject_uint64_t((void*)&shader_name_bind_func_table[3].bind_func, (uint64_t)&shader_bind_membrane);
    inject_uint64_t((void*)&shader_name_bind_func_table[4].bind_func, (uint64_t)&shader_bind_eye_ball);
    inject_uint64_t((void*)&shader_name_bind_func_table[5].bind_func, (uint64_t)&shader_bind_tone_map);

    // Patch font shader index
    inject_uint32_t((void*)0x00000001401982D8, SHADER_FT_FONT);
}

static HGLRC FASTCALL glut_create_context(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int32_t a5) {
    HDC& hDc = *(HDC*)(glut_handle + 0x55F20);

    wrap_addresses();

    if (true/*a5*/) {
        const int32_t attrib_list[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            0,
        };

        typedef HGLRC(GLAPIENTRY* PFNWGLCREATECONTEXTGLUTPROC)(HDC hDc);

        HGLRC tmp_ctx = wglCreateContextGLUT(hDc);;
        wglMakeCurrentGLUT(hDc, tmp_ctx);
        PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB
            = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddressGLUT("wglCreateContextAttribsARB");
        HGLRC ctx = _wglCreateContextAttribsARB(hDc, 0, attrib_list);
        wglMakeCurrentGLUT(hDc, ctx);
        wglDeleteContextGLUT(tmp_ctx);
        return ctx;
    }
    else 
        return wglCreateContextGLUT(hDc);
}
