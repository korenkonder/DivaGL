/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "auth_3d.hpp"
#include "light_param/fog.hpp"
#include "mdl/disp_manager.hpp"
#include "rob/rob.hpp"
#include "object.hpp"
#include "render_context.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include "types.hpp"
#include <Helpers.h>

enum auth_3d_camera_auxiliary_flags {
    AUTH_3D_CAMERA_AUXILIARY_EXPOSURE      = 0x01,
    AUTH_3D_CAMERA_AUXILIARY_GAMMA         = 0x02,
    AUTH_3D_CAMERA_AUXILIARY_SATURATE      = 0x04,
    AUTH_3D_CAMERA_AUXILIARY_AUTO_EXPOSURE = 0x08,
};

enum auth_3d_ep_type {
    AUTH_3D_EP_NONE         = 0x00,
    AUTH_3D_EP_LINEAR       = 0x01,
    AUTH_3D_EP_CYCLE        = 0x02,
    AUTH_3D_EP_CYCLE_OFFSET = 0x03,
};

enum auth_3d_fog_flags {
    AUTH_3D_FOG_DENSITY = 0x01,
    AUTH_3D_FOG_START   = 0x02,
    AUTH_3D_FOG_END     = 0x04,
    AUTH_3D_FOG_COLOR   = 0x08,
};

enum auth_3d_key_type {
    AUTH_3D_KEY_NONE    = 0x00,
    AUTH_3D_KEY_STATIC  = 0x01,
    AUTH_3D_KEY_LINEAR  = 0x02,
    AUTH_3D_KEY_HERMITE = 0x03,
    AUTH_3D_KEY_HOLD    = 0x04,
};

enum auth_3d_light_flags {
    AUTH_3D_LIGHT_AMBIENT    = 0x01,
    AUTH_3D_LIGHT_DIFFUSE    = 0x02,
    AUTH_3D_LIGHT_SPECULAR   = 0x04,
    AUTH_3D_LIGHT_TONE_CURVE = 0x08,
};

enum auth_3d_object_texture_transform_flags {
    AUTH_3D_OBJECT_TEX_TRANSFORM_COVERAGE_U        = 0x001,
    AUTH_3D_OBJECT_TEX_TRANSFORM_COVERAGE_V        = 0x002,
    AUTH_3D_OBJECT_TEX_TRANSFORM_REPEAT_U          = 0x004,
    AUTH_3D_OBJECT_TEX_TRANSFORM_REPEAT_V          = 0x008,
    AUTH_3D_OBJECT_TEX_TRANSFORM_TRANSLATE_FRAME_U = 0x010,
    AUTH_3D_OBJECT_TEX_TRANSFORM_TRANSLATE_FRAME_V = 0x020,
    AUTH_3D_OBJECT_TEX_TRANSFORM_OFFSET_U          = 0x040,
    AUTH_3D_OBJECT_TEX_TRANSFORM_OFFSET_V          = 0x080,
    AUTH_3D_OBJECT_TEX_TRANSFORM_ROTATE_FRAME      = 0x100,
    AUTH_3D_OBJECT_TEX_TRANSFORM_ROTATE            = 0x200,
};

enum auth_3d_post_process_flags {
    AUTH_3D_POST_PROCESS_LENS_FLARE = 0x01,
    AUTH_3D_POST_PROCESS_LENS_SHAFT = 0x02,
    AUTH_3D_POST_PROCESS_LENS_GHOST = 0x04,
    AUTH_3D_POST_PROCESS_RADIUS     = 0x08,
    AUTH_3D_POST_PROCESS_INTENSITY  = 0x10,
    AUTH_3D_POST_PROCESS_SCENE_FADE = 0x20,
};

struct kft3 {
    float_t frame;
    float_t value;
    float_t tangent1;
    float_t tangent2;
};

static_assert(sizeof(kft3) == 0x10, "\"kft3\" struct should have a size of 0x10");

