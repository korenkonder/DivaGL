/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    LocusHistoryX::LocusHistoryX::Data::Data() :scale() {

    }

    LocusHistoryX::LocusHistoryX(size_t size) {
        data.reserve(size);
    }

    LocusHistoryX::~LocusHistoryX() {

    }

    void LocusHistoryX::Append(RenderElementX* rend_elem, ParticleInstX* ptcl_inst) {
        LocusHistoryX::Data* hist_data = data.data();
        size_t size = data.size();
        vec3 temp = rend_elem->translation;
        if (ptcl_inst->data.data.flags & PARTICLE_EMITTER_LOCAL && ptcl_inst->data.emitter) {
            vec3 emit_trans;
            mat4_get_translation(&ptcl_inst->data.emitter->mat, &emit_trans);
            temp += emit_trans;
            if (ptcl_inst->data.data.flags & PARTICLE_ROTATE_LOCUS)
                mat4_transform_vector(&ptcl_inst->data.emitter->mat_rot, &temp, &temp);
        }

        LocusHistoryX::Data locus_history;
        locus_history.color = rend_elem->color;
        locus_history.translation = temp;
        locus_history.scale = rend_elem->scale_particle.x * rend_elem->scale.x * rend_elem->scale_all;
        if (size < 1)
            data.push_back(locus_history);
        else if (size == 1) {
            locus_history.translation = hist_data->translation;
            if (data.capacity() > 1)
                data.push_back(locus_history);
            hist_data->translation = temp;
        }
        else {
            vec3 temp1 = data[size - 1].translation;

            for (size_t i = size - 1; i > 0; i--)
                hist_data[i].translation = hist_data[i - 1].translation;

            if (size < data.capacity()) {
                locus_history.translation = temp1;
                data.push_back(locus_history);
            }
            hist_data->translation = temp;
        }
    }
}
