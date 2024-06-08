/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob.hpp"
#include "../../KKdLib/str_utils.hpp"
#include "../gl_state.hpp"
#include "../render_manager.hpp"
#include <Helpers.h>
#include <algorithm>

struct rob_chara_age_age_object_vertex {
    vec3 position;
    vec3 normal;
    vec4 tangent;
    vec2 texcoord;
};

static_assert(sizeof(rob_chara_age_age_object_vertex) == 0x30, "\"rob_chara_age_age_object_vertex\" struct should have a size of 0x30");

struct rob_chara_age_age_object {
    void* objset_info; // ObjsetInfo
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
#if SHARED_OBJECT_BUFFER
    obj_mesh_vertex_buffer_aft obj_vert_buf;
#else
    obj_mesh_vertex_buffer obj_vert_buf;
#endif
    obj_mesh_index_buffer obj_index_buf;
    vec3 trans[10];
    int32_t disp_count;
    int32_t count;
    bool field_C3C;

    void disp(size_t chara_index,
        bool npr, bool reflect, const vec3& a5, bool chara_color);
};

static_assert(sizeof(rob_chara_age_age_object) == 0xC40, "\"rob_chara_age_age_object\" struct should have a size of 0xC40");

struct rob_chara_age_age_data {
    int32_t index;
    int32_t part_id;
    vec3 trans;
    float_t field_14;
    float_t rot_z;
    int32_t field_1C;
    float_t rot_speed;
    float_t gravity;
    float_t scale;
    float_t alpha;
    mat4 mat_scale;
    mat4 mat;
    mat4 prev_parent_mat;
    float_t remaining;
    bool alive;
};

static_assert(sizeof(rob_chara_age_age_data) == 0xF8, "\"rob_chara_age_age_data\" struct should have a size of 0xF8");

struct rob_chara_age_age {
    rob_chara_age_age_data data[10];
    float_t frame;
    float_t field_9B4;
    mat4 mat;
    float_t step;
    bool visible;
    bool skip;
    float_t move_cancel;
    float_t alpha;
    rob_chara_age_age_object object;
    bool npr;
    float_t rot_speed;
    bool step_full;

    void disp(size_t chara_id,
        bool npr, bool reflect, const vec3& a5, bool chara_color);
};

static_assert(sizeof(rob_chara_age_age) == 0x1658, "\"rob_chara_age_age\" struct should have a size of 0x1658");

struct rob_chara_item_adjust_x {
    mat4 mat;
    vec3 trans;
    float_t scale;

    rob_chara_item_adjust_x();

    void reset();
};

struct rob_chara_arm_adjust_x {
    float_t next_value;
    float_t prev_value;
    int32_t start_frame;
    float_t duration;
    float_t scale;

    rob_chara_arm_adjust_x();

    void reset();
};

const mat4* (FASTCALL* rob_chara_bone_data_get_mats_mat)(size_t rob_bone_data, size_t index)
    = (const mat4 * (FASTCALL*)(size_t rob_bone_data, size_t index))0x0000000140419520;
bool (FASTCALL* pv_osage_manager_array_get_disp)(int32_t* chara_id)
    = (bool (FASTCALL*)(int32_t * chara_id))0x00000001404F9130;
const char* (FASTCALL* chara_index_get_auth_3d_name)(chara_index chara_index)
    = (const char* (FASTCALL*)(chara_index chara_index))0x0000000140508100;
void(FASTCALL* sub_1405163C0)(rob_chara* rob_chr, int32_t index, mat4* mat)
    = (void(FASTCALL*)(rob_chara* rob_chr, int32_t index, mat4 * mat))0x00000001405163C0;
float_t(FASTCALL* rob_chara_get_max_face_depth)(rob_chara* rob_chr)
    = (float_t(FASTCALL*)(rob_chara* rob_chr))0x0000000140516510;
const mat4* (FASTCALL* rob_chara_get_bone_data_mat)(rob_chara* rob_chr, mot_bone_index index)
    = (const mat4 * (FASTCALL*)(rob_chara* rob_chr, mot_bone_index index))0x0000000140516730;
const mat4* (FASTCALL* sub_140516740)(rob_chara* rob_chr)
    = (const mat4 * (FASTCALL*)(rob_chara* rob_chr))0x0000000140516740;