struct auth_3d_key {
    auth_3d_key_type type;
    float_t value;
    float_t max_frame;
    auth_3d_ep_type ep_type_pre;
    auth_3d_ep_type ep_type_post;
    float_t frame_delta;
    float_t value_delta;
    prj::vector<kft3> keys_vec;
    size_t length;
    kft3* keys;
};

static_assert(sizeof(auth_3d_key) == 0x48, "\"auth_3d_key\" struct should have a size of 0x48");

struct auth_3d_vec3 {
    auth_3d_key x;
    auth_3d_key y;
    auth_3d_key z;
};

static_assert(sizeof(auth_3d_vec3) == 0xD8, "\"auth_3d_vec3\" struct should have a size of 0xD8");

struct auth_3d_rgba {
    auth_3d_key r;
    auth_3d_key g;
    auth_3d_key b;
    auth_3d_key a;
    bool has_r;
    bool has_g;
    bool has_b;
    bool has_a;
    int32_t field_124;
    vec4 value;
    bool has_got[4];
};

static_assert(sizeof(auth_3d_rgba) == 0x140, "\"auth_3d_rgba\" struct should have a size of 0x140");

struct auth_3d_model_transform {
    mat4 mat;
    auth_3d_vec3 scale;
    auth_3d_vec3 rotation;
    auth_3d_vec3 translation;
    auth_3d_key visibility;
    vec3 scale_value;
    vec3 rotation_value;
    vec3 translation_value;
    bool visible;
};

static_assert(sizeof(auth_3d_model_transform) == 0x338, "\"auth_3d_model_transform\" struct should have a size of 0x338");

struct auth_3d_curve {
    auth_3d_key curve;
    prj::string name;
    float_t value;
};

static_assert(sizeof(auth_3d_curve) == 0x70, "\"auth_3d_curve\" struct should have a size of 0x70");

struct auth_3d_object_curve {
    auth_3d_curve* curve;
    prj::string name;
    float_t frame_offset;
    float_t value;
};

static_assert(sizeof(auth_3d_object_curve) == 0x30, "\"auth_3d_object_curve\" struct should have a size of 0x30");

struct auth_3d_object_texture_pattern {
    prj::string name;
    int32_t texture_id;
    auth_3d_object_curve pattern;
};

static_assert(sizeof(auth_3d_object_texture_pattern) == 0x58, "\"auth_3d_object_texture_pattern\" struct should have a size of 0x58");

struct auth_3d_object_texture_transform {
    auth_3d_object_texture_transform_flags flags;
    prj::string name;
    int32_t texture_id;
    auth_3d_key coverageU;
    auth_3d_key coverageV;
    auth_3d_key repeatU;
    auth_3d_key repeatV;
    auth_3d_key translateFrameU;
    auth_3d_key translateFrameV;
    auth_3d_key offsetU;
    auth_3d_key offsetV;
    auth_3d_key rotateFrame;
    auth_3d_key rotate;
    mat4 mat;
    vec3 scale_value;
    vec3 repeat_value;
    vec3 rotate_value;
    vec3 translateFrame_value;
};

static_assert(sizeof(auth_3d_object_texture_transform) == 0x370, "\"auth_3d_object_texture_transform\" struct should have a size of 0x370");

struct auth_3d_object_hrc;

struct auth_3d_object {
    auth_3d_model_transform model_transform;
    int64_t field_338;
    prj::string name;
    prj::string uid_name;
    prj::string parent_name;
    prj::string parent_node;
    auth_3d_object_curve pattern;
    auth_3d_object_curve morph;
    prj::vector<auth_3d_object_texture_pattern> texture_pattern;
    prj::vector<auth_3d_object_texture_transform >texture_transform;
    prj::vector<auth_3d_object*> children_object;
    prj::vector<auth_3d_object_hrc*> children_object_hrc;
    object_info object_info;
    bool reflect;
    bool refract;
};

static_assert(sizeof(auth_3d_object) == 0x488, "\"auth_3d_object\" struct should have a size of 0x488");

