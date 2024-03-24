/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../gl_state.hpp"

namespace Glitter {
    void Scene::Disp(DispType disp_type) {
        if (!(flags & SCENE_NOT_DISP))
            render_scene.Disp(disp_type);
    }

    void Scene::CalcDisp(Scene* sc) {
        if (!(sc->flags & SCENE_NOT_DISP))
            sc->render_scene.CalcDisp();
    }

    SceneX::SceneX(SceneCounter counter, uint32_t hash, EffectGroupX* eff_group, bool a5) {
        this->counter = counter;
        this->hash = hash;
        flags = SCENE_NONE;
        emission = 1.0f;
        effect_group = eff_group;
        delta_frame_history = 0.0f;
        skip = false;
        fade_frame = -1.0f;
        fade_frame_left = -1.0f;
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

    bool SceneX::Copy(EffectInstX* eff_inst, Glitter::SceneX* dst) {
        if (!(flags & SCENE_FLAG_3))
            return flags;

        for (SceneEffectX& i : effects)
            if (i.ptr && i.disp && i.ptr->id == eff_inst->id) {
                i.ptr->Copy(eff_inst, dst->emission);
                return true;
            }
        return false;
    }

    void SceneX::CalcDisp() {
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

    void SceneX::Ctrl(float_t delta_frame) {
        static int32_t call_count;
        for (SceneEffectX& i : effects) {
            if (!i.ptr || !i.disp)
                continue;

            EffectInstX* eff = i.ptr;
            bool v13 = false;
            bool v14 = false;
            if (v14)
                v13 = true;

            float_t _delta_frame = delta_frame;
            if (v14)
                _delta_frame = 0.0f;

            float_t req_frame = eff->req_frame;
            eff->req_frame = 0.0f;

            req_frame -= _delta_frame;
            while (true) {
                while (true) {
                    if (v13 || _delta_frame > 0.0f)
                        eff->Ctrl(_delta_frame, emission);

                    if (req_frame <= 10.0f)
                        break;

                    _delta_frame = 10.0f;
                    req_frame -= 10.0f;
                }

                if (req_frame <= 0.0f)
                    break;

                _delta_frame = req_frame;
                req_frame = -1.0f;
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

    void SceneX::GetStartEndFrame(int32_t* start_frame, int32_t* end_frame) {
        for (SceneEffectX& i : effects) {
            if (!i.ptr || !i.disp)
                continue;

            EffectInstX* effect = i.ptr;
            if (effect) {
                int32_t life_time = effect->data.life_time;
                if (start_frame && effect->data.appear_time < *start_frame)
                    *start_frame = effect->data.appear_time;

                for (EmitterInstX*& j : effect->emitters) {
                    if (!j)
                        continue;

                    EmitterInstX* emitter = j;
                    if (life_time < emitter->data.life_time)
                        life_time = emitter->data.life_time;
                }

                life_time += effect->data.appear_time;

                if (end_frame && life_time > *end_frame)
                    *end_frame = life_time;
            }
        }
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

    bool SceneX::ResetCheckInit(float_t* a2) {
        if (!(flags & SCENE_FLAG_3) || !effects.size())
            return false;

        for (SceneEffectX& i : effects)
            if (i.disp && i.ptr && i.ptr->ResetCheckInit(this, a2))
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

    bool SceneX::SetExtColor(bool set, uint32_t effect_hash, float_t r, float_t g, float_t b, float_t a) {
        if (effect_hash == hash_murmurhash_empty) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->SetExtColor(set, r, g, b, a);
            return true;
        }
        else
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr && i.ptr->data.name_hash == effect_hash) {
                    i.ptr->SetExtColor(set, r, g, b, a);
                    return true;
                }
        return false;
    }

    bool SceneX::SetExtColorByID(bool set, size_t id, float_t r, float_t g, float_t b, float_t a) {
        if (!id) {
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr)
                    i.ptr->SetExtColor(set, r, g, b, a);
            return true;
        }
        else
            for (SceneEffectX& i : effects)
                if (i.disp && i.ptr && i.ptr->id == id) {
                    i.ptr->SetExtColor(set, r, g, b, a);
                    return true;
                }
        return false;
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
