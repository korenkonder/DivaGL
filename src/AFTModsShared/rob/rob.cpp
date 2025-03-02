/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob.hpp"
#include "../../KKdLib/str_utils.hpp"

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

rob_chara* rob_chara_array = (rob_chara*)0x00000001411B76A0;

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
    void (FASTCALL * rob_chara_item_equip_object__skp_load)(rob_chara_item_equip_object * This, void* can_prop)
        = (void (FASTCALL*)(rob_chara_item_equip_object * This, void* can_prop))0x00000001405F43E0;
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
