/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "object.hpp"
#include "../KKdLib/hash.hpp"
#include "../AFTModsShared/file_handler.hpp"
#include "mdl/disp_manager.hpp"
#include "pv_game/firstread.hpp"
#include "gl_state.hpp"
#include "wrap.hpp"
#include <Helpers.h>

struct BufObjMgr {
    int32_t vb_peak_size;
    int32_t vb_all_size;
    int32_t ib_peak_size;
    int32_t ib_all_size;
};

struct ObjsetInfo {
    p_file_handler obj_file_handler;
    bool obj_loaded;
    p_file_handler tex_file_handler;
    bool tex_loaded;
    prj::shared_ptr<void*> alloc_handler; // prj::stack_allocator
    obj_set* obj_set;
    prj::vector<std::pair<uint32_t, int32_t>> obj_id_data;
    bool field_50;
    int32_t field_54;
    int32_t tex_num;
    prj::vector<GLuint> gentex;
    prj::vector<std::pair<int32_t, int32_t>> tex_id_data;
    bool field_90;
    texture** textures;
    int32_t set_id;
    int32_t objvb_num;
    obj_vertex_buffer* objvb;
    int32_t objib_num;
    obj_index_buffer* objib;
    obj_vertex_buffer* field_C0;
    obj_index_buffer* field_C8;
    int32_t load_count;

    void index_buffer_free();
    bool index_buffer_load();
    void vertex_buffer_free();
    bool vertex_buffer_load();
};

static_assert(sizeof(ObjsetInfo) == 0xD8, "\"ObjsetInfo\" struct should have a size of 0xD8");

obj_mesh_index_buffer* (FASTCALL* objset_info_storage_get_obj_mesh_index_buffer)(uint32_t obj_info, int32_t a2)
    = (obj_mesh_index_buffer * (FASTCALL*)(uint32_t obj_info, int32_t a2))0x000000014045A250;
obj_mesh_vertex_buffer* (FASTCALL* objset_info_storage_get_obj_mesh_vertex_buffer)(uint32_t obj_info, int32_t a2)
    = (obj_mesh_vertex_buffer * (FASTCALL*)(uint32_t obj_info, int32_t a2))0x000000014045A480;
GLuint(FASTCALL* objset_info_storage_get_set_texture)(int32_t set, int32_t tex_id)
    = (GLuint(FASTCALL*)(int32_t set, int32_t tex_id))0x000000014045A8F0;
prj::vector<GLuint>* (FASTCALL* objset_info_storage_get_set_gentex)(int32_t set)
    = (prj::vector<GLuint> * (FASTCALL*)(int32_t set))0x000000014045A9E0;

static BufObjMgr& bufobj_mgr = *(BufObjMgr*)0x00000001411A34D0;

static int32_t load_set_id;

static GLuint create_index_buffer(size_t size, const void* data);
static GLuint create_vertex_buffer(size_t size, const void* data, bool dynamic = false);
static void free_index_buffer(GLuint buffer);
static void free_vertex_buffer(GLuint buffer);

static int32_t remove_degenerate_triangle_indices(
    uint16_t* dst_index_array, const int32_t num_index, uint16_t* src_index_array);

static void obj_vertex_add_bone_weight(vec4& bone_weight, vec4i16& bone_index, int16_t index, float_t weight);
static void obj_vertex_validate_bone_data(vec4& bone_weight, vec4i16& bone_index);
static uint32_t obj_vertex_format_get_vertex_size(obj_vertex_format format);
static uint32_t obj_vertex_format_get_vertex_size_comp1(obj_vertex_format format);
static uint32_t obj_vertex_format_get_vertex_size_comp2(obj_vertex_format format);

obj_mesh_index_buffer_divagl::obj_mesh_index_buffer_divagl() {

}

obj_mesh_index_buffer_divagl::obj_mesh_index_buffer_divagl(
    const obj_mesh_index_buffer& other) : obj_mesh_index_buffer(other) {

}

bool obj_mesh_index_buffer_divagl::load(obj_mesh* mesh) {
    size_t num_index = 0;
    for (int32_t i = 0; i < mesh->num_submesh; i++)
        if (mesh->submesh_array[i].index_format == OBJ_INDEX_U16)
            num_index += mesh->submesh_array[i].num_index;

    if (!num_index) {
        buffer = 0;
        return true;
    }

    uint16_t* indices = force_malloc<uint16_t>(num_index);
    obj_mesh_index_buffer_divagl::fill_data(indices, mesh);

    bool ret = load_data(num_index * sizeof(uint16_t), indices);
    free_def(indices);
    return ret;
}

bool obj_mesh_index_buffer_divagl::load_data(size_t size, const void* data) {
    if (!size)
        return false;

    buffer = create_index_buffer(size, data);
    return true;
}

void obj_mesh_index_buffer_divagl::unload() {
    free_index_buffer(buffer);
    buffer = 0;
}