struct auth_3d_object_model_transform {
    mat4 mat;
    mat4 mat_inner;
    auth_3d_vec3 translation;
    auth_3d_vec3 rotation;
    auth_3d_vec3 scale;
    auth_3d_key visibility;
    vec3 translation_value;
    vec3 rotation_value;
    vec3 scale_value;
    bool visible;
    mat4 mat_rot;
    int64_t field_3B8;
    int32_t field_3C0;
    mat4 field_3C4;
    vec3 field_404;
    float_t frame;
    bool has_rotation;
    bool has_translation;
    bool has_scale;
    bool has_visibility;
};

static_assert(sizeof(auth_3d_object_model_transform) == 0x418, "\"auth_3d_object_model_transform\" struct should have a size of 0x418");

struct auth_3d_object_node {
    prj::string name;
    int32_t bone_id;
    int32_t parent;
    auth_3d_object_model_transform model_transform;
    mat4* mat;
    mat4 joint_orient;
};

static_assert(sizeof(auth_3d_object_node) == 0x488, "\"auth_3d_object_node\" struct should have a size of 0x488");

struct auth_3d_object_hrc {
    prj::vector<auth_3d_object_node> node;
    prj::vector<mat4> node_bone_mats;
    mat4* bone_mats;
    prj::string name;
    prj::string uid_name;
    object_info object_info;
    bool reflect;
    bool refract;
    bool shadow;
    prj::string parent_name;
    prj::string parent_node;
    prj::vector<auth_3d_object*> children_object;
    prj::vector<int32_t> children_object_parent_node;
    prj::vector<auth_3d_object_hrc*> children_object_hrc;
    prj::vector<int32_t> children_object_hrc_parent_node;
};

static_assert(sizeof(auth_3d_object_hrc) == 0x120, "\"auth_3d_object_hrc\" struct should have a size of 0x120");

struct auth_3d_object_instance {
    prj::string name;
    prj::string uid_name;
    object_info object_info;
    bool shadow;
    auth_3d_object_model_transform model_transform;
    prj::vector<int32_t> object_bone_indices;
    prj::vector<mat4> mats;
};

static_assert(sizeof(auth_3d_object_instance) == 0x490, "\"auth_3d_object_instance\" struct should have a size of 0x490");

struct auth_3d_m_object_hrc {
    prj::string name;
    auth_3d_object_model_transform model_transform;
    prj::vector<auth_3d_object_node> node;
    prj::vector<auth_3d_object_instance> instance;
};

static_assert(sizeof(auth_3d_m_object_hrc) == 0x468, "\"auth_3d_m_object_hrc\" struct should have a size of 0x468");

struct auth_3d_time_event {
    float_t frame;
    int32_t type;
    size_t event_index;
};

static_assert(sizeof(auth_3d_time_event) == 0x10, "\"auth_3d_time_event\" struct should have a size of 0x10");

struct auth_3d_play_control {
    float_t begin;
    float_t size;
    float_t fps;
};

static_assert(sizeof(auth_3d_play_control) == 0x0C, "\"auth_3d_play_control\" struct should have a size of 0x0C");

struct auth_3d_point {
    prj::string name;
    auth_3d_model_transform model_transform;
};

static_assert(sizeof(auth_3d_point) == 0x358, "\"auth_3d_point\" struct should have a size of 0x358");

struct auth_3d_camera_root_view_point {
    auth_3d_model_transform model_transform;
    auth_3d_key roll;
    bool has_fov;
    bool fov_is_horizontal;
    auth_3d_key fov;
    float_t aspect;
    auth_3d_key focal_length;
    float_t camera_aperture_w;
    float_t camera_aperture_h;
};

static_assert(sizeof(auth_3d_camera_root_view_point) == 0x428, "\"auth_3d_camera_root_view_point\" struct should have a size of 0x428");

struct auth_3d_camera_root {
    auth_3d_model_transform model_transform;
    auth_3d_camera_root_view_point view_point;
    auth_3d_model_transform interest;
};