bool(FASTCALL* rob_chara_array_check_visibility)(size_t rob_chr_smth, int32_t chara_id)
    = (bool(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x0000000140531F50;
rob_chara* (FASTCALL* rob_chara_array_get)(size_t rob_chr_smth, int32_t chara_id)
    = (rob_chara * (FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x0000000140532030;
size_t(FASTCALL* get_rob_chara_smth)() = (size_t(FASTCALL*)())0x00000001405320E0;
rob_chara_item_equip* (FASTCALL* rob_chara_array_get_item_equip)(size_t rob_chr_smth, int32_t chara_id)
    = (rob_chara_item_equip * (FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x0000000140532120;
size_t(FASTCALL* rob_chara_array_get_bone_data)(size_t rob_chr_smth, int32_t chara_id)
    = (size_t(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001405320F0;
bool (FASTCALL* rob_chara_pv_data_array_check_chara_id)(size_t rob_chr_smth, int32_t chara_id)
    = (bool (FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001405327B0;

static rob_chara_age_age* rob_chara_age_age_array = (rob_chara_age_age*)0x00000001411E83C0;

static bool& chara_reflect = *(bool*)0x00000001411ADAFC;
static bool& chara_refract = *(bool*)0x00000001411ADAFD;

const mat4* rob_chara_item_equip_mat = 0;

rob_chara_item_adjust_x rob_chara_item_adjust_x_array[ROB_CHARA_COUNT];
rob_chara_arm_adjust_x rob_chara_arm_adjust_x_array[ROB_CHARA_COUNT];

static void rob_chara_age_age_array_disp(int32_t chara_id, bool reflect, bool chara_color);

SkinParam::CollisionParam::CollisionParam() : type(), node_idx(), pos() {
    radius = 0.2f;
}

void RobOsage::SetAirRes(float_t air_res) {
    skin_param_ptr->air_res = air_res;
}

void RobOsage::SetColiR(float_t coli_r) {
    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++)
        i->data_ptr->skp_osg_node.coli_r = coli_r;
}

void RobOsage::SetForce(float_t force, float_t force_gain) {
    skin_param_ptr->force = force;
    skin_param_ptr->force_gain = force_gain;
    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++) {
        i->data_ptr->force = force;
        force = force * force_gain;
    }
}

void RobOsage::SetHinge(float_t hinge_y, float_t hinge_z) {
    hinge_y = min_def(hinge_y, 179.0f);
    hinge_z = min_def(hinge_z, 179.0f);
    hinge_y = hinge_y * DEG_TO_RAD_FLOAT;
    hinge_z = hinge_z * DEG_TO_RAD_FLOAT;
    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++) {
        RobOsageNodeData* data = i->data_ptr;
        data->skp_osg_node.hinge.ymin = -hinge_y;
        data->skp_osg_node.hinge.ymax = hinge_y;
        data->skp_osg_node.hinge.zmin = -hinge_z;
        data->skp_osg_node.hinge.zmax = hinge_z;
    }
}

void RobOsage::SetInitRot(float_t init_rot_y, float_t init_rot_z) {
    skin_param_ptr->init_rot.y = init_rot_y * DEG_TO_RAD_FLOAT;
    skin_param_ptr->init_rot.z = init_rot_z * DEG_TO_RAD_FLOAT;
}

void RobOsage::SetMotionResetData(uint32_t motion_id, float_t frame) {
    osage_reset = true;
    auto elem = motion_reset_data.find({ motion_id, (int32_t)prj::roundf(frame * 1000.0f) });
    if (elem != motion_reset_data.end() && elem->second.size() + 1 == nodes.size())
        reset_data_list = &elem->second;
}

// 0x140480F40
void RobOsage::SetNodesExternalForce(vec3* external_force, float_t strength) {
    if (!external_force) {
        RobOsageNode* i_begin = nodes.data() + 1;
        RobOsageNode* i_end = nodes.data() + nodes.size();
        for (RobOsageNode* i = i_begin; i != i_end; i++)
            i->external_force = 0.0f;
        return;
    }

    vec3 v4 = *external_force;
    size_t exf = osage_setting.exf;
    size_t v8 = 0;
    if (exf >= 4) {
        float_t strength4 = strength * strength * strength * strength;
        v8 = ((exf - 4) / 4 + 1) * 4;
        for (size_t v10 = v8 / 4; v10; v10--)
            v4 *= strength4;
    }

    if (v8 < exf)
        for (size_t v12 = exf - v8; v12; v12--)
            v4 *= strength;

    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++) {
        i->external_force = v4;
        v4 *= strength;
    }
}

// 0x140481540
void RobOsage::SetNodesForce(float_t force) {
    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++)
        i->force = force;
}

void RobOsage::SetRot(float_t rot_y, float_t rot_z) {
    skin_param_ptr->rot.y = rot_y * DEG_TO_RAD_FLOAT;
    skin_param_ptr->rot.z = rot_z * DEG_TO_RAD_FLOAT;
}

void rob_chara_item_equip_object::disp(const mat4* mat) {
    if (obj_info.is_null())
        return;

    mdl::ObjFlags flags = disp_manager->get_obj_flags();
    mdl::ObjFlags chara_flags = flags;
    if (fabsf(alpha - 1.0f) > 0.000001f)
        enum_or(chara_flags, obj_flags);
    else
        enum_and(chara_flags, ~(mdl::OBJ_ALPHA_ORDER_3 | mdl::OBJ_ALPHA_ORDER_2 | mdl::OBJ_ALPHA_ORDER_1));
    disp_manager->set_obj_flags(chara_flags);
    if (can_disp) {
        disp_manager->entry_obj_by_object_info_object_skin(obj_info,
            &texture_pattern, &texture_data, alpha, mats, ex_data_bone_mats.data(), 0, mat);

        rob_chara_item_equip_mat = mat;

        for (ExNodeBlock*& i : node_blocks)
            i->__vftable->Disp(i);
    }
    disp_manager->set_obj_flags(flags);
}

int32_t rob_chara_item_equip_object::get_bone_index(const char* name) {
    int32_t(FASTCALL * sub_1401F13B0)(int32_t a1, const char* a2)
        = (int32_t(FASTCALL*)(int32_t a1, const char* a2))0x00000001401F13B0;

    int32_t bone_index = sub_1401F13B0(0, name);
    if (bone_index == -1)
        for (auto& i : ex_bones)
            if (!str_utils_compare(name, i.first))
                return 0x8000 | i.second;
    return bone_index;
}

bone_node* rob_chara_item_equip_object::get_bone_node(
    int32_t bone_index) {
    if (!(bone_index & 0x8000))
        return &bone_nodes[bone_index & 0x7FFF];
    else if ((bone_index & 0x7FFF) < ex_data_bone_nodes.size())
        return &ex_data_bone_nodes[bone_index & 0x7FFF];
    return 0;
}

bone_node* rob_chara_item_equip_object::get_bone_node(const char* name) {
    return get_bone_node(get_bone_index(name));
}

void rob_chara_item_equip_object::skp_load(void* can_prop) {
    void (FASTCALL * rob_chara_item_equip_object__skp_load)(rob_chara_item_equip_object * _this, void* can_prop)
        = (void (FASTCALL*)(rob_chara_item_equip_object * _this, void* can_prop))0x00000001405F43E0;
    rob_chara_item_equip_object__skp_load(this, can_prop);
}

rob_chara_item_equip_object* rob_chara_item_equip::get_item_equip_object(item_id id) {
    if (id >= ITEM_BODY && id <= ITEM_ITEM16)
        return &item_equip_object[id];
    return 0;
}

const mat4* rob_chara_get_adjust_data_mat(rob_chara* rob_chr) {
    return &rob_chr->data.adjust_data.mat;
}

const mat4* rob_chara_get_item_adjust_data_mat(rob_chara* rob_chr) {
    return &rob_chara_item_adjust_x_array[rob_chr->chara_id].mat;
}

HOOK(void, FASTCALL, RobCloth__UpdateVertexBuffer, 0x000000014021CF00, obj_mesh* mesh, obj_mesh_vertex_buffer* vertex_buffer,
    CLOTHNode* nodes, float_t facing, uint16_t* indices, bool double_sided) {
    if (!mesh || !vertex_buffer || (mesh->vertex_format
        & (OBJ_VERTEX_NORMAL | OBJ_VERTEX_POSITION)) != (OBJ_VERTEX_NORMAL | OBJ_VERTEX_POSITION))
        return;

    vertex_buffer->cycle_index();
    GLArrayBuffer buffer = vertex_buffer->get_buffer();
    size_t data = (size_t)buffer.MapMemory();
    if (!data)
        return;

    int32_t indices_count = *indices++;

    if (double_sided)
        indices_count /= 2;

    bool tangent = !!(mesh->vertex_format & OBJ_VERTEX_TANGENT);

    switch (mesh->attrib.m.compression) {
    case 0:
    default:
        if (tangent)
            for (int32_t i = 2; i; i--, facing = -1.0f) {
                for (int32_t j = indices_count; j; j--, indices++) {
                    CLOTHNode* node = &nodes[*indices];

                    *(vec3*)data = node->trans;
                    *(vec3*)(data + 0x0C) = node->normal * facing;
                    *(vec3*)(data + 0x18) = node->tangent;
                    *(float_t*)(data + 0x24) = node->tangent_sign;

                    data += mesh->size_vertex;
                }

                if (!double_sided)
                    break;
            }
        else
            for (int32_t i = 2; i; i--, facing = -1.0f) {
                for (int32_t j = indices_count; j; j--, indices++) {
                    CLOTHNode* node = &nodes[*indices];

                    *(vec3*)data = node->trans;
                    *(vec3*)(data + 0x0C) = node->normal * facing;

                    data += mesh->size_vertex;
                }

                if (!double_sided)
                    break;
            }
        break;
    case 1:
        if (tangent)
            for (int32_t i = 2; i; i--, facing = -1.0f) {
                for (int32_t j = indices_count; j; j--, indices++) {
                    CLOTHNode* node = &nodes[*indices];

                    *(vec3*)data = node->trans;

                    vec3_to_vec3i16(node->normal * (32767.0f * facing), *(vec3i16*)(data + 0x0C));
                    *(int16_t*)(data + 0x12) = 0;

                    vec4 tangent;
                    *(vec3*)&tangent = node->tangent;
                    tangent.w = node->tangent_sign;
                    vec4_to_vec4i16(tangent * 32767.0f, *(vec4i16*)(data + 0x14));

                    data += mesh->size_vertex;
                }

                if (!double_sided)
                    break;
            }
        else
            for (int32_t i = 2; i; i--, facing = -1.0f) {
                for (int32_t j = indices_count; j; j--, indices++) {
                    CLOTHNode* node = &nodes[*indices];

                    *(vec3*)data = node->trans;

                    vec3_to_vec3i16(node->normal * (32767.0f * facing), *(vec3i16*)(data + 0x0C));
                    *(int16_t*)(data + 0x12) = 0;

                    data += mesh->size_vertex;
                }

                if (!double_sided)
                    break;
            }
    case 2:
        if (tangent)
            for (int32_t i = 2; i; i--, facing = -1.0f) {
                for (int32_t j = indices_count; j; j--, indices++) {
                    CLOTHNode* node = &nodes[*indices];

                    *(vec3*)data = node->trans;

                    vec3i16 normal_int;
                    vec3_to_vec3i16(node->normal * 511.0f, normal_int);
                    *(uint32_t*)(data + 0x0C) = (((uint32_t)0 & 0x03) << 30)
                        | (((uint32_t)normal_int.z & 0x3FF) << 20)
                        | (((uint32_t)normal_int.y & 0x3FF) << 10)
                        | ((uint32_t)normal_int.x & 0x3FF);

                    vec4 tangent;
                    *(vec3*)&tangent = node->tangent;
                    tangent.w = node->tangent_sign;

                    vec4i16 tangent_int;
                    vec4_to_vec4i16(tangent * 511.0f, tangent_int);
                    *(uint32_t*)(data + 0x10) = (((uint32_t)clamp_def(tangent_int.w, -1, 1) & 0x03) << 30)
                        | (((uint32_t)tangent_int.z & 0x3FF) << 20)
                        | (((uint32_t)tangent_int.y & 0x3FF) << 10)
                        | ((uint32_t)tangent_int.x & 0x3FF);

                    data += mesh->size_vertex;
                }

                if (!double_sided)
                    break;
            }
        else
            for (int32_t i = 2; i; i--, facing = -1.0f) {
                for (int32_t j = indices_count; j; j--, indices++) {
                    CLOTHNode* node = &nodes[*indices];

                    *(vec3*)data = node->trans;

                    vec3i16 normal_int;
                    vec3_to_vec3i16(node->normal * 511.0f, normal_int);
                    *(uint32_t*)(data + 0x0C) = (((uint32_t)0 & 0x03) << 30)
                        | (((uint32_t)normal_int.z & 0x3FF) << 20)
                        | (((uint32_t)normal_int.y & 0x3FF) << 10)
                        | ((uint32_t)normal_int.x & 0x3FF);

                    data += mesh->size_vertex;
                }

                if (!double_sided)
                    break;
            }
        break;
    }

    buffer.UnmapMemory();
}


HOOK(void, FASTCALL, sub_1405044B0, 0x00000001405044B0, rob_chara* rob_chr) {
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr->chara_id];
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr->chara_id];
    if (arm_adjust.duration > 0) {
        float_t blend = 1.0f;
        if (fabsf(arm_adjust.duration) > 0.000001f)
            blend = (rob_chr->data.motion.frame_data.frame
                - (float_t)arm_adjust.start_frame) / arm_adjust.duration;

        blend = clamp_def(blend, 0.0f, 1.0f);
        if (fabsf(blend - 1.0f) <= 0.000001f)
            arm_adjust.duration = -1.0f;
        arm_adjust.scale = lerp_def(arm_adjust.prev_value, arm_adjust.next_value, blend);

        static float_t(FASTCALL * chara_size_table_get_value)(uint32_t id)
            = (float_t(FASTCALL*)(uint32_t id))0x00000001405103C0;

        float_t default_scale = chara_size_table_get_value(1);
        item_adjust.scale = default_scale + (rob_chr->data.adjust_data.scale
            - default_scale) * arm_adjust.scale;
    }

    originalsub_1405044B0(rob_chr);
}

static void rob_chara_data_adjuct_set_trans(rob_chara_adjust_data* rob_chr_adj,
    rob_chara_item_adjust_x* rob_chr_itm_adj_x, vec3& trans, bool pos_adjust, vec3* global_trans) {
    float_t scale = rob_chr_adj->scale;
    float_t item_scale = rob_chr_itm_adj_x->scale;
    vec3 _offset = rob_chr_adj->offset;
    if (global_trans)
        _offset.y += global_trans->y;

    vec3 _trans = trans;
    vec3 _item_trans = trans;
    if (rob_chr_adj->height_adjust) {
        _trans.y += rob_chr_adj->pos_adjust_y;
        _item_trans.y += rob_chr_adj->pos_adjust_y;
    }
    else {
        vec3 temp = (_trans - _offset) * scale + _offset;
        vec3 arm_temp = (_item_trans - _offset) * item_scale + _offset;

        if (!rob_chr_adj->offset_x) {
            _trans.x = temp.x;
            _item_trans.x = arm_temp.x;
        }

        if (!rob_chr_adj->offset_y) {
            _trans.y = temp.y;
            _item_trans.y = arm_temp.y;
        }

        if (!rob_chr_adj->offset_z) {
            _trans.z = temp.z;
            _item_trans.z = arm_temp.z;
        }
    }

    if (pos_adjust) {
        _trans = rob_chr_adj->pos_adjust + _trans;
        _item_trans = rob_chr_adj->pos_adjust + _item_trans;
    }

    rob_chr_adj->trans = _trans - trans * scale;
    rob_chr_itm_adj_x->trans = _item_trans - trans * item_scale;
}

HOOK(void, FASTCALL, rob_chara_set_data_adjust_mat, 0x00000001405050D0,
    rob_chara* rob_chr, rob_chara_adjust_data* rob_chr_adj, bool pos_adjust) {
    mat4 mat = *rob_chara_bone_data_get_mats_mat((size_t)rob_chr->bone_data, ROB_BONE_N_HARA_CP);

    vec3 trans;
    mat4_transpose(&mat, &mat);
    mat4_get_translation(&mat, &trans);

    rob_chara_item_adjust_x* item_adjust = &rob_chara_item_adjust_x_array[rob_chr->chara_id];

    vec3* global_trans = 0;
    if (rob_chr_adj->get_global_trans) {
        static vec3* (FASTCALL * rob_chara_bone_data_get_global_trans)(size_t rob_bone_data)
            = (vec3 * (FASTCALL*)(size_t rob_bone_data))0x0000000140419360;

        global_trans = rob_chara_bone_data_get_global_trans((size_t)rob_chr->bone_data);
    }
    rob_chara_data_adjuct_set_trans(rob_chr_adj, item_adjust, trans, pos_adjust, global_trans);

    float_t scale = rob_chr_adj->scale;
    mat4_scale(scale, scale, scale, &rob_chr_adj->mat);
    mat4_set_translation(&rob_chr_adj->mat, &rob_chr_adj->trans);
    mat4_transpose(&rob_chr_adj->mat, &rob_chr_adj->mat);

    float_t item_scale = item_adjust->scale;
    mat4_scale(item_scale, item_scale, item_scale, &item_adjust->mat);
    mat4_set_translation(&item_adjust->mat, &item_adjust->trans);
    mat4_transpose(&item_adjust->mat, &item_adjust->mat);
}

HOOK(void, FASTCALL, rob_chara_reset_data, 0x0000000140507210, rob_chara* rob_chr, rob_chara_pv_data* pv_data) {
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr->chara_id];
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr->chara_id];

    item_adjust.reset();
    arm_adjust.reset();
    originalrob_chara_reset_data(rob_chr, pv_data);
    item_adjust.scale = rob_chr->data.adjust_data.scale;
}

static void sub_140512C20(rob_chara_item_equip* rob_itm_equip) {
    if (rob_itm_equip->field_D0.is_null() || rob_itm_equip->field_D4 == -1)
        return;

    mat4 mat = mat4_identity;
    const char* name;
    if (rob_itm_equip->field_DC == 1) {
        name = "kl_te_r_wj";
        mat4_translate(0.0f, 0.0f, 0.082f, &mat);
        mat4_mul_rotate_zyx(&mat, (float_t)(-90.9 * DEG_TO_RAD), 0.0f, (float_t)(-179.5 * DEG_TO_RAD), &mat);
    }
    if (rob_itm_equip->field_DC == 2) {
        name = "kl_te_l_wj";
        mat4_translate(0.0f, 0.0015f, -0.0812f, &mat);
        mat4_mul_rotate_zyx(&mat, (float_t)(-34.5 * DEG_TO_RAD), 0.0f, (float_t)(-179.5 * DEG_TO_RAD), &mat);
    }
    else
        name = "j_1_hyoutan_000wj";

    bone_node* node = rob_itm_equip->item_equip_object[rob_itm_equip->field_D4].get_bone_node(name);
    if (!node || !node->mat)
        return;

    mat4_transpose(&mat, &mat);
    mat4_mul(&mat, node->mat, &mat);
    int32_t tex_pat_count = (int32_t)rob_itm_equip->texture_pattern.size();
    if (tex_pat_count)
        disp_manager->set_texture_pattern(tex_pat_count, rob_itm_equip->texture_pattern.data());
    disp_manager->entry_obj_by_object_info(&mat, rob_itm_equip->field_D0);
    if (tex_pat_count)
        disp_manager->set_texture_pattern();
}

static float_t sub_140512F60(rob_chara_item_equip* rob_itm_equip) {
    mat4* mats = rob_itm_equip->matrices;
    float_t y = 1.0f;
    y = min_def(y, mats[  0].row1.w);
    y = min_def(y, mats[175].row1.w);
    y = min_def(y, mats[181].row1.w);
    y = min_def(y, mats[105].row1.w);
    y = min_def(y, mats[140].row1.w);
    return y;
}

HOOK(void, FASTCALL, rob_chara_item_equip_disp, 0x0000000140512950,
    rob_chara_item_equip* rob_item_equip, int32_t chara_id) {

    mdl::ObjFlags flags = (mdl::ObjFlags)0;
    if (chara_reflect)
        enum_or(flags, mdl::OBJ_CHARA_REFLECT);
    if (chara_refract)
        enum_or(flags, mdl::OBJ_REFRACT);

    mdl::DispManager& disp_manager = *::disp_manager;
    if (rob_item_equip->shadow_type != -1) {
        if (rob_item_equip->field_A0 & 0x04) {
            vec3 pos = rob_item_equip->position;
            pos.y -= 0.2f;
            shadow_ptr_get()->field_1D0[rob_item_equip->shadow_type].push_back(pos);

            float_t v9;
            if (sub_140512F60(rob_item_equip) <= -0.2f)
                v9 = -0.5f;
            else
                v9 = 0.05f;
            shadow_ptr_get()->field_1C0[rob_item_equip->shadow_type] = v9;
            disp_manager.set_shadow_type(rob_item_equip->shadow_type);
            enum_or(flags, mdl::OBJ_SHADOW);
        }

        if (rob_item_equip->field_A0 & 0x01)
            enum_or(flags, mdl::OBJ_4);
    }
    disp_manager.set_obj_flags(flags);
    disp_manager.set_chara_color(rob_item_equip->chara_color);

    vec4 temp_texture_color_coeff;
    disp_manager.get_texture_color_coeff(temp_texture_color_coeff);

    disp_manager.set_texture_color_coefficients(rob_item_equip->texture_color_coefficients);
    disp_manager.set_wet_param(rob_item_equip->wet);
    render_manager->field_31C |= rob_item_equip->npr_flag;
    sub_140512C20(rob_item_equip);
    rob_chara_age_age_array_disp(chara_id, chara_reflect, rob_item_equip->chara_color);

    if (rob_item_equip->item_equip_range)
        for (int32_t i = rob_item_equip->first_item_equip_object; i < rob_item_equip->max_item_equip_object; i++)
            rob_item_equip->item_equip_object[i].disp(&rob_item_equip->mat);
    else {
        for (int32_t i = ITEM_ATAMA; i < ITEM_MAX; i++) {
            mdl::ObjFlags chara_flags = (mdl::ObjFlags)0;
            if (!rob_item_equip->field_18[i]) {
                if (chara_reflect)
                    enum_or(chara_flags, mdl::OBJ_CHARA_REFLECT);
                if (chara_refract)
                    enum_or(chara_flags, mdl::OBJ_REFRACT);
            }

            mdl::ObjFlags flags = (mdl::ObjFlags)(mdl::OBJ_4 | mdl::OBJ_SHADOW);
            if (i == ITEM_HARA)
                flags = (mdl::ObjFlags)0;

            if (!(rob_item_equip->field_A0 & 0x04))
                enum_and(flags, ~mdl::OBJ_SHADOW);

            disp_manager.set_obj_flags((mdl::ObjFlags)(chara_flags | flags | mdl::OBJ_SSS));
            rob_item_equip->item_equip_object[i].disp(&rob_item_equip->mat);
        }
    }
    disp_manager.set_texture_color_coefficients(temp_texture_color_coeff);
    disp_manager.set_wet_param();
    disp_manager.set_chara_color();
    disp_manager.set_obj_flags();
    disp_manager.set_shadow_type();
}

HOOK(void, FASTCALL, rob_chara_set_chara_size, 0x0000000140516810, rob_chara* rob_chr, float_t value) {
    originalrob_chara_set_chara_size(rob_chr, value);
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr->chara_id];
    item_adjust.scale = value;
}

