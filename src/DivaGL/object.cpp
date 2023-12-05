/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "object.hpp"
#include "gl_state.hpp"
#include "wrap.hpp"
#include <Helpers.h>

uint32_t(FASTCALL* obj_database_get_object_info)(const char* name)
    = (uint32_t(FASTCALL*)(const char* name))0x0000000140459F80;
obj* (FASTCALL* object_info_get_object)(object_info obj_info)
    = (obj * (FASTCALL*)(object_info obj_info))0x000000014045A140;
obj_mesh_index_buffer* (FASTCALL* object_info_get_mesh_index_buffer)(object_info obj_info, int32_t a2)
    = (obj_mesh_index_buffer * (FASTCALL*)(object_info obj_info, int32_t a2))0x000000014045A250;
obj_skin* (FASTCALL* obj_database_get_object_skin)(object_info obj_info)
    = (obj_skin * (FASTCALL*)(object_info obj_info))0x000000014045A3E0;
obj_mesh_vertex_buffer* (FASTCALL* object_info_get_mesh_vertex_buffer)(object_info obj_info, int32_t a2)
    = (obj_mesh_vertex_buffer * (FASTCALL*)(object_info obj_info, int32_t a2))0x000000014045A480;
GLuint(FASTCALL* obj_database_get_obj_set_texture)(int32_t set, int32_t tex_id)
    = (GLuint(FASTCALL*)(int32_t set, int32_t tex_id))0x000000014045A8F0;
prj::vector<GLuint>* (FASTCALL* obj_database_get_obj_set_textures)(int32_t set)
    = (prj::vector<GLuint> * (FASTCALL*)(int32_t set))0x000000014045A9E0;

void obj_mesh_vertex_buffer::cycle_index() {
    if (++index >= count)
        index = 0;
}

GLuint obj_mesh_vertex_buffer::get_buffer() {
    if (index < count)
        return buffers[index];
    return 0;
}

GLsizeiptr obj_mesh_vertex_buffer::get_size() {
    if (buffers[0]) {
        GLint buffer;
        GLint size;
        glGetIntegervDLL(GL_ARRAY_BUFFER_BINDING, &buffer);
        glBindBuffer(target, buffers[0]);
        glGetBufferParameterivARB(target, GL_BUFFER_SIZE, &size);
        glBindBuffer(target, buffer);
        return size;
    }
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

void obj_skin_set_matrix_buffer(obj_skin* s, mat4* matrices,
    mat4* ex_data_matrices, mat4* matrix_buffer, const mat4* mat, const mat4* global_mat) {
    if (!s->num_bone)
        return;

    if (mat)
        for (uint32_t i = 0; i < s->num_bone; i++) {
            mat4 temp;
            int32_t bone_id = s->bone_id_array[i];
            if (bone_id & 0x8000)
                mat4_mul(&ex_data_matrices[bone_id & 0x7FFF], mat, &temp);
            else
                mat4_mul(&matrices[bone_id], mat, &temp);

            mat4_mul(global_mat, &temp, &temp);
            mat4_mul(&temp, &s->bone_matrix_array[i], &matrix_buffer[i]);
        }
    else
        for (uint32_t i = 0; i < s->num_bone; i++) {
            mat4 temp;
            int32_t bone_id = s->bone_id_array[i];
            if (bone_id & 0x8000)
                temp = ex_data_matrices[bone_id & 0x7FFF];
            else
                temp = matrices[bone_id];

            mat4_mul(global_mat, &temp, &temp);
            mat4_mul(&temp, &s->bone_matrix_array[i], &matrix_buffer[i]);
        }
}
