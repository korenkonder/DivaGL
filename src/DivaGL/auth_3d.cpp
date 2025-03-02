/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "auth_3d.hpp"
#include "../KKdLib/hash.hpp"
#include "../AFTModsShared/light_param/fog.hpp"
#include "../AFTModsShared/light_param/light.hpp"
#include "../AFTModsShared/canonical_properties.hpp"
#include "../AFTModsShared/file_handler.hpp"
#include "../AFTModsShared/frame_rate_control.hpp"
#include "../AFTModsShared/types.hpp"
#include "mdl/disp_manager.hpp"
#include "pv_game/firstread.hpp"
#include "rob/rob.hpp"
#include "render_context.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include <Helpers.h>

enum auth_3d_material_list_flags {
    AUTH_3D_MATERIAL_LIST_BLEND_COLOR = 0x01,
    AUTH_3D_MATERIAL_LIST_EMISSION    = 0x02,
};

struct auth_3d_material_list {
    auth_3d_material_list_flags flags;
    auth_3d_rgba blend_color;
    auth_3d_rgba emission;
    uint64_t hash;
};

struct struc_8 {
    float_t max_frame;
    void* data;
    size_t data_size;
    bool field_18;
    CanonicalProperties CanProp;
    int32_t field_60;
    int32_t field_64;
    int64_t field_68;
    bool field_70;
    void* binary_data;
    size_t binary_size;
    bool field_88;
};

static_assert(sizeof(struc_8) == 0x90, "\"struc_8\" struct should have a size of 0x90");

static void(FASTCALL* auth_3d_rgba__free)(auth_3d_rgba* rgba)
= (void(FASTCALL*)(auth_3d_rgba * rgba))0x00000001401B8900;
static void (FASTCALL* auth_3d_rgba__ctrl)(auth_3d_rgba* rgba, float_t frame)
    = (void (FASTCALL*)(auth_3d_rgba * rgba, float_t frame))0x00000001401D4070;
static bool (FASTCALL* auth_3d_key__parse)(auth_3d_key* k, struc_8* a2, prj::string_range_capacity str_rng_cap)
= (bool (FASTCALL*)(auth_3d_key * k, struc_8 * a2, prj::string_range_capacity str_rng_cap))0x00000001401DB1A0;
static bool (FASTCALL* auth_3d_rgba__parse)(auth_3d_rgba* rgba, struc_8* a2, prj::string_range_capacity str_rng_cap)
    = (bool (FASTCALL*)(auth_3d_rgba * rgba, struc_8 * a2, prj::string_range_capacity str_rng_cap))0x00000001401DB520;

static void auth_3d_material_list_ctrl(auth_3d_material_list* ml, float_t frame);
static bool auth_3d_material_list_parse(auth_3d_material_list* ml, struc_8* a2, prj::string_range_capacity str_rng_cap);
static void auth_3d_set_material_list(auth_3d* auth);