HOOK(void, FASTCALL, sub_140526FD0, 0x0000000140526FD0,
    rob_chara_item_cos_data* item_cos_data, int32_t item_no, item_table_item* item) {
    if (!(item->attr & 0x0C))
        return;

    if (!(item->attr & 0x08)) {
        std::vector<item_cos_texture_change_tex> tex_chg_vec;
        for (const item_table_item_data_tex& i : item->data.tex) {
            item_cos_texture_change_tex chg_tex;
            chg_tex.org = texture_manager_get_texture(i.org);
            chg_tex.chg = texture_manager_get_texture(i.chg);
            chg_tex.changed = false;
            tex_chg_vec.push_back(chg_tex);
        }
        item_cos_data->texture_change.insert({ item_no, tex_chg_vec });
        return;
    }
    else if (item->data.col.size() <= 0)
        return;

    std::vector<int32_t> chg_tex_ids;
    if (item->attr & 0x04)
        for (const item_table_item_data_tex& i : item->data.tex)
            chg_tex_ids.push_back(i.chg);
    else
        for (const item_table_item_data_col& i : item->data.col)
            chg_tex_ids.push_back(i.tex_id);

    std::vector<item_cos_texture_change_tex> tex_chg_vec;
    size_t index = 0;
    for (int32_t& i : chg_tex_ids) {
        size_t j = &i - chg_tex_ids.data();
        texture* tex = texture_manager_get_texture(i);
        if (!tex) {
            index++;
            continue;
        }

        bool changed = false;
        if (item->data.col[j].flag & 0x01) {
            tex = texture_create_copy_texture_apply_color_tone(
                texture_manager_get_copy_id(0x30), tex, &item->data.col[j].col_tone);
            changed = true;
        }

        item_cos_texture_change_tex chg_tex;
        chg_tex.org = texture_manager_get_texture(item->data.col[j].tex_id);
        chg_tex.chg = tex;
        chg_tex.changed = changed;
        tex_chg_vec.push_back(chg_tex);
    }
    item_cos_data->texture_change.insert({ item_no, tex_chg_vec });
}

