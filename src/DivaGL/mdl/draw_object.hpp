/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"
#include "../object.hpp"
#include "../render_context.hpp"

namespace mdl {
    extern void draw(obj_primitive_type primitive_type, uint32_t count,
        uint16_t start, uint16_t end, obj_index_format index_format, size_t indices);
    extern void draw_etc_obj(mdl::EtcObj* etc);
    extern void draw_sub_mesh(const ObjSubMeshArgs* args, const mat4* model,
        void(*func)(const ObjSubMeshArgs* args));
    extern void draw_sub_mesh_show_vector(const ObjSubMeshArgs* args, const mat4* model, int32_t show_vector);
    extern void draw_sub_mesh_default(const ObjSubMeshArgs* args);
    extern void draw_sub_mesh_default_instanced(const ObjSubMeshArgs* args, const mat4* mat);
    extern void draw_sub_mesh_sss(const ObjSubMeshArgs* args);
    extern void draw_sub_mesh_reflect(const ObjSubMeshArgs* args);
    extern void draw_sub_mesh_reflect_reflect_map(const ObjSubMeshArgs* args);
    extern void draw_sub_mesh_shadow(const ObjSubMeshArgs* args);
    extern void draw_sub_mesh_translucent(const ObjSubMeshArgs* args);
};

extern void draw_object_model_mat_load(const mat4& mat);
extern void model_mat_face_camera_position(const mat4* view, const mat4* src, mat4* dst);
extern void model_mat_face_camera_view(const mat4* view, const mat4* src, mat4* dst);
