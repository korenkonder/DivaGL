/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../gl_state.hpp"

namespace Glitter {
    GltParticleManager* glt_particle_manager = (GltParticleManager*)0x000000141199BB0;

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
}