static_assert(sizeof(auth_3d_camera_root) == 0xA98, "\"auth_3d_camera_root\" struct should have a size of 0xA98");

struct auth_3d_camera_auxiliary {
    auth_3d_camera_auxiliary_flags flags;
    auth_3d_key exposure;
    auth_3d_key gamma;
    auth_3d_key saturate;
    auth_3d_key auto_exposure;
    float_t exposure_value;
    float_t gamma_value;
    float_t saturate_value;
    float_t auto_exposure_value;
};

static_assert(sizeof(auth_3d_camera_auxiliary) == 0x138, "\"auth_3d_camera_auxiliary\" struct should have a size of 0x138");

struct auth_3d_light {
    auth_3d_light_flags flags;
    auth_3d_light_flags flags_init;
    light_id id;
    prj::string name;
    prj::string type;
    auth_3d_model_transform position;
    auth_3d_model_transform spot_direction;
    auth_3d_rgba Ambient;
    auth_3d_rgba Diffuse;
    auth_3d_rgba Specular;
    auth_3d_rgba Tone_Curve;
    vec4 Ambient_init;
    vec4 Diffuse_init;
    vec4 Specular_init;
    vec4 Tone_Curve_init;
};

static_assert(sizeof(auth_3d_light) == 0xC00, "\"auth_3d_light\" struct should have a size of 0xC00");

struct auth_3d_fog {
    auth_3d_fog_flags flags;
    auth_3d_fog_flags flags_init;
    fog_id id;
    prj::string name;
    auth_3d_key density;
    auth_3d_key start;
    auth_3d_key end;
    float_t density_value;
    float_t start_value;
    float_t end_value;
    auth_3d_rgba color;
    float_t density_init;
    float_t start_init;
    float_t end_init;
    vec4 color_init;
};

static_assert(sizeof(auth_3d_fog) == 0x278, "\"auth_3d_fog\" struct should have a size of 0x278");

struct auth_3d_post_process {
    auth_3d_post_process_flags flags;
    auth_3d_post_process_flags flags_init;
    auth_3d_key lens_flare;
    auth_3d_key lens_shaft;
    auth_3d_key lens_ghost;
    auth_3d_rgba radius;
    auth_3d_rgba intensity;
    auth_3d_rgba scene_fade;
    vec3 lens_init;
    vec4 radius_init_value;
    vec4 intensity_init_value;
    vec4 scene_fade_value;
    vec3 lens_value;
};

static_assert(sizeof(auth_3d_post_process) == 0x4E8, "\"auth_3d_post_process\" struct should have a size of 0x4E8");

struct auth_3d_dof {
    bool has_dof;
    auth_3d_model_transform model_transform;
};

static_assert(sizeof(auth_3d_dof) == 0x340, "\"auth_3d_dof\" struct should have a size of 0x340");

struct auth_3d_chara {
    prj::string name;
    auth_3d_model_transform model_transform;
    int32_t index;
};

static_assert(sizeof(auth_3d_chara) == 0x360, "\"auth_3d_chara\" struct should have a size of 0x360");

struct auth_3d_motion {
    prj::string name;
};

static_assert(sizeof(auth_3d_motion) == 0x20, "\"auth_3d_motion\" struct should have a size of 0x20");

struct auth_3d_auth_2d {
    prj::string name;
};

static_assert(sizeof(auth_3d_auth_2d) == 0x20, "\"auth_3d_auth_2d\" struct should have a size of 0x20");

