/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../gl_state.hpp"

namespace Glitter {
    GPM = (GltParticleManager*)0x000000141199BB0;

    void GltParticleManager::Disp(GPM) {
        if (glt_particle_manager->flags & PARTICLE_MANAGER_NOT_DISP)
            return;

        for (Scene*& i : glt_particle_manager->scenes)
            if (i)
                Scene::CalcDisp(i);
    }

    void (FASTCALL* GltParticleManager__DispScenes)(GPM, DispType disp_type)
        = (void (FASTCALL*)(GPM, DispType disp_type))0x00000001403A1020;
}
