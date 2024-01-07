/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"
#include "../gl_array_buffer.hpp"
#include "../gl_element_array_buffer.hpp"
#include "../color.hpp"
#include "../object.hpp"
#include "../shadow.hpp"

#define TEXTURE_PATTERN_COUNT 24
#define TEXTURE_TRANSFORM_COUNT 24

namespace mdl {
    enum EtcObjType {
        ETC_OBJ_TEAPOT = 0,
        ETC_OBJ_GRID,
        ETC_OBJ_CUBE,
        ETC_OBJ_SPHERE,
        ETC_OBJ_PLANE,
        ETC_OBJ_CONE,
        ETC_OBJ_LINE,
        ETC_OBJ_CROSS,
        ETC_OBJ_MAX,
    };

    enum ObjKind {
        OBJ_KIND_NORMAL = 0,
        OBJ_KIND_ETC,
        OBJ_KIND_USER,
        OBJ_KIND_TRANSLUCENT,
        OBJ_KIND_MAX,
    };

    enum ObjFlags : uint32_t {
        OBJ_SHADOW                = 0x00000001,
        OBJ_2                     = 0x00000002,
        OBJ_4                     = 0x00000004,
        OBJ_8                     = 0x00000008,
        OBJ_10                    = 0x00000010,
        OBJ_20                    = 0x00000020,
        OBJ_40                    = 0x00000040,
        OBJ_SHADOW_OBJECT         = 0x00000080,
        OBJ_CHARA_REFLECT         = 0x00000100,
        OBJ_REFLECT               = 0x00000200,
        OBJ_REFRACT               = 0x00000400,
        OBJ_800                   = 0x00000800,
        OBJ_TRANSLUCENT_NO_SHADOW = 0x00001000,
        OBJ_SSS                   = 0x00002000,
        OBJ_4000                  = 0x00004000,
        OBJ_8000                  = 0x00008000,
        OBJ_ALPHA_ORDER_1         = 0x00010000,
        OBJ_ALPHA_ORDER_2         = 0x00020000,
        OBJ_ALPHA_ORDER_3         = 0x00040000,
        OBJ_80000                 = 0x00080000,
        OBJ_100000                = 0x00100000,
        OBJ_200000                = 0x00200000,
        OBJ_400000                = 0x00400000,
        OBJ_800000                = 0x00800000,
        OBJ_USER                  = 0x01000000,
        OBJ_2000000               = 0x02000000,
        OBJ_4000000               = 0x04000000,
        OBJ_8000000               = 0x08000000,
        OBJ_10000000              = 0x10000000,
        OBJ_20000000              = 0x20000000,
        OBJ_40000000              = 0x40000000,
        OBJ_NO_TRANSLUCENCY       = 0x80000000,
    };

    enum ObjType {
        OBJ_TYPE_OPAQUE = 0,
        OBJ_TYPE_TRANSLUCENT,
        OBJ_TYPE_TRANSLUCENT_NO_SHADOW,
        OBJ_TYPE_TRANSPARENT,
        OBJ_TYPE_SHADOW_CHARA,
        OBJ_TYPE_SHADOW_STAGE,
        OBJ_TYPE_TYPE_6,
        OBJ_TYPE_TYPE_7,
        OBJ_TYPE_SHADOW_OBJECT_CHARA,
        OBJ_TYPE_SHADOW_OBJECT_STAGE,
        OBJ_TYPE_REFLECT_CHARA_OPAQUE,
        OBJ_TYPE_REFLECT_CHARA_TRANSLUCENT,
        OBJ_TYPE_REFLECT_CHARA_TRANSPARENT,
        OBJ_TYPE_REFLECT_OPAQUE,
        OBJ_TYPE_REFLECT_TRANSLUCENT,
        OBJ_TYPE_REFLECT_TRANSPARENT,
        OBJ_TYPE_REFRACT_OPAQUE,
        OBJ_TYPE_REFRACT_TRANSLUCENT,
        OBJ_TYPE_REFRACT_TRANSPARENT,
        OBJ_TYPE_SSS,
        OBJ_TYPE_OPAQUE_ALPHA_ORDER_1,
        OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_1,
        OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_1,
        OBJ_TYPE_OPAQUE_ALPHA_ORDER_2,
        OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_2,
        OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_2,
        OBJ_TYPE_OPAQUE_ALPHA_ORDER_3,
        OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_3,
        OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_3,
        OBJ_TYPE_USER,
        OBJ_TYPE_MAX,
    };
}

