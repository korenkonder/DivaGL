/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "texture.hpp"
#include "types.hpp"

#define SHARED_OBJECT_BUFFER (1)

enum obj_index_format : uint32_t {
    OBJ_INDEX_U8  = 0x00,
    OBJ_INDEX_U16 = 0x01,
    OBJ_INDEX_U32 = 0x02,
};

enum obj_material_aniso_direction : uint32_t {
    OBJ_MATERIAL_ANISO_DIRECTION_NORMAL = 0,
    OBJ_MATERIAL_ANISO_DIRECTION_U      = 1,
    OBJ_MATERIAL_ANISO_DIRECTION_V      = 2,
    OBJ_MATERIAL_ANISO_DIRECTION_RADIAL = 3,
};

enum obj_material_blend_factor : uint32_t {
    OBJ_MATERIAL_BLEND_ZERO              = 0,
    OBJ_MATERIAL_BLEND_ONE               = 1,
    OBJ_MATERIAL_BLEND_SRC_COLOR         = 2,
    OBJ_MATERIAL_BLEND_INVERSE_SRC_COLOR = 3,
    OBJ_MATERIAL_BLEND_SRC_ALPHA         = 4,
    OBJ_MATERIAL_BLEND_INVERSE_SRC_ALPHA = 5,
    OBJ_MATERIAL_BLEND_DST_ALPHA         = 6,
    OBJ_MATERIAL_BLEND_INVERSE_DST_ALPHA = 7,
    OBJ_MATERIAL_BLEND_DST_COLOR         = 8,
    OBJ_MATERIAL_BLEND_INVERSE_DST_COLOR = 9,
    OBJ_MATERIAL_BLEND_ALPHA_SATURATE = 10,
};

enum obj_material_bump_map_type : uint32_t {
    OBJ_MATERIAL_BUMP_MAP_NONE = 0,
    OBJ_MATERIAL_BUMP_MAP_DOT  = 1,
    OBJ_MATERIAL_BUMP_MAP_ENV  = 2,
};

enum obj_material_color_source_type : uint32_t {
    OBJ_MATERIAL_COLOR_SOURCE_MATERIAL_COLOR = 0,
    OBJ_MATERIAL_COLOR_SOURCE_VERTEX_COLOR   = 1,
    OBJ_MATERIAL_COLOR_SOURCE_VERTEX_MORPH   = 2,
};

enum obj_material_shader_lighting_type : uint32_t {
    OBJ_MATERIAL_SHADER_LIGHTING_LAMBERT  = 0x00,
    OBJ_MATERIAL_SHADER_LIGHTING_CONSTANT = 0x01,
    OBJ_MATERIAL_SHADER_LIGHTING_PHONG    = 0x02,
};

enum obj_material_specular_quality : uint32_t {
    OBJ_MATERIAL_SPECULAR_QUALITY_LOW  = 0,
    OBJ_MATERIAL_SPECULAR_QUALITY_HIGH = 1,
};

enum obj_material_texture_type : uint32_t {
    OBJ_MATERIAL_TEXTURE_NONE               = 0x00,
    OBJ_MATERIAL_TEXTURE_COLOR              = 0x01,
    OBJ_MATERIAL_TEXTURE_NORMAL             = 0x02,
    OBJ_MATERIAL_TEXTURE_SPECULAR           = 0x03,
    OBJ_MATERIAL_TEXTURE_HEIGHT             = 0x04,
    OBJ_MATERIAL_TEXTURE_REFLECTION         = 0x05,
    OBJ_MATERIAL_TEXTURE_TRANSLUCENCY       = 0x06,
    OBJ_MATERIAL_TEXTURE_TRANSPARENCY       = 0x07,
    OBJ_MATERIAL_TEXTURE_ENVIRONMENT_SPHERE = 0x08,
    OBJ_MATERIAL_TEXTURE_ENVIRONMENT_CUBE   = 0x09,
};

enum obj_material_texture_coordinate_translation_type : uint32_t {
    OBJ_MATERIAL_TEXTURE_COORDINATE_TRANSLATION_NONE   = 0x00,
    OBJ_MATERIAL_TEXTURE_COORDINATE_TRANSLATION_UV     = 0x01,
    OBJ_MATERIAL_TEXTURE_COORDINATE_TRANSLATION_SPHERE = 0x02,
    OBJ_MATERIAL_TEXTURE_COORDINATE_TRANSLATION_CUBE   = 0x03,
};

