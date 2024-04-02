/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "texture.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/txp.hpp"
#include "gl_state.hpp"
#include "types.hpp"
#include <Helpers.h>

texture* (FASTCALL* texture_init)(texture_id id)
    = (texture * (FASTCALL*)(texture_id id))0x000000014069A390;
void(FASTCALL* texture_free)(texture* tex)
    = (void(FASTCALL*)(texture * tex))0x000000014069DA70;
int32_t(FASTCALL* texture_info_get_id)(const char* name)
    = (int32_t(FASTCALL*)(const char* name))0x000000014069CBD0;
texture* (FASTCALL* texture_handler_get_texture)(uint32_t id)
    = (texture * (FASTCALL*)(uint32_t id))0x000000014069CD70;

static void texture_bind(GLenum target, GLuint texture);
static void texture_get_format_type_by_internal_format(GLenum internal_format, GLenum* format, GLenum* type);
static uint32_t texture_get_height_mip_level(texture* tex, int32_t mip_level);
static GLuint texture_get_working_internal_format(GLuint internal_format);
static int32_t texture_get_size(GLenum internal_format, int32_t width, int32_t height);
static int32_t texture_get_size_mip_level(texture* tex, int32_t mip_level);
static uint32_t texture_get_width_mip_level(texture* tex, int32_t mip_level);
static int32_t texture_load(GLenum target, GLenum internal_format,
    int32_t width, int32_t height, int32_t level, void* data);
static texture* texture_load_tex(texture_id id, GLenum target,
    GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, void** data_ptr, bool use_high_anisotropy);
static GLenum texture_txp_get_gl_internal_format(txp* t);

uint32_t texture::get_height_align_mip_level(uint8_t mip_level) {
    if (flags & TEXTURE_BLOCK_COMPRESSION)
        return max_def((uint32_t)height >> mip_level, 4u);
    else
        return max_def((uint32_t)height >> mip_level, 1u);
}

uint32_t texture::get_width_align_mip_level(uint8_t mip_level) {
    if (flags & TEXTURE_BLOCK_COMPRESSION)
        return max_def((uint32_t)width >> mip_level, 4u);
    else
        return max_def((uint32_t)width >> mip_level, 1u);
}

