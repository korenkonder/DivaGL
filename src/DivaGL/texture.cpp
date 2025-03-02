/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "texture.hpp"
#include "gl_state.hpp"
#include "../AFTModsShared/types.hpp"
#include <Helpers.h>

struct texture_handler {
    texture tex;
    int32_t field_2C;
    int32_t field_30;
    int32_t field_34;

    texture_handler();
};

static_assert(sizeof(texture_handler) == 0x30, "\"texture_handler\" struct should have a size of 0x30");

struct texture_manager {
    prj::map<texture_id, texture_handler> textures;
    int16_t entry_count;
    int16_t alloc_count;
    __declspec(align(8)) int32_t texmem_now_size;
    int32_t texmem_peak_size;
    int32_t texmem_now_size_by_type[4];
    int32_t texmem_peak_size_by_type[4];
    int32_t copy_count;
    bool field_44;
};

static_assert(sizeof(texture_manager) == 0x48, "\"texture_manager\" struct should have a size of 0x48");

static void texture_bind(GLenum target, GLuint texture);
static void texture_get_format_type_by_internal_format(GLenum internal_format, GLenum* format, GLenum* type);
static GLuint texture_get_working_internal_format(GLuint internal_format);
static int32_t texture_load_pixels(GLenum target, GLenum internal_format,
    int32_t width, int32_t height, int32_t level, const void* data);
static texture* texture_load_tex(texture_id id, GLenum target,
    GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, const void** data_ptr, bool use_high_anisotropy);
static void texture_set_params(GLenum target, int32_t max_mipmap_level, bool use_high_anisotropy);

texture_manager*& texture_manager_work_ptr = *(texture_manager**)0x000000014CC96988;

