/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../../KKdLib/str_utils.hpp"
#include "../rob/rob.hpp"
#include "../auth_3d.hpp"
#include "../object.hpp"

namespace Glitter {
    EffectInstX::ExtAnim::ExtAnim() {
        object_index = -1;
        mesh_index = -1;
        a3da_id = -1;
        object_is_hrc = false;
        a3da_uid = -1;
        object = {};
        instance_id = 0;
        mesh_name = 0;
        mat = mat4_identity;
        translation = 0.0f;
    }

    EffectInstX::ExtAnim::~ExtAnim() {

    }

    EffectInstX::EffectInstX(EffectX* eff, size_t id, SceneX* sc,
        bool appear_now, bool init, uint8_t load_flags) : init() {
        effect = eff;
        data = eff->data;
        color = 1.0f;
        scale_all = 1.0f;
        this->id = id;
        translation = eff->translation;
        rotation = eff->rotation;
        scale = eff->scale;
        frame0 = 0.0f;
        frame1 = 0.0f;
        mat = mat4_identity;
        flags = (EffectInstFlag)0;
        random = 0;
        req_frame = 0.0f;
        ext_color = 0.0f;
        ext_anim_scale = 0.0f;
        some_scale = -1.0f;

        if (appear_now)
            data.appear_time = 0;
        else
            frame0 = -(float_t)data.appear_time;

        mat_rot = mat4_identity;
        mat_rot_eff_rot = mat4_identity;
        random_shared.Reset();
        ext_anim = 0;
        render_scene = {};

        Glitter::counter->Increment();

        if (load_flags & 0x01)
            enum_or(flags, EFFECT_INST_FLAG_23);

        if (data.flags & EFFECT_USE_SEED)
            random = data.seed;
        else
            random = Glitter::counter->GetValue();
        random_shared.value = random;

        if (!(eff->data.flags & EFFECT_LOCAL) && data.ext_anim) {
            EffectInstX::ExtAnim* inst_ext_anim = new EffectInstX::ExtAnim;
            if (inst_ext_anim) {
                EffectX::ExtAnim* ext_anim = data.ext_anim;
                if (ext_anim->flags & EFFECT_EXT_ANIM_SET_ONCE)
                    enum_or(flags, EFFECT_INST_SET_EXT_ANIM_ONCE);
                if (ext_anim->flags & EFFECT_EXT_ANIM_TRANS_ONLY)
                    enum_or(flags, EFFECT_INST_EXT_ANIM_TRANS_ONLY);
                if (ext_anim->flags & EFFECT_EXT_ANIM_NO_TRANS_X)
                    enum_or(flags, EFFECT_INST_NO_EXT_ANIM_TRANS_X);
                if (ext_anim->flags & EFFECT_EXT_ANIM_NO_TRANS_Y)
                    enum_or(flags, EFFECT_INST_NO_EXT_ANIM_TRANS_Y);
                if (ext_anim->flags & EFFECT_EXT_ANIM_NO_TRANS_Z)
                    enum_or(flags, EFFECT_INST_NO_EXT_ANIM_TRANS_Z);
                if (ext_anim->flags & EFFECT_EXT_ANIM_GET_THEN_UPDATE)
                    enum_or(flags, EFFECT_INST_GET_EXT_ANIM_THEN_UPDATE);

                if (ext_anim->flags & EFFECT_EXT_ANIM_CHARA_ANIM) {
                    inst_ext_anim->chara_index = ext_anim->chara_index;
                    inst_ext_anim->bone_index = ext_anim->bone_index;
                    enum_or(flags, EFFECT_INST_CHARA_ANIM);
                }
                else {
                    inst_ext_anim->object = ext_anim->object;
                    inst_ext_anim->a3da_uid = ext_anim->a3da_uid;
                    inst_ext_anim->instance_id = ext_anim->instance_id;
                }

                if (ext_anim->mesh_name[0])
                    inst_ext_anim->mesh_name = ext_anim->mesh_name;
                else
                    inst_ext_anim->mesh_name = 0;
            }
            ext_anim = inst_ext_anim;
            enum_or(flags, EFFECT_INST_HAS_EXT_ANIM);
            enum_or(flags, EFFECT_INST_HAS_EXT_ANIM_NON_INIT);
            enum_or(flags, EFFECT_INST_GET_EXT_ANIM_MAT);
        }

        emitters.reserve(eff->emitters.size());
        for (EmitterX*& i : eff->emitters) {
            if (!i)
                continue;

            EmitterInstX* emitter = new EmitterInstX(i, this, sc->emission);
            if (emitter)
                emitters.push_back(emitter);
        }

        if (!init)
            ResetInit(sc);
    }

