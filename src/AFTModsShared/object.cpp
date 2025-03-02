/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "object.hpp"
#include "../KKdLib/hash.hpp"
#include "../AFTModsShared/file_handler.hpp"
#include <Helpers.h>

void (FASTCALL* objset_info_storage_unload_set)(int32_t set)
    = (void (FASTCALL*)(int32_t set))0x00000001404599B0;
obj_mesh* (FASTCALL* objset_info_storage_get_obj_mesh_by_index)(uint32_t obj_info, int32_t mesh_index)
    = (obj_mesh * (FASTCALL*)(uint32_t obj_info, int32_t mesh_index))0x0000000140459D40;
int32_t(FASTCALL* objset_info_storage_get_obj_mesh_index)(uint32_t obj_info, const char* mesh_name)
    = (int32_t(FASTCALL*)(uint32_t obj_info, const char* mesh_name))0x0000000140459DE0;
const char* (FASTCALL* object_database_get_obj_name)(uint32_t obj_info)
    = (const char* (FASTCALL*)(uint32_t obj_info))0x0000000140459F20;
uint32_t(FASTCALL* object_database_get_object_info)(const char* name)
    = (uint32_t(FASTCALL*)(const char* name))0x0000000140459F80;
obj* (FASTCALL* objset_info_storage_get_obj)(uint32_t obj_info)
    = (obj * (FASTCALL*)(uint32_t obj_info))0x000000014045A140;
obj_skin* (FASTCALL* objset_info_storage_get_obj_skin)(uint32_t obj_info)
    = (obj_skin * (FASTCALL*)(uint32_t obj_info))0x000000014045A3E0;
int32_t(FASTCALL* objset_info_storage_get_set_obj_id)(int32_t set_index, int32_t obj_index)
    = (int32_t(FASTCALL*)(int32_t set_index, int32_t obj_index))0x000000014045A750;
int32_t(FASTCALL* object_database_get_set_id)(int32_t set_index)
    = (int32_t(FASTCALL*)(int32_t set_index))0x000000014045AA10;
int32_t(FASTCALL* object_database_get_object_set_id)(const char* name)
    = (int32_t(FASTCALL*)(const char* name))0x000000014045AA60;
const char* (FASTCALL* objset_info_storage_get_set_name)(int32_t set_id)
    = (const char* (FASTCALL*)(int32_t set_id))0x000000014045AC80;
int32_t(FASTCALL* objset_info_storage_get_set_tex_num)(int32_t set)
    = (int32_t(FASTCALL*)(int32_t set))0x000000014045ADE0;
texture** (FASTCALL* objset_info_storage_get_set_textures)(int32_t set)
    = (texture * *(FASTCALL*)(int32_t set))0x000000014045AE20;
int32_t(FASTCALL* objset_info_storage_load_set)(int32_t set)
    = (int32_t(FASTCALL*)(int32_t set))0x000000014045C6A0;
bool (FASTCALL* objset_info_storage_load_obj_set_check_not_read)(int32_t set)
    = (bool(FASTCALL*)(int32_t set))0x000000014045DA60;

obj_material_shader_lighting_type obj_material_shader_attrib::get_lighting_type() const {
    if (!m.is_lgt_diffuse && !m.is_lgt_specular)
        return OBJ_MATERIAL_SHADER_LIGHTING_CONSTANT;
    else if (!m.is_lgt_specular)
        return OBJ_MATERIAL_SHADER_LIGHTING_LAMBERT;
    else
        return OBJ_MATERIAL_SHADER_LIGHTING_PHONG;
}

int32_t obj_texture_attrib::get_blend() const {
    switch (m.blend) {
    case 4:
        return 2;
    case 6:
        return 1;
    case 16:
        return 3;
    default:
        return 0;
    }
}

void obj_mesh_vertex_buffer::cycle_index() {
    if (++index >= count)
        index = 0;
}

AFTGLuint obj_mesh_vertex_buffer::get_buffer() {
    if (index < count)
        return buffers[index];
    return 0;
}

inline int32_t obj_material_texture_type_get_texcoord_index(
    obj_material_texture_type type, int32_t index) {
    switch (type) {
    case OBJ_MATERIAL_TEXTURE_COLOR:
        if (index < 2)
            return index;
    case OBJ_MATERIAL_TEXTURE_NORMAL:
    case OBJ_MATERIAL_TEXTURE_SPECULAR:
        return 0;
    case OBJ_MATERIAL_TEXTURE_TRANSLUCENCY:
    case OBJ_MATERIAL_TEXTURE_TRANSPARENCY:
        return 1;
    }
    return -1;
}

inline int32_t obj_material_texture_type_get_texture_index(
    obj_material_texture_type type, int32_t base_index) {
    switch (type) {
    case OBJ_MATERIAL_TEXTURE_COLOR:
        if (base_index < 2)
            return base_index;
    case OBJ_MATERIAL_TEXTURE_NORMAL:
        return 2;
    case OBJ_MATERIAL_TEXTURE_SPECULAR:
        return 3;
    case OBJ_MATERIAL_TEXTURE_TRANSLUCENCY:
        return 1;
    case OBJ_MATERIAL_TEXTURE_TRANSPARENCY:
        return 4;
    case OBJ_MATERIAL_TEXTURE_ENVIRONMENT_SPHERE: // AFT
    case OBJ_MATERIAL_TEXTURE_ENVIRONMENT_CUBE:
        return 5;
    }
    return -1;
}

void obj_skin_set_matrix_buffer(obj_skin* s, const mat4* matrices,
    const mat4* ex_data_matrices, mat4* matrix_buffer, const mat4* mat, const mat4& global_mat) {
    if (!s->num_bone)
        return;

    uint32_t* bone_id = s->bone_id_array;
    mat4* bone_matrix = s->bone_matrix_array;
    if (mat)
        for (int32_t i = 0; i < s->num_bone; i++, bone_id++, bone_matrix++, matrix_buffer++) {
            mat4 temp;
            if (*bone_id & 0x8000)
                mat4_mul(&ex_data_matrices[*bone_id & 0x7FFF], mat, &temp);
            else
                mat4_mul(&matrices[*bone_id], mat, &temp);

            mat4_mul(&global_mat, &temp, &temp);
            mat4_mul(&temp, bone_matrix, matrix_buffer);
        }
    else
        for (int32_t i = 0; i < s->num_bone; i++, bone_id++, bone_matrix++, matrix_buffer++) {
            mat4 temp;
            if (*bone_id & 0x8000)
                temp = ex_data_matrices[*bone_id & 0x7FFF];
            else
                temp = matrices[*bone_id];

            mat4_mul(&global_mat, &temp, &temp);
            mat4_mul(&temp, bone_matrix, matrix_buffer);
        }
}
