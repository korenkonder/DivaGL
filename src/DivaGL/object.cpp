/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "object.hpp"
#include "mdl/disp_manager.hpp"
#include "gl_state.hpp"
#include "wrap.hpp"
#include <Helpers.h>

struct BufObjMgr {
    int32_t vb_peak_size;
    int32_t vb_all_size;
    int32_t ib_peak_size;
    int32_t ib_all_size;
};

uint32_t(FASTCALL* object_database_get_object_info)(const char* name)
    = (uint32_t(FASTCALL*)(const char* name))0x0000000140459F80;
obj* (FASTCALL* object_info_get_object)(object_info obj_info)
    = (obj * (FASTCALL*)(object_info obj_info))0x000000014045A140;
obj_mesh_index_buffer* (FASTCALL* object_info_get_mesh_index_buffer)(object_info obj_info, int32_t a2)
    = (obj_mesh_index_buffer * (FASTCALL*)(object_info obj_info, int32_t a2))0x000000014045A250;
obj_skin* (FASTCALL* object_database_get_object_skin)(object_info obj_info)
    = (obj_skin * (FASTCALL*)(object_info obj_info))0x000000014045A3E0;
obj_mesh_vertex_buffer* (FASTCALL* object_info_get_mesh_vertex_buffer)(object_info obj_info, int32_t a2)
    = (obj_mesh_vertex_buffer * (FASTCALL*)(object_info obj_info, int32_t a2))0x000000014045A480;
int32_t(FASTCALL* object_database_get_set_obj_id)(int32_t set_index, int32_t obj_index)
    = (int32_t(FASTCALL*)(int32_t set_index, int32_t obj_index))0x000000014045A750;
GLuint(FASTCALL* object_database_get_set_texture)(int32_t set, int32_t tex_id)
    = (GLuint(FASTCALL*)(int32_t set, int32_t tex_id))0x000000014045A8F0;
prj::vector<GLuint>* (FASTCALL* object_database_get_set_gentex)(int32_t set)
    = (prj::vector<GLuint> * (FASTCALL*)(int32_t set))0x000000014045A9E0;
int32_t(FASTCALL* object_database_get_set_id)(int32_t set_index)
    = (int32_t(FASTCALL*)(int32_t set_index))0x000000014045AA10;

static BufObjMgr& bufobj_mgr = *(BufObjMgr*)0x00000001411A34D0;

static GLuint create_index_buffer(size_t size, const void* data);
static GLuint create_vertex_buffer(size_t size, const void* data, bool dynamic = false);
static void free_index_buffer(GLuint buffer);
static void free_vertex_buffer(GLuint buffer);

static void obj_vertex_add_bone_weight(vec4& bone_weight, vec4i16& bone_index, int16_t index, float_t weight);
static void obj_vertex_validate_bone_data(vec4& bone_weight, vec4i16& bone_index);
static uint32_t obj_vertex_format_get_vertex_size(obj_vertex_format format);

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

bool obj_mesh_index_buffer::load(obj_mesh* mesh) {
    size_t num_index = 0;
    for (uint32_t i = 0; i < mesh->num_submesh; i++)
        if (mesh->submesh_array[i].index_format == OBJ_INDEX_U16)
            num_index += mesh->submesh_array[i].num_index;

    if (!num_index) {
        buffer = 0;
        return true;
    }

    uint16_t* indices = force_malloc<uint16_t>(num_index);
    obj_mesh_index_buffer::fill_data(indices, mesh);

    bool ret = load_data(num_index * sizeof(uint16_t), indices);
    free_def(indices);
    return ret;
}

bool obj_mesh_index_buffer::load_data(size_t size, const void* data) {
    if (!size)
        return false;

    buffer = create_index_buffer(size, data);
    return true;
}

void obj_mesh_index_buffer::unload() {
    free_index_buffer(buffer);
    buffer = 0;
}

void* obj_mesh_index_buffer::fill_data(void* data, obj_mesh* mesh) {
    uint16_t* indices = (uint16_t*)data;
    for (uint32_t i = 0; i < mesh->num_submesh; i++) {
        obj_sub_mesh& sub_mesh = mesh->submesh_array[i];
        if (sub_mesh.index_format == OBJ_INDEX_U16) {
            memmove(indices, sub_mesh.index_array, sizeof(uint16_t) * sub_mesh.num_index);
            indices += sub_mesh.num_index;
        }
    }

    indices = (uint16_t*)data;
    for (uint32_t i = 0, offset = 0; i < mesh->num_submesh; i++) {
        obj_sub_mesh& sub_mesh = mesh->submesh_array[i];

        sub_mesh.first_index = 0;
        sub_mesh.last_index = 0;
        sub_mesh.index_offset = 0;
        if (sub_mesh.index_format != OBJ_INDEX_U16)
            continue;

        uint16_t first_index = 0xFFFF;
        uint16_t last_index = 0;
        for (uint32_t j = sub_mesh.num_index; j; j--) {
            uint16_t index = *indices++;
            if (index == 0xFFFF)
                continue;

            if (first_index > index)
                first_index = index;
            if (last_index < index)
                last_index = index;
        }

        sub_mesh.first_index = first_index;
        sub_mesh.last_index = last_index;
        sub_mesh.index_offset = (int32_t)(offset * sizeof(uint16_t));
        offset += sub_mesh.num_index;
    }
    return (void*)indices;
}

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
        gl_state_bind_element_array_buffer(buffers[0]);
        glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
        gl_state_bind_element_array_buffer(buffer);
        return size;
    }
    return 0;
}