    EffectInstX::~EffectInstX() {
        for (EmitterInstX*& i : emitters)
            delete i;

        delete ext_anim;
    }

    void EffectInstX::CalcDisp() {
        render_scene.CalcDisp();
    }

    void EffectInstX::Copy(EffectInstX* dst, float_t emission) {
        dst->frame0 = frame0;
        dst->frame1 = frame1;
        dst->color = color;
        dst->translation = translation;
        dst->rotation = rotation;
        dst->scale = scale;
        dst->mat = mat;
        dst->mat = mat_rot;
        dst->random = random;

        EffectInstX* x_dst = dynamic_cast<EffectInstX*>(dst);
        if (x_dst && emitters.size() == x_dst->emitters.size()) {
            x_dst->mat_rot_eff_rot = mat_rot_eff_rot;

            size_t count = emitters.size();
            for (size_t i = 0; i < count; i++)
                emitters.data()[i]->Copy(x_dst->emitters.data()[i], emission);
        }
    }

    void EffectInstX::Ctrl(float_t delta_frame, float_t emission) {
        GetExtAnim();
        if (flags & EFFECT_INST_GET_EXT_ANIM_THEN_UPDATE)
            return;

        GetValue();

        vec3& rot = rotation;

        mat4 mat;
        if (GetExtAnimMat(&mat)) {
            mat4_normalize_rotation(&mat, &mat_rot);
            mat4_clear_trans(&mat_rot, &mat_rot);

            mat_rot_eff_rot = mat_rot;
            mat4_mul_rotate_zyx(&mat_rot, &rot, &mat_rot);

            mat4_mul_translate(&mat, &translation, &mat);
        }
        else {
            mat_rot = mat4_identity;
            mat_rot_eff_rot = mat4_identity;
            mat4_rotate_zyx(&rot, &mat_rot);

            mat4_translate(&translation, &mat);
        }

        vec3 scale = this->scale * scale_all;

        mat4_mul_rotate_zyx(&mat, &rot, &mat);
        mat4_scale_rot(&mat, &scale, &this->mat);

        for (EmitterInstX*& i : emitters)
            if (i)
                i->Ctrl(this, delta_frame);

        if (frame0 >= 0.0f)
            if (frame0 < (float_t)data.life_time) {
                for (EmitterInstX*& i : emitters)
                    if (i)
                        i->Emit(delta_frame, emission);
            }
            else if (data.flags & EFFECT_LOOP)
                frame0 -= (float_t)data.life_time;
            else
                Free(emission, false);

        render_scene.Ctrl(delta_frame, true);
        frame0 += delta_frame;
    }

    void EffectInstX::Disp(DispType disp_type) {
        render_scene.Disp(disp_type);
    }

    void EffectInstX::Free(float_t emission, bool free) {
        enum_or(flags, EFFECT_INST_FREE);
        for (EmitterInstX*& i : emitters)
            if (i)
                i->Free(emission, free);
    }

    size_t EffectInstX::GetCtrlCount(ParticleType type) {
        return render_scene.GetCtrlCount(type);
    }

    size_t EffectInstX::GetDispCount(ParticleType type) {
        return render_scene.GetDispCount(type);
    }

    bool EffectInstX::HasEnded(bool a2) {
        if (!(flags & EFFECT_INST_FREE))
            return false;
        else if (!a2)
            return true;

        for (EmitterInstX*& i : emitters)
            if (i && !i->HasEnded(a2))
                return false;
        return true;
    }