HOOK(void, FASTCALL, auth_3d_parse, 0x00000001401C15B0, auth_3d* auth) {
    const int32_t state_before = auth->state;
    originalauth_3d_parse(auth);
    const int32_t state_after = auth->state;
    extern bool reflect_full;
    if (!reflect_full || state_before != 1 || state_after != 2)
        return;

    extern const firstread* firstread_ptr;
    if (!firstread_ptr || !firstread_ptr->auth_3d_array)
        return;

    const firstread_auth_3d_array* auth_3d_array = firstread_ptr->auth_3d_array;

    for (uint32_t i = 0; i < auth_3d_array->num_auth_3d; i++) {
        if (auth_3d_array->auth_3d_array[i].uid != auth->uid)
            continue;

        const firstread_auth_3d* frg_auth_3d = &auth_3d_array->auth_3d_array[i];

        auto obj_begin = auth->object.begin();
        auto obj_end = auth->object.end();
        const uint32_t num_object = frg_auth_3d->num_object;
        for (uint32_t j = 0; j < num_object; j++) {
            const uint64_t name_hash = hash_utf8_xxh3_64bits(frg_auth_3d->object_array[j].name);
            for (auth_3d_object& k : auth->object) {
                if (name_hash != hash_utf8_xxh3_64bits(k.name.c_str()))
                    continue;

                const firstread_auth_3d_object* frg_a3d_obj = &frg_auth_3d->object_array[j];
                k.uid_name.assign(frg_a3d_obj->uid_name);
                k.object_info = object_database_get_object_info(frg_a3d_obj->uid_name);

                k.reflect = k.uid_name.find("_REFLECT") != -1;
                k.refract = k.uid_name.find("_REFRACT") != -1;
                break;
            }
        }

        auto obj_list_begin = auth->object_list.begin();
        auto obj_list_end = auth->object_list.end();
        const uint32_t num_object_list = frg_auth_3d->num_object_list;
        for (uint32_t j = 0; j < num_object_list; j++) {
            const uint64_t name_hash = hash_utf8_xxh3_64bits(frg_auth_3d->object_list_array[j].name);
            for (auto obj_list = obj_list_begin; obj_list != obj_list_end; )
                if (name_hash == hash_utf8_xxh3_64bits((*obj_list)->name.c_str())) {
                    auth->object_list.erase(obj_list);
                    obj_list_end = auth->object_list.end();
                    break;
                }
                else
                    obj_list++;
        }
        break;
    }
}

HOOK(void, FASTCALL, auth_3d_curve_array_free, 0x00000001401C2E30,
    prj::vector<auth_3d_curve>* vec, auth_3d_curve* begin, auth_3d_curve* end) {
    while (begin != end) {
        if (begin->name.capacity() >= 0x10)
            _operator_delete(((uint64_t**)&begin->name)[0]);
        ((uint64_t*)&begin->name)[3] = 15;
        ((uint64_t*)&begin->name)[2] = 0;
        ((char*)&begin->name)[0] = 0;

        if (begin->type == 1) {
            auth_3d_material_list* ml = (auth_3d_material_list*)begin->curve.keys;
            auth_3d_rgba__free(&ml->blend_color);
            auth_3d_rgba__free(&ml->emission);
            _operator_delete(ml);
        }
        else {
            if (((kft3**)&begin->curve.keys_vec)[0]) {
                _operator_delete(((kft3**)&begin->curve.keys_vec)[0]);
                ((kft3**)&begin->curve.keys_vec)[0] = 0;
                ((kft3**)&begin->curve.keys_vec)[1] = 0;
                ((kft3**)&begin->curve.keys_vec)[2] = 0;
            }
        }
        begin++;
    }
}

HOOK(void, FASTCALL, auth_3d_m_object_hrc_disp, 0x00000001401D0760, auth_3d_m_object_hrc* moh, auth_3d* auth) {
    if (!auth->visible || !moh->model_transform.visible)
        return;

    mdl::DispManager& disp_manager = *::disp_manager;

    for (auth_3d_object_instance& i : moh->instance) {
        if (!i.model_transform.visible)
            continue;

        mdl::ObjFlags flags = mdl::OBJ_SSS;
        shadow_type_enum shadow_type = SHADOW_CHARA;
        if (auth->shadow || i.shadow) {
            enum_or(flags, mdl::OBJ_4 | mdl::OBJ_SHADOW);
            shadow_type = SHADOW_STAGE;
        }
        if (auth->alpha < 1.0f)
            enum_or(flags, auth->obj_flags);

        disp_manager.set_obj_flags(flags);
        disp_manager.set_shadow_type(shadow_type);

        Shadow* shad = shadow_ptr_get();
        if (shad && (flags & mdl::OBJ_SHADOW)) {
            disp_manager.set_shadow_type(SHADOW_STAGE);

            mat4* m = &moh->model_transform.mat;
            for (auth_3d_object_node& j : moh->node)
                if (j.mat) {
                    m = j.mat;
                    break;
                }

            mat4 mat = i.model_transform.mat;
            mat4_mul(m, &mat, &mat);
            mat4_transpose(&mat, &mat);

            vec3 pos = *(vec3*)&mat.row3;
            pos.y -= 0.2f;
            shad->field_1D0[shadow_type].push_back(pos);
        }

        if (i.mats.size())
            disp_manager.entry_obj_by_object_info_object_skin(
                i.object_info, 0, 0, auth->alpha, i.mats.data(), 0, 0, i.model_transform.mat);
    }

    disp_manager.set_obj_flags();
    disp_manager.set_shadow_type(SHADOW_CHARA);
}