enum obj_material_vertex_translation_type : uint32_t {
    OBJ_MATERIAL_VERTEX_TRANSLATION_DEFAULT  = 0,
    OBJ_MATERIAL_VERTEX_TRANSLATION_ENVELOPE = 1,
    OBJ_MATERIAL_VERTEX_TRANSLATION_MORPHING = 2,
};

enum obj_primitive_type : uint32_t {
    OBJ_PRIMITIVE_POINTS         = 0x00,
    OBJ_PRIMITIVE_LINES          = 0x01,
    OBJ_PRIMITIVE_LINE_STRIP     = 0x02,
    OBJ_PRIMITIVE_LINE_LOOP      = 0x03,
    OBJ_PRIMITIVE_TRIANGLES      = 0x04,
    OBJ_PRIMITIVE_TRIANGLE_STRIP = 0x05,
    OBJ_PRIMITIVE_TRIANGLE_FAN   = 0x06,
    OBJ_PRIMITIVE_QUADS          = 0x07,
    OBJ_PRIMITIVE_QUAD_STRIP     = 0x08,
    OBJ_PRIMITIVE_POLYGON        = 0x09,
};

enum obj_vertex_format : uint32_t {
    OBJ_VERTEX_NONE        = 0x0000,
    OBJ_VERTEX_POSITION    = 0x0001,
    OBJ_VERTEX_NORMAL      = 0x0002,
    OBJ_VERTEX_TANGENT     = 0x0004,
    OBJ_VERTEX_BINORMAL    = 0x0008,
    OBJ_VERTEX_TEXCOORD0   = 0x0010,
    OBJ_VERTEX_TEXCOORD1   = 0x0020,
    OBJ_VERTEX_TEXCOORD2   = 0x0040,
    OBJ_VERTEX_TEXCOORD3   = 0x0080,
    OBJ_VERTEX_COLOR0      = 0x0100,
    OBJ_VERTEX_COLOR1      = 0x0200,
    OBJ_VERTEX_BONE_WEIGHT = 0x0400,
    OBJ_VERTEX_BONE_INDEX  = 0x0800,
    OBJ_VERTEX_BONE_DATA   = 0x0C00,
    OBJ_VERTEX_UNKNOWN     = 0x1000,
};

struct obj_axis_aligned_bounding_box {
    vec3 center;
    vec3 size;
};

struct obj_bounding_box {
    vec3 center;
    vec3 size;
};

struct obj_bounding_sphere {
    vec3 center;
    float_t radius;
};

struct obj_shader_compo_member {
    uint32_t color : 1;
    uint32_t color_a : 1;
    uint32_t color_l1 : 1;
    uint32_t color_l1_a : 1;
    uint32_t color_l2 : 1;
    uint32_t color_l2_a : 1;
    uint32_t transparency : 1;
    uint32_t specular : 1;
    uint32_t normal_01 : 1;
    uint32_t normal_02 : 1;
    uint32_t envmap : 1;
    uint32_t color_l3 : 1;
    uint32_t color_l3_a : 1;
    uint32_t translucency : 1;
    uint32_t flag_14 : 1;
    uint32_t override_ibl : 1;
    uint32_t dummy : 16;
};

union obj_shader_compo {
    obj_shader_compo_member m;
    uint32_t w;
};

struct obj_material_shader_attrib_member {
    obj_material_vertex_translation_type vtx_trans_type : 2;
    obj_material_color_source_type col_src : 2;
    uint32_t is_lgt_diffuse : 1;
    uint32_t is_lgt_specular : 1;
    uint32_t is_lgt_per_pixel : 1;
    uint32_t is_lgt_double : 1;
    obj_material_bump_map_type bump_map_type : 2;
    uint32_t fresnel_type : 4;
    uint32_t line_light : 4;
    uint32_t recieve_shadow : 1;
    uint32_t cast_shadow : 1;
    obj_material_specular_quality specular_quality : 1;
    obj_material_aniso_direction aniso_direction : 2;
    uint32_t dummy : 9;
};

union obj_material_shader_attrib {
    obj_material_shader_attrib_member m;
    uint32_t w;

    obj_material_shader_lighting_type get_lighting_type() const;
};

struct obj_texture_attrib_member {
    uint32_t repeat_u : 1;
    uint32_t repeat_v : 1;
    uint32_t mirror_u : 1;
    uint32_t mirror_v : 1;
    uint32_t ignore_alpha : 1;
    uint32_t blend : 5;
    uint32_t alpha_blend : 5;
    uint32_t border : 1;
    uint32_t clamp2edge : 1;
    uint32_t filter : 3;
    uint32_t mipmap : 2;
    uint32_t mipmap_bias : 7;
    uint32_t flag_29 : 1;
    uint32_t anisotropic_filter : 2;
};