struct auth_3d {
    int32_t uid;
    int32_t id;
    bool enable;
    bool camera_root_update;
    bool visible;
    bool repeat;
    bool field_C;
    bool ended;
    bool left_right_reverse;
    bool once;
    mat4 mat;
    float_t alpha;
    mdl::ObjFlags obj_flags;
    bool field_58;
    prj::map<uint32_t, uint32_t> field_60;
    prj::vector<texture_pattern_struct> texture_pattern;
    int32_t chara_id;
    object_info object_info;
    mat4* bone_mats;
    bool shadow;
    chara_index src_chara;
    chara_index dst_chara;
    int32_t pos;
    int64_t field_A8;
    void* frame_rate; // FrameRateControl
    float_t frame;
    float_t req_frame;
    float_t max_frame;
    bool frame_changed;
    float_t frame_offset;
    float_t last_frame;
    float_t fov_scale;
    prj::vector<auth_3d_object_hrc*>* field_D8;
    float_t blend;
    int64_t field_E8;
    int64_t field_F0;
    bool paused;
    prj::vector<struct auth_3d_event*> event;
    prj::vector<auth_3d_time_event> event_time;
    auth_3d_time_event* event_time_next;
    auth_3d_play_control play_control;
    prj::vector<auth_3d_point> point;
    prj::vector<auth_3d_curve> curve;
    prj::vector<auth_3d_camera_root> camera_root;
    auth_3d_camera_auxiliary camera_auxiliary;
    prj::vector<auth_3d_light> light;
    prj::vector<auth_3d_fog> fog;
    auth_3d_post_process post_process;
    auth_3d_dof dof;
    prj::vector<auth_3d_chara> chara;
    prj::vector<auth_3d_motion> motion;
    prj::vector<auth_3d_object> object;
    prj::vector<auth_3d_object*> object_list;
    prj::vector<auth_3d_object_hrc> object_hrc;
    prj::vector<auth_3d_object_hrc*> object_hrc_list;
    prj::vector<auth_3d_m_object_hrc> m_object_hrc;
    prj::vector<auth_3d_m_object_hrc*> m_object_hrc_list;
    prj::vector<std::string> field_BE0;
    prj::vector<std::string> object_list_string;
    prj::vector<std::string> object_hrc_list_string;
    prj::vector<std::string> m_object_hrc_list_string;
    prj::vector<auth_3d_auth_2d> auth_2d;
    int32_t state;
    prj::vector<struct auth_3d_event_adapter*> field_C60;
};

