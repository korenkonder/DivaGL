/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../gl_state.hpp"
#include <Helpers.h>

namespace Glitter {
    GltParticleManager* glt_particle_manager = (GltParticleManager*)0x000000141199BB0;
    GltParticleManagerX* glt_particle_manager_x;

    void GltParticleManager::DispScenes(DispType disp_type) {
        if (flags & PARTICLE_MANAGER_NOT_DISP)
            return;

        for (Scene*& i : scenes)
            if (i)
                i->Disp(disp_type);

        gl_state_bind_vertex_array(0);
        gl_state_disable_blend();
        gl_state_enable_cull_face();
        gl_state_disable_depth_test();
    }

    void GltParticleManager::Disp(GPM) {
        if (GPM_VAL->flags & PARTICLE_MANAGER_NOT_DISP)
            return;

        for (Scene*& i : GPM_VAL->scenes)
            if (i)
                Scene::CalcDisp(i);
    }

    GltParticleManagerX::GltParticleManagerX() : frame_rate(),
        flags(), scene_load_counter(), draw_selected() {
        init_buffers_base = 5;
        init_buffers = 5;
        init_delta_frame_base = 20.0f;
        init_delta_frame = 20.0f;
        emission = 1.5f;
        delta_frame = 2.0f;
        draw_all = true;
        draw_all_mesh = true;
        scenes.reserve(0x100);
        file_readers.reserve(0x100);
        app::TaskWork::add_task(this, "GLITTER_X_TASK", 2);
    }

    GltParticleManagerX::~GltParticleManagerX() {
        FreeEffects();
    }

    bool GltParticleManagerX::init() {
        frame_rate = get_sys_frame_rate();
        return true;
    }

    bool GltParticleManagerX::ctrl() {
        if (flags & PARTICLE_MANAGER_READ_FILES) {
            init_delta_frame = init_delta_frame_base;
            init_buffers = init_buffers_base;

            for (auto i = file_readers.begin(); i != file_readers.end();)
                if (!*i || (*i)->ReadFarc()) {
                    delete* i;
                    i = file_readers.erase(i);
                }
                else
                    i++;

            if (!file_readers.size())
                enum_and(flags, ~PARTICLE_MANAGER_READ_FILES);
        }

        if (!(flags & PARTICLE_MANAGER_PAUSE)) {
            if (frame_rate)
                delta_frame = frame_rate->get_delta_frame();
            else
                delta_frame = get_delta_frame();

            CtrlScenes();
        }
        return false;
    }

    bool GltParticleManagerX::dest() {
        return true;
    }

    void GltParticleManagerX::disp() {
        if (flags & PARTICLE_MANAGER_NOT_DISP)
            return;

        for (SceneX*& i : scenes)
            if (i)
                i->CalcDisp();
    }

    void GltParticleManagerX::basic() {
        BasicEffectGroups();
    }

    bool GltParticleManagerX::AppendEffectGroup(uint32_t hash, EffectGroupX* eff_group, FileReaderX* file_read) {
        if (effect_groups.find(hash) != effect_groups.end())
            return false;

        eff_group->load_count = file_read->load_count;
        eff_group->emission = file_read->emission;
        if (file_read->emission <= 0.0f)
            eff_group->emission = emission;
        eff_group->hash = hash;

        effect_groups.insert({ hash, eff_group });
        return true;
    }

    void GltParticleManagerX::BasicEffectGroups() {
        for (auto i = effect_groups.begin(); i != effect_groups.end(); ) {
            if (i->second->load_count > 0) {
                i++;
                continue;
            }

            uint32_t hash = i->first;
            for (auto j = scenes.begin(); j != scenes.end();) {
                SceneX* scene = *j;
                if (!scene || scene->hash != hash) {
                    j++;
                    continue;
                }

                delete* j;
                j = scenes.erase(j);
            }

            delete i->second;
            i = effect_groups.erase(i);
        }

        bool local = false;
        for (auto i = scenes.begin(); i != scenes.end();) {
            SceneX* scene = *i;
            if (!(scene->flags & SCENE_ENDED)) {
                if (!local && scene->CanDisp(DISP_LOCAL, true))
                    local = true;
                i++;
                continue;
            }

            if (scene->fade_frame_left > 0.0f) {
                scene->fade_frame_left -= delta_frame;
                if (scene->fade_frame_left <= 0.0f)
                    scene->fade_frame_left = -1.0f;
            }

            bool free_scene = true;
            for (auto j = scene->effects.begin(); j != scene->effects.end();)
                if (!j->ptr || scene->fade_frame_left < 0.0f) {
                    delete j->ptr;
                    j = scene->effects.erase(j);
                }
                else {
                    free_scene = false;
                    j++;
                }

            if (free_scene) {
                delete* i;
                i = scenes.erase(i);
            }
            else
                i++;
        }

        if (local)
            enum_or(flags, PARTICLE_MANAGER_LOCAL);
        else
            enum_and(flags, ~PARTICLE_MANAGER_LOCAL);
    }

