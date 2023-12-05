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

static void obj_vertex_add_bone_weight(vec4& bone_weight, vec4i16& bone_index, int16_t index, float_t weight);
static void obj_vertex_validate_bone_data(vec4& bone_weight, vec4i16& bone_index);
static uint32_t obj_vertex_format_get_vertex_size(obj_vertex_format format);

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

bool obj_mesh_vertex_buffer::load(obj_mesh* mesh) {
    if (!mesh->num_vertex)
        return false;

    uint32_t size_vertex = obj_vertex_format_get_vertex_size(mesh->vertex_format);
    mesh->size_vertex = size_vertex;
    void* vertex = force_malloc((size_t)size_vertex * mesh->num_vertex);
    obj_mesh_vertex_buffer::fill_data(vertex, mesh);

    int32_t(FASTCALL * obj_mesh_vertex_buffer__load_data)(obj_mesh_vertex_buffer * mesh_vb,
        uint32_t data_size, const void* data, uint32_t count)
        = (int32_t(FASTCALL*)(obj_mesh_vertex_buffer * mesh_vb,
            uint32_t data_size, const void* data, uint32_t count))0x0000000140461650;

    bool ret = obj_mesh_vertex_buffer__load_data(this,
        (size_t)size_vertex * mesh->num_vertex, vertex, mesh->attrib.m.double_buffer ? 2 : 1) >= 0;
    free_def(vertex);
    return ret;
}

void* obj_mesh_vertex_buffer::fill_data(void* data, obj_mesh* mesh) {
    obj_vertex_format vertex_format = mesh->vertex_format;
    obj_mesh_vertex_array vtx = mesh->vertex_array;
    uint32_t size_vertex = mesh->size_vertex;
    uint32_t num_vertex = mesh->num_vertex;
    size_t d = (size_t)data;
    if (vertex_format & OBJ_VERTEX_POSITION) {
        vec3* position = vtx.position;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec3*)d = *position++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 12;
    }

    if (vertex_format & OBJ_VERTEX_NORMAL) {
        vec3* normal = vtx.normal;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec3*)d = *normal++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 12;
    }
    
    if (vertex_format & OBJ_VERTEX_TANGENT) {
        vec4* tangent = vtx.tangent;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec4*)d = *tangent++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 16;
    }

    if (vertex_format & OBJ_VERTEX_BINORMAL) {
        vec3* binormal = vtx.binormal;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec3*)d = *binormal++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 12;
    }

    if (vertex_format & OBJ_VERTEX_TEXCOORD0) {
        vec2* texcoord0 = vtx.texcoord0;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec2*)d = *texcoord0++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 8;
    }
    
    if (vertex_format & OBJ_VERTEX_TEXCOORD1) {
        vec2* texcoord1 = vtx.texcoord1;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec2*)d = *texcoord1++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 8;
    }
    
    if (vertex_format & OBJ_VERTEX_TEXCOORD2) {
        vec2* texcoord2 = vtx.texcoord2;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec2*)d = *texcoord2++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 8;
    }
    
    if (vertex_format & OBJ_VERTEX_TEXCOORD3) {
        vec2* texcoord3 = vtx.texcoord3;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec2*)d = *texcoord3++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 8;
    }
    
    if (vertex_format & OBJ_VERTEX_COLOR0) {
        vec4* color0 = vtx.color0;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec4*)d = *color0++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 16;
    }
    
    if (vertex_format & OBJ_VERTEX_COLOR1) {
        vec4* color1 = vtx.color1;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec4*)d = *color1++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 16;
    }
    
    if ((vertex_format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA) {
        vec4* bone_weight = vtx.bone_weight;
        vec4* bone_index = vtx.bone_index;
        for (uint32_t i = num_vertex; i; i--) {
            vec4 _bone_weight = *bone_weight++;

            int32_t bone_index_x = (int32_t)bone_index->x;
            int32_t bone_index_y = (int32_t)bone_index->y;
            int32_t bone_index_z = (int32_t)bone_index->z;
            int32_t bone_index_w = (int32_t)bone_index->w;
            bone_index++;
            
            vec4i16 _bone_index;
            _bone_index.x = (int16_t)(bone_index_x >= 0 ? bone_index_x / 3 : -1);
            _bone_index.y = (int16_t)(bone_index_y >= 0 ? bone_index_y / 3 : -1);
            _bone_index.z = (int16_t)(bone_index_z >= 0 ? bone_index_z / 3 : -1);
            _bone_index.w = (int16_t)(bone_index_w >= 0 ? bone_index_w / 3 : -1);
            obj_vertex_validate_bone_data(_bone_weight, _bone_index);

            *(vec4*)d = _bone_weight;
            *(vec4i16*)(d + 16) = _bone_index;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 24;
    }

    if (vertex_format & OBJ_VERTEX_UNKNOWN) {
        vec4* unknown = vtx.unknown;
        for (uint32_t i = num_vertex; i; i--) {
            *(vec4*)d = *unknown++;
            d += size_vertex;
        }
        d -= (size_t)size_vertex * num_vertex;
        d += 16;
    }

    return (void*)d;
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
            mat4_transpose(&matrix_buffer[i], &matrix_buffer[i]);
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
            mat4_transpose(&matrix_buffer[i], &matrix_buffer[i]);
        }
}