void* obj_mesh_index_buffer_divagl::fill_data(void* data, obj_mesh* mesh) {
    uint16_t* indices = (uint16_t*)data;
    for (int32_t i = 0; i < mesh->num_submesh; i++) {
        obj_sub_mesh& sub_mesh = mesh->submesh_array[i];
        if (sub_mesh.index_format == OBJ_INDEX_U16) {
            memmove(indices, sub_mesh.index_array, sizeof(uint16_t) * sub_mesh.num_index);
            indices += sub_mesh.num_index;
        }
    }

    indices = (uint16_t*)data;
    for (int32_t i = 0, offset = 0; i < mesh->num_submesh; i++) {
        obj_sub_mesh& sub_mesh = mesh->submesh_array[i];

        sub_mesh.first_index = 0;
        sub_mesh.last_index = 0;
        sub_mesh.index_offset = 0;
        if (sub_mesh.index_format != OBJ_INDEX_U16)
            continue;

        uint16_t first_index = 0xFFFF;
        uint16_t last_index = 0;
        for (int32_t j = sub_mesh.num_index; j > 0; j--) {
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

#if SHARED_OBJECT_BUFFER
obj_mesh_vertex_buffer_divagl::obj_mesh_vertex_buffer_divagl() : size(), offset() {
#else
obj_mesh_vertex_buffer_divagl::obj_mesh_vertex_buffer_divagl() {
#endif

}

#if SHARED_OBJECT_BUFFER
obj_mesh_vertex_buffer_divagl::obj_mesh_vertex_buffer_divagl(
    const obj_mesh_vertex_buffer& other) : obj_mesh_vertex_buffer(other), size(), offset() {
#else
obj_mesh_vertex_buffer_divagl::obj_mesh_vertex_buffer_divagl(
    const obj_mesh_vertex_buffer& other) : obj_mesh_vertex_buffer(other) {
#endif

}

void obj_mesh_vertex_buffer_divagl::cycle_index() {
    if (++index >= count)
        index = 0;
}

GLuint obj_mesh_vertex_buffer_divagl::get_buffer() {
    if (index < count)
        return buffers[index];
    return 0;
}

size_t obj_mesh_vertex_buffer_divagl::get_offset() {
#if SHARED_OBJECT_BUFFER
    if (buffers[0])
        return offset;
    return 0;
#endif
}

GLsizeiptr obj_mesh_vertex_buffer_divagl::get_size() {
#if SHARED_OBJECT_BUFFER
    if (buffers[0])
        return size;
#else
    if (buffers[0]) {
        GLint buffer;
        GLint size;
        glGetIntegervDLL(GL_ARRAY_BUFFER_BINDING, &buffer);
        gl_state_bind_element_array_buffer(buffers[0]);
        glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
        gl_state_bind_element_array_buffer(buffer);
        return size;
    }
#endif
    return 0;
}

bool obj_mesh_vertex_buffer_divagl::load(obj_mesh* mesh, bool dynamic) {
    if (!mesh->num_vertex)
        return false;

    uint32_t size_vertex;
    switch (mesh->attrib.m.compression) {
    case 0:
    default:
        size_vertex = obj_vertex_format_get_vertex_size(mesh->vertex_format);
        break;
    case 1:
        size_vertex = obj_vertex_format_get_vertex_size_comp1(mesh->vertex_format);
        break;
    case 2:
        size_vertex = obj_vertex_format_get_vertex_size_comp2(mesh->vertex_format);
        break;
    }

    mesh->size_vertex = size_vertex;

    void* vertex = force_malloc((size_t)size_vertex * mesh->num_vertex);
    obj_mesh_vertex_buffer_divagl::fill_data(vertex, mesh);

    bool ret = load_data((size_t)size_vertex * mesh->num_vertex,
        vertex, mesh->attrib.m.double_buffer ? 2 : 1, dynamic);
    free_def(vertex);
    return ret;
}

bool obj_mesh_vertex_buffer_divagl::load_data(size_t size, const void* data, int32_t count, bool dynamic) {
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

void obj_mesh_vertex_buffer_divagl::unload() {
    for (int32_t i = 0; i < count; i++) {
        free_vertex_buffer(buffers[i]);
        buffers[i] = 0;
    }

    count = 0;
    buffers[0] = 0;
    index = 0;
}

void* obj_mesh_vertex_buffer_divagl::fill_data(void* data, obj_mesh* mesh) {
    obj_vertex_format vertex_format = mesh->vertex_format;
    obj_mesh_vertex_array vtx = mesh->vertex_array;
    uint32_t size_vertex = mesh->size_vertex;
    int32_t num_vertex = mesh->num_vertex;
    size_t d = (size_t)data;
    switch (mesh->attrib.m.compression) {
    case 0:
    default:
        if (vertex_format & OBJ_VERTEX_POSITION) {
            vec3* position = vtx.position;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec3*)d = *position++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 12;
        }

        if (vertex_format & OBJ_VERTEX_NORMAL) {
            vec3* normal = vtx.normal;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec3*)d = *normal++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 12;
        }

        if (vertex_format & OBJ_VERTEX_TANGENT) {
            vec4* tangent = vtx.tangent;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec4*)d = *tangent++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 16;
        }

        if (vertex_format & OBJ_VERTEX_BINORMAL) {
            vec3* binormal = vtx.binormal;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec3*)d = *binormal++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 12;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD0) {
            vec2* texcoord0 = vtx.texcoord0;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec2*)d = *texcoord0++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD1) {
            vec2* texcoord1 = vtx.texcoord1;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec2*)d = *texcoord1++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD2) {
            vec2* texcoord2 = vtx.texcoord2;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec2*)d = *texcoord2++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD3) {
            vec2* texcoord3 = vtx.texcoord3;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec2*)d = *texcoord3++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }

        if (vertex_format & OBJ_VERTEX_COLOR0) {
            vec4* color0 = vtx.color0;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec4*)d = *color0++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 16;
        }

        if (vertex_format & OBJ_VERTEX_COLOR1) {
            vec4* color1 = vtx.color1;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec4*)d = *color1++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 16;
        }

        if ((vertex_format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA) {
            vec4* bone_weight = vtx.bone_weight;
            vec4* bone_index = vtx.bone_index;
            for (int32_t i = num_vertex; i > 0; i--) {
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
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec4*)d = *unknown++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 16;
        }
        break;
    case 1:
        if (vertex_format & OBJ_VERTEX_POSITION) {
            vec3* position = vtx.position;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec3*)d = *position++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 12;
        }

        if (vertex_format & OBJ_VERTEX_NORMAL) {
            vec3* normal = vtx.normal;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec3_to_vec3i16(*normal++ * 32767.0f, *(vec3i16*)d);
                *(int16_t*)(d + 6) = 0;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }

        if (vertex_format & OBJ_VERTEX_TANGENT) {
            vec4* tangent = vtx.tangent;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec4_to_vec4i16(*tangent++ * 32767.0f, *(vec4i16*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD0) {
            vec2* texcoord0 = vtx.texcoord0;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec2_to_vec2h(*texcoord0++, *(vec2h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD1) {
            vec2* texcoord1 = vtx.texcoord1;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec2_to_vec2h(*texcoord1++, *(vec2h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD2) {
            vec2* texcoord2 = vtx.texcoord2;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec2_to_vec2h(*texcoord2++, *(vec2h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD3) {
            vec2* texcoord3 = vtx.texcoord3;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec2_to_vec2h(*texcoord3++, *(vec2h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_COLOR0) {
            vec4* color0 = vtx.color0;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec4_to_vec4h(*color0++, *(vec4h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }

        if ((vertex_format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA) {
            vec4* bone_weight = vtx.bone_weight;
            vec4* bone_index = vtx.bone_index;
            for (int32_t i = num_vertex; i > 0; i--) {
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

                vec4_to_vec4u16(_bone_weight * 65535.0f, *(vec4u16*)d);
                *(vec4i16*)(d + 16) = _bone_index;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 16;
        }
        break;
    case 2:
        if (vertex_format & OBJ_VERTEX_POSITION) {
            vec3* position = vtx.position;
            for (int32_t i = num_vertex; i > 0; i--) {
                *(vec3*)d = *position++;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 12;
        }

        if (vertex_format & OBJ_VERTEX_NORMAL) {
            vec3* normal = vtx.normal;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec3i16 normal_int;
                vec3_to_vec3i16(*normal++ * 511.0f, normal_int);
                *(uint32_t*)d = (((uint32_t)0 & 0x03) << 30)
                    | (((uint32_t)normal_int.z & 0x3FF) << 20)
                    | (((uint32_t)normal_int.y & 0x3FF) << 10)
                    | ((uint32_t)normal_int.x & 0x3FF);
                *(int16_t*)(d + 6) = 0;
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_TANGENT) {
            vec4* tangent = vtx.tangent;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec4i16 tangent_int;
                vec4_to_vec4i16(*tangent++ * 511.0f, tangent_int);
                *(uint32_t*)d = (((uint32_t)clamp_def(tangent_int.w, -1, 1) & 0x03) << 30)
                    | (((uint32_t)tangent_int.z & 0x3FF) << 20)
                    | (((uint32_t)tangent_int.y & 0x3FF) << 10)
                    | ((uint32_t)tangent_int.x & 0x3FF);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD0) {
            vec2* texcoord0 = vtx.texcoord0;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec2_to_vec2h(*texcoord0++, *(vec2h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD1) {
            vec2* texcoord1 = vtx.texcoord1;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec2_to_vec2h(*texcoord1++, *(vec2h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD2) {
            vec2* texcoord2 = vtx.texcoord2;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec2_to_vec2h(*texcoord2++, *(vec2h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_TEXCOORD3) {
            vec2* texcoord3 = vtx.texcoord3;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec2_to_vec2h(*texcoord3++, *(vec2h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 4;
        }

        if (vertex_format & OBJ_VERTEX_COLOR0) {
            vec4* color0 = vtx.color0;
            for (int32_t i = num_vertex; i > 0; i--) {
                vec4_to_vec4h(*color0++, *(vec4h*)d);
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }

        if ((vertex_format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA) {
            vec4* bone_weight = vtx.bone_weight;
            vec4* bone_index = vtx.bone_index;
            for (int32_t i = num_vertex; i > 0; i--) {
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

                vec4i16 bone_weight_int;
                vec4_to_vec4i16(_bone_weight * 1023.0f, bone_weight_int);
                *(uint32_t*)d = (((uint32_t)0 & 0x03) << 30)
                    | (((uint32_t)bone_weight_int.z & 0x3FF) << 20)
                    | (((uint32_t)bone_weight_int.y & 0x3FF) << 10)
                    | ((uint32_t)bone_weight_int.x & 0x3FF);

                vec4i bone_index;
                vec4i16_to_vec4i(_bone_index, bone_index);
                vec4i_to_vec4u8(bone_index, *(vec4u8*)(d + 4));
                d += size_vertex;
            }
            d -= (size_t)size_vertex * num_vertex;
            d += 8;
        }
        break;
    }

    return (void*)((size_t)data + (size_t)size_vertex * num_vertex);
}

/*#if SHARED_OBJECT_BUFFER
void obj_mesh_vertex_buffer_aft::cycle_index() {
    if (++index >= count)
        index = 0;
}

GLuint obj_mesh_vertex_buffer_aft::get_buffer() {
    if (index < count)
        return buffers[index];
    return 0;
}

#endif*/

#if SHARED_OBJECT_BUFFER
obj_index_buffer::obj_index_buffer() : mesh_num(), mesh_data(), buffer() {
#else
obj_index_buffer::obj_index_buffer() : mesh_num(), mesh_data() {
#endif

}

bool obj_index_buffer::load(obj* obj) {
    if (!obj)
        return false;

    mesh_num = obj->num_mesh;
    mesh_data = new obj_mesh_index_buffer_divagl[obj->num_mesh];
    if (!mesh_data)
        return false;

#if SHARED_OBJECT_BUFFER
    size_t buffer_size = 0;
    for (int32_t i = 0; i < mesh_num; i++) {
        obj_mesh& mesh = obj->mesh_array[i];
        size_t num_index = 0;
        for (int32_t j = 0; j < mesh.num_submesh; j++)
            num_index += mesh.submesh_array[j].num_index;
        buffer_size += num_index * sizeof(uint16_t);
    }

    void* index = force_malloc(buffer_size);
    if (!index) {
        buffer = 0;
        unload();
        return false;
    }

    void* data = index;
    for (int32_t i = 0; i < mesh_num; i++) {
        uint32_t offset = (uint32_t)((size_t)data - (size_t)index);
        data = obj_mesh_index_buffer_divagl::fill_data(data, &obj->mesh_array[i]);

        obj_mesh& mesh = obj->mesh_array[i];
        for (int32_t j = 0; j < mesh.num_submesh; j++)
            mesh.submesh_array[j].index_offset += offset;
    }

    buffer = create_index_buffer(buffer_size, index);
    if (!buffer) {
        free_def(index);
        unload();
        return false;
    }

    free_def(index);

    for (int32_t i = 0; i < mesh_num; i++)
        mesh_data[i].buffer = buffer;
#else
    for (int32_t i = 0; i < mesh_num; i++)
        if (!mesh_data[i].load(&obj->mesh_array[i]))
            return false;
#endif
    return true;
}

void obj_index_buffer::unload() {
    if (mesh_data) {
#if SHARED_OBJECT_BUFFER
        free_index_buffer(buffer);
        buffer = 0;
#else
        for (int32_t i = 0; i < mesh_num; i++)
            mesh_data[i].unload();
#endif
        delete[] mesh_data;
    }
    mesh_data = 0;
    mesh_num = 0;
#if SHARED_OBJECT_BUFFER
    buffer = 0;
#endif
}

#if SHARED_OBJECT_BUFFER
obj_vertex_buffer::obj_vertex_buffer() : mesh_num(), mesh_data(), buffers() {
#else
obj_vertex_buffer::obj_vertex_buffer() : mesh_num(), mesh_data() {
#endif

}

bool obj_vertex_buffer::load(obj* obj) {
    if (!obj)
        return false;

    mesh_num = obj->num_mesh;
    mesh_data = new obj_mesh_vertex_buffer_divagl[obj->num_mesh];
    if (!mesh_data)
        return false;

#if SHARED_OBJECT_BUFFER
    size_t buffer_size = 0;
    bool double_buffer = false;
    for (int32_t i = 0; i < mesh_num; i++) {
        obj_mesh& mesh = obj->mesh_array[i];
        if (!mesh.num_vertex)
            continue;

        mesh.attrib.m.compression = 0;

        uint32_t size_vertex;
        switch (mesh.attrib.m.compression) {
        case 0:
        default:
            size_vertex = obj_vertex_format_get_vertex_size(mesh.vertex_format);
            break;
        case 1:
            size_vertex = obj_vertex_format_get_vertex_size_comp1(mesh.vertex_format);
            break;
        case 2:
            size_vertex = obj_vertex_format_get_vertex_size_comp2(mesh.vertex_format);
            break;
        }
        mesh.size_vertex = size_vertex;

        buffer_size += (size_t)size_vertex * mesh.num_vertex;
        double_buffer |= !!mesh.attrib.m.double_buffer;
    }

    int32_t count = double_buffer ? 2 : 1;

    void* vertex = force_malloc(buffer_size);
    if (!vertex) {
        for (int32_t i = 0; i < count; i++)
            buffers[i] = 0;
        unload();
        return false;
    }

    void* data = vertex;
    for (int32_t i = 0; i < mesh_num; i++) {
        obj_mesh_vertex_buffer_divagl& mesh_buffer = mesh_data[i];
        mesh_buffer.offset = (size_t)data - (size_t)vertex;
        mesh_buffer.count = count;
        mesh_buffer.size = (GLsizeiptr)buffer_size;

        data = obj_mesh_vertex_buffer_divagl::fill_data(data, &obj->mesh_array[i]);
    }

    for (int32_t i = 0; i < count; i++) {
        buffers[i] = create_vertex_buffer(buffer_size, vertex);
        if (!buffers[i]) {
            free_def(vertex);
            unload();
            return false;
        }
    }

    free_def(vertex);

    for (int32_t i = 0; i < mesh_num; i++)
        memcpy(mesh_data[i].buffers, buffers, count * sizeof(GLuint));
#else
    for (int32_t i = 0; i < mesh_num; i++)
        if (!mesh_data[i].load(&obj->mesh_array[i]))
            return false;
#endif
    return true;
}

void obj_vertex_buffer::unload() {
    if (mesh_data) {
#if SHARED_OBJECT_BUFFER
        for (int32_t i = 0; i < mesh_data[0].count; i++)
            free_vertex_buffer(buffers[i]);
#else
        for (int32_t i = 0; i < mesh_num; i++)
            mesh_data[i].unload();
#endif
        delete[] mesh_data;
    }
    mesh_data = 0;
    mesh_num = 0;
#if SHARED_OBJECT_BUFFER
    buffers[0] = 0;
#endif
}

HOOK(bool, FASTCALL, obj_mesh_vertex_buffer__load, 0x0000000140458280, obj_mesh_vertex_buffer_divagl* objvb, obj_mesh* mesh) {
    return objvb->load(mesh, true);
}

HOOK(bool, FASTCALL, ObjsetInfo__index_buffer_load, 0x00000001404588F0, ObjsetInfo* info) {
    return info->index_buffer_load();
}

HOOK(bool, FASTCALL, ObjsetInfo__vertex_buffer_load, 0x00000001404589B0, ObjsetInfo* info) {
    return info->vertex_buffer_load();
}

HOOK(void, FASTCALL, ObjsetInfo__index_buffer_free, 0x0000000140459B40, ObjsetInfo* info) {
    info->index_buffer_free();
}

HOOK(void, FASTCALL, ObjsetInfo__vertex_buffer_free, 0x0000000140459C70, ObjsetInfo* info) {
    info->vertex_buffer_free();
}

HOOK(obj_mesh_index_buffer*, FASTCALL, objset_info_storage_get_obj_mesh_index_buffer, 0x000000014045A250, uint32_t obj_info, int32_t a2) {
    bool(FASTCALL * objset_info_storage_check_set)(int32_t set)
        = (bool(FASTCALL*)(int32_t set))0x00000001404577D0;
    int32_t(FASTCALL * objset_info_storage_get_obj_index)(int32_t set, uint32_t object_id)
        = (int32_t(FASTCALL*)(int32_t set, uint32_t object_id))0x000000014045A7A0;
    ObjsetInfo* (FASTCALL * objset_info_storage_get_objset_info)(int32_t set)
        = (ObjsetInfo * (FASTCALL*)(int32_t set))0x000000014045AC00;

    const object_info _obj_info = obj_info;
    if (objset_info_storage_check_set(_obj_info.set_id))
        return 0;

    ObjsetInfo* info = objset_info_storage_get_objset_info(_obj_info.set_id);
    if (!info || !info->objvb)
        return 0;

    int32_t obj_index = objset_info_storage_get_obj_index(_obj_info.set_id, _obj_info.id);
    if (obj_index >= 0) {
        if (!a2)
            return info->objib[obj_index].mesh_data;
        else if (info->field_C8)
            return info->field_C8[obj_index].mesh_data;
    }
    return 0;
}

HOOK(obj_mesh_vertex_buffer*, FASTCALL, objset_info_storage_get_obj_mesh_vertex_buffer, 0x000000014045A480, uint32_t obj_info, int32_t a2) {
    bool(FASTCALL * objset_info_storage_check_set)(int32_t set)
        = (bool(FASTCALL*)(int32_t set))0x00000001404577D0;
    int32_t(FASTCALL * objset_info_storage_get_obj_index)(int32_t set, uint32_t obj_id)
        = (int32_t(FASTCALL*)(int32_t set, uint32_t obj_id))0x000000014045A7A0;
    ObjsetInfo* (FASTCALL * objset_info_storage_get_objset_info)(int32_t set)
        = (ObjsetInfo * (FASTCALL*)(int32_t set))0x000000014045AC00;

    const object_info _obj_info = obj_info;
    if (objset_info_storage_check_set(_obj_info.set_id))
        return 0;

    ObjsetInfo* info = objset_info_storage_get_objset_info(_obj_info.set_id);
    if (!info || !info->objvb)
        return 0;

    int32_t obj_index = objset_info_storage_get_obj_index(_obj_info.set_id, _obj_info.id);
    if (obj_index >= 0) {
        if (!a2)
            return info->objvb[obj_index].mesh_data;
        else if (info->field_C0)
            return info->field_C0[obj_index].mesh_data;
    }
    return 0;
}

HOOK(obj_set*, FASTCALL, obj_set_parse_file, 0x000000014045CBC0,
    prj::shared_ptr<prj::stack_allocator>& alloc, __int64 set_file, int32_t& result) {
    obj_set* set = originalobj_set_parse_file(alloc, set_file, result);

    extern bool reflect_full;
    extern const firstread* firstread_ptr;
    if (reflect_full && firstread_ptr && firstread_ptr->objset_array && load_set_id != -1) {
        const firstread_objset_array* objset_array = firstread_ptr->objset_array;
        if (objset_array->objset_id_array && objset_array->objset_array) {
            const uint32_t num_objset = objset_array->num_objset;
            for (uint32_t i = 0; i < num_objset; i++) {
                if (objset_array->objset_id_array[i] != load_set_id || !objset_array->objset_array[i].data)
                    continue;

                const firstread_objset* frb_set = firstread_objset::read(alloc,
                    objset_array->objset_array[i].data, objset_array->objset_array[i].size);
                if (!frb_set || !set->obj_data || !frb_set->obj_data)
                    break;

                const int32_t frb_obj_num = frb_set->obj_num;
                for (int32_t j = 0; j < frb_obj_num; j++) {
                    const int32_t obj_num = set->obj_num;
                    for (int32_t k = 0; k < obj_num; k++) {
                        if (frb_set->obj_data[j].id != set->obj_id_data[k])
                            continue;

                        obj* obj = set->obj_data[k];
                        const firstread_obj* frb_obj = &frb_set->obj_data[j];
                        if (obj->mesh_array && frb_obj->mesh_array) {
                            const int32_t frb_num_mesh = frb_obj->num_mesh;
                            int32_t index_match = -1;
                            for (int32_t l = 0; l < frb_num_mesh; l++) {
                                const uint64_t name_hash = hash_utf8_xxh3_64bits(frb_obj->mesh_array[l].name);
                                const int32_t num_mesh = obj->num_mesh;
                                for (int32_t m = 0; m < num_mesh; m++) {
                                    if (name_hash != hash_utf8_xxh3_64bits(obj->mesh_array[m].name))
                                        continue;
                                    else if (frb_obj->mesh_array[l].index != (uint8_t)-1) {
                                        if (++index_match != frb_obj->mesh_array[l].index)
                                            continue;
                                    }

                                    obj_mesh* mesh = &obj->mesh_array[m];
                                    const firstread_obj_mesh* frb_mesh = &frb_obj->mesh_array[l];
                                    if (frb_mesh->remove) {
                                        mesh->flags = 0;
                                        mesh->bounding_sphere = {};
                                        mesh->num_submesh = 0;
                                        mesh->submesh_array = 0;
                                        mesh->vertex_format = (obj_vertex_format)0;
                                        mesh->size_vertex = 0;
                                        mesh->num_vertex = 0;
                                        for (int32_t n = 0; n < 20; n++)
                                            mesh->vertex_array_ptr[n] = 0;
                                        mesh->attrib = {};
                                        mesh->vertex_flags = 0;
                                        break;
                                    }

                                    if (mesh->submesh_array && mesh->num_submesh) {
                                        const int32_t num_submesh = mesh->num_submesh;
                                        for (int32_t n = 0; n < num_submesh; n++) {
                                            obj_sub_mesh* submesh = &mesh->submesh_array[n];
                                            submesh->attrib.m.no_reflect = frb_mesh->attrib.m.no_reflect;
                                            submesh->attrib.m.reflect = frb_mesh->attrib.m.reflect;
                                            submesh->attrib.m.reflect_cam_back = frb_mesh->attrib.m.reflect_cam_back;
                                        }
                                    }

                                    if (mesh->submesh_array && frb_mesh->submesh_array
                                        && mesh->num_submesh && mesh->num_submesh == frb_mesh->num_submesh) {
                                        const int32_t num_submesh = mesh->num_submesh;
                                        int32_t num_submesh_split = num_submesh;
                                        for (int32_t n = 0; n < num_submesh; n++) {
                                            const firstread_obj_sub_mesh* frb_submesh = &frb_mesh->submesh_array[n];
                                            if (frb_submesh->split)
                                                num_submesh_split++;
                                        }

                                        if (num_submesh < num_submesh_split) {
                                            obj_sub_mesh* submesh_array = mesh->submesh_array;
                                            mesh->num_submesh = num_submesh_split;
                                            mesh->submesh_array = alloc->allocate<obj_sub_mesh>(num_submesh_split);
                                            for (int32_t n = 0, o = 0; n < num_submesh; n++, o++) {
                                                const firstread_obj_sub_mesh* frb_submesh = &frb_mesh->submesh_array[n];
                                                mesh->submesh_array[o] = submesh_array[n];
                                                if (frb_submesh->split)
                                                    mesh->submesh_array[++o] = submesh_array[n];
                                            }
                                        }

                                        for (int32_t n = 0, o = 0; n < num_submesh; n++, o++) {
                                            obj_sub_mesh* submesh = &mesh->submesh_array[o];
                                            const firstread_obj_sub_mesh* frb_submesh = &frb_mesh->submesh_array[n];

                                            if (frb_submesh->split) {
                                                firstread_obj_sub_mesh_split* split = frb_submesh->split;
                                                if (split->material_index[0] != -1)
                                                    submesh->material_index = split->material_index[0];
                                                if (split->num_index[0] && split->index_array[0]) {
                                                    submesh->bounding_sphere = split->bounding_sphere[0];
                                                    submesh->index_format = split->index_format;
                                                    submesh->num_index = split->num_index[0];
                                                    submesh->index_array = (void*)split->index_array[0];
                                                }
                                                submesh->attrib = split->attrib[0];

                                                obj_sub_mesh* submesh_before = &mesh->submesh_array[++o];
                                                if (split->material_index[1] != -1)
                                                    submesh_before->material_index = split->material_index[1];
                                                if (split->num_index[1] && split->index_array[1]) {
                                                    submesh_before->bounding_sphere = split->bounding_sphere[1];
                                                    submesh_before->index_format = split->index_format;
                                                    submesh_before->num_index = split->num_index[1];
                                                    submesh_before->index_array = (void*)split->index_array[1];
                                                }
                                                submesh_before->attrib = split->attrib[1];
                                                continue;
                                            }
                                            else if (frb_submesh->num_index && frb_submesh->index_array) {
                                                submesh->bounding_sphere = frb_submesh->bounding_sphere;
                                                submesh->index_format = frb_submesh->index_format;
                                                submesh->num_index = frb_submesh->num_index;
                                                submesh->index_array = (void*)frb_submesh->index_array;
                                            }

                                            submesh->attrib = frb_submesh->attrib;
                                        }
                                    }

                                    if (frb_mesh->vertex_format && frb_mesh->num_vertex) {
                                        mesh->vertex_format = frb_mesh->vertex_format;
                                        mesh->size_vertex = frb_mesh->size_vertex;
                                        mesh->num_vertex = frb_mesh->num_vertex;
                                        for (int32_t n = 0; n < 20; n++)
                                            mesh->vertex_array_ptr[n] = frb_mesh->vertex_array_ptr[n];
                                    }

                                    mesh->attrib = frb_mesh->attrib;
                                    break;
                                }
                            }
                        }

                        if (frb_obj->num_material && frb_obj->material_array) {
                            obj->num_material = frb_obj->num_material;
                            obj->material_array = (obj_material_data*)frb_obj->material_array;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    return set;
}

HOOK(bool, FASTCALL, _objset_info_storage_load_obj_set_check_not_read, 0x000000014045DA60, int32_t set) {
    load_set_id = set;
    bool ret = original_objset_info_storage_load_obj_set_check_not_read(set);
    load_set_id = -1;
    return ret;
}

HOOK(void, FASTCALL, obj_mesh_vertex_buffer__unload, 0x0000000140461870, obj_mesh_vertex_buffer_divagl* objvb) {
    objvb->unload();
}

void object_patch() {
#if SHARED_OBJECT_BUFFER
    WRITE_MEMORY(0x000000014045A4E7, uint8_t, sizeof(obj_vertex_buffer));
#endif

    INSTALL_HOOK(obj_mesh_vertex_buffer__load);
    INSTALL_HOOK(ObjsetInfo__index_buffer_load);
    INSTALL_HOOK(ObjsetInfo__vertex_buffer_load);
    INSTALL_HOOK(ObjsetInfo__index_buffer_free);
    INSTALL_HOOK(ObjsetInfo__vertex_buffer_free);
    INSTALL_HOOK(objset_info_storage_get_obj_mesh_index_buffer);
    INSTALL_HOOK(objset_info_storage_get_obj_mesh_vertex_buffer);
    INSTALL_HOOK(obj_set_parse_file);
    INSTALL_HOOK(_objset_info_storage_load_obj_set_check_not_read);
    INSTALL_HOOK(obj_mesh_vertex_buffer__unload);
}

#pragma warning(push)
#pragma warning(disable: 6385)
void ObjsetInfo::index_buffer_free() {
    if (objib) {
        for (int32_t i = 0; i < objib_num; i++)
            objib[i].unload();
        prj::HeapCMallocFree(prj::HeapCMallocSystem, objvb);
    }

    objib = 0;
    objib_num = 0;
}

bool ObjsetInfo::index_buffer_load() {
    ::obj_set* set = obj_set;
    objib_num = set->obj_num;
    objib = new (prj::HeapCMallocAllocate(prj::HeapCMallocSystem,
        sizeof(obj_index_buffer) * set->obj_num, "OBJIB")) obj_index_buffer[set->obj_num];
    if (!objib)
        return true;

    for (int32_t i = 0; i < set->obj_num; i++)
        if (!objib[i].load(set->obj_data[i]))
            return false;
    return true;
}

void ObjsetInfo::vertex_buffer_free() {
    if (objvb) {
        for (int32_t i = 0; i < objvb_num; i++)
            objvb[i].unload();
        prj::HeapCMallocFree(prj::HeapCMallocSystem, objvb);
    }

    objvb = 0;
    objvb_num = 0;
}

bool ObjsetInfo::vertex_buffer_load() {
    ::obj_set* set = obj_set;
    objvb_num = set->obj_num;
    objvb = new (prj::HeapCMallocAllocate(prj::HeapCMallocSystem,
        sizeof(obj_vertex_buffer) * set->obj_num, "OBJVB")) obj_vertex_buffer[set->obj_num];
    if (!objvb)
        return true;

    for (int32_t i = 0; i < set->obj_num; i++)
        if (!objvb[i].load(set->obj_data[i]))
            return false;
    return true;
}
#pragma warning(pop)

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

static int32_t remove_degenerate_triangle_indices(
    uint16_t* dst_index_array, const int32_t num_index, uint16_t* src_index_array) {
    if (!num_index)
        return 0;

    dst_index_array[0] = src_index_array[0];

    int32_t src_index = 1;
    int32_t dst_index = 1;
    int32_t strip_length = 1;
    while (src_index < num_index - 4)
        if (src_index_array[src_index] != src_index_array[src_index + 1]) {
            dst_index_array[dst_index++] = src_index_array[src_index];
            strip_length++;
            src_index++;
        }
        else if (src_index_array[src_index + 3] == src_index_array[src_index + 4]) {
            dst_index_array[dst_index++] = src_index_array[src_index];
            dst_index_array[dst_index++] = 0xFFFF;
            dst_index_array[dst_index++] = src_index_array[src_index + 4];

            if (strip_length % 2) {
                dst_index_array[dst_index++] = src_index_array[src_index + 4];
                strip_length = 0;
            }
            else
                strip_length = 1;
            src_index += 5;
        }
        else if (src_index_array[src_index - 1] != src_index_array[src_index + 2]
            || src_index_array[src_index + 1] != src_index_array[src_index + 4]) {
            dst_index_array[dst_index++] = src_index_array[src_index];
            dst_index_array[dst_index++] = 0xFFFF;
            dst_index_array[dst_index++] = src_index_array[src_index + 3];

            if (!(strip_length % 2)) {
                dst_index_array[dst_index++] = src_index_array[src_index + 3];
                strip_length = 0;
            }
            else
                strip_length = 1;
            src_index += 4;
        }
        else {
            dst_index_array[dst_index++] = src_index_array[src_index];
            strip_length++;
            src_index += 5;
        }

    if (src_index < num_index) {
        src_index_array += src_index;
        dst_index_array += dst_index;
        dst_index += num_index - src_index;
        while (src_index++ < num_index)
            *dst_index_array++ = *src_index_array++;
    }
    return dst_index;
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

inline static uint32_t obj_vertex_format_get_vertex_size_comp1(obj_vertex_format format) {
    uint32_t size = 0;
    if (format & OBJ_VERTEX_POSITION)
        size += 12;
    if (format & OBJ_VERTEX_NORMAL)
        size += 8;
    if (format & OBJ_VERTEX_TANGENT)
        size += 8;
    if (format & OBJ_VERTEX_TEXCOORD0)
        size += 4;
    if (format & OBJ_VERTEX_TEXCOORD1)
        size += 4;
    if (format & OBJ_VERTEX_TEXCOORD2)
        size += 4;
    if (format & OBJ_VERTEX_TEXCOORD3)
        size += 4;
    if (format & OBJ_VERTEX_COLOR0)
        size += 8;
    if ((format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA)
        size += 16;
    return size;
}

inline static uint32_t obj_vertex_format_get_vertex_size_comp2(obj_vertex_format format) {
    uint32_t size = 0;
    if (format & OBJ_VERTEX_POSITION)
        size += 12;
    if (format & OBJ_VERTEX_NORMAL)
        size += 4;
    if (format & OBJ_VERTEX_TANGENT)
        size += 4;
    if (format & OBJ_VERTEX_TEXCOORD0)
        size += 4;
    if (format & OBJ_VERTEX_TEXCOORD1)
        size += 4;
    if (format & OBJ_VERTEX_TEXCOORD2)
        size += 4;
    if (format & OBJ_VERTEX_TEXCOORD3)
        size += 4;
    if (format & OBJ_VERTEX_COLOR0)
        size += 8;
    if ((format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA)
        size += 8;
    return size;
}
