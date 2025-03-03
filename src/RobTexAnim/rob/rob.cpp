/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob.hpp"
#include "../../AFTModsShared/auth_3d.hpp"
#include <Helpers.h>

#define TEXTURE_PATTERN_COUNT 24
#define TEXTURE_TRANSFORM_COUNT 24

static void(FASTCALL* item_table_handler_array_get_item_objset)(::chara_index chara_index,
    int32_t item_no, prj::list<int32_t>*& objset)
    = (void(FASTCALL*)(::chara_index chara_index,
        int32_t item_no, prj::list<int32_t>*& objset))0x000000014052C270;

HOOK(void, FASTCALL, TaskRobLoad__UnloadCharaItem, 0x0000000140525560,
    ::chara_index chara_index, int32_t item_no) {
    if (!item_no)
        return;

    prj::list<int32_t>* item_objset = 0;
    item_table_handler_array_get_item_objset(chara_index, item_no, item_objset);
    if (!item_objset)
        return;

    for (const int32_t& i : *item_objset)
        if (i != -1) {
            objset_info_storage_unload_set(i);
            auth_3d_data_unload_category(objset_info_storage_get_set_name(i));
        }
}

HOOK(void, FASTCALL, TaskRobLoad__LoadCharaItem, 0x000000014052B0F0,
    ::chara_index chara_index, int32_t item_no) {
    if (!item_no)
        return;

    prj::list<int32_t>* item_objset = 0;
    item_table_handler_array_get_item_objset(chara_index, item_no, item_objset);
    if (!item_objset)
        return;

    for (const int32_t& i : *item_objset)
        if (i != -1) {
            objset_info_storage_load_set(i);
            auth_3d_data_load_category(objset_info_storage_get_set_name(i));
        }
}

HOOK(bool, FASTCALL, TaskRobLoad__LoadCharaItemCheckNotRead, 0x000000014052D4C0,
    ::chara_index chara_index, int32_t item_no) {
    if (!item_no)
        return false;

    prj::list<int32_t>* item_objset = 0;
    item_table_handler_array_get_item_objset(chara_index, item_no, item_objset);
    if (!item_objset)
        return true;

    for (const int32_t& i : *item_objset)
        if (i != -1 && objset_info_storage_load_obj_set_check_not_read(i)) {
            if (auth_3d_data_check_category_loaded(objset_info_storage_get_set_name(i)))
                return true;
        }
    return false;
}

HOOK(rob_chara_item_equip_object*, FASTCALL, rob_chara_item_equip_object__rob_chara_item_equip_object,
    0x00000001405ED890, rob_chara_item_equip_object* This) {
    This->auth_obj_index = -1;
    This->auth_3d_id = -1;
    return originalrob_chara_item_equip_object__rob_chara_item_equip_object(This);
}

HOOK(void, FASTCALL, rob_chara_item_equip_object_ctrl, 0x00000001405F2310, rob_chara_item_equip_object* itm_eq_obj) {
    originalrob_chara_item_equip_object_ctrl(itm_eq_obj);

    if (itm_eq_obj->auth_3d_id != -1 && itm_eq_obj->auth_obj_index == -1
        && (*(auth_3d_id*)&itm_eq_obj->auth_3d_id).check_loaded()) {
        const char* obj_name = object_database_get_obj_name(itm_eq_obj->obj_info);

        auth_3d* auth = (*(auth_3d_id*)&itm_eq_obj->auth_3d_id).get_auth_3d();
        for (auth_3d_object& i : auth->object)
            if (i.uid_name.compare(obj_name)) {
                itm_eq_obj->auth_obj_index = (int32_t)(&i - auth->object.data());
                break;
            }
    }
}