    void EffectInstX::Reset(SceneX* sc) {
        frame0 = -(float_t)data.appear_time;
        frame1 = 0.0;

        flags = (EffectInstFlag)0;
        if (!(data.flags & EFFECT_LOCAL) && data.ext_anim) {
            if (ext_anim) {
                EffectX::ExtAnim* ext_anim = data.ext_anim;
                if (ext_anim->flags & EFFECT_EXT_ANIM_SET_ONCE)
                    enum_or(flags, EFFECT_INST_SET_EXT_ANIM_ONCE);
                if (ext_anim->flags & EFFECT_EXT_ANIM_TRANS_ONLY)
                    enum_or(flags, EFFECT_INST_EXT_ANIM_TRANS_ONLY);
                if (ext_anim->flags & EFFECT_EXT_ANIM_NO_TRANS_X)
                    enum_or(flags, EFFECT_INST_NO_EXT_ANIM_TRANS_X);
                if (ext_anim->flags & EFFECT_EXT_ANIM_NO_TRANS_Y)
                    enum_or(flags, EFFECT_INST_NO_EXT_ANIM_TRANS_Y);
                if (ext_anim->flags & EFFECT_EXT_ANIM_NO_TRANS_Z)
                    enum_or(flags, EFFECT_INST_NO_EXT_ANIM_TRANS_Z);
                if (ext_anim->flags & EFFECT_EXT_ANIM_GET_THEN_UPDATE)
                    enum_or(flags, EFFECT_INST_GET_EXT_ANIM_THEN_UPDATE);
                if (ext_anim->flags & EFFECT_EXT_ANIM_CHARA_ANIM)
                    enum_or(flags, EFFECT_INST_CHARA_ANIM);
            }
            enum_or(flags, EFFECT_INST_HAS_EXT_ANIM);
            enum_or(flags, EFFECT_INST_HAS_EXT_ANIM_NON_INIT);
            enum_or(flags, EFFECT_INST_GET_EXT_ANIM_MAT);
        }

        for (EmitterInstX*& i : emitters)
            if (i)
                i->Reset();

        ResetInit(sc);
    }

    bool EffectInstX::ResetCheckInit(SceneX* sc, float_t* init_delta_frame) {
        if (init)
            return ResetInit(sc, init_delta_frame);
        return false;
    }

    bool EffectInstX::ResetInit(SceneX* sc, float_t* init_delta_frame) {
        if (data.start_time < 1)
            return false;

        if (!init && sc->effect_group && sc->effect_group->scene
            && sc->effect_group->scene->Copy(this, sc)) {
            frame1 = (float_t)data.start_time;
            return false;
        }

        float_t delta_frame = 2.0f;
        float_t start_time = (float_t)data.start_time - frame1;
        bool ret = false;
        if (init_delta_frame) {
            if (start_time >= *init_delta_frame) {
                start_time = *init_delta_frame;
                *init_delta_frame = 0.0f;
                ret = true;
            }
            else
                *init_delta_frame -= start_time;
        }

        const float_t emission = sc->emission;
        while (start_time > 0.0f) {
            enum_or(flags, EFFECT_INST_JUST_INIT);
            if (delta_frame > start_time)
                delta_frame -= start_time;

            if (frame0 >= 0.0f) {
                GetValue();

                vec3 trans = translation;
                vec3 rot = rotation;
                vec3 scale = this->scale * scale_all;

                mat4 mat;
                mat4_translate(&trans, &mat);
                mat4_mul_rotate_zyx(&mat, &rot, &mat);
                mat4_scale_rot(&mat, &scale, &this->mat);

                for (EmitterInstX*& i : emitters)
                    if (i)
                        i->CtrlInit(this, delta_frame);

                if (frame0 < (float_t)data.life_time) {
                    for (EmitterInstX*& i : emitters)
                        if (i)
                            i->Emit(delta_frame, emission);
                }
                else if (data.flags & EFFECT_LOOP)
                    frame0 -= (float_t)data.life_time;
                else
                    Free(emission, false);

                render_scene.Ctrl(delta_frame, false);
            }

            frame0 += delta_frame;
            frame1 += delta_frame;
            start_time -= delta_frame;
        }

        CtrlMat();
        return ret;
    }

