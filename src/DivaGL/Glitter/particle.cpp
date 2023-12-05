/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    void Particle::CreateBuffer(size_t data) {
        ParticleType type = (ParticleType) * (int32_t*)(data + 0x40 + 0x00 + 0x08);
        GLuint& vbo = ((GLuint*)(data + 0x40 + 0x160))[0];
        int32_t max_count = *(int32_t*)(data + 0x40 + 0x164);
        GLuint& vao = ((GLuint*)(data + 0x40 + 0x170))[0];
        GLuint& ebo = ((GLuint*)(data + 0x40 + 0x170))[1];
        Glitter::CreateBuffer(max_count, type == PARTICLE_QUAD, vao, vbo, ebo);

        DrawListData*& draw_list = *(DrawListData**)(data + 0x40 + 0x170 + sizeof(GLuint) * 2);
        draw_list = new DrawListData;
    }

    void Particle::DeleteBuffer(size_t data) {
        DrawListData*& draw_list = *(DrawListData**)(data + 0x40 + 0x170 + sizeof(GLuint) * 2);
        if (draw_list) {
            delete draw_list;
            draw_list = 0;
        }

        GLuint& vbo = ((GLuint*)(data + 0x40 + 0x160))[0];
        GLuint& vao = ((GLuint*)(data + 0x40 + 0x170))[0];
        GLuint& ebo = ((GLuint*)(data + 0x40 + 0x170))[1];
        Glitter::DeleteBuffer(vao, vbo, ebo);
    }
}
