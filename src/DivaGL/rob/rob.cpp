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

enum ExNodeType {
    EX_NONE       = 0x00,
    EX_OSAGE      = 0x01,
    EX_EXPRESSION = 0x02,
    EX_CONSTRAINT = 0x03,
    EX_CLOTH      = 0x04,
};

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

struct bone_node_expression_data {
    vec3 position;
    vec3 rotation;
    vec3 scale;
    vec3 parent_scale;
};

static_assert(sizeof(bone_node_expression_data) == 0x30, "\"bone_node_expression_data\" struct should have a size of 0x30");

struct bone_node {
    const char* name;
    mat4* mat;
    bone_node* parent;
    bone_node_expression_data exp_data;
    mat4* ex_data_mat;
};

static_assert(sizeof(bone_node) == 0x50, "\"bone_node\" struct should have a size of 0x50");

struct opd_blend_data {
    int32_t motion_id;
    float_t frame;
    float_t frame_count;
    bool use_blend;
    int32_t type; // MotionBlendType
    float_t blend;
};

static_assert(sizeof(opd_blend_data) == 0x18, "\"opd_blend_data\" struct should have a size of 0x18");

struct ExNodeBlock;

struct ExNodeBlock_vtbl {
    ExNodeBlock* (FASTCALL* Dispose)(ExNodeBlock* node, uint8_t);
    void(FASTCALL* Field8)(ExNodeBlock* node);
    void(FASTCALL* Field10)(ExNodeBlock* node);
    void(FASTCALL* Field18)(ExNodeBlock* node, int32_t stage, bool disable_external_force);
    void(FASTCALL* Field20)(ExNodeBlock* node);
    void(FASTCALL* SetOsagePlayData)(ExNodeBlock* node);
    void(FASTCALL* Disp)(ExNodeBlock* node);
    void(FASTCALL* Reset)(ExNodeBlock* node);
    void(FASTCALL* Field40)(ExNodeBlock* node);
    void(FASTCALL* Field48)(ExNodeBlock* node);
    void(FASTCALL* Field50)(ExNodeBlock* node);
    void(FASTCALL* Field58)(ExNodeBlock* node);
};

struct rob_chara_item_equip_object;

struct ExNodeBlock {
    ExNodeBlock_vtbl* __vftable;
    bone_node* bone_node_ptr;
    ExNodeType type;
    const char* name;
    bone_node* parent_bone_node;
    prj::string parent_name;
    ExNodeBlock* parent_node;
    rob_chara_item_equip_object* item_equip_object;
    bool field_58;
    bool field_59;
    bool has_children_node;
};

struct rob_chara_item_equip;

struct rob_chara_item_equip_object {
    size_t index;
    mat4* mats;
    object_info obj_info;
    int32_t field_14;
    prj::vector<texture_pattern_struct> texture_pattern;
    texture_data_struct texture_data;
    bool null_blocks_data_set;
    bone_node_expression_data exp_data;
    float_t alpha;
    mdl::ObjFlags obj_flags;
    bool can_disp;
    int32_t field_A4;
    mat4* mat;
    int32_t osage_iterations;
    bone_node* bone_nodes;
    prj::vector<ExNodeBlock*> node_blocks;
    prj::vector<bone_node> ex_data_bone_nodes;
    prj::vector<mat4> ex_data_bone_mats;
    prj::vector<mat4> ex_data_mats;
    prj::vector<std::pair<const char*, uint32_t>> ex_bones;
    int64_t field_138;
    prj::vector<void*> null_blocks; // ExNullBlock
    prj::vector<void*> osage_blocks; // ExOsageBlock
    prj::vector<void*> constraint_blocks; // ExConstraintBlock
    prj::vector<void*> expression_blocks; // ExExpressionBlock
    prj::vector<void*> cloth_blocks; // ExClothBlock
    bool field_1B8;
    size_t osage_nodes_count;
    bool use_opd;
    obj_skin_ex_data* skin_ex_data;
    obj_skin* skin;
    rob_chara_item_equip* item_equip;

    void disp(const mat4* mat);
    int32_t get_bone_index(const char* name);
    bone_node* get_bone_node(int32_t bone_index);
    bone_node* get_bone_node(const char* name);
};

static_assert(sizeof(rob_chara_item_equip_object) == 0x1E8, "\"rob_chara_item_equip_object\" struct should have a size of 0x1E8");