union obj_texture_attrib {
    obj_texture_attrib_member m;
    uint32_t w;

    int32_t get_blend() const;
};

struct obj_texture_shader_attrib_member {
    obj_material_texture_type tex_type : 4;
    int32_t uv_idx : 4;
    obj_material_texture_coordinate_translation_type texcoord_trans : 3;
    uint32_t dummy : 21;
};

union obj_texture_shader_attrib {
    obj_texture_shader_attrib_member m;
    uint32_t w;
};

struct obj_material_texture_data {
    obj_texture_attrib attrib;
    uint32_t tex_index;
    obj_texture_shader_attrib shader_info;
    char ex_shader[8];
    float_t weight;
    mat4 tex_coord_mat;
    uint32_t reserved[8];
};

struct obj_material_attrib_member {
    uint32_t alpha_texture : 1;
    uint32_t alpha_material : 1;
    uint32_t punch_through : 1;
    uint32_t double_sided : 1;
    uint32_t normal_dir_light : 1;
    obj_material_blend_factor src_blend_factor : 4;
    obj_material_blend_factor dst_blend_factor : 4;
    uint32_t blend_operation : 3;
    uint32_t zbias : 4;
    uint32_t no_fog : 1;
    uint32_t translucent_priority : 6;
    uint32_t has_fog_height : 1;
    uint32_t flag_28 : 1;
    uint32_t fog_height : 1;
    uint32_t flag_30 : 1;
    uint32_t flag_31 : 1;
};

union obj_material_attrib {
    obj_material_attrib_member m;
    uint32_t w;
};

struct obj_material_color {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 emission;
    float_t shininess;
    float_t intensity;
};

struct obj_material {
    obj_shader_compo shader_compo;
    union {
        char name[8];
        int32_t index;
    } shader;
    obj_material_shader_attrib shader_info;
    obj_material_texture_data texdata[8];
    obj_material_attrib attrib;
    obj_material_color color;
    vec3 center;
    float_t radius;
    char name[64];
    float_t bump_depth;
    uint32_t reserved[15];
};

struct obj_material_data {
    int32_t num_of_textures;
    obj_material material;
};

struct obj_sub_mesh_attrib_member {
    uint32_t recieve_shadow : 1;
    uint32_t cast_shadow : 1;
    uint32_t transparent : 1;
    uint32_t cloth : 1;
    uint32_t dummy : 28;
};

union obj_sub_mesh_attrib {
    obj_sub_mesh_attrib_member m;
    uint32_t w;
};

struct obj_sub_mesh {
    uint32_t flags;
    obj_bounding_sphere bounding_sphere;
    uint32_t material_index;
    uint8_t uv_index[8];
    int32_t num_bone_index;
    uint16_t* bone_index_array;
    uint32_t bones_per_vertex;
    obj_primitive_type primitive_type;
    obj_index_format index_format;
    int32_t num_index;
    uint16_t* index_array;
    obj_sub_mesh_attrib attrib;
    uint32_t reserved[4];
    obj_axis_aligned_bounding_box* axis_aligned_bounding_box;
    uint16_t first_index;
    uint16_t last_index;
    uint32_t index_offset;
};

static_assert(sizeof(obj_sub_mesh) == 0x70, "\"obj_sub_mesh\" struct should have a size of 0x70");

struct obj_mesh_attrib_member {
    uint32_t double_buffer : 1;
    uint32_t billboard_y_axis : 1;
    uint32_t translucent_sort_by_radius : 1;
    uint32_t billboard : 1;
    uint32_t dummy : 26; // 28
    uint32_t compression : 2; // Own stuff
};

union obj_mesh_attrib {
    obj_mesh_attrib_member m;
    uint32_t w;
};

struct obj_mesh_vertex_array {
    vec3* position;
    vec3* normal;
    vec4* tangent;
    vec3* binormal;
    vec2* texcoord0;
    vec2* texcoord1;
    vec2* texcoord2;
    vec2* texcoord3;
    vec4* color0;
    vec4* color1;
    vec4* bone_weight;
    vec4* bone_index;
    vec4* unknown;
    void* field_68;
    void* field_70;
    void* field_78;
    void* field_80;
    void* field_88;
    void* field_90;
    void* field_98;
};

static_assert(sizeof(obj_mesh_vertex_array) == 0xA0, "\"obj_mesh_vertex_array\" struct should have a size of 0xA0");

