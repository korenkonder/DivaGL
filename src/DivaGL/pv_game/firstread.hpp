/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/vec.hpp"
#include "../../AFTModsShared/object.hpp"
#include "../../AFTModsShared/stage.hpp"
#include "../../AFTModsShared/types.hpp"

struct firstread_str_array_string {
    int32_t id;
    const char* str;
};

struct firstread_str_array_lang {
    int32_t lang_id;
    uint32_t num_string;
    const firstread_str_array_string* string_array;
};

struct firstread_str_array {
    uint32_t signature;
    uint32_t num_lang;
    const firstread_str_array_lang* lang_array;

    static const firstread_str_array* read(const void* data, size_t size);
};

struct firstread_auth_3d_object {
    const char* name;
    const char* uid_name;
};

struct firstread_auth_3d_object_list {
    const char* name;
    bool remove;
};

struct firstread_auth_3d {
    int32_t uid;
    uint32_t num_object;
    uint32_t num_object_list;
    const firstread_auth_3d_object* object_array;
    const firstread_auth_3d_object_list* object_list_array;
};

struct firstread_auth_3d_array {
    uint32_t signature;
    uint32_t num_auth_3d;
    const firstread_auth_3d* auth_3d_array;

    static const firstread_auth_3d_array* read(const void* data, size_t size);
};

struct firstread_obj_sub_mesh_split {
    obj_bounding_sphere bounding_sphere[2];
    int32_t material_index[2];
    obj_index_format index_format;
    int32_t num_index[2];
    const void* index_array[2];
    obj_sub_mesh_attrib attrib[2];
};

struct firstread_obj_sub_mesh {
    obj_bounding_sphere bounding_sphere;
    obj_index_format index_format;
    int32_t num_index;
    const void* index_array;
    obj_sub_mesh_attrib attrib;
    firstread_obj_sub_mesh_split* split;
};

struct firstread_obj_mesh {
    firstread_obj_sub_mesh* submesh_array;
    int32_t num_submesh;
    obj_vertex_format vertex_format;
    uint32_t size_vertex;
    int32_t num_vertex;
    union {
        obj_mesh_vertex_array vertex_array;
        void* vertex_array_ptr[20];
    };
    obj_mesh_attrib attrib;
    bool remove;
    uint8_t index;
    uint8_t pad[2];
    char name[0x40];
};

struct firstread_obj {
    int32_t id;
    uint16_t num_mesh;
    uint16_t num_material;
    firstread_obj_mesh* mesh_array;
    const obj_material_data* material_array;
};

struct firstread_objset {
    uint32_t signature;
    int32_t obj_num;
    const firstread_obj* obj_data;

    static const firstread_objset* read(
        prj::shared_ptr<prj::stack_allocator>& alloc, const void* data, size_t size);
};

struct firstread_objset_data {
    const void* data;
    size_t size;
};

struct firstread_objset_array {
    uint32_t signature;
    uint32_t num_objset;
    const int32_t* objset_id_array;
    const firstread_objset_data* objset_array;

    static const firstread_objset_array* read(const void* data, size_t size);
};

struct firstread_stage_data {
    const char* name;
    uint32_t pv_id;
    uint8_t reflect_type;
    uint8_t reflect_full;
    const stage_data_reflect* reflect;
};

struct firstread_stage_data_array {
    uint32_t signature;
    uint32_t num_stage_data;
    const firstread_stage_data* stage_data_array;

    static const firstread_stage_data_array* read(const void* data, size_t size);
};

struct firstread {
    uint32_t signature;
    uint32_t pad[0x03];
    const firstread_str_array* str_array;
    size_t str_array_size;
    const firstread_auth_3d_array* auth_3d_array;
    size_t auth_3d_array_size;
    const firstread_objset_array* objset_array;
    size_t objset_array_size;
    const firstread_stage_data_array* stage_data_array;
    size_t stage_data_array_size;

    static const firstread* read(const void* data, size_t size);
};