bool obj_mesh_vertex_buffer::load(obj_mesh* mesh, bool dynamic) {
    if (!mesh->num_vertex)
        return false;

    uint32_t size_vertex = obj_vertex_format_get_vertex_size(mesh->vertex_format);
    mesh->size_vertex = size_vertex;

    void* vertex = force_malloc((size_t)size_vertex * mesh->num_vertex);
    obj_mesh_vertex_buffer::fill_data(vertex, mesh);

    bool ret = load_data((size_t)size_vertex * mesh->num_vertex,
        vertex, mesh->attrib.m.double_buffer ? 2 : 1, dynamic);
    free_def(vertex);
    return ret;
}

bool obj_mesh_vertex_buffer::load_data(size_t size, const void* data, int32_t count, bool dynamic) {
    if (!size || count > 3)
        return false;

    this->count = count;

    for (int32_t i = 0; i < count; i++) {
        buffers[i] = create_vertex_buffer(size, data, dynamic);
        if (!buffers[i]) {
            unload();
            return false;
        }
    }
    return true;
}

void obj_mesh_vertex_buffer::unload() {
    for (int32_t i = 0; i < count; i++) {
        free_vertex_buffer(buffers[i]);
        buffers[i] = 0;
    }

    count = 0;
    buffers[0] = 0;
    index = 0;
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

HOOK(bool, FASTCALL, obj_mesh_index_buffer__load, 0x0000000140458040, obj_mesh_index_buffer* objib, obj_mesh* mesh) {
    return objib->load(mesh);
}

HOOK(bool, FASTCALL, obj_mesh_vertex_buffer__load, 0x000000140458A70, obj_mesh_vertex_buffer* objvb, obj_mesh* mesh) {
    return objvb->load(mesh);
}

HOOK(bool, FASTCALL, obj_mesh_vertex_buffer__load_dynamic, 0x0000000140458280, obj_mesh_vertex_buffer* objvb, obj_mesh* mesh) {
    return objvb->load(mesh, true);
}

HOOK(void, FASTCALL, obj_mesh_vertex_buffer__unload, 0x0000000140461870, obj_mesh_vertex_buffer* objvb) {
    objvb->unload();
}

HOOK(void, FASTCALL, obj_mesh_index_buffer__unload, 0x0000000140461900, obj_mesh_index_buffer* objib) {
    objib->unload();
}

void object_patch() {
    WRITE_CALL(0x0000000140458860, 0x000000140458A70);
    WRITE_MEMORY(0x0000000140459BB5, uint8_t,
        0x48, 0x8D, 0x0C, 0x30, 0x0F, 0x1F, 0x00);

    INSTALL_HOOK(obj_mesh_index_buffer__load);
    INSTALL_HOOK(obj_mesh_vertex_buffer__load);
    INSTALL_HOOK(obj_mesh_vertex_buffer__load_dynamic);
    INSTALL_HOOK(obj_mesh_vertex_buffer__unload);
    INSTALL_HOOK(obj_mesh_index_buffer__unload);
}

static GLuint create_index_buffer(size_t size, const void* data) {
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    gl_state_bind_element_array_buffer(buffer);
    if (GL_VERSION_4_4)
        glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)size, data, 0);
    else
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
    gl_state_bind_element_array_buffer(0);

    if (glGetErrorDLL()) {
        glDeleteBuffers(1, &buffer);
        return 0;
    }

    bufobj_mgr.ib_all_size += (int32_t)size;
    bufobj_mgr.ib_peak_size = max_def(bufobj_mgr.ib_peak_size, bufobj_mgr.ib_all_size);
    return buffer;
}

static GLuint create_vertex_buffer(size_t size, const void* data, bool dynamic) {
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    gl_state_bind_array_buffer(buffer);
    if (GL_VERSION_4_4)
        glBufferStorage(GL_ARRAY_BUFFER, (GLsizeiptr)size, data,
            dynamic ? GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT : 0);
    else
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data,
            dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    gl_state_bind_array_buffer(0);

    if (glGetErrorDLL()) {
        glDeleteBuffers(1, &buffer);
        return 0;
    }

    bufobj_mgr.vb_all_size += (int32_t)size;
    bufobj_mgr.vb_peak_size = max_def(bufobj_mgr.vb_peak_size, bufobj_mgr.vb_all_size);
    return buffer;
}

static void free_index_buffer(GLuint buffer) {
    if (!buffer)
        return;

    disp_manager->check_index_buffer(buffer);

    GLint size;
    gl_state_bind_element_array_buffer(buffer);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    gl_state_bind_element_array_buffer(0);
    bufobj_mgr.ib_all_size -= size;

    glDeleteBuffers(1, &buffer);
    glGetErrorDLL();
}

static void free_vertex_buffer(GLuint buffer) {
    if (!buffer)
        return;

    disp_manager->check_vertex_buffer(buffer);

    GLint size = 0;
    gl_state_bind_array_buffer(buffer);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    gl_state_bind_array_buffer(0);
    bufobj_mgr.vb_all_size -= size;

    glDeleteBuffers(1, &buffer);
    glGetErrorDLL();
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
