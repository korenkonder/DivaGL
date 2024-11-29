/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    void Scene::Disp(DispType disp_type) {
        if (!(flags & SCENE_NOT_DISP))
            render_scene.Disp(disp_type);
    }

    void Scene::CalcDisp() {
        if (!(flags & SCENE_NOT_DISP))
            render_scene.CalcDisp();
    }

    SceneX::SceneX(SceneCounter counter, uint32_t hash, EffectGroupX* eff_group, bool a5) {
        this->counter = counter;
        this->hash = hash;
        flags = SCENE_NONE;
        emission = 1.0f;
        effect_group = eff_group;
        fade_frame_left = -1.0f;
        fade_frame = -1.0f;
        effect_group = eff_group;
        frame_rate = 0;

        if (eff_group) {
            effects.reserve(eff_group->effects.size());
            emission = eff_group->emission;
            if (a5)
                enum_or(flags, SCENE_FLAG_3);
        }
    }

    SceneX::~SceneX() {
        for (SceneEffectX& i : effects)
            delete i.ptr;
    }

    void SceneX::CalcDisp() {
        if (flags & SCENE_NOT_DISP)
            return;

        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp)
                i.ptr->CalcDisp();
    }

    bool SceneX::CanDisp(DispType disp_type, bool a3) {
        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp && i.ptr->render_scene.CanDisp(disp_type, a3))
                return true;
        return false;
    }

    void SceneX::CheckUpdate(float_t delta_frame) {
        if (delta_frame > 0.0f)
            enum_and(flags, ~SCENE_PAUSE);
        else
            enum_or(flags, SCENE_PAUSE);

        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp)
                i.ptr->CheckUpdate();
    }

    bool SceneX::Copy(EffectInstX* eff_inst, Glitter::SceneX* dst) {
        if (!(flags & SCENE_FLAG_3))
            return false;

        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp && i.ptr->id == eff_inst->id) {
                i.ptr->Copy(eff_inst, dst->emission);
                return true;
            }
        return false;
    }

    void SceneX::Ctrl(float_t delta_frame) {
        static int32_t call_count;
        for (SceneEffectX& i : effects) {
            if (!i.ptr || !i.disp)
                continue;

            EffectInstX* eff = i.ptr;

            bool step = false;
            bool just_init = !!(eff->flags & EFFECT_INST_JUST_INIT);
            if (!(flags & SCENE_PAUSE) || just_init)
                step = true;

            float_t _delta_frame = 0.0f;
            if (!just_init)
                _delta_frame = delta_frame;

            float_t req_frame = eff->req_frame;
            eff->req_frame = 0.0f;

            float_t remain_frame = req_frame - _delta_frame;
            while (true) {
                if (step || _delta_frame > 0.0f) {
                    eff->CtrlFlags(_delta_frame);
                    eff->Ctrl(_delta_frame);
                    eff->Emit(_delta_frame, emission);
                    eff->RenderSceneCtrl(_delta_frame);
                }

                if (remain_frame > 10.0f) {
                    _delta_frame = 10.0f;
                    remain_frame -= 10.0f;
                }
                else if (remain_frame > 0.0f) {
                    _delta_frame = remain_frame;
                    remain_frame = -1.0f;
                }
                else
                    break;
            }
        }
    }

    void SceneX::Disp(DispType disp_type) {
        if (flags & SCENE_NOT_DISP)
            return;

        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp)
                i.ptr->Disp(disp_type);
    }

    void SceneX::DispMesh() {
        if (flags & SCENE_NOT_DISP)
            return;

        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp)
                i.ptr->DispMesh();
    }

    size_t SceneX::GetCtrlCount(ParticleType ptcl_type) {
        size_t ctrl = 0;
        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp)
                ctrl += i.ptr->GetCtrlCount(ptcl_type);
        return ctrl;
    }

    size_t SceneX::GetDispCount(ParticleType ptcl_type) {
        size_t disp = 0;
        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp)
                disp += i.ptr->GetDispCount(ptcl_type);
        return disp;
    }

    float_t SceneX::GeFrameLifeTime(int32_t* life_time, size_t id) {
        float_t frame = 0.0f;
        if (!id)
            for (SceneEffectX& i : effects) {
                if (!i.disp || !i.ptr)
                    continue;

                EffectInstX* eff_inst = i.ptr;
                if (frame < eff_inst->frame0)
                    frame = eff_inst->frame0;

                if (life_time && *life_time < eff_inst->data.life_time)
                    *life_time = eff_inst->data.life_time;
            }
        else
            for (SceneEffectX& i : effects) {
                if (!i.disp || !i.ptr || !i.ptr->id == id)
                    continue;

                EffectInstX* eff_inst = i.ptr;
                if (frame < eff_inst->frame0)
                    frame = eff_inst->frame0;

                if (life_time && *life_time < eff_inst->data.life_time)
                    *life_time = eff_inst->data.life_time;
                break;
            }
        return frame;
    }

    bool SceneX::FreeEffect(uint32_t effect_hash, bool free) {
        if (effect_hash == hash_murmurhash_empty) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->Free(emission, free);
            return true;
        }

        for (SceneEffectX& i : effects)
            if (i.disp && i.ptr && i.ptr->data.name_hash == effect_hash) {
                i.ptr->Free(emission, free);
                return true;
            }
        return false;
    }

    bool SceneX::FreeEffectByID(size_t id, bool free) {
        if (!id) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->Free(emission, free);
            return true;
        }

        for (SceneEffectX& i : effects)
            if (i.disp && i.ptr && i.ptr->id == id) {
                i.ptr->Free(emission, free);
                return true;
            }
        return false;
    }

    bool SceneX::HasEnded(bool a2) {
        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp && !i.ptr->HasEnded(a2))
                return false;
        return true;
    }

    bool SceneX::HasEnded(size_t id, bool a3) {
        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp && i.ptr->id == id && !i.ptr->HasEnded(a3))
                return false;
        return true;
    }

    void SceneX::InitEffect(EffectX* eff, size_t id, bool appear_now, uint8_t load_flags) {
        if (!eff)
            return;

        bool init = !!(flags & SCENE_FLAG_3);
        if (!init)
            for (SceneEffectX& i : effects)
                if (i.ptr && i.disp && i.ptr->id == id) {
                    i.ptr->Reset(this);
                    return;
                }

        SceneEffectX effect;
        effect.ptr = new EffectInstX(eff, id, this, appear_now, init, load_flags);
        effect.disp = true;
        effects.push_back(effect);

#if defined(CRE_DEV)
        if (type == Glitter::X)
            enum_and(flags, ~SCENE_ENDED);
#endif
    }

    bool SceneX::ResetCheckInit(float_t* init_delta_frame) {
        if (!(flags & SCENE_FLAG_3) || !effects.size())
            return false;

        for (SceneEffectX& i : effects)
            if (i.disp && i.ptr && i.ptr->ResetCheckInit(this, init_delta_frame))
                return true;
        return false;
    }

    bool SceneX::ResetEffect(uint32_t effect_hash, size_t* id) {
        if (effect_hash == hash_murmurhash_empty) {
            for (SceneEffectX& i : effects)
                if (i.ptr)
                    delete i.ptr;
            effects.clear();
        }
        else {
            for (auto i = effects.begin(); i != effects.end();)
                if (!i->ptr) {
                    delete i->ptr;
                    i = effects.erase(i);
                }
                else if (i->ptr->data.name_hash == effect_hash) {
                    if (id)
                        *id = i->ptr->id;

                    delete i->ptr;
                    i = effects.erase(i);
                    break;
                }
                else
                    i++;
        }
        return false;
    }

    void SceneX::SetEnded() {
        if (!(flags & SCENE_ENDED)) {
            enum_and(flags, SCENE_ENDED);
            fade_frame_left = fade_frame;
        }
    }

    void SceneX::SetExtAnimMat(mat4* mat, size_t id) {
        if (!id) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->SetExtAnimMat(mat);
        }
        else
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr && i.ptr->id == id) {
                    i.ptr->SetExtAnimMat(mat);
                    break;
                }
    }

    bool SceneX::SetExtColor(float_t r, float_t g, float_t b, float_t a, bool set, uint32_t effect_hash) {
        if (effect_hash == hash_murmurhash_empty) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->SetExtColor(r, g, b, a, set);
            return true;
        }
        else
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr && i.ptr->data.name_hash == effect_hash) {
                    i.ptr->SetExtColor(r, g, b, a, set);
                    return true;
                }
        return false;
    }

    bool SceneX::SetExtColorByID(float_t r, float_t g, float_t b, float_t a, bool set, size_t id) {
        if (!id) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->SetExtColor(r, g, b, a, set);
            return true;
        }
        else
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr && i.ptr->id == id) {
                    i.ptr->SetExtColor(r, g, b, a, set);
                    return true;
                }
        return false;
    }

    void SceneX::SetExtScale(float_t scale, size_t id) {
        if (!id) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->SetExtScale(scale);
        }
        else
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr && i.ptr->id == id) {
                    i.ptr->SetExtScale(scale);
                    break;
                }
    }

    void SceneX::SetFrameRate(FrameRateControl* frame_rate) {
        this->frame_rate = frame_rate;
    }

    void SceneX::SetReqFrame(size_t id, float_t req_frame) {
        if (!id) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->req_frame = req_frame;
        }
        else
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr && i.ptr->id == id) {
                    i.ptr->req_frame = req_frame;
                    break;
                }
    }

    SceneCounter::SceneCounter(uint32_t counter) {
        this->index = 0;
        this->counter = counter;
    }

    SceneCounter::SceneCounter(uint32_t index, uint32_t counter) {
        this->index = index;
        this->counter = counter;
    }
}