void texture_apply_color_tone(texture* chg_tex, texture* org_tex, color_tone* col_tone) {
    for (int32_t i = 0; i <= org_tex->max_mipmap_level; i++) {
        int32_t size = texture_get_size_mip_level(org_tex, i);
        void* data = prj::HeapCMallocAllocate(prj::HeapCMallocTemp, size, "imgf_color_tone_cpu()");
        if (!data)
            break;

        int32_t width_align = org_tex->get_width_align_mip_level(i);
        int32_t height_align = org_tex->get_height_align_mip_level(i);
        if (org_tex->flags & TEXTURE_BLOCK_COMPRESSION) {
            texture_bind(org_tex->target, org_tex->tex);
            glGetCompressedTexImage(org_tex->target, i, data);
            if (org_tex->internal_format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
                dxt1_image_apply_color_tone(width_align, height_align, size, (dxt1_block*)data, col_tone);
            else
                dxt5_image_apply_color_tone(width_align, height_align, size, (dxt5_block*)data, col_tone);

            texture_bind(chg_tex->target, chg_tex->tex);
            uint32_t width = texture_get_width_mip_level(org_tex, i);
            uint32_t height = texture_get_width_mip_level(org_tex, i);
            glCompressedTexSubImage2D(chg_tex->target, i, 0, 0, width, height,
                chg_tex->internal_format, size, data);
        }
        else if (org_tex->internal_format == GL_RGB5) {
            texture_bind(org_tex->target, org_tex->tex);
            glGetTexImageDLL(org_tex->target, i, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
            rgb565_image_apply_color_tone(width_align, height_align, size, (rgb565*)data, col_tone);

            texture_bind(chg_tex->target, chg_tex->tex);
            uint32_t width = texture_get_width_mip_level(org_tex, i);
            uint32_t height = texture_get_width_mip_level(org_tex, i);
            glTexSubImage2DDLL(chg_tex->target, i, 0, 0, width, height,
                GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        }
        gl_state_get_error();
        prj::HeapCMallocFree(prj::HeapCMallocTemp, data);
    }
    texture_bind(org_tex->target, 0);
}

texture* texture_copy(texture_id id, texture* org_tex) {
    if (org_tex->target != GL_TEXTURE_2D)
        return 0;

    texture_bind(org_tex->target, org_tex->tex);
    std::vector<void*> vec_data;
    for (int32_t i = 0; i <= org_tex->max_mipmap_level; i++) {
        void* data = force_malloc(texture_get_size_mip_level(org_tex, i));
        if (!data)
            break;

        if (org_tex->flags & TEXTURE_BLOCK_COMPRESSION) {
            glGetCompressedTexImage(org_tex->target, i, data);
        }
        else {
            GLenum format;
            GLenum type;
            texture_get_format_type_by_internal_format(org_tex->internal_format, &format, &type);
            glGetTexImageDLL(org_tex->target, i, format, type, data);
        }
        gl_state_get_error();
        vec_data.push_back(data);
    }
    texture_bind(org_tex->target, 0);

    texture* tex = texture_load_tex_2d(id, org_tex->internal_format,
        org_tex->width, org_tex->height, org_tex->max_mipmap_level, vec_data.data(), true);

    for (void*& i : vec_data)
        free_def(i);

    return tex;
}

texture* texture_load_tex_2d(texture_id id, GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, void** data_ptr, bool use_high_anisotropy) {
    return texture_load_tex(id, GL_TEXTURE_2D, internal_format,
        width, height, max_mipmap_level, data_ptr, use_high_anisotropy);
}

texture* texture_load_tex_cube_map(texture_id id, GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, void** data_ptr) {
    return texture_load_tex(id, GL_TEXTURE_CUBE_MAP, internal_format,
        width, height, max_mipmap_level, data_ptr, false);
}

texture* texture_txp_load(txp* t, texture_id id) {
    if (!t || !t->mipmaps.size())
        return 0;

    int32_t count = t->array_size * t->mipmaps_count;
    void** data_ptr = force_malloc<void*>(count);
    for (uint32_t i = 0, k = 0; i < t->array_size; i++)
        for (uint32_t j = 0; j < t->mipmaps_count; j++, k++)
            data_ptr[k] = t->mipmaps[k].data.data();

    GLenum internal_format = texture_txp_get_gl_internal_format(t);
    int32_t width = t->mipmaps[0].width;
    int32_t height = t->mipmaps[0].height;
    int32_t max_mipmap_level = t->mipmaps_count - 1;

    texture* tex;
    if (t->has_cube_map)
        tex = texture_load_tex_cube_map(id, internal_format, width, height, max_mipmap_level, data_ptr);
    else
        tex = texture_load_tex_2d(id, internal_format, width, height, max_mipmap_level, data_ptr, true);
    free_def(data_ptr);
    return tex;
}

void texture_txp_store(texture* tex, txp* t) {
    if (tex->target != GL_TEXTURE_2D && tex->target != GL_TEXTURE_CUBE_MAP)
        return;

    int32_t max_mipmap_level = tex->max_mipmap_level;

    t->has_cube_map = tex->target == GL_TEXTURE_CUBE_MAP;
    t->array_size = t->has_cube_map ? 6 : 1;
    t->mipmaps_count = max_mipmap_level + 1;
    t->mipmaps.resize((size_t)t->mipmaps_count * t->array_size);

    txp_format format = TXP_RGBA8;
    switch (tex->internal_format) {
    case GL_ALPHA8:
        format = TXP_A8;
        break;
    case GL_RGB8:
        format = TXP_RGB8;
        break;
    case GL_RGBA8:
        format = TXP_RGBA8;
        break;
    case GL_RGB5:
        format = TXP_RGB5;
        break;
    case GL_RGB5_A1:
        format = TXP_RGB5A1;
        break;
    case GL_RGBA4:
        format = TXP_RGBA4;
        break;
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        format = TXP_BC1;
        break;
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        format = TXP_BC1a;
        break;
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        format = TXP_BC2;
        break;
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        format = TXP_BC3;
        break;
    case GL_COMPRESSED_RED_RGTC1_EXT:
        format = TXP_BC4;
        break;
    case GL_COMPRESSED_RED_GREEN_RGTC2_EXT:
        format = TXP_BC5;
        break;
    case GL_LUMINANCE8:
        format = TXP_L8;
        break;
    case GL_LUMINANCE8_ALPHA8:
        format = TXP_L8A8;
        break;
    }

    auto get_tex_mipmap_data = [&](txp_mipmap& mipmap, GLenum target, int32_t mip_level) {
        mipmap.width = texture_get_width_mip_level(tex, mip_level);
        mipmap.height = texture_get_height_mip_level(tex, mip_level);
        mipmap.format = format;
        mipmap.size = texture_get_size_mip_level(tex, mip_level);
        mipmap.data.resize(mipmap.size);

        if (tex->flags & TEXTURE_BLOCK_COMPRESSION)
            glGetCompressedTexImage(target, mip_level, mipmap.data.data());
        else {
            GLenum format;
            GLenum type;
            texture_get_format_type_by_internal_format(tex->internal_format, &format, &type);
            glGetTexImageDLL(target, mip_level, format, type, mipmap.data.data());
        }
        gl_state_get_error();
    };

    texture_bind(tex->target, tex->tex);
    int32_t size = 0;
    if (t->has_cube_map)
        for (int32_t i = 0; i < 6; i++)
            for (int32_t j = 0; j <= max_mipmap_level; j++) {
                static const GLenum target_cube_map_array[] = {
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                };

                get_tex_mipmap_data(t->mipmaps[i * ((int64_t)max_mipmap_level + 1) + j],
                    target_cube_map_array[i], j);
            }
    else
        for (int32_t i = 0; i <= max_mipmap_level; i++)
            get_tex_mipmap_data(t->mipmaps[i], GL_TEXTURE_2D, i);
    texture_bind(tex->target, 0);
}

void texture_array_free(texture** arr) {
    if (!arr)
        return;

    for (texture** i = arr; *i; i++)
        texture_free(*i);
    free_def(arr);
}

void texture_set_params(GLenum target, int32_t max_mipmap_level, bool use_high_anisotropy) {
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

void texture_params_get(GLuint tex_0, texture_param* tex_0_param,
    GLuint tex_1, texture_param* tex_1_param, GLuint tex_2, texture_param* tex_2_param) {
    gl_state_disable_depth_test();
    if (tex_0_param) {
        tex_0_param->width = 0;
        tex_0_param->height = 0;
        if (tex_0) {
            gl_state_bind_texture_2d(tex_0);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_0_param->width);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_0_param->height);
        }
    }

    if (tex_1_param) {
        tex_1_param->width = 0;
        tex_1_param->height = 0;
        if (tex_1) {
            gl_state_bind_texture_2d(tex_1);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_1_param->width);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_1_param->height);
        }
    }

    if (tex_2_param) {
        tex_2_param->width = 0;
        tex_2_param->height = 0;
        if (tex_2) {
            gl_state_bind_texture_2d(tex_2);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_2_param->width);
            glGetTexLevelParameterivDLL(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_2_param->height);
        }
    }
    gl_state_bind_texture_2d(0);

    if (tex_0_param)
        glViewportDLL(0, 0, tex_0_param->width, tex_0_param->height);
}