    bool GltParticleManagerX::CheckHasLocalEffect() {
        return !!(flags & PARTICLE_MANAGER_LOCAL);
    }

    bool GltParticleManagerX::CheckNoFileReaders(uint32_t hash) {
        auto elem = effect_groups.find(hash);
        if (elem == effect_groups.end())
            return false;

        for (FileReaderX*& i : file_readers)
            if (i && i->hash == hash)
                return false;
        return true;
    }

    bool GltParticleManagerX::CheckSceneEnded(SceneCounter scene_counter) {
        if (!scene_counter)
            return false;

        for (SceneX*& i : scenes)
            if (i && i->counter.counter == scene_counter.counter) {
                if (!scene_counter.index)
                    return !i->HasEnded(true);
                else
                    return i->HasEnded(scene_counter.index, true);
            }
        return false;
    }

    void GltParticleManagerX::CheckSceneHasLocalEffect(SceneX* sc) {
        if (flags & PARTICLE_MANAGER_LOCAL && sc->CanDisp(DISP_LOCAL, false))
            enum_or(flags, PARTICLE_MANAGER_LOCAL);

        sc->fade_frame = 30.0f;
    }

    void GltParticleManagerX::CtrlScenes() {
        for (auto i = scenes.begin(); i != scenes.end();) {
            SceneX* scene = *i;
            if (!scene) {
                delete scene;
                i = scenes.erase(i);
            }
            else if (!(scene->flags & SCENE_ENDED)) {
                float_t delta_frame;
                if (scene->frame_rate)
                    delta_frame = scene->frame_rate->get_delta_frame();
                else
                    delta_frame = this->delta_frame;
                scene->Ctrl(delta_frame);
                i++;
            }
            else
                i++;
        }
    }

    void GltParticleManagerX::DecrementInitBuffersByCount(int32_t count) {
        init_buffers = max_def(init_buffers - count, 0);
    }

    void GltParticleManagerX::DispScenes(DispType disp_type) {
        if (flags & PARTICLE_MANAGER_NOT_DISP)
            return;

        for (SceneX*& i : scenes)
            if (i)
                i->Disp(disp_type);

        gl_state_bind_vertex_array(0);
        gl_state_disable_blend();
        gl_state_enable_cull_face();
        gl_state_disable_depth_test();
    }

    void GltParticleManagerX::FreeEffects() {
        scenes.clear();
        file_readers.clear();
    }

    void GltParticleManagerX::FreeSceneEffect(SceneCounter scene_counter, bool force_kill) {
        if (!scene_counter)
            return;

        for (auto i = scenes.begin(); i != scenes.end();) {
            SceneX* scene = *i;
            if (!scene || scene_counter.counter != scene->counter.counter) {
                i++;
                continue;
            }

            bool free = true;
            if (scene_counter.index) {
                scene->FreeEffectByID(scene_counter.index, force_kill);
                free = scene->HasEnded(true);
            }

            if (free) {
                if (!(scene->flags & SCENE_ENDED)) {
                    enum_or(scene->flags, SCENE_ENDED);
                    scene->fade_frame_left = scene->fade_frame;
                }
                i++;
            }
            break;
        }
    }

    void GltParticleManagerX::FreeSceneEffect(uint32_t effect_group_hash, uint32_t effect_hash, bool force_kill) {
        if (effect_hash == hash_murmurhash_empty)
            return;

        if (effect_group_hash == hash_murmurhash_empty) {
            for (SceneX*& i : scenes)
                if (i && i->FreeEffect(effect_hash, force_kill))
                    break;
        }
        else
            for (SceneX*& i : scenes)
                if (i && i->hash == effect_group_hash && i->FreeEffect(effect_hash, force_kill))
                    break;
    }

