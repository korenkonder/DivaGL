/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    void Particle::CreateBuffer(size_t data) {
        ParticleType type = (ParticleType) * (int32_t*)(data + 0x40 + 0x00 + 0x08);
        GLArrayBuffer& vbo = *(GLArrayBuffer*)(data + 0x40 + 0x160);
        int32_t max_count = *(int32_t*)(data + 0x40 + 0x164);
        GLuint& vao = *(GLuint*)(data + 0x40 + 0x170);
        GLElementArrayBuffer& ebo = *(GLElementArrayBuffer*)(data + 0x40 + 0x174);
        Glitter::CreateBuffer(max_count, type == PARTICLE_QUAD, vao, vbo, ebo);

        DrawListData*& draw_list = *(DrawListData**)(data + 0x40 + 0x178);
        draw_list = new DrawListData;
    }

    void Particle::DeleteBuffer(size_t data) {
        DrawListData*& draw_list = *(DrawListData**)(data + 0x40 + 0x178);
        if (draw_list) {
            delete draw_list;
            draw_list = 0;
        }

        GLArrayBuffer& vbo = *(GLArrayBuffer*)(data + 0x40 + 0x160);
        GLuint& vao = *(GLuint*)(data + 0x40 + 0x170);
        GLElementArrayBuffer& ebo = *(GLElementArrayBuffer*)(data + 0x40 + 0x174);
        Glitter::DeleteBuffer(vao, vbo, ebo);
    }
}