texture* texture_create_copy_texture_apply_color_tone(
    texture_id id, texture* org_tex, const color_tone* col_tone) {
    if (!org_tex || !col_tone || org_tex->target != GL_TEXTURE_2D)
        return 0;

    GLenum format = GL_ZERO;
    GLenum type = GL_ZERO;
    if (!(org_tex->flags & TEXTURE_BLOCK_COMPRESSION))
        texture_get_format_type_by_internal_format(org_tex->internal_format, &format, &type);

    std::vector<void*> vec_data;
    vec_data.reserve((size_t)org_tex->max_mipmap_level + 1);

    texture_bind(org_tex->target, org_tex->glid);
    for (int32_t i = 0; i <= org_tex->max_mipmap_level; i++) {
        int32_t size = org_tex->get_size_mip_level(i);
        void* data = prj::HeapCMallocAllocate(prj::HeapCMallocTemp,
            org_tex->get_size_mip_level(i), "create_copy_texture_apply_color_tone()");
        if (!data)
            break;

        if (org_tex->flags & TEXTURE_BLOCK_COMPRESSION) {
            int32_t width_align = org_tex->get_width_align_mip_level(i);
            int32_t height_align = org_tex->get_height_align_mip_level(i);

            glGetCompressedTexImage(org_tex->target, i, data);
            if (org_tex->internal_format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
                dxt1_image_apply_color_tone(width_align, height_align, size, (dxt1_block*)data, col_tone);
            else
                dxt5_image_apply_color_tone(width_align, height_align, size, (dxt5_block*)data, col_tone);
        }
        else if (org_tex->internal_format == GL_RGB5) {
            int32_t width_align = org_tex->get_width_align_mip_level(i);
            int32_t height_align = org_tex->get_height_align_mip_level(i);

            glGetTexImageDLL(org_tex->target, i, format, type, data);
            rgb565_image_apply_color_tone(width_align, height_align, size, (rgb565*)data, col_tone);
        }
        else
            glGetTexImageDLL(org_tex->target, i, format, type, data);
        gl_state_get_error();
        vec_data.push_back(data);
    }
    texture_bind(org_tex->target, 0);

    texture* tex = texture_load_tex_2d(id, org_tex->internal_format,
        org_tex->width, org_tex->height, org_tex->max_mipmap_level, (const void**)vec_data.data(), true);

    for (void*& i : vec_data)
        prj::HeapCMallocFree(prj::HeapCMallocTemp, i);

    return tex;
}

inline texture_id texture_manager_get_copy_id(uint32_t id) {
    if (!texture_manager_work_ptr || id != 0x30)
        return texture_id(-1, -1);

    int32_t& copy_count = texture_manager_work_ptr->copy_count;
    texture_id tex_id(0x30, copy_count);
    if (copy_count < 0x0FFFFFFF)
        copy_count++;
    else
        copy_count = 0;
    return tex_id;

}

HOOK(void, FASTCALL, texture_apply_color_tone, 0x00000001403B5DF0,
    texture* chg_tex, texture* org_tex, color_tone* col_tone) {
    if (!chg_tex || !org_tex || !col_tone
        || chg_tex->internal_format != org_tex->internal_format
        || chg_tex->width != org_tex->width
        || chg_tex->height != org_tex->height
        || chg_tex->max_mipmap_level != org_tex->max_mipmap_level)
        return;

    for (int32_t i = 0; i <= org_tex->max_mipmap_level; i++) {
        int32_t size = org_tex->get_size_mip_level(i);
        void* data = prj::HeapCMallocAllocate(prj::HeapCMallocTemp, size, "imgf_color_tone_cpu()");
        if (!data)
            break;

        if (org_tex->flags & TEXTURE_BLOCK_COMPRESSION) {
            int32_t width_align = org_tex->get_width_align_mip_level(i);
            int32_t height_align = org_tex->get_height_align_mip_level(i);

            texture_bind(org_tex->target, org_tex->glid);
            glGetCompressedTexImage(org_tex->target, i, data);
            if (org_tex->internal_format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
                dxt1_image_apply_color_tone(width_align, height_align, size, (dxt1_block*)data, col_tone);
            else
                dxt5_image_apply_color_tone(width_align, height_align, size, (dxt5_block*)data, col_tone);

            texture_bind(chg_tex->target, chg_tex->glid);
            int32_t width = org_tex->get_width_mip_level(i);
            int32_t height = org_tex->get_height_mip_level(i);
            glCompressedTexSubImage2D(chg_tex->target, i, 0, 0, width, height,
                chg_tex->internal_format, size, data);
        }
        else if (org_tex->internal_format == GL_RGB5) {
            int32_t width_align = org_tex->get_width_align_mip_level(i);
            int32_t height_align = org_tex->get_height_align_mip_level(i);

            texture_bind(org_tex->target, org_tex->glid);
            glGetTexImageDLL(org_tex->target, i, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
            rgb565_image_apply_color_tone(width_align, height_align, size, (rgb565*)data, col_tone);

            texture_bind(chg_tex->target, chg_tex->glid);
            int32_t width = org_tex->get_width_mip_level(i);
            int32_t height = org_tex->get_height_mip_level(i);
            glTexSubImage2DDLL(chg_tex->target, i, 0, 0, width, height,
                GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        }
        gl_state_get_error();
        prj::HeapCMallocFree(prj::HeapCMallocTemp, data);
    }
    texture_bind(org_tex->target, 0);
}

HOOK(texture*, FASTCALL, texture_alloc, 0x000000014069A390, texture_id id) {
    if (!texture_manager_work_ptr)
        return 0;

    auto elem = texture_manager_work_ptr->textures.find(id);
    if (elem != texture_manager_work_ptr->textures.end()) {
        elem->second.tex.ref_count++;
        texture_manager_work_ptr->entry_count++;
        return &elem->second.tex;
    }

    auto ret = texture_manager_work_ptr->textures.insert({ id, texture_handler() });
    if (!ret.second)
        return 0;

    texture* tex = &ret.first->second.tex;
    tex->ref_count = 1;
    tex->id = id;
    texture_manager_work_ptr->alloc_count++;
    texture_manager_work_ptr->entry_count++;
    return tex;
}

HOOK(int32_t, FASTCALL, texture_get_size, 0x000000014069A6D0, GLenum internal_format, int32_t width, int32_t height) {
    int32_t size = width * height;
    switch (internal_format) {
    case GL_ALPHA8:
    case GL_LUMINANCE8:
    case GL_INTENSITY8:
    case GL_R8:
        return size;
    case GL_LUMINANCE8_ALPHA8:
    case GL_RGB5:
    case GL_RGBA4:
    case GL_RGB5_A1:
    case GL_DEPTH_COMPONENT16:
    case GL_RG8:
        return size * 2;
    case GL_RGB8:
    case GL_RGBA8:
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32:
    case GL_R32F:
    case GL_RGBA16F:
    case GL_DEPTH24_STENCIL8:
    case GL_DEPTH_COMPONENT32F:
    case GL_R11F_G11F_B10F:
    case GL_RGB9_E5:
        return size * 4;
    case GL_RG32F:
        return size * 8;
    case GL_RGBA32F:
        return size * 16;
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RED_RGTC1:
        width = align_val(width, 4);
        height = align_val(height, 4);
        return width * height / 2;
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
    case GL_COMPRESSED_RG_RGTC2:
        width = align_val(width, 4);
        height = align_val(height, 4);
        return width * height;
    default:
        return 0;
    }
}

HOOK(texture*, FASTCALL, texture_create_copy_texture, 0x000000014069B550,
    texture_id id, texture* org_tex) {
    if (org_tex->target != GL_TEXTURE_2D)
        return 0;

    GLenum format = GL_ZERO;
    GLenum type = GL_ZERO;
    if (!(org_tex->flags & TEXTURE_BLOCK_COMPRESSION))
        texture_get_format_type_by_internal_format(org_tex->internal_format, &format, &type);

    prj::vector<void*> vec_data;
    vec_data.reserve((size_t)org_tex->max_mipmap_level + 1);

    texture_bind(org_tex->target, org_tex->glid);
    for (int32_t i = 0; i <= org_tex->max_mipmap_level; i++) {
        void* data = prj::HeapCMallocAllocate(prj::HeapCMallocTemp,
            org_tex->get_size_mip_level(i), "create_copy_texture()");
        if (!data)
            break;

        if (org_tex->flags & TEXTURE_BLOCK_COMPRESSION)
            glGetCompressedTexImage(org_tex->target, i, data);
        else
            glGetTexImageDLL(org_tex->target, i, format, type, data);
        gl_state_get_error();
        vec_data.push_back(data);
    }
    texture_bind(org_tex->target, 0);

    texture* tex = texture_load_tex_2d(id, org_tex->internal_format,
        org_tex->width, org_tex->height, org_tex->max_mipmap_level, (const void**)vec_data.data(), true);

    for (void*& i : vec_data)
        prj::HeapCMallocFree(prj::HeapCMallocTemp, i);

    return tex;
}

HOOK(texture*, FASTCALL, texture_load_tex_cube_map, 0x000000014069B860,
    texture_id id, GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, const void** data_ptr) {
    return texture_load_tex(id, GL_TEXTURE_CUBE_MAP, internal_format,
        width, height, max_mipmap_level, data_ptr, false);
}

HOOK(texture*, FASTCALL, texture_load_tex_2d, 0x000000014069B8E0, 
    texture_id id, GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, const void** data_ptr, bool use_high_anisotropy) {
    return texture_load_tex(id, GL_TEXTURE_2D, internal_format,
        width, height, max_mipmap_level, data_ptr, use_high_anisotropy);
}

HOOK(void, FASTCALL, texture_release, 0x000000014069DA70, texture* tex) {
    if (!texture_manager_work_ptr || !tex)
        return;

    texture_manager_work_ptr->entry_count--;
    if (tex->ref_count > 1) {
        tex->ref_count--;
        return;
    }

    if (tex->glid) {
        glDeleteTextures(1, &tex->glid);
        tex->glid = 0;
    }

    texture_manager_work_ptr->alloc_count--;
    texture_manager_work_ptr->texmem_now_size -= tex->size_texmem;
    texture_manager_work_ptr->texmem_now_size_by_type[tex->id.id >> 4] -= tex->size_texmem;
    texture_manager_work_ptr->textures.erase(tex->id);
}

HOOK(texture*, FASTCALL, texture_manager_get_texture, 0x000000014069CD70, uint32_t id) {
    auto elem = texture_manager_work_ptr->textures.find(*(texture_id*)&id);
    if (elem != texture_manager_work_ptr->textures.end())
        return &elem->second.tex;
    return 0;
}

void texture_patch() {
    INSTALL_HOOK(texture_apply_color_tone);
    INSTALL_HOOK(texture_alloc);
    INSTALL_HOOK(texture_get_size);
    INSTALL_HOOK(texture_create_copy_texture);
    INSTALL_HOOK(texture_load_tex_cube_map);
    INSTALL_HOOK(texture_load_tex_2d);
    INSTALL_HOOK(texture_release);
    INSTALL_HOOK(texture_manager_get_texture);
}

texture_handler::texture_handler() : field_2C(), field_30(), field_34() {

}

inline static void texture_bind(GLenum target, GLuint texture) {
    switch (target) {
    case GL_TEXTURE_2D:
        gl_state_bind_texture_2d(texture);
        break;
    case GL_TEXTURE_CUBE_MAP:
        gl_state_bind_texture_cube_map(texture);
        break;
    }
}

static void texture_get_format_type_by_internal_format(GLenum internal_format, GLenum* format, GLenum* type) {
    GLenum _format;
    GLenum _type;
    switch (internal_format) {
    case GL_ALPHA8:
        _format = GL_R;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_LUMINANCE8:
        _format = GL_R;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_LUMINANCE8_ALPHA8:
        _format = GL_RG;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_INTENSITY8:
        _format = GL_R;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_RGB5:
        _format = GL_RGB;
        _type = GL_UNSIGNED_SHORT_5_6_5_REV;
        break;
    case GL_RGB8:
        _format = GL_RGB;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_RGBA4:
        _format = GL_RGBA;
        _type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
        break;
    case GL_RGB5_A1:
        _format = GL_RGBA;
        _type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
        break;
    case GL_RGBA8:
        _format = GL_RGBA;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32:
        _format = GL_DEPTH_COMPONENT;
        _type = GL_FLOAT;
        break;
    case GL_RG8:
        _format = GL_RG;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_R32F:
        _format = GL_RED;
        _type = GL_FLOAT;
        break;
    case GL_RG32F:
        _format = GL_RG;
        _type = GL_FLOAT;
        break;
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        _format = GL_RGB;
        _type = GL_ZERO;
        break;
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        _format = GL_RGBA;
        _type = GL_ZERO;
        break;
    case GL_RGBA32F:
        _format = GL_RGBA;
        _type = GL_FLOAT;
        break;
    case GL_RGBA16F:
        _format = GL_RGBA;
        _type = GL_HALF_FLOAT;
        break;
    case GL_DEPTH24_STENCIL8:
        _format = GL_DEPTH_STENCIL;
        _type = GL_UNSIGNED_INT_24_8;
        break;
    case GL_R11F_G11F_B10F:
        _format = GL_RGB;
        _type = GL_UNSIGNED_INT_10F_11F_11F_REV;
        break;
    case GL_RGB9_E5:
        _format = GL_RGB;
        _type = GL_UNSIGNED_INT_5_9_9_9_REV;
        break;
    case GL_DEPTH_COMPONENT32F:
        _format = GL_DEPTH_COMPONENT;
        _type = GL_FLOAT;
        break;
    case GL_COMPRESSED_RED_RGTC1:
        _format = GL_RED;
        _type = GL_ZERO;
        break;
    case GL_COMPRESSED_RG_RGTC2:
        _format = GL_RG;
        _type = GL_ZERO;
        break;
    default:
        _format = GL_ZERO;
        _type = GL_ZERO;
        break;
    }

    if (format)
        *format = _format;
    if (type)
        *type = _type;
}

inline static GLuint texture_get_working_internal_format(GLuint internal_format) {
    switch (internal_format) {
    case GL_ALPHA8:
        return GL_R8;
    case GL_LUMINANCE8:
        return GL_R8;
    case GL_LUMINANCE8_ALPHA8:
        return GL_RG8;
    default:
        return internal_format;
    }
}

static int32_t texture_load_pixels(GLenum target, GLenum internal_format,
    int32_t width, int32_t height, int32_t level, const void* data) {
    glPixelStoreiDLL(GL_UNPACK_ALIGNMENT, 1);
    switch (internal_format) {
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
    case GL_COMPRESSED_RED_RGTC1_EXT:
    case GL_COMPRESSED_RED_GREEN_RGTC2_EXT: {
        int32_t size = texture_get_size(internal_format, width, height);
        glCompressedTexImage2D(target, level, internal_format, width, height, 0, size, data);
    } break;
    default: {
        GLenum format;
        GLenum type;
        texture_get_format_type_by_internal_format(internal_format, &format, &type);
        glTexImage2DDLL(target, level, internal_format, width, height, 0, format, type, data);
    } break;
    }
    return -(gl_state_get_error() != GL_ZERO);
}

static texture* texture_load_tex(texture_id id, GLenum target,
    GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, const void** data_ptr, bool use_high_anisotropy) {
    texture* tex = texture_alloc(id);
    if (tex->ref_count > 1)
        return tex;

    glGenTextures(1, &tex->glid);
    texture_bind(target, tex->glid);
    texture_set_params(target, max_mipmap_level, use_high_anisotropy);

    GLint swizzle[4];
    switch (internal_format) {
    case GL_ALPHA8:
        swizzle[0] = GL_ZERO;
        swizzle[1] = GL_ZERO;
        swizzle[2] = GL_ZERO;
        swizzle[3] = GL_RED;
        glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        break;
    case GL_COMPRESSED_RED_RGTC1_EXT:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_RED;
        swizzle[2] = GL_RED;
        swizzle[3] = GL_ONE;
        glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        break;
    case GL_COMPRESSED_RED_GREEN_RGTC2_EXT:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_GREEN;
        swizzle[2] = GL_ZERO;
        swizzle[3] = GL_RED;
        glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        break;
    case GL_LUMINANCE8:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_RED;
        swizzle[2] = GL_RED;
        swizzle[3] = GL_ONE;
        glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        break;
    case GL_LUMINANCE8_ALPHA8:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_RED;
        swizzle[2] = GL_RED;
        swizzle[3] = GL_GREEN;
        glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        break;
    case GL_INTENSITY8:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_RED;
        swizzle[2] = GL_RED;
        swizzle[3] = GL_RED;
        glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        break;
    }

    GLuint working_internal_format = texture_get_working_internal_format(internal_format);

    int32_t size = 0;
    if (target == GL_TEXTURE_CUBE_MAP)
        for (int32_t i = 0; i < 6; i++)
            for (int32_t j = 0; j <= max_mipmap_level; j++) {
                int32_t mip_width = max_def(width >> j, 1);
                int32_t mip_height = max_def(height >> j, 1);
                const void* data;
                if (data_ptr)
                    data = data_ptr[i * (max_mipmap_level + 1) + j];
                else
                    data = 0;

                static const GLenum target_cube_map_array[] = {
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                };

                if (texture_load_pixels(target_cube_map_array[i],
                    working_internal_format, mip_width, mip_height, j, data) < 0)
                    goto fail;
                size += texture_get_size(internal_format, mip_width, mip_height);
            }
    else
        for (int32_t i = 0; i <= max_mipmap_level; i++) {
            int32_t mip_width = max_def(width >> i, 1);
            int32_t mip_height = max_def(height >> i, 1);
            const void* data;
            if (data_ptr)
                data = data_ptr[i];
            else
                data = 0;

            if (texture_load_pixels(target,
                working_internal_format, mip_width, mip_height, i, data) < 0)
                goto fail;
            size += texture_get_size(internal_format, mip_width, mip_height);
        }

    texture_bind(target, 0);

    tex->target = target;
    tex->width = (int16_t)width;
    tex->height = (int16_t)height;
    tex->size_texmem = size;
    tex->internal_format = internal_format;
    tex->max_mipmap_level = max_mipmap_level;

    switch (internal_format) {
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
    case GL_COMPRESSED_RED_RGTC1_EXT:
    case GL_COMPRESSED_RED_GREEN_RGTC2_EXT:
        enum_or(tex->flags, TEXTURE_BLOCK_COMPRESSION);
        break;
    }

    texture_manager_work_ptr->texmem_now_size += tex->size_texmem;
    texture_manager_work_ptr->texmem_peak_size = max_def(
        texture_manager_work_ptr->texmem_peak_size,
        texture_manager_work_ptr->texmem_now_size);

    texture_manager_work_ptr->texmem_now_size_by_type[id.id >> 4] += tex->size_texmem;
    texture_manager_work_ptr->texmem_peak_size_by_type[id.id >> 4] = max_def(
        texture_manager_work_ptr->texmem_peak_size_by_type[id.id >> 4],
        texture_manager_work_ptr->texmem_now_size_by_type[id.id >> 4]);
    return tex;

fail:
    texture_bind(target, 0);
    texture_release(tex);
    return 0;
}

static void texture_set_params(GLenum target, int32_t max_mipmap_level, bool use_high_anisotropy) {
    glTexParameteriDLL(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteriDLL(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteriDLL(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    static const vec4 border_color = 0.0f;
    glTexParameterfvDLL(target, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);
    glTexParameteriDLL(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteriDLL(target, GL_TEXTURE_MIN_FILTER,
        max_mipmap_level > 0 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteriDLL(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteriDLL(target, GL_TEXTURE_MAX_LEVEL, max_mipmap_level);

    float_t max_anisotropy;
    if (use_high_anisotropy)
        max_anisotropy = 16.0f;
    else
        max_anisotropy = 1.0f;
    glTexParameterfDLL(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
}