HOOK(void, FASTCALL, auth_3d_object_hrc_disp, 0x00000001401D04A0, auth_3d_object_hrc* oh, auth_3d* auth) {
    if (!auth->visible || !oh->node.front().model_transform.visible)
        return;

    mdl::DispManager& disp_manager = *::disp_manager;

    mdl::ObjFlags flags = mdl::OBJ_SSS;
    if (auth->shadow | oh->shadow)
        enum_or(flags, mdl::OBJ_4 | mdl::OBJ_SHADOW);
    if (oh->reflect)
        enum_or(flags, mdl::OBJ_NO_TRANSLUCENCY | mdl::OBJ_REFLECT);
    if (oh->refract)
        enum_or(flags, mdl::OBJ_NO_TRANSLUCENCY | mdl::OBJ_REFRACT);
    if (auth->alpha < 1.0f)
        enum_or(flags, auth->obj_flags);

    disp_manager.set_obj_flags(flags);
    disp_manager.set_shadow_type(SHADOW_CHARA);

    mat4 mat = mat4_identity;
    if (auth->chara_id >= 0 && auth->chara_id < ROB_CHARA_COUNT) {
        size_t rob_chara_smth = get_rob_chara_smth();
        if (rob_chara_pv_data_array_check_chara_id(rob_chara_smth, auth->chara_id)) {
            rob_chara* rob_chr = rob_chara_array_get(rob_chara_smth, auth->chara_id);
            mat4_mul(auth->chara_item
                ? rob_chara_get_item_adjust_data_mat(rob_chr)
                : rob_chara_get_adjust_data_mat(rob_chr),
                sub_140516740(rob_chr), &mat);
            if (auth->chara_id)
                disp_manager.set_shadow_type(SHADOW_STAGE);
        }
    }
    else if (flags & mdl::OBJ_SHADOW) {
        disp_manager.set_shadow_type(SHADOW_STAGE);

        mat4 mat = oh->node.front().model_transform.mat;
        for (auth_3d_object_node& i : oh->node)
            if (i.mat) {
                mat = *i.mat;
                break;
            }
        mat4_transpose(&mat, &mat);

        Shadow* shad = shadow_ptr_get();
        if (shad) {
            vec3 pos;
            mat4_get_translation(&mat, &pos);
            pos.y -= 0.2f;
            shad->field_1D0[SHADOW_STAGE].push_back(pos);
        }
    }

    if (oh->node_bone_mats.size())
        disp_manager.entry_obj_by_object_info_object_skin(
            oh->object_info, 0, 0, auth->alpha, oh->node_bone_mats.data(), 0, 0, mat);

    disp_manager.set_obj_flags();
    disp_manager.set_shadow_type(SHADOW_CHARA);

    extern void FASTCALL implOfauth_3d_object_disp(auth_3d_object * o, auth_3d * auth);

    for (auth_3d_object*& i : oh->children_object)
        implOfauth_3d_object_disp(i, auth);

    for (auth_3d_object_hrc*& i : oh->children_object_hrc)
        implOfauth_3d_object_hrc_disp(i, auth);
}