struct morph_struct {
    object_info object;
    float_t weight;
};

static_assert(sizeof(morph_struct) == 0x08, "\"morph_struct\" struct should have a size of 0x08");

struct texture_pattern_struct {
    uint32_t src;
    uint32_t dst;

    texture_pattern_struct();
    texture_pattern_struct(uint32_t src, uint32_t dst);
};

static_assert(sizeof(texture_pattern_struct) == 0x08, "\"texture_pattern_struct\" struct should have a size of 0x08");

struct texture_transform_struct {
    uint32_t id;
    mat4 mat;

    texture_transform_struct();
    texture_transform_struct(uint32_t id, const mat4& mat);
};

static_assert(sizeof(texture_transform_struct) == 0x44, "\"texture_pattern_struct\" struct should have a size of 0x44");

struct texture_data_struct {
    int32_t field_0;
    vec3 texture_color_coefficients;
    vec3 texture_color_offset;
    vec3 texture_specular_coefficients;
    vec3 texture_specular_offset;
};

static_assert(sizeof(texture_data_struct) == 0x34, "\"texture_pattern_struct\" struct should have a size of 0x34");

namespace mdl {
    struct ObjSubMeshArgs {
        const obj_sub_mesh* sub_mesh;
        const obj_mesh* mesh;
        const obj_material_data* material;
        const prj::vector<GLuint>* textures;
        int32_t mat_count;
        const mat4* mats;
        GLuint vertex_buffer;
        size_t vertex_buffer_offset;
        GLuint index_buffer;
        bool set_blend_color;
        bool chara_color;
        vec4 blend_color;
        bool self_shadow;
        shadow_type_enum shadow;
        GLuint morph_vertex_buffer;
        size_t morph_vertex_buffer_offset;
        float_t morph_weight;
        int32_t texture_pattern_count;
        texture_pattern_struct texture_pattern_array[TEXTURE_PATTERN_COUNT];
        vec4 texture_color_coefficients;
        vec4 texture_color_offset;
        vec4 texture_specular_coefficients;
        vec4 texture_specular_offset;
        int32_t texture_transform_count;
        texture_transform_struct texture_transform_array[TEXTURE_TRANSFORM_COUNT];
        int32_t field_7C4;
        int32_t field_7C8;
        int32_t instances_count;
        const mat4* instances_mat;
        void(FASTCALL* func)(const ObjSubMeshArgs*);
        const ObjSubMeshArgs* func_data;

        ObjSubMeshArgs();
    };

    struct EtcObjTeapot {
        float_t size;

        EtcObjTeapot();
    };

    struct EtcObjGrid {
        int32_t w;
        int32_t h;
        int32_t ws;
        int32_t hs;

        EtcObjGrid();
    };

    struct EtcObjCube {
        vec3 size;
        bool wire;

        EtcObjCube();
    };

    struct EtcObjSphere {
        float_t radius;
        int32_t slices;
        int32_t stacks;
        bool wire;

        EtcObjSphere();
    };

    struct EtcObjPlane {
        int32_t w;
        int32_t h;

        EtcObjPlane();
    };

    struct EtcObjCone {
        float_t base;
        float_t height;
        int32_t slices;
        int32_t stacks;
        bool wire;

        EtcObjCone();
    };

    struct EtcObjLine {
        vec3 pos[2];

        EtcObjLine();
    };

    struct EtcObjCross {
        float_t size;

        EtcObjCross();
    };

    struct EtcObj {
        union Data {
            EtcObjTeapot teapot;
            EtcObjGrid grid;
            EtcObjCube cube;
            EtcObjSphere sphere;
            EtcObjPlane plane;
            EtcObjCone cone;
            EtcObjLine line;
            EtcObjCross cross;

            Data();
        };

        EtcObjType type;
        color4u8_bgra color;
        bool fog;
        Data data;
        GLsizei count; // Added
        size_t offset; // Added

        EtcObj(EtcObjType type);
        ~EtcObj();
    };

    typedef void(*UserArgsFunc)(void* data, mat4* mat);

    struct UserArgs {
        UserArgsFunc func;
        void* data;

        UserArgs();
    };

    struct ObjTranslucentArgs {
        int32_t count;
        ObjSubMeshArgs* sub_mesh[40];

        ObjTranslucentArgs();
    };

    struct ObjData {
        union Args {
            ObjSubMeshArgs sub_mesh;
            EtcObj etc;
            UserArgs user;
            ObjTranslucentArgs translucent;