    void EffectInstX::CtrlMat() {
        if (!(flags & EFFECT_INST_JUST_INIT))
            return;

        vec3 trans = translation;
        vec3 rot = rotation;
        vec3 scale = this->scale * scale_all;

        mat4 mat;
        mat4_translate(&trans, &mat);
        mat4_mul_rotate_zyx(&mat, &rot, &mat);
        mat4_scale_rot(&mat, &scale, &mat);
        this->mat = mat;

        for (EmitterInstX*& i : emitters)
            if (i)
                i->CtrlMat(this);

        enum_and(flags, ~EFFECT_INST_JUST_INIT);
    }

    DispType EffectInstX::GetDispType() {
        if (data.flags & EFFECT_LOCAL)
            return DISP_LOCAL;
        else if (data.flags & EFFECT_ALPHA)
            return DISP_ALPHA;
        else if (flags & EFFECT_INST_FLAG_23)
            return DISP_TYPE_2;
        return DISP_NORMAL;
    }

    void EffectInstX::GetExtAnim() {
        if (!ext_anim)
            return;

        if (!(flags & EFFECT_INST_HAS_EXT_ANIM) || (flags & EFFECT_INST_SET_EXT_ANIM_ONCE
                && flags & EFFECT_INST_HAS_EXT_ANIM_TRANS))
            return;

        bool set_flags = false;

        if (data.ext_anim->flags & EFFECT_INST_CHARA_ANIM)
            enum_or(flags, EFFECT_INST_HAS_EXT_ANIM_NON_INIT);

        if (flags & EFFECT_INST_CHARA_ANIM) {
            size_t rob_chara_smth = get_rob_chara_smth();
            rob_chara* rob_chr = rob_chara_array_get(rob_chara_smth, ext_anim->chara_index);
            if (!rob_chr)
                return;

            mat4 mat;
            mat4_transpose(rob_chara_get_adjust_data_mat(rob_chr), &mat);

            vec3 scale;
            mat4_get_scale(&mat, &scale);
            ext_anim_scale = scale - 1.0f;
            ext_anim_scale.z = 0.0f;
            enum_or(flags, EFFECT_INST_HAS_EXT_ANIM_SCALE);

            if (rob_chara_array_check_visibility(rob_chara_smth, ext_anim->chara_index)
                || !(data.ext_anim->flags & EFFECT_EXT_ANIM_NO_DRAW_IF_NO_DATA))
                set_flags = true;
            else
                set_flags = false;

            int32_t bone_index = ext_anim->bone_index;
            if (bone_index != -1) {
                const mat4* bone_mat = rob_chara_get_bone_data_mat(rob_chr, (mot_bone_index)bone_index);
                if (bone_mat)
                    SetExtAnim(&mat, bone_mat, 0, set_flags);
            }
            else
                SetExtAnim(&mat, 0, 0, set_flags);
        }
        else if (flags & EFFECT_INST_GET_EXT_ANIM_MAT) {
            const mat4* obj_mat = 0;
            if (ext_anim->a3da_id != -1)
                obj_mat = ((auth_3d_id*)&ext_anim->a3da_id)->get_auth_3d_object_mat(
                    ext_anim->object_index, ext_anim->object_is_hrc);

            if (!obj_mat) {
                ext_anim->a3da_id = auth_3d_data_get_auth_3d_id(ext_anim->a3da_uid, ext_anim->object,
                    &ext_anim->object_index, &ext_anim->object_is_hrc, ext_anim->instance_id);
                if (ext_anim->a3da_id == -1)
                    return;

                ext_anim->mesh_index = -1;
                obj_mat = ((auth_3d_id*)&ext_anim->a3da_id)->get_auth_3d_object_mat(
                    ext_anim->object_index, ext_anim->object_is_hrc);
                if (!obj_mat)
                    return;
            }

            mat4 mat = mat4_identity;

            int32_t chara_id = ((auth_3d_id*)&ext_anim->a3da_id)->get_chara_id();
            if (chara_id >= 0 && chara_id < ROB_CHARA_COUNT) {
                rob_chara* rob_chr = rob_chara_array_get(get_rob_chara_smth(), chara_id);
                if (rob_chr) {
                    mat4_transpose(rob_chara_get_adjust_data_mat(rob_chr), &mat);

                    vec3 scale;
                    mat4_get_scale(&mat, &scale);
                    ext_anim_scale = scale - 1.0f;
                    ext_anim_scale.z = 0.0f;
                    enum_or(flags, EFFECT_INST_HAS_EXT_ANIM_SCALE);
                }
            }

            if (ext_anim->mesh_name) {
                if (ext_anim->mesh_index == -1)
                    ext_anim->mesh_index = objset_info_storage_get_obj_mesh_index(ext_anim->object, ext_anim->mesh_name);

                if (ext_anim->mesh_index != -1) {
                    obj_mesh* mesh = objset_info_storage_get_obj_mesh_by_index(ext_anim->object, ext_anim->mesh_index);
                    if (mesh) {
                        vec3* trans = &mesh->bounding_sphere.center;
                        SetExtAnim(&mat, obj_mat, trans, true);
                    }
                }
            }
            else
                SetExtAnim(&mat, obj_mat, 0, true);

        }
        else if (ext_anim->object.not_null()) {
            if (ext_anim->mesh_index == -1) {
                if (!ext_anim->mesh_name)
                    return;

                ext_anim->mesh_index = objset_info_storage_get_obj_mesh_index(ext_anim->object, ext_anim->mesh_name);
            }

            if (ext_anim->mesh_index != -1) {
                obj_mesh* mesh = objset_info_storage_get_obj_mesh_by_index(ext_anim->object, ext_anim->mesh_index);
                if (mesh) {
                    vec3* trans = &mesh->bounding_sphere.center;
                    SetExtAnim(0, 0, trans, true);
                }
            }
        }
    }