HOOK(void, FASTCALL, auth_3d_object_disp, 0x00000001401D0970, auth_3d_object* o, auth_3d* auth) {
    if (!o->model_transform.visible)
        return;

    if (!auth->visible) {
        if (auth->pos)
            spr::put_rgb_cross(o->model_transform.mat);

        for (auth_3d_object*& i : o->children_object)
            implOfauth_3d_object_disp(i, auth);
        for (auth_3d_object_hrc*& i : o->children_object_hrc)
            implOfauth_3d_object_hrc_disp(i, auth);
        return;
    }

    mat4 mat = o->model_transform.mat;

    mdl::DispManager& disp_manager = *::disp_manager;

    if (auth->chara_id >= 0 && auth->chara_id < ROB_CHARA_COUNT) {
        size_t rob_chara_smth = get_rob_chara_smth();
        if (rob_chara_pv_data_array_check_chara_id(rob_chara_smth, auth->chara_id)) {
            rob_chara* rob_chr = rob_chara_array_get(rob_chara_smth, auth->chara_id);
            mat4 m;
            mat4_mul(auth->chara_item
                ? rob_chara_get_item_adjust_data_mat(rob_chr)
                : rob_chara_get_adjust_data_mat(rob_chr),
                sub_140516740(rob_chr), &m);
            mat4_mul(&mat, &m, &mat);
            disp_manager.set_shadow_type(auth->chara_id ? SHADOW_STAGE : SHADOW_CHARA);
        }
    }

    mdl::ObjFlags flags = (mdl::ObjFlags)0;
    if (auth->shadow)
        enum_or(flags, mdl::OBJ_4 | mdl::OBJ_SHADOW);
    if (o->reflect)
        enum_or(flags, mdl::OBJ_NO_TRANSLUCENCY | mdl::OBJ_REFLECT);
    if (o->refract)
        enum_or(flags, mdl::OBJ_NO_TRANSLUCENCY | mdl::OBJ_REFRACT);

    disp_manager.set_obj_flags(flags);

    char buf[0x80];
    int32_t tex_pat_count = 0;
    texture_pattern_struct tex_pat[TEXTURE_PATTERN_COUNT];

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

    if (tex_pat_count)
        disp_manager.set_texture_pattern(tex_pat_count, tex_pat);

    int32_t tex_trans_count = 0;
    texture_transform_struct tex_trans[TEXTURE_TRANSFORM_COUNT];

    for (auth_3d_object_texture_transform& i : o->texture_transform) {
        if (tex_trans_count >= TEXTURE_TRANSFORM_COUNT || i.texture_id == -1
            || !texture_manager_get_texture(i.texture_id))
            continue;

        tex_trans[tex_trans_count].id = i.texture_id;
        tex_trans[tex_trans_count].mat = i.mat;
        tex_trans_count++;
    }

    if (tex_trans_count)
        disp_manager.set_texture_transform(tex_trans_count, tex_trans);

    const char* uid_name = o->uid_name.c_str();
    int32_t uid_name_length = (int32_t)o->uid_name.size();

    if (uid_name_length <= 3)
        disp_manager.entry_obj_by_object_info(mat, o->object_info);
    else if (o->morph.curve) {
        float_t morph = o->morph.value;
        int32_t morph_int = (int32_t)morph;
        morph = fmodf(morph, 1.0f);
        if (morph > 0.0f && morph < 1.0f) {
            sprintf_s(buf, sizeof(buf), "%.*s%03d", uid_name_length - 3, uid_name, morph_int + 1);
            object_info morph_obj_info = object_database_get_object_info(buf);
            if (morph_obj_info.is_null())
                morph_obj_info = o->object_info;
            disp_manager.set_morph(morph_obj_info, morph);

            sprintf_s(buf, sizeof(buf), "%.*s%03d", uid_name_length - 3, uid_name, morph_int);
            object_info obj_info = object_database_get_object_info(buf);
            if (auth->alpha < 0.999f)
                disp_manager.entry_obj_by_object_info(mat, obj_info, auth->alpha);
            else
                disp_manager.entry_obj_by_object_info(mat, obj_info);
            disp_manager.set_morph({}, 0.0f);
        }
        else {
            if (morph >= 1.0f)
                morph_int++;

            sprintf_s(buf, sizeof(buf), "%.*s%03d", uid_name_length - 3, uid_name, morph_int);
            object_info obj_info = object_database_get_object_info(buf);
            if (obj_info.is_null())
                obj_info = o->object_info;
            disp_manager.entry_obj_by_object_info(mat, obj_info);
        }
    }
    else if (o->pattern.curve) {
        sprintf_s(buf, sizeof(buf), "%.*s%03d",
            uid_name_length - 3, uid_name, (int32_t)prj::roundf(o->pattern.value));
        object_info obj_info = object_database_get_object_info(buf);
        disp_manager.entry_obj_by_object_info(mat, obj_info);
    }
    else
        disp_manager.entry_obj_by_object_info(mat, o->object_info);

    disp_manager.set_texture_transform();
    disp_manager.set_texture_pattern();
    disp_manager.set_obj_flags();

    if (auth->pos)
        spr::put_rgb_cross(o->model_transform.mat);

    for (auth_3d_object*& i : o->children_object)
        implOfauth_3d_object_disp(i, auth);
    for (auth_3d_object_hrc*& i : o->children_object_hrc)
        implOfauth_3d_object_hrc_disp(i, auth);
}