static void obj_vertex_add_bone_weight(vec4& bone_weight, vec4i16& bone_index, int16_t index, float_t weight) {
    if (bone_index.x < 0) {
        bone_index.x = index;
        bone_weight.x = weight;
    }
    else if (bone_index.y < 0) {
        bone_index.y = index;
        bone_weight.y = weight;
    }
    else if (bone_index.z < 0) {
        bone_index.z = index;
        bone_weight.z = weight;
    }
    else if (bone_index.w < 0) {
        bone_index.w = index;
        bone_weight.w = weight;
    }
}

static void obj_vertex_validate_bone_data(vec4& bone_weight, vec4i16& bone_index) {
    vec4 _bone_weight = { 0.0f, 0.0f, 0.0f, 0.0f };
    vec4i16 _bone_index = { -1, -1, -1, -1 };

    if (bone_index.x >= 0 && bone_weight.x > 0.0f)
        obj_vertex_add_bone_weight(_bone_weight, _bone_index, bone_index.x, bone_weight.x);

    if (bone_index.y >= 0 && bone_weight.y > 0.0f)
        obj_vertex_add_bone_weight(_bone_weight, _bone_index, bone_index.y, bone_weight.y);

    if (bone_index.z >= 0 && bone_weight.z > 0.0f)
        obj_vertex_add_bone_weight(_bone_weight, _bone_index, bone_index.z, bone_weight.z);

    if (bone_index.w >= 0 && bone_weight.w > 0.0f)
        obj_vertex_add_bone_weight(_bone_weight, _bone_index, bone_index.w, bone_weight.w);

    float_t sum = _bone_weight.x + _bone_weight.y + _bone_weight.z + _bone_weight.w;
    if (sum > 0.0f && fabsf(sum - 1.0f) > 0.000001f)
        _bone_weight *= 1.0f / sum;

    bone_weight = _bone_weight;
    bone_index = _bone_index;
}

inline static uint32_t obj_vertex_format_get_vertex_size(obj_vertex_format format) {
    uint32_t size = 0;
    if (format & OBJ_VERTEX_POSITION)
        size += 12;
    if (format & OBJ_VERTEX_NORMAL)
        size += 12;
    if (format & OBJ_VERTEX_TANGENT)
        size += 16;
    if (format & OBJ_VERTEX_BINORMAL)
        size += 12;
    if (format & OBJ_VERTEX_TEXCOORD0)
        size += 8;
    if (format & OBJ_VERTEX_TEXCOORD1)
        size += 8;
    if (format & OBJ_VERTEX_TEXCOORD2)
        size += 8;
    if (format & OBJ_VERTEX_TEXCOORD3)
        size += 8;
    if (format & OBJ_VERTEX_COLOR0)
        size += 16;
    if (format & OBJ_VERTEX_COLOR1)
        size += 16;
    if ((format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA)
        size += 24;
    if (format & OBJ_VERTEX_UNKNOWN)
        size += 16;
    return size;
}
