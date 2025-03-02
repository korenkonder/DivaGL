/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob.hpp"
#include "../../KKdLib/str_utils.hpp"
#include "../mdl/disp_manager.hpp"
#include "../gl_state.hpp"
#include "../object.hpp"
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
    obj_mesh_vertex_buffer obj_vert_buf;
    obj_mesh_index_buffer obj_index_buf;
    vec3 pos[10];
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
    vec3 pos;
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

struct mothead_func_data {
    rob_chara* rob_chr;
    rob_chara_data* rob_chr_data;
    rob_chara_data* field_10;
    rob_chara* field_18;
    rob_chara_data* field_20;
    struc_223* field_28;
};

static_assert(sizeof(mothead_func_data) == 0x30, "\"mothead_func_data\" struct should have a size of 0x30");

struct rob_chara_item_adjust_x {
    mat4 mat;
    vec3 pos;
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

static rob_chara_age_age* rob_chara_age_age_array = (rob_chara_age_age*)0x00000001411E83C0;

static bool& chara_reflect = *(bool*)0x00000001411ADAFC;
static bool& chara_refract = *(bool*)0x00000001411ADAFD;

const mat4* rob_chara_item_equip_mat = 0;

rob_chara_item_adjust_x rob_chara_item_adjust_x_array[ROB_CHARA_COUNT];
rob_chara_arm_adjust_x rob_chara_arm_adjust_x_array[ROB_CHARA_COUNT];

static void rob_chara_age_age_array_disp(int32_t chara_id, bool reflect, bool chara_color);

const mat4* rob_chara_get_item_adjust_data_mat(rob_chara* rob_chr) {
    return &rob_chara_item_adjust_x_array[rob_chr - rob_chara_array].mat;
}

HOOK(void, FASTCALL, RobCloth__UpdateVertexBuffer, 0x000000014021CF00, obj_mesh* mesh, obj_mesh_vertex_buffer* vertex_buffer,
    CLOTHNode* nodes, float_t facing, uint16_t* indices, bool double_sided) {
    if (!mesh || !vertex_buffer || (mesh->vertex_format
        & (OBJ_VERTEX_NORMAL | OBJ_VERTEX_POSITION)) != (OBJ_VERTEX_NORMAL | OBJ_VERTEX_POSITION))
        return;

    vertex_buffer->cycle_index();
    GL::ArrayBuffer buffer = vertex_buffer->get_buffer();
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

                    *(vec3*)data = node->pos;
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

                    *(vec3*)data = node->pos;
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

                    *(vec3*)data = node->pos;

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

                    *(vec3*)data = node->pos;

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

                    *(vec3*)data = node->pos;

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

                    *(vec3*)data = node->pos;

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
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr - rob_chara_array];
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

static void rob_chara_data_adjuct_set_pos(rob_chara_adjust_data* rob_chr_adj,
    rob_chara_item_adjust_x* rob_chr_itm_adj_x, const vec3& pos, bool pos_adjust, const vec3* global_position) {
    float_t scale = rob_chr_adj->scale;
    float_t item_scale = rob_chr_itm_adj_x->scale;
    vec3 _offset = rob_chr_adj->offset;
    if (global_position)
        _offset.y += global_position->y;

    vec3 _pos = pos;
    vec3 _item_pos = pos;
    if (rob_chr_adj->height_adjust) {
        _pos.y += rob_chr_adj->pos_adjust_y;
        _item_pos.y += rob_chr_adj->pos_adjust_y;
    }
    else {
        vec3 temp = (_pos - _offset) * scale + _offset;
        vec3 arm_temp = (_item_pos - _offset) * item_scale + _offset;

        if (!rob_chr_adj->offset_x) {
            _pos.x = temp.x;
            _item_pos.x = arm_temp.x;
        }

        if (!rob_chr_adj->offset_y) {
            _pos.y = temp.y;
            _item_pos.y = arm_temp.y;
        }

        if (!rob_chr_adj->offset_z) {
            _pos.z = temp.z;
            _item_pos.z = arm_temp.z;
        }
    }

    if (pos_adjust) {
        _pos = rob_chr_adj->pos_adjust + _pos;
        _item_pos = rob_chr_adj->pos_adjust + _item_pos;
    }

    rob_chr_adj->pos = _pos - pos * scale;
    rob_chr_itm_adj_x->pos = _item_pos - pos * item_scale;
}

HOOK(void, FASTCALL, rob_chara_set_data_adjust_mat, 0x00000001405050D0,
    rob_chara* rob_chr, rob_chara_adjust_data* rob_chr_adj, bool pos_adjust) {
    mat4 mat = *rob_chara_bone_data_get_mats_mat((size_t)rob_chr->bone_data, ROB_BONE_N_HARA_CP);

    vec3 pos;
    mat4_transpose(&mat, &mat);
    mat4_get_translation(&mat, &pos);

    rob_chara_item_adjust_x* item_adjust = &rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];

    vec3* global_position = 0;
    if (rob_chr_adj->get_global_position) {
        static vec3* (FASTCALL * rob_chara_bone_data_get_global_position)(size_t rob_bone_data)
            = (vec3 * (FASTCALL*)(size_t rob_bone_data))0x0000000140419360;

        global_position = rob_chara_bone_data_get_global_position((size_t)rob_chr->bone_data);
    }
    rob_chara_data_adjuct_set_pos(rob_chr_adj, item_adjust, pos, pos_adjust, global_position);