HOOK(void, FASTCALL, auth_3d__disp, 0x00000001401D1230, auth_3d* auth) {
    if (auth->state != 2 || !auth->enable || auth->bone_mats)
        return;

    auth_3d_set_material_list(auth);
    extern bool reflect_full;
    mdl::obj_reflect_enable = reflect_full && auth->reflect;
    originalauth_3d__disp(auth);
    mdl::obj_reflect_enable = false;
    disp_manager->set_material_list();
}

HOOK(void, FASTCALL, auth_3d_curve__ctrl, 0x00000001401D4140, auth_3d_curve* c, float_t frame) {
    if (c->type == 1)
        auth_3d_material_list_ctrl((auth_3d_material_list*)c->curve.keys, frame);
    else
        originalauth_3d_curve__ctrl(c, frame);
}

HOOK(bool, FASTCALL, auth_3d_curve_parse, 0x00000001401DA230,
    auth_3d_curve* c, struc_8* a2, prj::string_range_capacity str_rng_cap) {
    prj::string_range_capacity name = prj::string_range_capacity(str_rng_cap, ".name");
    c->name.assign(a2->CanProp.GetValueString(name.range));

    prj::string_range_capacity ml_str = prj::string_range_capacity(str_rng_cap, ".ml");
    if (a2->CanProp.GetValue(ml_str.range)) {
        auth_3d_material_list* ml = (auth_3d_material_list*)_operator_new(sizeof(auth_3d_material_list));
        memset(ml, 0, sizeof(auth_3d_material_list));
        auth_3d_material_list_parse(ml, a2, ml_str);
        ml->hash = hash_fnv1a64m(c->name.data(), c->name.size());
        c->curve.keys = (kft3*)ml;
        c->type = 1;
    }
    else {
        prj::string_range_capacity cv = prj::string_range_capacity(str_rng_cap, ".cv");
        auth_3d_key__parse(&c->curve, a2, cv);
        c->type = 0;
    }
    return true;
}