            Args();
            ~Args();
        };

        ObjKind kind;
        mat4 mat;
        float_t view_z;
        float_t radius;
        Args args;
        
        ObjData();
        ~ObjData();

        void init_etc(const mat4* mat, mdl::EtcObj* etc);
        void init_sub_mesh(const mat4* mat, float_t radius,
            const obj_sub_mesh* sub_mesh, const obj_mesh* mesh, const obj_material_data* material,
            const prj::vector<GLuint>* textures, int32_t mat_count, const mat4* mats,
            GLuint vertex_buffer, size_t vertex_buffer_offset, GLuint index_buffer,
            const vec4& blend_color, GLuint morph_vertex_buffer,
            size_t morph_vertex_buffer_offset, int32_t instances_count, mat4* instances_mat,
            void(FASTCALL* func)(const ObjSubMeshArgs*), const ObjSubMeshArgs* func_data);
        void init_translucent(const mat4* mat, ObjTranslucentArgs* translucent);
        void init_user(const mat4* mat, UserArgsFunc func, void* data);
    };

    struct CullingCheck {
        struct Info {
            int32_t objects;
            int32_t meshes;
            int32_t submesh_array;
            int pad;
        };

        Info passed;
        Info culled;
        Info passed_prev;
        Info culled_prev;
    };

    static_assert(sizeof(mdl::CullingCheck) == 0x40, "\"mdl::CullingCheck\" struct should have a size of 0x40");

    struct DispManager {
        struct vertex_array {
            GLuint vertex_buffer;
            size_t vertex_buffer_offset;
            GLuint morph_vertex_buffer;
            size_t morph_vertex_buffer_offset;
            GLuint index_buffer;
            int32_t alive_time;
            GLuint vertex_array;
            bool vertex_attrib_array[16];
            obj_vertex_format vertex_format;
            GLsizei size_vertex;
            GLuint vertex_attrib_buffer_binding[16];
            int32_t texcoord_array[2];

            void reset_vertex_attrib();
        };

        struct etc_vertex_array {
            GLArrayBuffer vertex_buffer;
            GLElementArrayBuffer index_buffer;
            int32_t alive_time;
            GLuint vertex_array;
            EtcObj::Data data;
            EtcObjType type;
            GLsizei count;
            size_t offset;
            GLsizei wire_count;
            size_t wire_offset;
            size_t max_vtx;
            size_t max_idx;
        };

        mdl::ObjFlags obj_flags;
        shadow_type_enum shadow_type;
        int32_t field_8;
        int32_t field_C;
        prj::list<mdl::ObjData*> obj[mdl::OBJ_TYPE_MAX];
        mdl::CullingCheck culling;
        int32_t put_index;
        bool show_alpha_center;
        bool show_mat_center;
        bool object_culling;
        bool object_sort;
        bool chara_color;
        int32_t buff_offset;
        int32_t buff_max;
        int32_t buff_size;
        size_t buff;
        morph_struct morph;
        int32_t texture_pattern_count;
        texture_pattern_struct texture_pattern_array[TEXTURE_PATTERN_COUNT];
        vec4 texture_color_coefficients;
        vec4 texture_color_offset;
        vec4 texture_specular_coefficients;
        vec4 texture_specular_offset;
        float_t wet_param;
        int32_t texture_transform_count;
        texture_transform_struct texture_transform_array[TEXTURE_TRANSFORM_COUNT];
        bool(FASTCALL* culling_func)(obj_bounding_sphere*);