    bool EffectInstX::GetExtAnimMat(mat4* mat) {
        if (!(flags & EFFECT_INST_HAS_EXT_ANIM_TRANS) || !ext_anim)
            return false;

        mat4_mul_translate(&ext_anim->mat, &ext_anim->translation, mat);
        return true;
    }

    void EffectInstX::GetExtColor(float_t& r, float_t& g, float_t& b, float_t& a) {
        if (!(flags & EFFECT_INST_EXT_COLOR))
            return;

        if (flags & EFFECT_INST_SET_EXT_COLOR) {
            if (ext_color.x >= 0.0f)
                r = ext_color.x;
            if (ext_color.y >= 0.0f)
                g = ext_color.y;
            if (ext_color.z >= 0.0f)
                b = ext_color.z;
            if (ext_color.w >= 0.0f)
                a = ext_color.w;
        }
        else {
            if (ext_color.x >= 0.0f)
                r *= ext_color.x;
            if (ext_color.y >= 0.0f)
                g *= ext_color.y;
            if (ext_color.z >= 0.0f)
                b *= ext_color.z;
            if (ext_color.w >= 0.0f)
                a *= ext_color.w;
        }

        if (r < 0.0f)
            r = 0.0f;
        if (g < 0.0f)
            g = 0.0f;
        if (b < 0.0f)
            b = 0.0f;
        if (a < 0.0f)
            a = 0.0f;
    }
    FogType EffectInstX::GetFog() {
        if (data.flags & EFFECT_FOG)
            return Glitter::FOG_DEPTH;
        else if (data.flags & EFFECT_FOG_HEIGHT)
            return Glitter::FOG_HEIGHT;
        return Glitter::FOG_NONE;
    }