HOOK(void, FASTCALL, sub_1405335C0, 0x0000001405335C0, struc_223* a1) {
    originalsub_1405335C0(a1);
    int32_t chara_id = ((rob_chara*)((size_t)a1 - 0x19C8))->chara_id;
    rob_chara_arm_adjust_x_array[chara_id].reset();
}

HOOK(void, FASTCALL, mothead_func_32, 0x0000000140533C00, struct mothead_func_data* func_data,
    void* data, const struct mothead_data* mhd_data, int64_t frame) {
    float_t v8 = (float_t)((int16_t*)data)[0];
    int32_t v5 = ((int32_t*)data)[1];
    float_t v9 = ((float_t*)data)[2];
    int32_t v10 = ((int32_t*)data)[3];

    if (v8 != (int16_t)0xFA0C || v10 != 0xD62721C5) { // X
        originalmothead_func_32(func_data, data, mhd_data, frame);
        return;
    }

    rob_chara_data* rob_chr_data = (rob_chara_data*)((size_t)func_data + 0x08);
    rob_chara* rob_chr = ((rob_chara*)((size_t)rob_chr_data - 0x440));

    float_t value = v9;
    float_t duration = (float_t)v5;
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr->chara_id];
    arm_adjust.next_value = value;
    arm_adjust.prev_value = arm_adjust.scale;
    arm_adjust.start_frame = (int32_t)frame;
    arm_adjust.duration = max_def(duration, 0.0f);
    return;
}