        void add_vertex_array(ObjSubMeshArgs* args);
        void add_vertex_array(EtcObj* etc, mat4& mat);
        void* alloc_data(int32_t size);
        ObjData* alloc_obj_data(ObjKind kind);
        mat4* alloc_mat4_array(int32_t count);
        void buffer_reset();
        void calc_obj_radius(mat4* view, mdl::ObjType type);
        void check_index_buffer(GLuint buffer);
        void check_vertex_arrays();
        void check_vertex_buffer(GLuint buffer);
        void draw(mdl::ObjType type, int32_t depth_mask = 0, bool a4 = true);
        void draw_translucent(mdl::ObjType type, int32_t alpha);
        /*void draw_show_vector(mdl::ObjType type, int32_t show_vector);*/
        void entry_list(ObjType type, ObjData* data);
        bool entry_obj(::obj* object, obj_mesh_vertex_buffer* obj_vertex_buf,
            obj_mesh_index_buffer* obj_index_buf, const mat4* mat,
            prj::vector<GLuint>* textures, vec4* blend_color, mat4* bone_mat, ::obj* object_morph,
            obj_mesh_vertex_buffer* obj_morph_vertex_buf, int32_t instances_count, mat4* instances_mat,
            void(*func)(const mdl::ObjSubMeshArgs*), const ObjSubMeshArgs* func_data,
            bool enable_bone_mat, bool local = false);
        void entry_obj_by_obj(const mat4* mat,
            ::obj* obj, prj::vector<GLuint>* textures, obj_mesh_vertex_buffer* obj_vert_buf,
            obj_mesh_index_buffer* obj_index_buf, mat4* bone_mat, float_t alpha);
#if SHARED_OBJECT_BUFFER
        void entry_obj_by_obj(const mat4* mat,
            ::obj* obj, prj::vector<GLuint>* textures, obj_mesh_vertex_buffer_aft* obj_vert_buf,
            obj_mesh_index_buffer* obj_index_buf, mat4* bone_mat, float_t alpha);
#endif
        bool entry_obj_by_object_info(const mat4* mat, object_info obj_info, mat4* bone_mat = 0);
        bool entry_obj_by_object_info(const mat4* mat, object_info obj_info,
            vec4* blend_color, mat4* bone_mat, int32_t instances_count, mat4* instances_mat,
            void(*func)(const ObjSubMeshArgs*), const ObjSubMeshArgs* func_data, bool enable_bone_mat, bool local = false);
        bool entry_obj_by_object_info(const mat4* mat, object_info obj_info, float_t alpha, mat4* bone_mat = 0);
        bool entry_obj_by_object_info(const mat4* mat, object_info obj_info,
            float_t r, float_t g, float_t b, float_t a, mat4* bone_mat = 0, bool local = false);
        bool entry_obj_by_object_info(const mat4* mat, object_info obj_info,
            vec4* blend_color, mat4* bone_mat = 0, bool local = false);
        void entry_obj_by_object_info_object_skin(object_info obj_info,
            prj::vector<texture_pattern_struct>* texture_pattern, texture_data_struct* texture_data, float_t alpha,
            mat4* matrices, mat4* ex_data_matrices, const mat4* mat, const mat4* global_mat);
        void entry_obj_etc(const mat4* mat, EtcObj* etc, bool local = false);
        void entry_obj_user(const mat4* mat, UserArgsFunc func, void* data, ObjType type);
        GLuint get_vertex_array(const ObjSubMeshArgs* args);
        GLuint get_vertex_array(const mdl::EtcObj* etc);
        bool get_chara_color();
        void get_morph(object_info& object, float_t& weight);
        void get_obj_center(mat4& mat, const mdl::ObjSubMeshArgs* args, vec3& center);
        int32_t get_obj_count(ObjType type);
        ObjFlags get_obj_flags();
        shadow_type_enum get_shadow_type();
        void get_texture_color_coeff(vec4& value);
        void get_texture_color_offset(vec4& value);
        void get_texture_pattern(int32_t& count, texture_pattern_struct*& value);
        void get_texture_specular_coeff(vec4& value);
        void get_texture_specular_offset(vec4& value);
        void get_texture_transform(int32_t& count, texture_transform_struct*& value);
        float_t get_wet_param();
        void obj_sort(mat4* view, ObjType type, int32_t compare_func);
        void refresh();
        void set_chara_color(bool value = false);
        void set_culling_finc(bool(FASTCALL* func)(obj_bounding_sphere*) = 0);
        void set_obj_flags(ObjFlags flags = (ObjFlags)0);
        void set_morph(object_info object = {}, float_t weight = 0.0f);
        void set_shadow_type(shadow_type_enum type = SHADOW_CHARA);
        void set_texture_color_coefficients(vec4& value);
        void set_texture_color_offset(vec4& value);
        void set_texture_pattern(int32_t count = 0, texture_pattern_struct* value = 0);
        void set_texture_specular_coefficients(vec4& value);
        void set_texture_specular_offset(vec4& value);
        void set_texture_transform(int32_t count = 0, texture_transform_struct* value = 0);
        void set_wet_param(float_t value = 0.0f);
    };

    static_assert(sizeof(mdl::DispManager) == 0x9D0, "\"mdl::DispManager\" struct should have a size of 0x9D0");

    extern void disp_manager_patch();
}

extern mdl::DispManager* disp_manager;
