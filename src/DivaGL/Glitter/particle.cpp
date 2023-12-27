/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    void Particle::CreateBuffer(Particle* ptcl) {
        Glitter::CreateBuffer(ptcl->sub.max_count,
            ptcl->sub.data.type == PARTICLE_QUAD, ptcl->sub.vao, ptcl->sub.vbo, ptcl->sub.ebo);

        ptcl->sub.draw_list = new DrawListData;
    }

    void Particle::DeleteBuffer(Particle* ptcl) {
        if (ptcl->sub.draw_list) {
            delete ptcl->sub.draw_list;
            ptcl->sub.draw_list = 0;
        }

        Glitter::DeleteBuffer(ptcl->sub.vao, ptcl->sub.vbo, ptcl->sub.ebo);
    }
}