HOOK(bool, FASTCALL, sub_14053ACA0, 0x000000014053ACA0, rob_chara* rob_chr, int32_t hand) {
    if (hand >= 2 || !rob_chr->data.motion.hand_adjust[hand].enable)
        return false;

    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr->chara_id];
    rob_chara_item_adjust_x item_adjust_temp = item_adjust;
    float_t chara_scale = rob_chr->data.adjust_data.scale;
    float_t adjust_scale = rob_chr->data.motion.hand_adjust[hand].current_scale;
    item_adjust.scale = adjust_scale / chara_scale;
    originalsub_14053ACA0(rob_chr, hand);
    item_adjust = item_adjust_temp;
    return true;
}

HOOK(void, FASTCALL, rob_chara_set_hand_adjust, 0x000000014053C070, rob_chara* rob_chr,
    rob_chara_data_hand_adjust* adjust, rob_chara_data_hand_adjust* prev_adjust) {
    if (!adjust->enable)
        return;

    static float_t(FASTCALL * chara_size_table_get_value)(uint32_t id)
        = (float_t(FASTCALL*)(uint32_t id))0x00000001405103C0;
    static float_t(FASTCALL * rob_chara_array_get_data_adjust_scale)(size_t rob_chr_smth, int32_t chara_id)
        = (float_t(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x0000000140531F90;

    float_t chara_scale = rob_chr->data.adjust_data.scale;
    float_t opposite_chara_scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), rob_chr->chara_id ? 0 : 1);
    bool chara_opposite_chara_same = fabsf(chara_scale - opposite_chara_scale) <= 0.000001f;

    switch (adjust->type) {
    case ROB_CHARA_DATA_HAND_ADJUST_NORMAL:
        adjust->scale = chara_size_table_get_value(1);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_SHORT:
        adjust->scale = chara_size_table_get_value(2);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_TALL:
        adjust->scale = chara_size_table_get_value(0);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_MIN:
        adjust->scale = min_def(opposite_chara_scale, chara_scale);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_MAX:
        adjust->scale = max_def(opposite_chara_scale, chara_scale);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_OPPOSITE_CHARA:
        adjust->scale = opposite_chara_scale;
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_1P:
        adjust->scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), 0);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_2P:
        adjust->scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), 1);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_3P:
        adjust->scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), 2);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_4P:
        adjust->scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), 3);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_ITEM: // X
        adjust->scale = rob_chara_item_adjust_x_array[rob_chr->chara_id].scale;
        break;
    }

    if (adjust->scale_select == 1 && adjust->enable_scale && chara_opposite_chara_same) {
        adjust->scale_select = 0;
        adjust->rotation_blend = prev_adjust->rotation_blend;
        adjust->disable_x = prev_adjust->disable_x;
        adjust->disable_y = prev_adjust->disable_y;
        adjust->disable_z = prev_adjust->disable_z;
        adjust->scale_blend = 1.0f;
    }

    float_t prev_scale;
    if (prev_adjust->enable || prev_adjust->scale_select == 1)
        prev_scale = prev_adjust->current_scale;
    else
        prev_scale = chara_scale;

    float_t scale;
    if (adjust->scale_select == 1)
        scale = adjust->scale;
    else
        scale = chara_scale;

    if (fabsf(prev_scale - scale * adjust->scale_blend) <= 0.000001f || adjust->duration <= adjust->current_time
        || fabsf(adjust->duration - adjust->current_time) <= 0.000001f) {
        adjust->current_scale = lerp_def(prev_scale, scale, adjust->scale_blend);
        if (fabsf(adjust->current_scale - chara_scale) <= 0.000001f
            && adjust->type != ROB_CHARA_DATA_HAND_ADJUST_ITEM) {
            adjust->current_scale = chara_scale;
            adjust->enable = false;
        }
    }
    else {
        float_t t = (adjust->current_time + 1.0f) / (adjust->duration + 1.0f);
        adjust->current_scale = lerp_def(prev_scale, scale, t * adjust->scale_blend);
        adjust->current_time += rob_chr->data.motion.step_data.frame;
    }
}

