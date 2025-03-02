/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "../AFTModsShared/object.hpp"
#include "../AFTModsShared/types.hpp"
#include "texture.hpp"

#define SHARED_OBJECT_BUFFER (1)

struct obj_mesh_index_buffer_divagl : obj_mesh_index_buffer {
    obj_mesh_index_buffer_divagl();
    obj_mesh_index_buffer_divagl(const obj_mesh_index_buffer& other);

    bool load(obj_mesh* mesh);
    bool load_data(size_t size, const void* data);
    void unload();

    static void* fill_data(void* data, obj_mesh* mesh);
};

struct obj_mesh_vertex_buffer_divagl : obj_mesh_vertex_buffer {
#if SHARED_OBJECT_BUFFER
    GLsizeiptr size;
    size_t offset;
#endif

    obj_mesh_vertex_buffer_divagl();
    obj_mesh_vertex_buffer_divagl(const obj_mesh_vertex_buffer& other);

    void cycle_index();
    GLuint get_buffer();
    size_t get_offset();
    GLsizeiptr get_size();
    bool load(obj_mesh* mesh, bool dynamic = false);
    bool load_data(size_t size, const void* data, int32_t count, bool dynamic);
    void unload();

    static void* fill_data(void* data, obj_mesh* mesh);
};

struct obj_index_buffer {
    int32_t mesh_num;
    obj_mesh_index_buffer_divagl* mesh_data;
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
    obj_mesh_vertex_buffer_divagl* mesh_data;
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

extern obj_mesh_index_buffer* (FASTCALL* objset_info_storage_get_obj_mesh_index_buffer)(uint32_t obj_info, int32_t a2);
extern obj_mesh_vertex_buffer* (FASTCALL* objset_info_storage_get_obj_mesh_vertex_buffer)(uint32_t obj_info, int32_t a2);
extern GLuint(FASTCALL* objset_info_storage_get_set_texture)(int32_t set, int32_t tex_id);
extern prj::vector<GLuint>* (FASTCALL* objset_info_storage_get_set_gentex)(int32_t set);

extern void object_patch();