struct obj_mesh {
    uint32_t flags;
    obj_bounding_sphere bounding_sphere;
    int32_t num_submesh;
    obj_sub_mesh* submesh_array;
    obj_vertex_format vertex_format;
    uint32_t size_vertex;
    int32_t num_vertex;
    union {
        obj_mesh_vertex_array vertex_array;
        void* vertex_array_ptr[20];
    };
    obj_mesh_attrib attrib;
    uint32_t vertex_flags;
    uint32_t reserved[6];
    char name[0x40];
};

static_assert(sizeof(obj_mesh) == 0x130, "\"obj_mesh\" struct should have a size of 0x130");

struct obj {
    uint32_t version;
    uint32_t flags;
    obj_bounding_sphere bounding_sphere;
    int32_t num_mesh;
    obj_mesh* mesh_array;
    int32_t num_material;
    obj_material_data* material_array;
    uint32_t reserved[10];
};

static_assert(sizeof(obj) == 0x60, "\"obj\" struct should have a size of 0x60");

struct obj_skin_ex_data {
    int32_t num_osage_name;
    int32_t num_osage_node;
    const char** field_8;
    struct obj_skin_osage_node* osage_node_array;
    const char** osage_name_array;
    struct obj_skin_block* block_array;
    int32_t num_bone_name;
    const char** bone_name_array;
    struct obj_skin_osage_sibling_info* osage_sibling_infos;
    uint32_t cloth_count;
    uint32_t reserved[7];
};

static_assert(sizeof(obj_skin_ex_data) == 0x60, "\"obj_skin_ex_data\" struct should have a size of 0x60");

struct obj_skin {
    uint32_t* bone_id_array;
    mat4* bone_matrix_array;
    const char** bone_name_array;
    obj_skin_ex_data* ex_data;
    int32_t num_bone;
    int32_t bone_parent_ids_offset;
    int32_t field_28;
    int32_t field_2C;
    int32_t field_30;
};

static_assert(sizeof(obj_skin) == 0x38, "\"obj_skin\" struct should have a size of 0x38");

struct obj_set {
    uint32_t version;
    int32_t obj_num;
    uint32_t last_obj_id;
    obj** obj_data;
    obj_skin** obj_skin_data;
    const char** obj_name_data;
    uint32_t* obj_id_data;
    uint32_t* tex_id_data;
    int32_t tex_num;
    uint32_t reserved[2];
};

static_assert(sizeof(obj_set) == 0x48, "\"obj_set\" struct should have a size of 0x48");

struct object_info {
    uint16_t id;
    uint16_t set_id;

    inline object_info() {
        this->id = (uint16_t)-1;
        this->set_id = (uint16_t)-1;
    }

    inline object_info(uint16_t id, uint16_t set_id) {
        this->id = id;
        this->set_id = set_id;
    }

    inline object_info(uint32_t data) {
        this->id = (uint16_t)data;
        this->set_id = (uint16_t)(data >> 16);
    }

    inline bool is_null() const {
        return id == (uint16_t)-1 && set_id == (uint16_t)-1;
    }

    inline bool not_null() const {
        return id != (uint16_t)-1 || set_id != (uint16_t)-1;
    }
};

inline bool operator >(const object_info& left, const object_info& right) {
    return *(uint32_t*)&left > *(uint32_t*)&right;
}

inline bool operator <(const object_info& left, const object_info& right) {
    return *(uint32_t*)&left < *(uint32_t*)&right;
}

inline bool operator >=(const object_info& left, const object_info& right) {
    return *(uint32_t*)&left >= *(uint32_t*)&right;
}

inline bool operator <=(const object_info& left, const object_info& right) {
    return *(uint32_t*)&left <= *(uint32_t*)&right;
}

inline bool operator ==(const object_info& left, const object_info& right) {
    return *(uint32_t*)&left == *(uint32_t*)&right;
}

inline bool operator !=(const object_info& left, const object_info& right) {
    return *(uint32_t*)&left != *(uint32_t*)&right;
}

struct obj_mesh_index_buffer {
    GLuint buffer;

    obj_mesh_index_buffer();

    bool load(obj_mesh* mesh);
    bool load_data(size_t size, const void* data);
    void unload();

    static void* fill_data(void* data, obj_mesh* mesh);
};

#if !SHARED_OBJECT_BUFFER
static_assert(sizeof(obj_mesh_index_buffer) == 0x04, "\"obj_mesh_index_buffer\" struct should have a size of 0x04");
#endif