    void GltParticleManagerX::FreeScene(uint32_t effect_group_hash) {
        for (SceneX*& i : scenes)
            if (i && i->hash == effect_group_hash) {
                i->FreeEffect(hash_murmurhash_empty, true);
                break;
            }
    }

    void GltParticleManagerX::FreeScenes() {
        for (SceneX*& i : scenes)
            if (i)
                i->FreeEffect(hash_murmurhash_empty, true);
    }

    size_t GltParticleManagerX::GetCtrlCount(ParticleType type) {
        size_t ctrl = 0;
        for (SceneX*& i : scenes)
            if (i)
                ctrl += i->GetCtrlCount(type);
        return ctrl;
    }

    size_t GltParticleManagerX::GetDispCount(ParticleType type) {
        size_t disp = 0;
        for (SceneX*& i : scenes)
            if (i)
                disp += i->GetDispCount(type);
        return disp;
    }

    EffectGroupX* GltParticleManagerX::GetEffectGroup(uint32_t hash) {
        auto elem = effect_groups.find(hash);
        if (elem != effect_groups.end())
            return elem->second;
        return 0;
    }

    size_t GltParticleManagerX::GetEffectsCount(uint32_t hash) {
        auto elem = effect_groups.find(hash);
        if (elem != effect_groups.end())
            return elem->second->effects.size();
        return 0;
    }

    bool GltParticleManagerX::GetPause() {
        return flags & PARTICLE_MANAGER_PAUSE ? true : false;
    }

    float_t GltParticleManagerX::GetSceneFrameLifeTime(SceneCounter scene_counter, int32_t* life_time) {
        if (life_time)
            *life_time = 0;

        for (SceneX*& i : scenes)
            if (i && i->counter.counter == scene_counter.counter)
                return i->GeFrameLifeTime(life_time, scene_counter.index);
        return 0.0f;
    }

    SceneCounter GltParticleManagerX::GetSceneCounter(uint8_t index) {
        if (scene_load_counter + 1 < 0xFFFFFF && !(flags & PARTICLE_MANAGER_RESET_SCENE_COUNTER)) {
            scene_load_counter++;
            return SceneCounter(index, scene_load_counter);
        }

        if (scenes.size()) {
            enum_and(flags, ~PARTICLE_MANAGER_RESET_SCENE_COUNTER);
            scene_load_counter = 1;
            return SceneCounter(index, scene_load_counter);
        }

        uint32_t counter = 0;
        for (SceneX*& i : scenes)
            if (i && counter < i->counter)
                counter = i->counter;
        counter++;

        if (counter < 0xFFFFFF)
            enum_and(flags, ~PARTICLE_MANAGER_RESET_SCENE_COUNTER);
        else {
            enum_or(flags, PARTICLE_MANAGER_RESET_SCENE_COUNTER);
            for (uint32_t v7 = 1; ; v7++) {
                bool found = false;
                for (SceneX*& i : scenes)
                    if (i && scene_load_counter == i->counter.counter) {
                        found = true;
                        break;
                    }

                if (!found)
                    break;

                if (v7 >= 300)
                    return 0;
            }
        }
        return SceneCounter(index, counter);
    }

    uint32_t GltParticleManagerX::LoadFile(const char* file, const char* path, float_t emission, bool init_scene) {
        uint32_t effect_group_hash = hash_utf8_murmurhash(file);
        if (effect_group_hash == hash_murmurhash_empty)
            return effect_group_hash;
        uint32_t empty_hash = hash_murmurhash_empty;

        auto elem = effect_groups.find(effect_group_hash);
        if (elem != effect_groups.end()) {
            EffectGroupX* eff_group = elem->second;
            eff_group->emission = emission;
            if (eff_group->emission <= 0.0f)
                eff_group->emission = this->emission;
            eff_group->load_count++;
            return effect_group_hash;
        }

        for (FileReaderX*& i : file_readers)
            if (i && i->hash == effect_group_hash) {
                i->load_count++;
                i->emission = emission;
                return effect_group_hash;
            }

        FileReaderX* fr = new FileReaderX;
        if (!fr)
            return empty_hash;

        fr->init_scene = init_scene;

        if (!path)
            path = "rom/particle_x/";

        if (!fr->LoadFarc(path, file, effect_group_hash)) {
            delete fr;
            return empty_hash;
        }

        fr->emission = emission;
        file_readers.push_back(fr);
        enum_or(flags, PARTICLE_MANAGER_READ_FILES);
        return effect_group_hash;
    }