struct rob_chara_item_equip {
    bone_node* bone_nodes;
    mat4* matrices;
    rob_chara_item_equip_object* item_equip_object;
    int32_t field_18[31];
    bool item_equip_range;
    item_id first_item_equip_object;
    item_id max_item_equip_object;
    int32_t field_A0;
    shadow_type_enum shadow_type;
    vec3 position;
    prj::vector<texture_pattern_struct> texture_pattern;
    object_info field_D0;
    item_id field_D4;
    bool disable_update;
    int32_t field_DC;
    vec4 texture_color_coefficients;
    float_t wet;
    float_t wind_strength;
    bool chara_color;
    bool npr_flag;
    mat4 mat;
    mat4 field_13C[30];
    int32_t field_8BC;
    int32_t field_8C0;
    int32_t field_8C4;
    int32_t field_8C8;
    int32_t field_8CC;
    int32_t field_8D0;
    int32_t field_8D4;
    int32_t field_8D8;
    int32_t field_8DC;
    int32_t field_8E0;
    int32_t field_8E4;
    int32_t field_8E8;
    int32_t field_8EC;
    int32_t field_8F0;
    int32_t field_8F4;
    int32_t field_8F8;
    int32_t field_8FC;
    int64_t field_900;
    int64_t field_908;
    int64_t field_910;
    int64_t field_918;
    int64_t field_920;
    int64_t field_928;
    int64_t field_930;
    float_t osage_step;
    bool use_opd;
    prj::vector<opd_blend_data> opd_blend_data;
    bool parts_short;
    bool parts_append;
    bool parts_white_one_l;
};

static_assert(sizeof(rob_chara_item_equip) == 0x960, "\"rob_chara_item_equip\" struct should have a size of 0x960");

mat4* (FASTCALL* rob_chara_bone_data_get_mats_mat)(size_t rob_bone_data, size_t index)
    = (mat4 * (FASTCALL*)(size_t rob_bone_data, size_t index))0x0000000140419520;
void(FASTCALL* sub_1405163C0)(size_t rob_chr, int32_t index, mat4* mat)
    = (void(FASTCALL*)(size_t rob_chr, int32_t index, mat4 * mat))0x00000001405163C0;
float_t(FASTCALL* rob_chara_get_max_face_depth)(size_t rob_chr)
    = (float_t(FASTCALL*)(size_t rob_chr))0x0000000140516510;
mat4* (FASTCALL* rob_chara_get_bone_data_mat)(size_t rob_chr, mot_bone_index index)
    = (mat4 * (FASTCALL*)(size_t rob_chr, mot_bone_index index))0x0000000140516730;
mat4* (FASTCALL* sub_140516740)(size_t rob_chr)
    = (mat4 * (FASTCALL*)(size_t rob_chr))0x0000000140516740;
mat4* (FASTCALL* rob_chara_get_adjust_data_mat)(size_t rob_chr)
    = (mat4 * (FASTCALL*)(size_t rob_chr))0x0000000140516750;
bool(FASTCALL* rob_chara_array_check_visibility)(size_t rob_chr_smth, int32_t chara_id)
    = (bool(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x0000000140531F50;
size_t(FASTCALL* get_rob_chara_smth)() = (size_t(FASTCALL*)())0x00000001405320E0;
size_t(FASTCALL* rob_chara_array_get)(size_t rob_chr_smth, int32_t chara_id)
    = (size_t(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x0000000140532030;
size_t(FASTCALL* rob_chara_array_get_bone_data)(size_t rob_chr_smth, int32_t chara_id)
    = (size_t(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001405320F0;
bool (FASTCALL* rob_chara_pv_data_array_check_chara_id)(size_t rob_chr_smth, int32_t chara_id)
    = (bool (FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001405327B0;

static rob_chara_age_age* rob_chara_age_age_array = (rob_chara_age_age*)0x00000001411E83C0;

static bool& chara_reflect = *(bool*)0x00000001411ADAFC;
static bool& chara_refract = *(bool*)0x00000001411ADAFD;

const mat4* rob_chara_item_equip_mat = 0;

static void rob_chara_age_age_array_disp(int32_t chara_id, bool reflect, bool chara_color);

void rob_chara_age_age::disp(size_t chara_id,
    bool npr, bool reflect, const vec3& a5, bool chara_color) {
    if (alpha >= 0.1f && this->visible)
        object.disp(chara_id, npr || this->npr, reflect, a5, chara_color);
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
    int32_t (FASTCALL * sub_1401F13B0)(int32_t a1, const char* a2)
        = (int32_t (FASTCALL*)(int32_t a1, const char* a2))0x00000001401F13B0;

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
    mat4_mul(node->mat, &mat, &mat);
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

    static prj::vector<GLuint>*(FASTCALL * rob_chara_age_age_object__get_obj_set_texture)(rob_chara_age_age_object * rob_age_age_obj)
        = (prj::vector<GLuint>*(FASTCALL*)(rob_chara_age_age_object* rob_age_age_obj))0x000000014045A8E0;
    disp_manager->entry_obj_by_obj(&mat4_identity, &obj,
        rob_chara_age_age_object__get_obj_set_texture(this),
        &obj_vert_buf, &obj_index_buf, 0, 1.0f);
}

void rob_patch() {
    INSTALL_HOOK(rob_chara_item_equip_disp);
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