void texture_params_restore(texture_param* tex_0_param,
    texture_param* tex_1_param, texture_param* tex_2_param) {
    for (int32_t i = 0; i < 4; i++)
        gl_state_active_bind_texture_2d(i, 0);
}

bool texture_txp_set_load(txp_set* t, texture*** texs, uint32_t* ids) {
    if (!t || !texs || !ids)
        return false;

    size_t count = t->textures.size();
    *texs = force_malloc<texture*>(count + 1);
    texture** tex = *texs;
    for (size_t i = 0; i < count; i++)
        tex[i] = texture_txp_load(&t->textures[i], texture_id(0, ids[i]));
    tex[count] = 0;
    return true;
}

bool texture_txp_set_load(txp_set* t, texture*** texs, texture_id* ids) {
    if (!t || !texs || !ids)
        return false;

    size_t count = t->textures.size();
    *texs = force_malloc<texture*>(count + 1);
    texture** tex = *texs;
    for (size_t i = 0; i < count; i++)
        tex[i] = texture_txp_load(&t->textures[i], ids[i]);
    tex[count] = 0;
    return true;
}

HOOK(void, FASTCALL, texture_apply_color_tone, 0x00000001403B5DF0,
    texture* chg_tex, texture* org_tex, color_tone* col_tone) {
    texture_apply_color_tone(chg_tex, org_tex, col_tone);
}

HOOK(texture*, FASTCALL, texture_load_tex_cube_map, 0x000000014069B860,
    texture_id id, GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, void** data_ptr) {
    return texture_load_tex(id, GL_TEXTURE_CUBE_MAP, internal_format,
        width, height, max_mipmap_level, data_ptr, false);
}

HOOK(texture*, FASTCALL, texture_load_tex_2d, 0x000000014069B8E0, 
    texture_id id, GLenum internal_format, int32_t width, int32_t height,
    int32_t max_mipmap_level, void** data_ptr, bool use_high_anisotropy) {
    return texture_load_tex(id, GL_TEXTURE_2D, internal_format,
        width, height, max_mipmap_level, data_ptr, use_high_anisotropy);
}

