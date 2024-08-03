/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    EffectX::Data::Data() : appear_time(), life_time(), start_time(),
        ext_anim(), flags(), emission(), seed(), ext_anim_end_time() {
        color = 0xFFFFFFFF;
        name_hash = hash_murmurhash_empty;
    }

    EffectX::EffectX() {
        version = 0x07;
    }

    EffectX::~EffectX() {
        for (EmitterX*& i : emitters)
            delete i;

        if (data.ext_anim)
            free(data.ext_anim);
    }

    EffectX& EffectX::operator=(const EffectX& eff) {
        if (data.ext_anim)
            free(data.ext_anim);

        data = eff.data;
        version = eff.version;

        if (eff.data.ext_anim) {
            data.ext_anim = force_malloc<EffectX::ExtAnim>();
            *data.ext_anim = *eff.data.ext_anim;
        }

        for (EmitterX*& i : emitters)
            delete i;
        emitters.clear();

        emitters.reserve(eff.emitters.size());
        for (EmitterX* i : eff.emitters) {
            EmitterX* emit = new EmitterX;
            *emit = *i;
            emitters.push_back(emit);
        }
        return *this;
    }
}