void auth_3d_patch() {
    // Move both auth_3d_curve's value and type
    WRITE_MEMORY_STRING(0x00000001401A3DBE, "\x48\x8B"
        "\x43\x60\x48\x83\xC3\x70\x48\x83\xC7\x70\x48\x89\x47\xF8\x48\x8D"
        "\x43\xF8\x48\x3B\xC6\x75\x8B\x48\x8B\x5C\x24\x30\x48\x8B\xC7\x48"
        "\x8B\x74\x24\x38\x48\x83\xC4\x20\x5F\xC3", 0x2C);
    // Null both auth_3d_curve's value and type
    WRITE_MEMORY_STRING(0x00000001401A72FF, "\x4C\x89\x40\x38", 0x04);
    // Move both auth_3d_curve's value and type
    WRITE_MEMORY_STRING(0x00000001401B316A, "\x48\x8B\x43\x68\x48\x89"
        "\x47\x68\x48\x8B\xC7\x48\x8B\x5C\x24\x48\x48\x83\xC4\x30\x5F\xC3", 0x16);

    // Null auth_3d's shadow, chara_item and reflect
    WRITE_MEMORY_STRING(0x00000001401D7A78, "\x44\x89\xBF\x98\x00\x00\x00", 0x07);

    INSTALL_HOOK(auth_3d_parse);
    INSTALL_HOOK(auth_3d_curve_array_free);
    INSTALL_HOOK(auth_3d_m_object_hrc_disp);
    INSTALL_HOOK(auth_3d_object_hrc_disp);
    INSTALL_HOOK(auth_3d_object_disp);
    INSTALL_HOOK(auth_3d__disp);
    INSTALL_HOOK(auth_3d_curve__ctrl);
    INSTALL_HOOK(auth_3d_curve_parse);
}

static void auth_3d_material_list_ctrl(auth_3d_material_list* ml, float_t frame) {
    auth_3d_rgba__ctrl(&ml->blend_color, frame);
    auth_3d_rgba__ctrl(&ml->emission, frame);
}

static bool auth_3d_material_list_parse(auth_3d_material_list* ml, struc_8* a2, prj::string_range_capacity str_rng_cap) {
    ml->flags = (auth_3d_material_list_flags)0;
    prj::string_range_capacity blend_color = prj::string_range_capacity(str_rng_cap, ".blend_color");
    if (a2->CanProp.GetValue(blend_color.range)) {
        auth_3d_rgba__parse(&ml->blend_color, a2, blend_color);
        enum_or(ml->flags, AUTH_3D_MATERIAL_LIST_BLEND_COLOR);
    }

    prj::string_range_capacity emission = prj::string_range_capacity(str_rng_cap, ".emission");
    if (a2->CanProp.GetValue(emission.range)) {
        auth_3d_rgba__parse(&ml->emission, a2, emission);
        enum_or(ml->flags, AUTH_3D_MATERIAL_LIST_EMISSION);
    }
    return true;
}

static void auth_3d_set_material_list(auth_3d* auth) {
    int32_t mat_list_count = 0;
    material_list_struct mat_list[TEXTURE_PATTERN_COUNT];
    for (auth_3d_curve& i : auth->curve) {
        if (i.type != 1)
            continue;

        auth_3d_material_list* ml = (auth_3d_material_list*)i.curve.keys;
        if (!ml->flags)
            continue;

        if (ml->flags & AUTH_3D_MATERIAL_LIST_BLEND_COLOR) {
            vec4& blend_color = mat_list[mat_list_count].blend_color;
            vec4u8& has_blend_color = mat_list[mat_list_count].has_blend_color;

            blend_color = ml->blend_color.value;
            has_blend_color.x = ml->blend_color.has_got[0];
            has_blend_color.y = ml->blend_color.has_got[1];
            has_blend_color.z = ml->blend_color.has_got[2];
            has_blend_color.w = ml->blend_color.has_got[3];
        }
        else {
            mat_list[mat_list_count].blend_color = {};
            mat_list[mat_list_count].has_blend_color = {};
        }

        if (ml->flags & AUTH_3D_MATERIAL_LIST_EMISSION) {
            vec4& emission = mat_list[mat_list_count].emission;
            vec4u8& has_emission = mat_list[mat_list_count].has_emission;

            emission = ml->emission.value;
            has_emission.x = ml->emission.has_got[0];
            has_emission.y = ml->emission.has_got[1];
            has_emission.z = ml->emission.has_got[2];
            has_emission.w = ml->emission.has_got[3];
        }
        else {
            mat_list[mat_list_count].emission = {};
            mat_list[mat_list_count].has_emission = {};
        }

        mat_list[mat_list_count].hash = ml->hash;
        mat_list_count++;
    }

    disp_manager->set_material_list(mat_list_count, mat_list);
}