void texture_patch() {
    INSTALL_HOOK(texture_apply_color_tone);
    INSTALL_HOOK(texture_load_tex_cube_map);
    INSTALL_HOOK(texture_load_tex_2d);
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
        _format = GL_ALPHA;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_LUMINANCE8:
        _format = GL_LUMINANCE;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_LUMINANCE8_ALPHA8:
        _format = GL_LUMINANCE_ALPHA;
        _type = GL_UNSIGNED_BYTE;
        break;
    case GL_INTENSITY8:
        _format = GL_INTENSITY;
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

inline static uint32_t texture_get_height_mip_level(texture* tex, int32_t mip_level) {
    return max_def((uint32_t)tex->height >> mip_level, 1u);
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

static int32_t texture_get_size(GLenum internal_format, int32_t width, int32_t height) {
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

inline static int32_t texture_get_size_mip_level(texture* tex, int32_t mip_level) {
    return texture_get_size(tex->internal_format,
        texture_get_width_mip_level(tex, mip_level),
        texture_get_height_mip_level(tex, mip_level));
}

inline static uint32_t texture_get_width_mip_level(texture* tex, int32_t mip_level) {
    return max_def((uint32_t)tex->width >> mip_level, 1u);
}

static int32_t texture_load(GLenum target, GLenum internal_format,
    int32_t width, int32_t height, int32_t level, void* data) {
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
    int32_t max_mipmap_level, void** data_ptr, bool use_high_anisotropy) {
    texture* tex = texture_init(id);
    if (tex->init_count > 1)
        return tex;

    glGenTextures(1, &tex->tex);
    texture_bind(target, tex->tex);
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
    }

    GLuint working_internal_format = texture_get_working_internal_format(internal_format);

    int32_t size = 0;
    if (target == GL_TEXTURE_CUBE_MAP)
        for (int32_t i = 0; i < 6; i++)
            for (int32_t j = 0; j <= max_mipmap_level; j++) {
                int32_t mip_width = max_def(width >> j, 1);
                int32_t mip_height = max_def(height >> j, 1);
                void* data;
                if (data_ptr)
                    data = data_ptr[i * (max_mipmap_level + 1) + j];
                else
                    data = 0;

                static const GLenum target_cube_map_array[] = {
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                };

                if (texture_load(target_cube_map_array[i],
                    working_internal_format, mip_width, mip_height, j, data) < 0)
                    goto fail;
                size += texture_get_size(internal_format, mip_width, mip_height);
            }
    else
        for (int32_t i = 0; i <= max_mipmap_level; i++) {
            int32_t mip_width = max_def(width >> i, 1);
            int32_t mip_height = max_def(height >> i, 1);
            void* data;
            if (data_ptr)
                data = data_ptr[i];
            else
                data = 0;

            if (texture_load(target,
                working_internal_format, mip_width, mip_height, i, data) < 0)
                goto fail;
            size += texture_get_size(internal_format, mip_width, mip_height);
        }

    texture_bind(target, 0);

    tex->target = target;
    tex->width = (int16_t)width;
    tex->height = (int16_t)height;
    tex->size = size;
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
    return tex;

fail:
    texture_bind(target, 0);
    texture_free(tex);
    return 0;
}

static GLenum texture_txp_get_gl_internal_format(txp* t) {
    if (!t || !t->mipmaps.size())
        return GL_ZERO;

    switch (t->mipmaps[0].format) {
    case TXP_A8:
        return GL_ALPHA8;
    case TXP_RGB8:
        return GL_RGB8;
    case TXP_RGBA8:
        return GL_RGBA8;
    case TXP_RGB5:
        return GL_RGB5;
    case TXP_RGB5A1:
        return GL_RGB5_A1;
    case TXP_RGBA4:
        return GL_RGBA4;
    case TXP_BC1:
        return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
    case TXP_BC1a:
        return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    case TXP_BC2:
        return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    case TXP_BC3:
        return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    case TXP_BC4:
        return GL_COMPRESSED_RED_RGTC1_EXT;
    case TXP_BC5:
        return GL_COMPRESSED_RED_GREEN_RGTC2_EXT;
    case TXP_L8:
        return GL_LUMINANCE8;
    case TXP_L8A8:
        return GL_LUMINANCE8_ALPHA8;
    default:
        return GL_ZERO;
    }
}