void rob_patch() {
    INSTALL_HOOK(RobCloth__UpdateVertexBuffer);
    INSTALL_HOOK(rob_chara_set_data_adjust_mat);
    INSTALL_HOOK(rob_chara_reset_data);
    INSTALL_HOOK(sub_1405044B0);
    INSTALL_HOOK(rob_chara_item_equip_disp);
    INSTALL_HOOK(rob_chara_set_chara_size);
    INSTALL_HOOK(sub_140526FD0);
    INSTALL_HOOK(sub_1405335C0);
    INSTALL_HOOK(mothead_func_32);
    INSTALL_HOOK(sub_14053ACA0);
    INSTALL_HOOK(rob_chara_set_hand_adjust);
}

void rob_chara_age_age_object::disp(size_t chara_index,
    bool npr, bool reflect, const vec3& a5, bool chara_color) {
    int32_t disp_count = this->disp_count;
    if (!objset_info || !disp_count)
        return;

    disp_count = min_def(disp_count, 10);

    std::pair<float_t, int32_t> v44[10];
    for (int32_t i = 0; i < disp_count; i++) {
        v44[i].first = vec3::dot(trans[i], a5);
        v44[i].second = i;
    }

    if (disp_count >= 2)
        std::sort(v44, v44 + disp_count,
            [](const std::pair<float_t, int32_t>& a, const std::pair<float_t, int32_t>& b) {
                return a.first < b.first;
            });

    obj_vert_buf.cycle_index();

    GLArrayBuffer buffer = obj_vert_buf.get_buffer();
    size_t vtx_data = (size_t)buffer.MapMemory();
    if (!vtx_data)
        return;

    size_t vertex_array_size = this->vertex_array_size;
    for (int32_t i = 0; i < disp_count; i++)
        memmove((void*)(vtx_data + vertex_array_size * i),
            (void*)((size_t)vertex_data + vertex_array_size * v44[i].second), vertex_array_size);

    buffer.UnmapMemory();

    mesh.num_vertex = disp_count * num_vertex;
    sub_mesh.num_index = disp_count * num_index;
    sub_mesh.material_index = npr ? 1 : 0;

    mdl::ObjFlags flags = (mdl::ObjFlags)(mdl::OBJ_SSS | mdl::OBJ_4 | mdl::OBJ_SHADOW);
    if (reflect)
        enum_or(flags, mdl::OBJ_CHARA_REFLECT);
    disp_manager->set_obj_flags(flags);
    disp_manager->set_chara_color(chara_color);
    disp_manager->set_shadow_type(chara_index ? SHADOW_STAGE : SHADOW_CHARA);

    static prj::vector<GLuint>* (FASTCALL * rob_chara_age_age_object__get_obj_set_texture)(rob_chara_age_age_object * rob_age_age_obj)
        = (prj::vector<GLuint>*(FASTCALL*)(rob_chara_age_age_object * rob_age_age_obj))0x000000014045A8E0;
    disp_manager->entry_obj_by_obj(&mat4_identity, &obj,
        rob_chara_age_age_object__get_obj_set_texture(this),
        &obj_vert_buf, &obj_index_buf, 0, 1.0f);
}