HOOK(void, FASTCALL, rob_chara_item_equip_object_disp, 0x00000001405F2700, rob_chara_item_equip_object* itm_eq_obj) {
    if (itm_eq_obj->obj_info.is_null())
        return;

    char buf[0x80];
    int32_t tex_pat_count = 0;
    texture_pattern_struct tex_pat[TEXTURE_PATTERN_COUNT];

    int32_t tex_trans_count = 0;
    texture_transform_struct tex_trans[TEXTURE_TRANSFORM_COUNT];

    static void(FASTCALL * disp_manager_set_texture_transform)(int32_t count, texture_transform_struct * data)
        = (void(FASTCALL*)(int32_t count, texture_transform_struct * data))0x000000014043A600;

    auth_3d_id& id = *(auth_3d_id*)&itm_eq_obj->auth_3d_id;
    if (id.check_not_empty()) {
        auth_3d* auth = id.get_auth_3d();
        if (itm_eq_obj->auth_obj_index >= 0 && itm_eq_obj->auth_obj_index < auth->object.size()) {
            auth_3d_object* o = &auth->object.data()[itm_eq_obj->auth_obj_index];

            for (auth_3d_object_texture_pattern& i : o->texture_pattern) {
                if (!i.pattern.curve || i.name.size() <= 3)
                    continue;
                else if (tex_pat_count >= TEXTURE_PATTERN_COUNT)
                    break;

                sprintf_s(buf, sizeof(buf), "%.*s%03d",
                    (int32_t)(i.name.size() - 3), i.name.c_str(), (int32_t)prj::roundf(i.pattern.value));

                tex_pat[tex_pat_count].src = texture_id(0x00, i.texture_id);
                tex_pat[tex_pat_count].dst = texture_id(0x00, texture_info_get_id(buf));
                tex_pat_count++;
            }

            for (auth_3d_object_texture_transform& i : o->texture_transform) {
                if (tex_trans_count >= TEXTURE_TRANSFORM_COUNT || i.texture_id == -1
                    || !texture_manager_get_texture(i.texture_id))
                    continue;

                tex_trans[tex_trans_count].id = i.texture_id;
                tex_trans[tex_trans_count].mat = i.mat;
                tex_trans_count++;
            }

            if (tex_trans_count > 0)
                disp_manager_set_texture_transform(tex_trans_count, tex_trans);
        }
    }

    prj::vector<texture_pattern_struct>& itm_tex_pat = itm_eq_obj->texture_pattern;

    if (tex_pat_count > 0)
        itm_tex_pat.insert(itm_tex_pat.end(), tex_pat, tex_pat + tex_pat_count);

    originalrob_chara_item_equip_object_disp(itm_eq_obj);

    if (tex_pat_count > 0)
        itm_tex_pat.resize(itm_tex_pat.size() - tex_pat_count);

    if (tex_trans_count > 0)
        disp_manager_set_texture_transform(0, 0);
}

HOOK(bool, FASTCALL, rob_chara_item_equip_object__init_members, 0x00000001405F36F0,
    rob_chara_item_equip_object* This, size_t index) {
    This->auth_obj_index = -1;
    (*(auth_3d_id*)&This->auth_3d_id).unload();
    return originalrob_chara_item_equip_object__init_members(This, index);
}

HOOK(bool, FASTCALL, rob_chara_item_equip_object_load_object_info_ex_data, 0x00000001405F6E30,
    rob_chara_item_equip_object* itm_eq_obj, uint32_t obj_info, bone_node* bone_nodes, bool osage_reset) {
    auth_3d_id id(objset_info_storage_get_set_name((*(object_info*)&obj_info).set_id));
    if (id.check_not_empty()) {
        id.read_file();
        id.set_enable(true);
        id.set_repeat(true);
        id.set_paused(false);
        id.set_visibility(false);

        itm_eq_obj->auth_obj_index = -1;
        (*(auth_3d_id*)&itm_eq_obj->auth_3d_id) = id;
    }
    else {
        itm_eq_obj->auth_obj_index = -1;
        itm_eq_obj->auth_3d_id = -1;
    }

    return originalrob_chara_item_equip_object_load_object_info_ex_data(itm_eq_obj, obj_info, bone_nodes, osage_reset);
}

void rob_patch() {
    INSTALL_HOOK(TaskRobLoad__UnloadCharaItem);
    INSTALL_HOOK(TaskRobLoad__LoadCharaItem);
    INSTALL_HOOK(TaskRobLoad__LoadCharaItemCheckNotRead);
    INSTALL_HOOK(rob_chara_item_equip_object__rob_chara_item_equip_object);
    INSTALL_HOOK(rob_chara_item_equip_object_ctrl);
    INSTALL_HOOK(rob_chara_item_equip_object_disp);
    INSTALL_HOOK(rob_chara_item_equip_object__init_members);
    INSTALL_HOOK(rob_chara_item_equip_object_load_object_info_ex_data);
}
