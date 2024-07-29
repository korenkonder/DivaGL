/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

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

    void RenderGroup::CreateBuffer(RenderGroup* rend_group, Particle* ptcl) {
        if (ptcl && ptcl->vao && ptcl->vbo && !ptcl->buffer_used) {
            rend_group->max_count = ptcl->max_count;
            rend_group->vao = ptcl->vao;
            rend_group->vbo = ptcl->vbo;
            rend_group->ebo = ptcl->ebo;
            rend_group->draw_list = ptcl->draw_list;
            rend_group->use_own_buffer = false;
            ptcl->buffer_used = true;
        }

        if (rend_group->use_own_buffer && rend_group->max_count > 0) {
            rend_group->buffer = (Buffer*)_operator_new(sizeof(Buffer) * rend_group->max_count);
            if (!rend_group->buffer)
                rend_group->max_count = 0;

            Glitter::CreateBuffer(rend_group->max_count, rend_group->type == PARTICLE_QUAD,
                rend_group->vao, rend_group->vbo, rend_group->ebo);
            rend_group->draw_list = new DrawListData;
        }
    }

    void RenderGroup::DeleteBuffer(RenderGroup* rend_group, Particle* ptcl) {
        if (rend_group->use_own_buffer) {
            DrawListData* draw_list = rend_group->draw_list;
            if (draw_list) {
                draw_list->clear();
                draw_list->shrink_to_fit();
                _operator_delete(draw_list);
                rend_group->draw_list = 0;
            }

            Glitter::DeleteBuffer(rend_group->vao, rend_group->vbo, rend_group->ebo);
        }
        else if (ptcl && ptcl->vao && ptcl->vbo)
            ptcl->buffer_used = false;
    }

    void RenderGroup::InitData(RenderGroup* rend_group) {
        rend_group->blend_mode = PARTICLE_BLEND_ZERO;
        rend_group->mask_blend_mode = PARTICLE_BLEND_ZERO;
        rend_group->texture = 0;
        rend_group->mask_texture = 0;

        rend_group->vao = 0;
        rend_group->ebo = {};
        rend_group->disp = 0;
        rend_group->mat_draw = mat4_identity;
        rend_group->draw_list = 0;
    }

    RenderGroupX::RenderGroupX(ParticleInstX* ptcl_inst) : flags(), type(), draw_type(), pivot(),
        z_offset(), count(), ctrl(), disp(), texture(), mask_texture(), frame(), elements(), max_count(),
        random_ptr(), disp_type(), fog_type(), vao(), vbo(), ebo(), particle(), use_culling(), use_camera() {
        split_u = 1;
        split_v = 1;
        split_uv = 1.0f;
        mat = mat4_identity;
        mat_rot = mat4_identity;
        mat_draw = mat4_identity;
        disp_type = DISP_NORMAL;
        emission = 1.0f;
        use_own_buffer = true;
        blend_mode = PARTICLE_BLEND_TYPICAL;
        mask_blend_mode = PARTICLE_BLEND_TYPICAL;

        object = -1;

        switch (ptcl_inst->data.data.type) {
        case PARTICLE_QUAD:
            if (ptcl_inst->data.data.count > 0)
                count = ptcl_inst->data.data.count;
            else
                count = 2500;
            max_count = 4 * count;
            break;
        case PARTICLE_LINE:
            count = (size_t)ptcl_inst->data.data.locus_history_size
                + ptcl_inst->data.data.locus_history_size_random;
            max_count = count;
            break;
        case PARTICLE_LOCUS:
            count = (size_t)ptcl_inst->data.data.locus_history_size
                + ptcl_inst->data.data.locus_history_size_random;
            max_count = 2 * count;
            break;
        case PARTICLE_MESH:
            count = 1280;
            max_count = 0;
            break;
        default:
            return;
        }

        random_ptr = ptcl_inst->data.random_ptr;
        particle = ptcl_inst;

        elements = new RenderElementX[count];
        if (!elements) {
            count = 0;
            max_count = 0;
            return;
        }

        memset(elements, 0, sizeof(RenderElementX) * count);
        if (!max_count || ptcl_inst->data.data.type == PARTICLE_MESH)
            return;

        bool is_quad = ptcl_inst->data.data.type == PARTICLE_QUAD;

        ParticleX* ptcl = particle->data.particle;
        if (ptcl && ptcl->vao && ptcl->vbo && !ptcl->buffer_used) {
            max_count = ptcl->max_count;
            vao = ptcl->vao;
            vbo = ptcl->vbo;
            ebo = ptcl->ebo;
            use_own_buffer = false;
            ptcl->buffer_used = true;
        }

        if (use_own_buffer)
            Glitter::CreateBuffer(max_count, is_quad, vao, vbo, ebo);

        if (!is_quad)
            draw_list.reserve(count);
    }

    RenderGroupX::~RenderGroupX() {
        DeleteBuffers(false);
    }

    RenderElementX* RenderGroupX::AddElement(RenderElementX* rend_elem) {
        if (!rend_elem)
            rend_elem = elements;

        size_t left_count = count - (rend_elem - elements);
        size_t index = 0;
        if (left_count < 1)
            return 0;

        while (rend_elem->alive) {
            index++;
            rend_elem++;
            if (index >= left_count)
                return 0;
        }

        rend_elem->alive = true;
        ctrl++;
        return rend_elem;
    }

    bool RenderGroupX::CannotDisp() {
        EffectInstX* effect;
        if (!particle)
            return true;
        else if (particle->data.effect)
            return (particle->data.effect->flags & EFFECT_INST_HAS_EXT_ANIM_NON_INIT) != 0;
        else if ((particle = particle->data.parent) && (effect = particle->data.effect))
            return (effect->flags & EFFECT_INST_HAS_EXT_ANIM_NON_INIT) != 0;
        else
            return true;
    }

    void RenderGroupX::CheckUseCamera() {
        if (particle)
            use_camera = particle->GetUseCamera();
        else
            use_camera = false;
    }

    void RenderGroupX::Copy(RenderGroupX* dst) {
        dst->flags = flags;
        dst->type = type;
        dst->draw_type = draw_type;
        dst->blend_mode = blend_mode;
        dst->mask_blend_mode = mask_blend_mode;
        dst->pivot = pivot;
        dst->split_u = split_u;
        dst->split_v = split_v;
        dst->split_uv = split_uv;
        dst->z_offset = z_offset;
        if (dst->count > count)
            dst->count = count;
        dst->ctrl = ctrl;
        dst->texture = texture;
        dst->mask_texture = mask_texture;
        dst->frame = frame;
        dst->mat = mat;
        dst->mat_rot = mat_rot;
        dst->mat_draw = mat_draw;
        if (dst->count == count) {
            memmove(dst->elements, elements, sizeof(RenderElementX) * dst->count);
            dst->ctrl = ctrl;
        }
    }

    void RenderGroupX::Ctrl(float_t delta_frame, bool copy_mats) {
        if (!particle)
            return;

        ParticleX::Data* ptcl_data = &particle->data.data;
        blend_mode = ptcl_data->blend_mode;
        mask_blend_mode = ptcl_data->mask_blend_mode;
        texture = ptcl_data->texture;
        mask_texture = ptcl_data->mask_texture;
        object.set_id = (uint16_t)ptcl_data->mesh.object_set;
        object.id = (uint16_t)ptcl_data->mesh.object;
        split_u = ptcl_data->split_u;
        split_v = ptcl_data->split_v;
        split_uv = ptcl_data->split_uv;
        type = ptcl_data->type;
        draw_type = ptcl_data->draw_type;
        z_offset = ptcl_data->z_offset;
        pivot = ptcl_data->pivot;
        flags = ptcl_data->flags;

        if (copy_mats && particle->data.emitter) {
            EmitterInstX* emitter = particle->data.emitter;
            mat = emitter->mat;
            mat_rot = emitter->mat_rot;
        }

        for (size_t ctrl = this->ctrl, i = 0; ctrl; i++) {
            if (!elements[i].alive)
                continue;

            CtrlParticle(&elements[i], delta_frame);
            ctrl--;
        }
        frame += delta_frame;
    }

    void RenderGroupX::CtrlParticle(RenderElementX* rend_elem, float_t delta_frame) {
        random_ptr->SetStep(rend_elem->step);
        if (!particle || (particle->data.data.flags & PARTICLE_LOOP
            && particle->HasEnded(false)) || rend_elem->frame >= rend_elem->life_time) {
            rend_elem->alive = false;
            if (rend_elem->locus_history) {
                delete rend_elem->locus_history;
                rend_elem->locus_history = 0;
            }
            ctrl--;
            return;
        }

        particle->AccelerateParticle(rend_elem, delta_frame, random_ptr);

        if (particle->data.data.draw_type == DIRECTION_PARTICLE_ROTATION
            || particle->data.data.type == PARTICLE_MESH)
            rend_elem->rotation += rend_elem->rotation_add * delta_frame;
        else
            rend_elem->rotation.z += rend_elem->rotation_add.z * delta_frame;

        vec2 uv_scroll = particle->data.data.uv_scroll_add
            * (particle->data.data.uv_scroll_add_scale * delta_frame);
        if (uv_scroll.x != 0.0f)
            rend_elem->uv_scroll.x = fmodf(rend_elem->uv_scroll.x + uv_scroll.x, 1.0f);
        if (uv_scroll.y != 0.0f)
            rend_elem->uv_scroll.y = fmodf(rend_elem->uv_scroll.y + uv_scroll.y, 1.0f);

        if (particle->data.data.sub_flags & PARTICLE_SUB_UV_2ND_ADD) {
            vec2 uv_scroll_2nd = particle->data.data.uv_scroll_2nd_add
                * (particle->data.data.uv_scroll_2nd_add_scale * delta_frame);
            if (uv_scroll_2nd.x != 0.0f)
                rend_elem->uv_scroll_2nd.x = fmodf(rend_elem->uv_scroll_2nd.x + uv_scroll_2nd.x, 1.0f);
            if (uv_scroll_2nd.y != 0.0f)
                rend_elem->uv_scroll_2nd.y = fmodf(rend_elem->uv_scroll_2nd.y + uv_scroll_2nd.y, 1.0f);
        }

        particle->StepUVParticle(rend_elem, delta_frame, random_ptr);
        rend_elem->disp = true;
        rend_elem->color = -1.0f;

        bool disp = true;
        float_t color_scale = -1.0f;
        if (particle->data.data.sub_flags & PARTICLE_SUB_USE_CURVE)
            disp = particle->GetValue(rend_elem, rend_elem->frame, random_ptr, &color_scale);

        if (particle->data.data.draw_type == DIRECTION_PARTICLE_ROTATION
            || fabsf(rend_elem->rotation.z) <= 0.000001f) {
            rend_elem->rot_z_cos = 1.0f;
            rend_elem->rot_z_sin = 0.0f;
        }
        else {
            rend_elem->rot_z_cos = cosf(rend_elem->rotation.z);
            rend_elem->rot_z_sin = sinf(rend_elem->rotation.z);
        }

        if (disp)
            particle->GetColor(rend_elem, color_scale);

        if (particle->data.data.type == PARTICLE_LOCUS)
            rend_elem->locus_history->Append(rend_elem, particle);

        rend_elem->frame += delta_frame;
        if (particle->data.data.flags & PARTICLE_LOOP && rend_elem->frame >= rend_elem->life_time)
            rend_elem->frame -= rend_elem->life_time;
    }

    void RenderGroupX::DeleteBuffers(bool a2) {
        ParticleX* ptcl = 0;
        if (particle) {
            ptcl = particle->data.particle;
            if (!a2)
                particle->data.render_group = 0;
            particle = 0;
        }

        if (use_own_buffer)
            Glitter::DeleteBuffer(vao, vbo, ebo);
        else if (ptcl && ptcl->vao && ptcl->vbo)
            ptcl->buffer_used = false;

        if (!a2 && elements) {
            Free();
            free_def(elements);
        }
    }

    void RenderGroupX::Emit(ParticleX::Data* ptcl_data,
        EmitterInstX* emit_inst, int32_t dup_count, int32_t count, float_t frame) {
        RenderElementX* element;
        int64_t i;
        int32_t index;
        uint8_t step;

        step = emit_inst->RandomGetStep();
        random_ptr->SetStep(1);
        for (element = 0, i = 0; i < dup_count; i++)
            for (index = 0; index < count; index++, element++) {
                element = AddElement(element);
                if (!element)
                    break;

                emit_inst->RandomStepValue();
                element->frame = frame;
                particle->EmitParticle(element, emit_inst, ptcl_data, index, step, random_ptr);
            }
    }

    void RenderGroupX::Free() {
        if (count <= 0) {
            ctrl = 0;
            return;
        }

        RenderElementX* elem = elements;
        for (size_t i = count; i; i--, elem++) {
            elem->alive = false;
            if (elem->locus_history) {
                delete elem->locus_history;
                elem->locus_history = 0;
            }
        }
        ctrl = 0;
    }

    void RenderGroupX::FreeData() {
        DeleteBuffers(true);
    }

    bool RenderGroupX::GetEmitterScale(vec3& emitter_scale) {
        if (particle) {
            Glitter::EmitterInstX* emit_inst = particle->data.emitter;
            if (emit_inst) {
                emitter_scale = emit_inst->scale * emit_inst->scale_all;
                return emitter_scale.x > 0.000001f || emitter_scale.y > 0.000001f || emitter_scale.z > 0.000001f;
            }
        }

        emitter_scale = 1.0f;
        return false;
    }

    bool RenderGroupX::GetExtAnimScale(vec3* ext_anim_scale, float_t* ext_scale) {
        if (particle)
            particle->GetExtAnimScale(ext_anim_scale, ext_scale);
        return false;
    }

    bool RenderGroupX::HasEnded() {
        if (particle)
            return particle->HasEnded(true);
        return true;
    }

    mat4 RenderGroupX::RotateMeshToEmitPosition(RenderGroupX* rend_group,
        RenderElementX* rend_elem, vec3* vec, vec3* trans) {
        vec3 vec2;
        mat4_get_translation(&rend_group->mat, &vec2);
        vec2 -= *trans;

        if (vec3::length_squared(vec2) < 0.000001f)
            vec2 = { 0.0f, 1.0f, 0.0f };
        else
            vec2 = vec3::normalize(vec2);

        vec3 vec1 = *vec;

        vec3 axis;
        float_t angle;
        axis_angle_from_vectors(&axis, &angle, &vec1, &vec2);

        mat4 mat = mat4_identity;
        mat4_mul_rotation(&mat, &axis, angle, &mat);
        return mat;
    }

    mat4 RenderGroupX::RotateMeshToPrevPosition(RenderGroupX* rend_group,
        RenderElementX* rend_elem, vec3* vec, vec3* trans) {
        vec3 vec2 = rend_elem->translation - rend_elem->translation_prev;

        if (vec3::length_squared(vec2) < 0.000001f)
            vec2 = vec3::normalize(rend_elem->base_direction);
        else
            vec2 = vec3::normalize(vec2);

        vec3 vec1 = *vec;

        vec3 axis;
        float_t angle;
        axis_angle_from_vectors(&axis, &angle, &vec1, &vec2);

        mat4 mat = mat4_identity;
        mat4_mul_rotation(&mat, &axis, angle, &mat);
        return mat;
    }

    mat4 RenderGroupX::RotateToEmitPosition(RenderGroupX* rend_group,
        RenderElementX* rend_elem, vec3* vec) {
        vec3 vec2;
        mat4_get_translation(&rend_elem->mat, &vec2);
        vec2 -= rend_elem->translation;

        if (vec3::length_squared(vec2) < 0.000001f)
            vec2 = { 0.0f, 1.0f, 0.0f };
        else
            vec3::normalize(vec2);

        vec3 vec1 = *vec;

        vec3 axis;
        float_t angle;
        axis_angle_from_vectors(&axis, &angle, &vec1, &vec2);

        mat4 mat = mat4_identity;
        mat4_mul_rotation(&mat, &axis, angle, &mat);
        return mat;
    }

    mat4 RenderGroupX::RotateToPrevPosition(RenderGroupX* rend_group,
        RenderElementX* rend_elem, vec3* vec) {
        vec3 vec2 = rend_elem->translation - rend_elem->translation_prev;

        if (vec3::length_squared(vec2) < 0.000001f)
            vec2 = vec3::normalize(rend_elem->base_direction);
        else
            vec2 = vec3::normalize(vec2);

        vec3 vec1 = *vec;

        vec3 axis;
        float_t angle;
        axis_angle_from_vectors(&axis, &angle, &vec1, &vec2);

        mat4 mat = mat4_identity;
        mat4_mul_rotation(&mat, &axis, angle, &mat);
        return mat;
    }
}
