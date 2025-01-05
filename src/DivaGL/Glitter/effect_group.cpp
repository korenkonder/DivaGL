/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../object.hpp"
#include "../texture.hpp"

namespace Glitter {
    EffectGroupX::EffectGroupX() : effects(), load_count(), resources(),
        scene(), not_loaded(), scene_init(), buffer_init() {
        emission = 1.0f;
#if SHARED_GLITTER_BUFFER
        buffer = 0;
        max_count = 0;
#endif
    }

    EffectGroupX::~EffectGroupX() {
#if SHARED_GLITTER_BUFFER
        ebo.Destroy();
        vbo.Destroy();

        free_def(buffer);
#endif

        for (MeshX& i : meshes)
            if (i.object_set != -1) {
                objset_info_storage_unload_set(i.object_set);
                i.load = false;
            }

        meshes.clear();

        if (resources) {
            texture_array_free(resources);
            resources = 0;
        }

#if SHARED_GLITTER_BUFFER
        if (scene) {
            delete scene;
            scene = 0;
        }
#endif

        for (EffectX*& i : effects)
            if (i) {
                delete i;
                i = 0;
            }
        effects.clear();

#if !SHARED_GLITTER_BUFFER
        if (scene) {
            delete scene;
            scene = 0;
        }
#endif
    }
}
