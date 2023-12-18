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
#include "resource.h"
#include "shader_ft.hpp"
#include "sprite.hpp"
#include "stage.hpp"
#include "texture.hpp"
#include <Helpers.h>

HOOK(int32_t, FASTCALL, data_init, 0x0000000140192FF0) {
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

    INSTALL_HOOK(shader_free);
    INSTALL_HOOK(shader_load_all_shaders);
    INSTALL_HOOK(shader_get_index_by_name);

    INSTALL_HOOK(env_set_blend_color);
    INSTALL_HOOK(env_set_offset_color);
}