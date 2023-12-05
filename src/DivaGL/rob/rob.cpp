/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob.hpp"
#include "../mdl/disp_manager.hpp"
#include "../gl_state.hpp"
#include "../object.hpp"
#include <Helpers.h>
#include <algorithm>

struct rob_chara_age_age_object_vertex {
    vec3 position;
    vec3 normal;
    vec4 tangent;
    vec2 texcoord;
};

struct rob_chara_age_age_object {
    void* obj_set_handler; // obj_set_handler
    int32_t obj_index;
    int32_t field_C;
    int32_t num_vertex;
    int32_t num_index;
    obj obj;
    obj_mesh mesh;
    obj_sub_mesh sub_mesh;
    obj_material_data material[2];
    obj_axis_aligned_bounding_box axis_aligned_bounding_box;
    rob_chara_age_age_object_vertex* vertex_data;
    int32_t vertex_data_size;
    int32_t vertex_array_size;
    obj_mesh_vertex_buffer obj_vert_buf;
    obj_mesh_index_buffer obj_index_buf;
    vec3 trans[10];
    int32_t disp_count;
    int32_t count;
    bool field_C3C;
};

mat4* (FASTCALL* rob_chara_bone_data_get_mats_mat)(size_t rob_bone_data, size_t index)
    = (mat4 * (FASTCALL*)(size_t rob_bone_data, size_t index))0x0000000140419520;
void(FASTCALL* sub_1405163C0)(size_t rob_chr, int32_t index, mat4* mat)
    = (void(FASTCALL*)(size_t rob_chr, int32_t index, mat4 * mat))0x00000001405163C0;
float_t(FASTCALL* rob_chara_get_max_face_depth)(size_t rob_chr)
    = (float_t(FASTCALL*)(size_t rob_chr))0x0000000140516510;
mat4* (FASTCALL* sub_140516740)(size_t rob_chr)
    = (mat4 * (FASTCALL*)(size_t rob_chr))0x0000000140516740;
mat4* (FASTCALL* rob_chara_get_adjust_data_mat)(size_t rob_chr)
    = (mat4 * (FASTCALL*)(size_t rob_chr))0x0000000140516750;
bool(FASTCALL* rob_chara_array_check_visibility)(size_t rob_chr, int32_t chara_id)
    = (bool(FASTCALL*)(size_t rob_chr, int32_t chara_id))0x0000000140531F50;
size_t(FASTCALL* get_rob_chara_smth)() = (size_t(FASTCALL*)())0x00000001405320E0;
size_t(FASTCALL* rob_chara_array_get)(size_t rob_chr_smth, int32_t chara_id)
    = (size_t(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x0000000140532030;
size_t(FASTCALL* rob_chara_array_get_bone_data)(size_t rob_chr_smth, int32_t chara_id)
    = (size_t(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001405320F0;
bool (FASTCALL* rob_chara_pv_data_array_check_chara_id)(size_t rob_chr_smth, int32_t chara_id)
    = (bool (FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001405327B0;

HOOK(void, FASTCALL, rob_chara_age_age_object__disp, 0x0000000140541D50,
    rob_chara_age_age_object* rob_age_age_obj, size_t chara_index, bool npr, bool reflect, vec3* a5, bool chara_color) {
    int32_t disp_count = rob_age_age_obj->disp_count;
    if (!rob_age_age_obj->obj_set_handler || !disp_count)
        return;

    disp_count = min_def(disp_count, 10);

    std::pair<float_t, int32_t> v44[10];
    for (int32_t i = 0; i < disp_count; i++) {
        v44[i].first = vec3::dot(rob_age_age_obj->trans[i], *a5);
        v44[i].second = i;
    }

    if (disp_count >= 2)
        std::sort(v44, v44 + disp_count,
            [](const std::pair<float_t, int32_t>& a, const std::pair<float_t, int32_t>& b) {
                return a.first < b.first;
            });

    rob_age_age_obj->obj_vert_buf.cycle_index();

    GLuint buffer = rob_age_age_obj->obj_vert_buf.get_buffer();
    size_t vtx_data;
    if (GL_VERSION_4_5) {
        vtx_data = (size_t)glMapNamedBuffer(buffer, GL_WRITE_ONLY);
        if (!vtx_data) {
            glUnmapNamedBuffer(buffer);
            return;
        }
    }
    else {
        gl_state_bind_array_buffer(buffer);
        vtx_data = (size_t)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        if (!vtx_data) {
            glUnmapBuffer(GL_ARRAY_BUFFER);
            gl_state_bind_array_buffer(0);
            return;
        }
    }

    size_t vertex_array_size = rob_age_age_obj->vertex_array_size;
    for (int32_t i = 0; i < disp_count; i++)
        memmove((void*)(vtx_data + vertex_array_size * i),
            (void*)((size_t)rob_age_age_obj->vertex_data + vertex_array_size * v44[i].second), vertex_array_size);

    if (GL_VERSION_4_5)
        glUnmapNamedBuffer(buffer);
    else {
        glUnmapBuffer(GL_ARRAY_BUFFER);
        gl_state_bind_array_buffer(0);
    }

    rob_age_age_obj->mesh.num_vertex = disp_count * rob_age_age_obj->num_vertex;
    rob_age_age_obj->sub_mesh.num_index = disp_count * rob_age_age_obj->num_index;
    rob_age_age_obj->sub_mesh.material_index = npr ? 1 : 0;

    mdl::ObjFlags flags = (mdl::ObjFlags)(mdl::OBJ_SSS | mdl::OBJ_4 | mdl::OBJ_SHADOW);
    if (reflect)
        enum_or(flags, mdl::OBJ_CHARA_REFLECT);
    disp_manager->set_obj_flags(flags);
    disp_manager->set_chara_color(chara_color);
    disp_manager->set_shadow_type(chara_index ? SHADOW_STAGE : SHADOW_CHARA);

    static prj::vector<GLuint>*(FASTCALL * rob_chara_age_age_object__get_obj_set_texture)(rob_chara_age_age_object * rob_age_age_obj)
        = (prj::vector<GLuint>*(FASTCALL*)(rob_chara_age_age_object* rob_age_age_obj))0x000000014045A8E0;
    disp_manager->entry_obj_by_obj(&mat4_identity, &rob_age_age_obj->obj,
        rob_chara_age_age_object__get_obj_set_texture(rob_age_age_obj),
        &rob_age_age_obj->obj_vert_buf, &rob_age_age_obj->obj_index_buf, 0, 1.0f);
}


void rob_patch() {
    INSTALL_HOOK(rob_chara_age_age_object__disp);
}
