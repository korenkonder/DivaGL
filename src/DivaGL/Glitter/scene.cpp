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
}