    float_t scale = rob_chr_adj->scale;
    mat4_scale(scale, scale, scale, &rob_chr_adj->mat);
    mat4_set_translation(&rob_chr_adj->mat, &rob_chr_adj->pos);
    mat4_transpose(&rob_chr_adj->mat, &rob_chr_adj->mat);

    float_t item_scale = item_adjust->scale;
    mat4_scale(item_scale, item_scale, item_scale, &item_adjust->mat);
    mat4_set_translation(&item_adjust->mat, &item_adjust->pos);
    mat4_transpose(&item_adjust->mat, &item_adjust->mat);
}

HOOK(void, FASTCALL, rob_chara_reset_data, 0x0000000140507210, rob_chara* rob_chr, rob_chara_pv_data* pv_data) {
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr - rob_chara_array];

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
    disp_manager->entry_obj_by_object_info(mat, rob_itm_equip->field_D0);
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
    extern bool reflect_full;
    mdl::obj_reflect_enable = reflect_full;

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
    render_manager.field_31C |= rob_item_equip->npr_flag;
    sub_140512C20(rob_item_equip);
    rob_chara_age_age_array_disp(chara_id, chara_reflect, rob_item_equip->chara_color);

    static void (FASTCALL * rob_chara_item_equip_object_disp)(rob_chara_item_equip_object* rob_bone_data)
        = (void (FASTCALL*)(rob_chara_item_equip_object* rob_bone_data))0x00000001405F2700;

    if (rob_item_equip->item_equip_range)
        for (int32_t i = rob_item_equip->first_item_equip_object; i < rob_item_equip->max_item_equip_object; i++)
            rob_chara_item_equip_object_disp(&rob_item_equip->item_equip_object[i]);
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
            rob_chara_item_equip_object_disp(&rob_item_equip->item_equip_object[i]);
        }
    }
    disp_manager.set_texture_color_coefficients(temp_texture_color_coeff);
    disp_manager.set_wet_param();
    disp_manager.set_chara_color();
    disp_manager.set_obj_flags();
    disp_manager.set_shadow_type();

    mdl::obj_reflect_enable = false;
}

HOOK(void, FASTCALL, rob_chara_set_chara_size, 0x0000000140516810, rob_chara* rob_chr, float_t value) {
    originalrob_chara_set_chara_size(rob_chr, value);
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];
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

HOOK(void, FASTCALL, rob_chara_item_equip_object_disp, 0x00000001405F2700, rob_chara_item_equip_object* itm_eq_obj) {
    rob_chara_item_equip_mat = &itm_eq_obj->item_equip->mat;
    originalrob_chara_item_equip_object_disp(itm_eq_obj);
    rob_chara_item_equip_mat = 0;
}

HOOK(void, FASTCALL, sub_1405335C0, 0x0000001405335C0, struc_223* a1) {
    originalsub_1405335C0(a1);
    rob_chara* rob_chr = (rob_chara*)((size_t)a1 - 0x19C8);
    rob_chara_arm_adjust_x_array[rob_chr - rob_chara_array].reset();
}

HOOK(void, FASTCALL, mothead_func_32, 0x0000000140533C00, mothead_func_data* func_data,
    void* data, const struct mothead_data* mhd_data, int64_t frame) {
    float_t v8 = (float_t)((int16_t*)data)[0];
    int32_t v5 = ((int32_t*)data)[1];
    float_t v9 = ((float_t*)data)[2];
    int32_t v10 = ((int32_t*)data)[3];

    if (v8 != (int16_t)0xFA0C || v10 != 0xD62721C5) { // X
        originalmothead_func_32(func_data, data, mhd_data, frame);
        return;
    }

    rob_chara* rob_chr = func_data->rob_chr;

    float_t value = v9;
    float_t duration = (float_t)v5;
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr - rob_chara_array];
    arm_adjust.next_value = value;
    arm_adjust.prev_value = arm_adjust.scale;
    arm_adjust.start_frame = (int32_t)frame;
    arm_adjust.duration = max_def(duration, 0.0f);
    return;
}

HOOK(bool, FASTCALL, sub_14053ACA0, 0x000000014053ACA0, rob_chara* rob_chr, int32_t hand) {
    if (hand >= 2 || !rob_chr->data.motion.hand_adjust[hand].enable)
        return false;

    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];
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
    float_t opposite_chara_scale = rob_chara_array_get_data_adjust_scale(
        get_rob_chara_smth(), rob_chr - rob_chara_array ? 0 : 1);
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
        adjust->scale = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array].scale;
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
    INSTALL_HOOK(rob_chara_item_equip_object_disp);
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
        v44[i].first = vec3::dot(pos[i], a5);
        v44[i].second = i;
    }

    if (disp_count >= 2)
        std::sort(v44, v44 + disp_count,
            [](const std::pair<float_t, int32_t>& a, const std::pair<float_t, int32_t>& b) {
                return a.first < b.first;
            });

    obj_vert_buf.cycle_index();

    GL::ArrayBuffer buffer = obj_vert_buf.get_buffer();
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
    disp_manager->entry_obj_by_obj(mat4_identity, &obj,
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
    mat4_translate(&pos, &mat);
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
    bool npr = !!render_manager.npr_param;
    mat4& view = camera_data.view;
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
