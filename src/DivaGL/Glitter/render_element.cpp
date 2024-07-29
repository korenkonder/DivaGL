/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    RenderElementX::RenderElementX() : alive(), uv_index(), disp(), frame(), life_time(), rebound_time(),
        frame_step_uv(), base_speed(), speed(), deceleration(), rot_z_cos(), rot_z_sin(),
        scale_all(), fade_out_frames(), fade_in_frames(), locus_history(), random(), step() {

    }

    void RenderElementX::InitLocusHistory(ParticleInstX* ptcl_inst, RandomX* random) {
        if (ptcl_inst->data.data.type == PARTICLE_LOCUS) {
            uint32_t locus_history_size = random->GetInt(
                -ptcl_inst->data.data.locus_history_size_random,
                ptcl_inst->data.data.locus_history_size_random)
                + ptcl_inst->data.data.locus_history_size;
            locus_history = new LocusHistoryX(locus_history_size);
        }
        else
            locus_history = 0;
    }

    void RenderElementX::InitMesh(EmitterInstX* emit_inst,
        ParticleX::Data* ptcl_data, int32_t index, RandomX* random) {
        if (ptcl_data->flags & (PARTICLE_EMITTER_LOCAL | PARTICLE_ROTATE_BY_EMITTER))
            base_translation = 0.0f;
        else
            mat4_get_translation(&emit_inst->mat, &base_translation);

        vec3 direction = ptcl_data->direction;
        vec3 scale;
        if (ptcl_data->flags & PARTICLE_EMITTER_LOCAL)
            scale = 1.0f;
        else
            scale = emit_inst->scale * emit_inst->scale_all;

        vec3 position = 0.0f;
        emit_inst->InitMesh(index, scale, position, direction, random);

        if (!(ptcl_data->flags & PARTICLE_EMITTER_LOCAL))
            mat4_transform_vector(&emit_inst->mat_rot, &position, &position);

        base_translation += position;
        translation = base_translation;
        translation_prev = base_translation;

        direction = vec3::normalize(direction
            + random->GetVec3(ptcl_data->direction_random));

        if (!(ptcl_data->flags & PARTICLE_EMITTER_LOCAL))
            mat4_transform_vector(&emit_inst->mat_rot, &direction, &direction);
        base_direction = direction;
        this->direction = direction;

        float_t speed = random->GetFloat(
            ptcl_data->speed_random) + ptcl_data->speed;
        float_t deceleration = random->GetFloat(
            ptcl_data->deceleration_random) + ptcl_data->deceleration;
        base_speed = speed;
        this->speed = speed;
        this->deceleration = max_def(deceleration, 0.0f);

        this->acceleration = ptcl_data->acceleration + ptcl_data->gravity
            + random->GetVec3(ptcl_data->acceleration_random);
    }
}
