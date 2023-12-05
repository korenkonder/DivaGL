/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../gl_state.hpp"

namespace Glitter {
    mat4 RenderGroup::RotateToEmitPosition(RenderGroup* rend_group,
        RenderElement* rend_elem, vec3* vec) {
        vec3 vec2;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL)
            vec2 = rend_elem->translation;
        else {
            mat4_get_translation(&rend_group->mat, &vec2);
            vec2 -= rend_elem->translation;
        }
        vec2 = vec3::normalize(vec2);

        mat4 mat;
        if (fabsf(vec2.y) >= 0.000001f) {
            mat = mat4_identity;
            vec3 vec1 = *vec;

            vec3 axis;
            float_t angle;
            axis_angle_from_vectors(&axis, &angle, &vec1, &vec2);
            mat4_mul_rotation(&mat, &axis, angle, &mat);
        }
        else
            mat4_rotate_z((float_t)M_PI, &mat);
        return mat;
    }

    mat4 RenderGroup::RotateToPrevPosition(RenderGroup* rend_group,
        RenderElement* rend_elem, vec3* vec) {
        vec3 vec2 = rend_elem->translation - rend_elem->translation_prev;

        if (vec3::length_squared(vec2) < 0.000001f)
            vec2 = rend_elem->translation;
        vec2 = vec3::normalize(vec2);

        mat4 mat;
        mat4_rotate_z((float_t)M_PI, &mat);
        if (fabsf(vec2.y) >= 0.000001f) {
            vec3 vec1 = *vec;

            vec3 axis;
            float_t angle;
            axis_angle_from_vectors(&axis, &angle, &vec1, &vec2);
            mat4_mul_rotation(&mat, &axis, angle, &mat);
        }
        return mat;
    }

    void RenderGroup::CreateBuffer(RenderGroup* rend_group) {
        if (rend_group->use_own_buffer) {
            if (rend_group->max_count > 0) {
                rend_group->buffer = (Buffer*)_operator_new(sizeof(Buffer) * rend_group->max_count);
                if (!rend_group->buffer)
                    rend_group->max_count = 0;

                ParticleType type = rend_group->type;
                GLuint& vbo = rend_group->vbo;
                int32_t max_count = rend_group->max_count;
                GLuint& vao = rend_group->vao;
                GLuint& ebo = rend_group->ebo;
                Glitter::CreateBuffer(max_count, type == PARTICLE_QUAD, vao, vbo, ebo);
                rend_group->draw_list = new DrawListData;
            }
        }
        else {
            size_t particle = *(size_t*)((size_t)rend_group->particle_inst + 0x98 + 0x178);
            rend_group->vao = ((GLuint*)(particle + 0x40 + 0x170))[0];
            rend_group->ebo = ((GLuint*)(particle + 0x40 + 0x170))[1];
            rend_group->draw_list = *(DrawListData**)(particle
                + 0x40 + 0x170 + sizeof(GLuint) * 2);
        }
    }

    void RenderGroup::DeleteBuffer(RenderGroup* rend_group) {
        if (rend_group->use_own_buffer) {
            DrawListData* draw_list = rend_group->draw_list;
            if (draw_list) {
                draw_list->clear();
                draw_list->shrink_to_fit();
                _operator_delete(draw_list);
                rend_group->draw_list = 0;
            }

            GLuint& vbo = rend_group->vbo;
            GLuint& vao = rend_group->vao;
            GLuint& ebo = rend_group->ebo;
            Glitter::DeleteBuffer(vao, vbo, ebo);
        }
        else {
            rend_group->vbo = 0;
            rend_group->ebo = 0;
            rend_group->draw_list = 0;
        }
    }
}
