/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/image.hpp"
#include "wrap.hpp"

enum texture_flags {
    TEXTURE_BLOCK_COMPRESSION = 0x01,
};

struct texture_id {
    uint32_t index : 24;
    uint32_t id : 8;

    inline texture_id() {
        id = 0;
        index = 0;
    }

    inline texture_id(uint8_t id, uint32_t index) {
        this->id = id;
        this->index = index;
    }
};

static_assert(sizeof(texture_id) == 0x04, "\"texture_id\" struct should have a size of 0x04");

constexpr bool operator==(const texture_id& left, const texture_id& right) {
    return (((uint64_t)left.id << 32) | left.index) == (((uint64_t)right.id << 32) | right.index);
}

constexpr bool operator!=(const texture_id& left, const texture_id& right) {
    return !(left == right);
}

constexpr bool operator<(const texture_id& left, const texture_id& right) {
    return (((uint64_t)left.id << 32) | left.index) < (((uint64_t)right.id << 32) | right.index);
}

constexpr bool operator>(const texture_id& left, const texture_id& right) {
    return right < left;
}

constexpr bool operator<=(const texture_id& left, const texture_id& right) {
    return !(right < left);
}

constexpr bool operator>=(const texture_id& left, const texture_id& right) {
    return !(left < right);
}

struct texture {
    int32_t ref_count;
    texture_id id;
    texture_flags flags;
    int16_t width;
    int16_t height;
    GLuint glid;
    GLenum target;
    GLenum internal_format;
    int32_t max_mipmap_level;
    int32_t size_texmem;

    texture();

    int32_t get_height_align_mip_level(uint8_t mip_level = 0) const;
    int32_t get_size_mip_level(uint8_t mip_level = 0) const;
    int32_t get_width_align_mip_level(uint8_t mip_level = 0) const;

    inline int32_t get_height_mip_level(uint8_t mip_level) const {
        return max_def(height >> mip_level, 1);
    }
    inline int32_t get_width_mip_level(uint8_t mip_level) const {
        return max_def(width >> mip_level, 1);
    }
};

static_assert(sizeof(texture) == 0x24, "\"texture\" struct should have a size of 0x24");

extern int32_t(FASTCALL* texture_info_get_id)(const char* name);
extern void (FASTCALL* txp_set_load)(const void* data, texture*** textures, uint32_t* texture_ids);

extern texture* texture_alloc(texture_id id);
extern void texture_apply_color_tone(texture* chg_tex,
    texture* org_tex, const color_tone* col_tone);
extern texture* texture_create_copy_texture(texture_id id, texture* org_tex);
extern texture* texture_create_copy_texture_apply_color_tone(
    texture_id id, texture* org_tex, const color_tone* col_tone);
extern texture* texture_load_tex_2d(texture_id id, GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, const void** data_ptr, bool use_high_anisotropy);
extern texture* texture_load_tex_cube_map(texture_id id, GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, const void** data_ptr);
extern void texture_release(texture* tex);

extern void texture_array_free(texture** arr);

extern void texture_set_params(GLenum target, int32_t max_mipmap_level, bool use_high_anisotropy);

extern texture_id texture_manager_get_copy_id(uint32_t id);
extern texture* texture_manager_get_texture(uint32_t id);
extern texture* texture_manager_get_texture(texture_id id);

extern void texture_patch();