void rob_chara_age_age::disp(size_t chara_id,
    bool npr, bool reflect, const vec3& a5, bool chara_color) {
    if (alpha >= 0.1f && this->visible)
        object.disp(chara_id, npr || this->npr, reflect, a5, chara_color);
}

rob_chara_item_adjust_x::rob_chara_item_adjust_x() : scale() {
    reset();
}

void rob_chara_item_adjust_x::reset() {
    mat4_translate(&trans, &mat);
    mat4_transpose(&mat, &mat);
    scale = 1.0f;
}

rob_chara_arm_adjust_x::rob_chara_arm_adjust_x() : next_value(),
prev_value(), start_frame(), duration(), scale() {
    reset();
}

void rob_chara_arm_adjust_x::reset() {
    next_value = 0.0f;
    prev_value = 0.0f;
    start_frame = -1;
    duration = -1.0f;
    scale = 1.0f;
}

static void rob_chara_age_age_disp(rob_chara_age_age* arr,
    int32_t chara_id, bool reflect, bool chara_color) {
    bool npr = !!render_manager->npr_param;
    mat4& view = camera_data->view;
    vec3 v11 = *(vec3*)&view.row2;
    arr[chara_id * 3 + 0].disp(chara_id, npr, reflect, v11, chara_color);
    arr[chara_id * 3 + 1].disp(chara_id, npr, reflect, v11, chara_color);
    arr[chara_id * 3 + 2].disp(chara_id, npr, reflect, v11, chara_color);
}

static void rob_chara_age_age_array_disp(int32_t chara_id, bool reflect, bool chara_color) {
    mdl::ObjFlags obj_flags = disp_manager->get_obj_flags();
    rob_chara_age_age_disp(rob_chara_age_age_array, chara_id, reflect, chara_color);
    disp_manager->set_obj_flags(obj_flags);
}