    SceneCounter GltParticleManagerX::LoadSceneEffect(uint32_t hash, bool appear_now, uint8_t load_flags) {
        if (hash == hash_murmurhash_empty)
            return 0;

        if (!(load_flags & 0x02) && scenes.size())
            for (SceneX*& i : scenes)
                if (i && i->ResetEffect(hash)) {
                    CheckSceneHasLocalEffect(i);

                    size_t id = 0;
                    for (SceneEffectX& j : i->effects)
                        if (j.ptr && j.disp && j.ptr->data.name_hash == hash) {
                            id = j.ptr->id;
                            break;
                        }

                    SceneCounter counter = i->counter;
                    counter.index = (uint8_t)id;
                    enum_and(i->flags, ~SCENE_ENDED);
                    return counter;
                }

        for (auto& i : effect_groups) {
            EffectGroupX* v8 = i.second;

            if (v8->effects.size() < 1)
                continue;

            bool found = false;
            for (EffectX*& j : v8->effects)
                if (j && j->data.name_hash == hash) {
                    found = true;
                    break;
                }

            if (!found)
                continue;

            if (hash == hash_murmurhash_empty)
                continue;

            if (v8->not_loaded)
                return 0;

            if (!(load_flags & 0x02))
                for (SceneX*& j : scenes)
                    if (j && j->hash == i.first) {
                        size_t id = 1;
                        for (EffectX*& k : v8->effects) {
                            if (!j)
                                continue;

                            if (k->data.name_hash == hash) {
                                j->InitEffect(k, id, appear_now, load_flags);
                                break;
                            }
                            id++;
                        }
                        CheckSceneHasLocalEffect(j);

                        SceneCounter counter = j->counter;
                        counter.index = (uint8_t)id;
                        enum_and(j->flags, ~SCENE_ENDED);
                        return counter;
                    }

            SceneCounter counter = GetSceneCounter();
            if (!counter)
                return 0;

            SceneX* scene = new SceneX(counter, i.first, i.second, false);
            if (!scene)
                return 0;

            size_t id = 1;
            for (EffectX*& j : v8->effects) {
                if (!j)
                    continue;

                if (j->data.name_hash == hash) {
                    scene->InitEffect(j, id, appear_now);
                    break;
                }
                id++;
            }
            CheckSceneHasLocalEffect(scene);
            scenes.push_back(scene);

            counter.index = (uint8_t)id;
            return counter;
        }
        return 0;
    }

    void GltParticleManagerX::SetInitDeltaFrame(float_t value) {
        init_delta_frame = value;
        if (value <= 0.0f)
            init_delta_frame = -1.0f;
    }

    void GltParticleManagerX::SetPause(bool value) {
        if (value)
            enum_or(flags, PARTICLE_MANAGER_PAUSE);
        else
            enum_and(flags, ~PARTICLE_MANAGER_PAUSE);
    }

    void GltParticleManagerX::SetSceneEffectExtColor(SceneCounter scene_counter, bool set,
        uint32_t effect_hash, float_t r, float_t g, float_t b, float_t a) {
        for (SceneX*& i : scenes)
            if (i && i->counter.counter == scene_counter.counter) {
                if (effect_hash == hash_murmurhash_empty)
                    i->SetExtColorByID(set, scene_counter.index, r, g, b, a);
                else
                    i->SetExtColor(set, effect_hash, r, g, b, a);
                break;
            }
    }

    void GltParticleManagerX::SetSceneEffectReqFrame(SceneCounter scene_counter, float_t req_frame) {
        for (SceneX*& i : scenes)
            if (i && i->counter.counter == scene_counter.counter) {
                i->SetReqFrame(scene_counter.index, req_frame);
                break;
            }
    }

    void GltParticleManagerX::SetSceneFrameRate(SceneCounter scene_counter, FrameRateControl* frame_rate) {
        for (SceneX*& i : scenes)
            if (i && i->counter.counter == scene_counter.counter) {
                i->SetFrameRate(frame_rate);
                break;
            }
    }

    void GltParticleManagerX::UnloadEffectGroup(uint32_t hash) {
        for (auto i = file_readers.begin(); i != file_readers.end();)
            if (!*i || (*i)->hash == hash) {
                delete* i;
                i = file_readers.erase(i);
            }
            else
                i++;

        auto elem = effect_groups.find(hash);
        if (elem != effect_groups.end())
            elem->second->load_count--;
    }
}