struct obj_mesh_vertex_buffer {
    int32_t count;
    GLuint buffers[3];
    int32_t index;
#if SHARED_OBJECT_BUFFER
    GLsizeiptr size;
    size_t offset;
#else
    GLenum target;
#endif

    obj_mesh_vertex_buffer();

    void cycle_index();
    GLuint get_buffer();
    size_t get_offset();
    GLsizeiptr get_size();
    bool load(obj_mesh* mesh, bool dynamic = false);
    bool load_data(size_t size, const void* data, int32_t count, bool dynamic);
    void unload();

    static void* fill_data(void* data, obj_mesh* mesh);
};

#if SHARED_OBJECT_BUFFER
struct obj_mesh_vertex_buffer_aft {
    int32_t count;
    GLuint buffers[3];
    int32_t index;
    GLenum target;

    void cycle_index();
    GLuint get_buffer();
    GLsizeiptr get_size();
};
#endif

#if SHARED_OBJECT_BUFFER
static_assert(sizeof(obj_mesh_vertex_buffer_aft) == 0x18, "\"obj_mesh_vertex_buffer_aft\" struct should have a size of 0x18");
#else
static_assert(sizeof(obj_mesh_vertex_buffer) == 0x18, "\"obj_mesh_vertex_buffer\" struct should have a size of 0x18");
#endif

struct obj_index_buffer {
    int32_t mesh_num;
    obj_mesh_index_buffer* mesh_data;
#if SHARED_OBJECT_BUFFER
    GLuint buffer;
#endif

    obj_index_buffer();

    bool load(obj* obj);
    void unload();
};

#if !SHARED_OBJECT_BUFFER
static_assert(sizeof(obj_index_buffer) == 0x10, "\"obj_index_buffer\" struct should have a size of 0x10");
#endif

struct obj_vertex_buffer {
    int32_t mesh_num;
    obj_mesh_vertex_buffer* mesh_data;
#if SHARED_OBJECT_BUFFER
    GLuint buffers[3];
#endif

    obj_vertex_buffer();

    bool load(obj* obj);
    void unload();
};

#if !SHARED_OBJECT_BUFFER
static_assert(sizeof(obj_vertex_buffer) == 0x10, "\"obj_vertex_buffer\" struct should have a size of 0x10");
#endif

extern void (FASTCALL* objset_info_storage_unload_set)(int32_t set);
extern obj_mesh* (FASTCALL* object_info_get_mesh_by_index)(object_info object, int32_t mesh_index);
extern int32_t(FASTCALL* objset_info_storage_get_obj_mesh_index)(object_info object, const char* mesh_name);
extern const char* (FASTCALL* object_database_get_object_name)(object_info obj_info);
extern uint32_t(FASTCALL* object_database_get_object_info)(const char* name);
extern obj* (FASTCALL* objset_info_storage_get_object)(object_info obj_info);
extern obj_mesh_index_buffer* (FASTCALL* objset_info_storage_get_obj_mesh_index_buffer)(object_info obj_info, int32_t a2);
extern obj_skin* (FASTCALL* objset_info_storage_get_obj_skin)(object_info obj_info);
extern obj_mesh_vertex_buffer* (FASTCALL* objset_info_storage_get_obj_mesh_vertex_buffer)(object_info obj_info, int32_t a2);
extern int32_t(FASTCALL* objset_info_storage_get_set_obj_id)(int32_t set_index, int32_t obj_index);
extern GLuint(FASTCALL* objset_info_storage_get_set_texture)(int32_t set, int32_t tex_id);
extern prj::vector<GLuint>* (FASTCALL* objset_info_storage_get_set_gentex)(int32_t set);
extern int32_t(FASTCALL* object_database_get_set_id)(int32_t set_index);
extern int32_t(FASTCALL* object_database_get_object_set_id)(const char* name);
extern int32_t(FASTCALL* objset_info_storage_get_set_tex_num)(int32_t set);
extern texture** (FASTCALL* objset_info_storage_get_set_textures)(int32_t set);
extern int32_t(FASTCALL* objset_info_storage_load_set)(int32_t set);
extern bool (FASTCALL* objset_info_storage_load_obj_set_check_not_read)(int32_t set);

extern int32_t obj_material_texture_type_get_texcoord_index(
    obj_material_texture_type type, int32_t index);
extern int32_t obj_material_texture_type_get_texture_index(
    obj_material_texture_type type, int32_t base_index);

extern void obj_skin_set_matrix_buffer(obj_skin* s, mat4* matrices,
    mat4* ex_data_matrices, mat4* matrix_buffer, const mat4* mat, const mat4* global_mat);

extern void object_patch();
