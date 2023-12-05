/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../gl_state.hpp"

namespace Glitter {
    void Scene::CalcDisp(Scene* sc) {
        if (!(sc->flags & SCENE_NOT_DISP))
            RenderScene::CalcDisp(&sc->render_scene);
    }
}