    void EffectInstX::GetValue() {
        AnimationX* anim = &effect->animation;
        size_t length = anim->curves.size();
        for (int32_t i = 0; i < length; i++) {
            CurveX* curve = anim->curves.data()[i];
            float_t value;
            if (!curve->GetValue(frame0, &value, random + i, &random_shared))
                continue;

            switch (curve->type) {
            case CURVE_TRANSLATION_X:
                translation.x = value;
                break;
            case CURVE_TRANSLATION_Y:
                translation.y = value;
                break;
            case CURVE_TRANSLATION_Z:
                translation.z = value;
                break;
            case CURVE_ROTATION_X:
                rotation.x = value;
                break;
            case CURVE_ROTATION_Y:
                rotation.y = value;
                break;
            case CURVE_ROTATION_Z:
                rotation.z = value;
                break;
            case CURVE_SCALE_X:
                scale.x = value;
                break;
            case CURVE_SCALE_Y:
                scale.y = value;
                break;
            case CURVE_SCALE_Z:
                scale.z = value;
                break;
            case CURVE_SCALE_ALL:
                scale_all = value;
                break;
            case CURVE_COLOR_R:
                color.x = value;
                break;
            case CURVE_COLOR_G:
                color.y = value;
                break;
            case CURVE_COLOR_B:
                color.z = value;
                break;
            case CURVE_COLOR_A:
                color.w = value;
                break;
            }
        }
    }

    bool EffectInstX::GetExtAnimScale(vec3* ext_anim_scale, float_t* some_scale) {
        if (!(flags & EFFECT_INST_HAS_EXT_ANIM_SCALE))
            return false;

        if (ext_anim_scale)
            *ext_anim_scale = this->ext_anim_scale;
        if (some_scale)
            *some_scale = this->some_scale;
        return true;
    }

    void EffectInstX::SetExtAnim(const mat4* a2, const mat4* a3, const vec3* trans, bool set_flags) {
        EffectInstFlag flags = this->flags;
        if (a2) {
            mat4 mat;
            if (a3) {
                mat4_transpose(a3, &mat);
                mat4_mul(&mat, a2, &mat);
            }
            else
                mat = *a2;

            if (flags & EFFECT_INST_EXT_ANIM_TRANS_ONLY) {
                ext_anim->mat = mat4_identity;
                mat4_get_translation(&mat, &ext_anim->translation);
            }
            else
                ext_anim->mat = mat;
        }

        if (flags & EFFECT_INST_NO_EXT_ANIM_TRANS_X) {
            ext_anim->mat.row3.x = 0.0f;
            ext_anim->translation.x = 0.0f;
        }
        else if (trans)
            ext_anim->translation.x = trans->x;

        if (flags & EFFECT_INST_NO_EXT_ANIM_TRANS_Y) {
            ext_anim->mat.row3.y = 0.0f;
            ext_anim->translation.y = 0.0f;
        }
        else if (trans)
            ext_anim->translation.y = trans->y;

        if (flags & EFFECT_INST_NO_EXT_ANIM_TRANS_Z) {
            ext_anim->mat.row3.z = 0.0f;
            ext_anim->translation.z = 0.0f;
        }
        else if (trans)
            ext_anim->translation.z = trans->z;

        if (set_flags) {
            if (flags & EFFECT_INST_GET_EXT_ANIM_THEN_UPDATE)
                enum_and(flags, ~EFFECT_INST_GET_EXT_ANIM_THEN_UPDATE);
            if (flags & EFFECT_INST_HAS_EXT_ANIM_NON_INIT)
                enum_and(flags, ~EFFECT_INST_HAS_EXT_ANIM_NON_INIT);
        }
        enum_or(flags, EFFECT_INST_HAS_EXT_ANIM_TRANS);
        this->flags = flags;
    }

    void EffectInstX::SetExtColor(bool set, float_t r, float_t g, float_t b, float_t a) {
        ext_color.x = r;
        ext_color.y = g;
        ext_color.z = b;
        ext_color.w = a;
        if (set)
            enum_or(flags, EFFECT_INST_SET_EXT_COLOR);
        else
            enum_and(flags, ~EFFECT_INST_SET_EXT_COLOR);
        enum_or(flags, EFFECT_INST_EXT_COLOR);
    }
}
