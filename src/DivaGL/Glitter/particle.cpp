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

    ParticleX::Data::Data() : flags(), life_time(), life_time_random(), fade_in(), fade_in_random(),
        fade_out(), fade_out_random(), type(), pivot(), draw_type(), z_offset(), speed(), speed_random(),
        deceleration(), deceleration_random(), reflection_coeff(), reflection_coeff_random(), rebound_plane_y(),
        uv_index_type(), uv_index(), frame_step_uv(), uv_index_start(), uv_index_end(), uv_index_count(),
        uv_scroll_add_scale(), uv_scroll_2nd_add_scale(), split_u(), split_v(), blend_mode(), mask_blend_mode(),
        sub_flags(), count(), locus_history_size(), locus_history_size_random(), draw_flags(), emission(),
        tex_hash(), mask_tex_hash(), texture(), mask_texture(), unk0(), unk1(), unk2(), unk3(), unk4(), mesh() {

    }

    ParticleX::ParticleX() {
        version = 0x05;
        data.pivot = PIVOT_MIDDLE_CENTER;
        data.scale = 1.0f;
        data.reflection_coeff = 1.0f;
        data.color = 1.0f;
        data.uv_index = 0;
        data.uv_index_start = 0;
        data.uv_index_end = 1;
        data.uv_scroll_add_scale = 1.0f;
        data.uv_scroll_2nd_add_scale = 1.0f;
        data.split_uv = 1.0f;
        data.split_u = 1;
        data.split_v = 1;
        data.sub_flags = PARTICLE_SUB_USE_CURVE;
        data.blend_mode = PARTICLE_BLEND_TYPICAL;
        data.mask_blend_mode = PARTICLE_BLEND_TYPICAL;
        data.tex_hash = hash_murmurhash_empty;
        data.mask_tex_hash = hash_murmurhash_empty;

        data.mesh.object_set = -1;
        data.mesh.object = -1;
        //data.mesh.mesh_name[0] = 0;
        //data.mesh.sub_mesh = -1;
    }

    ParticleX::~ParticleX() {

    }
}