static_assert(sizeof(auth_3d) == 0xC78, "\"auth_3d\" struct should have a size of 0xC78");

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
        if (shad && flags & mdl::OBJ_SHADOW) {
            disp_manager.set_shadow_type(SHADOW_STAGE);

            mat4* m = &moh->model_transform.mat;
            for (auth_3d_object_node& j : moh->node)
                if (j.mat) {
                    m = j.mat;
                    break;
                }

            mat4 mat = i.model_transform.mat;
            mat4 t = *m;
            mat4_mul(&mat, &t, &mat);

            vec3 pos = *(vec3*)&mat.row3;
            pos.y -= 0.2f;
            shad->field_1D0[shadow_type].push_back(pos);
        }

        if (i.mats.size())
            disp_manager.entry_obj_by_object_info_object_skin(
                i.object_info, 0, 0, auth->alpha, i.mats.data(), 0, 0, &i.model_transform.mat);
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
            size_t rob_chr = rob_chara_array_get(rob_chara_smth, auth->chara_id);
            mat4_mul(rob_chara_get_adjust_data_mat(rob_chr), sub_140516740(rob_chr), &mat);
            if (auth->chara_id)
                disp_manager.set_shadow_type(SHADOW_STAGE);
        }
    }
    else if (flags & mdl::OBJ_SHADOW) {
        disp_manager.set_shadow_type(SHADOW_STAGE);

        mat4* m = &oh->node.front().model_transform.mat;
        for (auth_3d_object_node& i : oh->node)
            if (i.mat) {
                m = i.mat;
                break;
            }

        Shadow* shad = shadow_ptr_get();
        if (shad) {
            vec3 pos = *(vec3*)&m->row3;
            pos.y -= 0.2f;
            shad->field_1D0[SHADOW_STAGE].push_back(pos);
        }
    }

    if (oh->node_bone_mats.size())
        disp_manager.entry_obj_by_object_info_object_skin(
            oh->object_info, 0, 0, auth->alpha, oh->node_bone_mats.data(), 0, 0, &mat);

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
            spr::put_rgb_cross(&o->model_transform.mat);

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
            size_t rob_chr = rob_chara_array_get(rob_chara_smth, auth->chara_id);
            mat4 m;
            mat4_mul(rob_chara_get_adjust_data_mat(rob_chr), sub_140516740(rob_chr), &m);
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

        tex_pat[tex_pat_count].src = i.texture_id;
        tex_pat[tex_pat_count].dst = texture_info_get_id(buf);
        tex_pat_count++;
    }

    if (tex_pat_count)
        disp_manager.set_texture_pattern(tex_pat_count, tex_pat);

    int32_t tex_trans_count = 0;
    texture_transform_struct tex_trans[TEXTURE_TRANSFORM_COUNT];

    for (auth_3d_object_texture_transform& i : o->texture_transform) {
        if (tex_trans_count >= TEXTURE_TRANSFORM_COUNT || i.texture_id == -1
            || !texture_handler_get_texture(i.texture_id))
            break;

        tex_trans[tex_trans_count].id = i.texture_id;
        tex_trans[tex_trans_count].mat = i.mat;
        tex_trans_count++;
    }

    if (tex_trans_count)
        disp_manager.set_texture_transform(tex_trans_count, tex_trans);

    const char* uid_name = o->uid_name.c_str();
    int32_t uid_name_length = (int32_t)o->uid_name.size();

    if (uid_name_length <= 3)
        disp_manager.entry_obj_by_object_info(&mat, o->object_info);
    else if (o->morph.curve) {
        float_t morph = o->morph.value;
        int32_t morph_int = (int32_t)morph;
        morph = fmodf(morph, 1.0f);
        if (morph > 0.0f && morph < 1.0f) {
            sprintf_s(buf, sizeof(buf), "%.*s%03d", uid_name_length - 3, uid_name, morph_int + 1);
            object_info morph_obj_info = obj_database_get_object_info(buf);
            if (morph_obj_info.is_null())
                morph_obj_info = o->object_info;
            disp_manager.set_morph(morph_obj_info, morph);

            sprintf_s(buf, sizeof(buf), "%.*s%03d", uid_name_length - 3, uid_name, morph_int);
            object_info obj_info = obj_database_get_object_info(buf);
            if (auth->alpha < 0.999f)
                disp_manager.entry_obj_by_object_info(&mat, obj_info, auth->alpha);
            else
                disp_manager.entry_obj_by_object_info(&mat, obj_info);
            disp_manager.set_morph({}, 0.0f);
        }
        else {
            if (morph >= 1.0f)
                morph_int++;

            sprintf_s(buf, sizeof(buf), "%.*s%03d", uid_name_length - 3, uid_name, morph_int);
            object_info obj_info = obj_database_get_object_info(buf);
            if (obj_info.is_null())
                obj_info = o->object_info;
            disp_manager.entry_obj_by_object_info(&mat, obj_info);
        }
    }
    else if (o->pattern.curve) {
        sprintf_s(buf, sizeof(buf), "%.*s%03d",
            uid_name_length - 3, uid_name, (int32_t)prj::roundf(o->pattern.value));
        object_info obj_info = obj_database_get_object_info(buf);
        disp_manager.entry_obj_by_object_info(&mat, obj_info);
    }
    else
        disp_manager.entry_obj_by_object_info(&mat, o->object_info);

    disp_manager.set_texture_transform();
    disp_manager.set_texture_pattern();
    disp_manager.set_obj_flags();

    if (auth->pos)
        spr::put_rgb_cross(&o->model_transform.mat);

    for (auth_3d_object*& i : o->children_object)
        implOfauth_3d_object_disp(i, auth);
    for (auth_3d_object_hrc*& i : o->children_object_hrc)
        implOfauth_3d_object_hrc_disp(i, auth);
}

void auth_3d_patch() {
    INSTALL_HOOK(auth_3d_m_object_hrc_disp);
    INSTALL_HOOK(auth_3d_object_hrc_disp);
    INSTALL_HOOK(auth_3d_object_disp);
}
