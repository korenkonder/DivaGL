/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "texture.hpp"

void(FASTCALL* texture_apply_color_tone)(texture* chg_tex, texture* org_tex, color_tone* col_tone)
    = (void(FASTCALL*)(texture * chg_tex, texture * org_tex, color_tone * col_tone))0x00000001403B5DF0;
texture* (FASTCALL* texture_alloc)(texture_id id) = (texture * (FASTCALL*)(texture_id id))0x000000014069A390;
int32_t(FASTCALL* texture_get_size)(AFTGLenum internal_format, int32_t width, int32_t height)
    = (int32_t(FASTCALL *)(AFTGLenum internal_format, int32_t width, int32_t height))0x000000014069A6D0;
texture* (FASTCALL* texture_create_copy_texture)(texture_id id, texture* org_tex)
= (texture * (FASTCALL*)(texture_id id, texture * org_tex))0x000000014069B550;
texture* (FASTCALL* texture_load_tex_cube_map)(texture_id id, uint32_t internal_format,
    int32_t width, int32_t height, int32_t max_mipmap_level, const void** data_ptr)
    = (texture * (FASTCALL*)(texture_id id, uint32_t internal_format,
        int32_t width, int32_t height, int32_t max_mipmap_level, const void** data_ptr))0x000000014069B860;
texture* (FASTCALL* texture_load_tex_2d)(texture_id id, uint32_t internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, const void** data_ptr, bool use_high_anisotropy)
    = (texture * (FASTCALL*)(texture_id id, uint32_t internal_format, int32_t width, int32_t height,
        int32_t max_mipmap_level, const void** data_ptr, bool use_high_anisotropy))0x000000014069B8E0;
int32_t(FASTCALL* texture_info_get_id)(const char* name)
    = (int32_t(FASTCALL*)(const char* name))0x000000014069CBD0;
texture* (FASTCALL* texture_manager_get_texture)(uint32_t id)
    = (texture * (FASTCALL*)(uint32_t id))0x000000014069CD70;
void (FASTCALL* txp_set_load)(const void* data, texture*** textures, uint32_t* texture_ids)
    = (void (FASTCALL*)(const void* data, texture * **textures, uint32_t * texture_ids))0x000000014069D4B0;
void(FASTCALL* texture_release)(texture* tex) = (void(FASTCALL*)(texture * tex))0x000000014069DA70;

texture::texture() : ref_count(), flags(), width(), height(),
glid(), target(), internal_format(), max_mipmap_level(), size_texmem() {

}

int32_t texture::get_height_align_mip_level(uint8_t mip_level) const {
    if (flags & TEXTURE_BLOCK_COMPRESSION)
        return max_def((uint32_t)height >> mip_level, 4u);
    else
        return max_def((uint32_t)height >> mip_level, 1u);
}

int32_t texture::get_size_mip_level(uint8_t mip_level) const {
    return texture_get_size(internal_format,
        get_width_mip_level(mip_level), get_height_mip_level(mip_level));
}

int32_t texture::get_width_align_mip_level(uint8_t mip_level) const {
    if (flags & TEXTURE_BLOCK_COMPRESSION)
        return max_def((uint32_t)width >> mip_level, 4u);
    else
        return max_def((uint32_t)width >> mip_level, 1u);
}

void texture_array_free(texture** arr) {
    if (!arr)
        return;

    for (texture** i = arr; *i; i++)
        texture_release(*i);
    free_def(arr);
}
