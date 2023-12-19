/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../gl_state.hpp"
#include "../render_context.hpp"
#include "../shader_ft.hpp"
#include "../wrap.hpp"

namespace Glitter {
    void RenderScene::CalcDisp(RenderScene* rend_sc) {
        rend_sc->disp_quad = 0;
        rend_sc->disp_line = 0;
        rend_sc->disp_locus = 0;

        bool (FASTCALL * RenderGroup_CannotDisp)(RenderGroup * rend_group)
            = (bool(FASTCALL*)(RenderGroup * rend_group))0x00000001403A7120;

        for (RenderGroup*& i : rend_sc->render_groups) {
            if (!i)
                continue;

            RenderGroup* rend_group = i;
            if (RenderGroup_CannotDisp(rend_group))
                continue;

            CalcDisp(rend_sc, rend_group);
        }
    }

    void RenderScene::CalcDisp(RenderScene* rend_sc, RenderGroup* rend_group) {
        rend_group->disp = 0;
        switch (rend_group->type) {
        case PARTICLE_QUAD:
            if (!rend_group->vao)
                return;

            CalcDispQuad(rend_group);
            rend_sc->disp_quad += (uint32_t)rend_group->disp;
            break;
        case PARTICLE_LINE:
            if (!rend_group->vao)
                return;

            CalcDispLine(rend_group);
            rend_sc->disp_line += (uint32_t)rend_group->disp;
            break;
        case PARTICLE_LOCUS:
            if (!rend_group->vao)
                return;

            CalcDispLocus(rend_group);
            rend_sc->disp_locus += (uint32_t)rend_group->disp;
            break;
        }
    }

    void RenderScene::CalcDispLine(RenderGroup* rend_group) {
        if (!rend_group->elements || rend_group->vbo.IsNull() || rend_group->ctrl < 1)
            return;

        size_t count = 0;
        RenderElement* elem = rend_group->elements;
        for (size_t i = rend_group->ctrl; i > 0; i--, elem++) {
            if (!elem->alive)
                continue;

            if (elem->locus_history) {
                size_t length = elem->locus_history->sub.vec.size();
                if (length > 1)
                    count += length;
            }
        }

        if (!count || count > rend_group->max_count)
            return;

        bool has_scale = false;
        vec3 scale = 0.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            mat4 mat;
            mat4_transpose(&rend_group->mat, &mat);
            mat4_get_scale(&mat, &scale);
            scale -= 1.0f;
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;
            mat4_normalize_rotation(&mat, &mat);
            mat4_transpose(&mat, &rend_group->mat_draw);
        }
        else
            rend_group->mat_draw = mat4_identity;

        Buffer* buf = (Buffer*)rend_group->vbo.MapMemory();
        if (!buf)
            return;

        elem = rend_group->elements;
        size_t disp = 0;
        rend_group->draw_list->clear();
        for (size_t i = rend_group->ctrl, index = 0; i > 0; elem++) {
            if (!elem->alive)
                continue;

            i--;
            LocusHistory* hist = elem->locus_history;
            if (elem->color.w < 0.01f || !hist || hist->sub.vec.size() < 2)
                continue;

            size_t j = 0;
            if (has_scale)
                for (LocusHistory::Data& hist_data : hist->sub.vec) {
                    vec3& pos = hist_data.translation;
                    buf->position = pos + (pos - elem->base_translation) * scale;
                    buf->uv[0] = 0.0f;
                    buf->uv[1] = 0.0f;
                    buf->color = hist_data.color;
                    j++;
                    buf++;
                }
            else
                for (LocusHistory::Data& hist_data : hist->sub.vec) {
                    buf->position = hist_data.translation;
                    buf->uv[0] = 0.0f;
                    buf->uv[1] = 0.0f;
                    buf->color = hist_data.color;
                    j++;
                    buf++;
                }

            if (j > 0) {
                disp += j;
                rend_group->draw_list->push_back({ (GLint)index, (GLsizei)j });
                index += j;
            }
        }
        rend_group->disp = disp;

        rend_group->vbo.UnmapMemory();
    }

    void RenderScene::CalcDispLocus(RenderGroup* rend_group) {
        if (!rend_group->elements || rend_group->vbo.IsNull() || rend_group->ctrl < 1)
            return;

        size_t count = 0;
        RenderElement* elem = rend_group->elements;
        for (size_t i = rend_group->ctrl; i > 0; i--, elem++) {
            if (!elem->alive)
                continue;

            if (elem->locus_history) {
                LocusHistory* hist = elem->locus_history;
                size_t length = elem->locus_history->sub.vec.size();
                if (length > 1)
                    count += 2 * length;
            }
        }

        if (!count || count > rend_group->max_count)
            return;

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data->view_matrix, &cam_view);
        mat4_transpose(&camera_data->inv_view_matrix, &cam_inv_view);

        vec3 x_vec = { 1.0f, 0.0f, 0.0f };
        if (rend_group->flags & PARTICLE_LOCAL) {
            mat4 mat;
            mat4_transpose(&rend_group->mat, &mat);
            mat4_mul(&cam_inv_view, &mat, &mat);
            mat4_mul(&cam_view, &mat, &mat);
            mat4_mul(&mat, &cam_inv_view, &mat);
            mat4_transpose(&mat, &rend_group->mat_draw);
        }
        else
            rend_group->mat_draw = mat4_identity;

        bool has_scale = false;
        vec3 scale = 0.0f;
        mat3 model_mat;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            mat4 mat;
            mat4_transpose(&rend_group->mat, &mat);
            mat4_get_scale(&mat, &scale);
            if (rend_group->flags & PARTICLE_SCALE)
                x_vec.x = scale.x;
            scale -= 1.0f;
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;

            mat4 mat_rot;
            mat4_transpose(&rend_group->mat_rot, &mat_rot);
            mat4_transform_vector(&mat_rot, &scale, &scale);

            mat4_to_mat3(&mat, &model_mat);
            mat3_normalize_rotation(&model_mat, &model_mat);
        }
        else
            model_mat = mat3_identity;

        mat3 cam_inv_view_mat3;
        mat4_to_mat3(&cam_inv_view, &cam_inv_view_mat3);
        mat3_invert(&cam_inv_view_mat3, &cam_inv_view_mat3);
        mat3_transform_vector(&cam_inv_view_mat3, &x_vec, &x_vec);

        Buffer* buf = (Buffer*)rend_group->vbo.MapMemory();
        if (!buf)
            return;

        elem = rend_group->elements;
        vec2 split_uv = rend_group->split_uv;
        Pivot pivot = rend_group->pivot;
        size_t disp = 0;
        rend_group->draw_list->clear();
        for (size_t i = rend_group->ctrl, index = 0; i > 0; elem++) {
            if (!elem->alive)
                continue;

            i--;
            LocusHistory* hist = elem->locus_history;
            if (elem->color.w < 0.01f || !hist || hist->sub.vec.size() < 2)
                continue;

            float_t uv_u = elem->uv.x + elem->uv_scroll.x;
            float_t uv_u_2nd = elem->uv.x + elem->uv_scroll.x + split_uv.x;
            float_t uv_v_2nd = elem->uv.y + elem->uv_scroll.y + split_uv.y;
            float_t uv_v_scale = split_uv.y / (float_t)(hist->sub.vec.size() - 1);

            uv_v_2nd = 1.0f - uv_v_2nd;

            size_t len = elem->locus_history->sub.vec.size();
            size_t j = 0;
            if (has_scale)
                for (LocusHistory::Data& hist_data : hist->sub.vec) {
                    vec3 pos = hist_data.translation;
                    vec3 pos_diff = (pos - elem->base_translation) * scale;
                    mat3_transform_vector(&model_mat, &pos_diff, &pos_diff);
                    pos += pos_diff;

                    float_t v00;
                    float_t v01;
                    CalcDispLocusSetPivot(pivot,
                        hist_data.scale * elem->scale.x * elem->scale_all,
                        v00, v01);

                    buf[0].position = pos + x_vec * v00;
                    buf[0].uv[0].x = uv_u;
                    buf[0].uv[0].y = uv_v_2nd + (float_t)j * uv_v_scale;
                    buf[0].uv[1] = buf[0].uv[0];
                    buf[0].color = hist_data.color;

                    buf[1].position = pos + x_vec * v01;
                    buf[1].uv[0].x = uv_u_2nd;
                    buf[1].uv[0].y = uv_v_2nd + (float_t)j * uv_v_scale;
                    buf[1].uv[1] = buf[1].uv[0];
                    buf[1].color = hist_data.color;
                    j++;
                    buf += 2;
                }
            else
                for (LocusHistory::Data& hist_data : hist->sub.vec) {
                    vec3 pos = hist_data.translation;

                    float_t v00;
                    float_t v01;
                    CalcDispLocusSetPivot(pivot,
                        hist_data.scale * elem->scale.x * elem->scale_all,
                        v00, v01);

                    buf[0].position = pos + x_vec * v00;
                    buf[0].uv[0].x = uv_u;
                    buf[0].uv[0].y = uv_v_2nd + (float_t)j * uv_v_scale;
                    buf[0].uv[1] = buf[0].uv[0];
                    buf[0].color = hist_data.color;

                    buf[1].position = pos + x_vec * v01;
                    buf[1].uv[0].x = uv_u_2nd;
                    buf[1].uv[0].y = uv_v_2nd + (float_t)j * uv_v_scale;
                    buf[1].uv[1] = buf[1].uv[0];
                    buf[1].color = hist_data.color;
                    j++;
                    buf += 2;
                }

            if (j > 0) {
                disp += j;
                rend_group->draw_list->push_back({ (GLint)index, (GLsizei)(j * 2) });
                index += j * 2;
            }
        }
        rend_group->disp = disp;

        rend_group->vbo.UnmapMemory();
    }

    void RenderScene::CalcDispQuad(RenderGroup* rend_group) {
        if (!rend_group->elements || rend_group->vbo.IsNull() || rend_group->ctrl < 1)
            return;

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data->view_matrix, &cam_view);
        mat4_transpose(&camera_data->inv_view_matrix, &cam_inv_view);

        mat4 model_mat;
        mat4 view_mat;
        mat4 inv_view_mat;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            mat4_transpose(&rend_group->mat, &model_mat);
            mat4_normalize_rotation(&model_mat, &view_mat);
            mat4_mul(&view_mat, &cam_view, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);
        }
        else {
            model_mat = mat4_identity;
            view_mat = cam_view;
            inv_view_mat = cam_inv_view;
        }

        if (rend_group->flags & PARTICLE_LOCAL) {
            mat4 mat;
            mat4_transpose(&rend_group->mat, &mat);
            mat4_mul(&inv_view_mat, &mat, &inv_view_mat);
            mat4_mul(&view_mat, &inv_view_mat, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);
        }

        mat4_mul(&view_mat, &cam_inv_view, &rend_group->mat_draw);
        mat4_transpose(&rend_group->mat_draw, &rend_group->mat_draw);

        mat4 dir_mat;
        switch (rend_group->draw_type) {
        case DIRECTION_BILLBOARD:
            mat4_clear_trans(&model_mat, &dir_mat);
            mat4_mul(&dir_mat, &inv_view_mat, &dir_mat);
            mat4_clear_trans(&dir_mat, &dir_mat);
            break;
        case DIRECTION_EMITTER_DIRECTION:
            mat4_transpose(&rend_group->mat_rot, &dir_mat);
            mat4_clear_trans(&dir_mat, &dir_mat);
            break;
        case DIRECTION_PREV_POSITION:
        case DIRECTION_EMIT_POSITION:
        case DIRECTION_PREV_POSITION_DUP:
            if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
                mat4_transpose(&rend_group->mat_rot, &dir_mat);
                mat4_clear_trans(&dir_mat, &dir_mat);
            }
            else
                dir_mat = mat4_identity;
            break;
        case DIRECTION_Y_AXIS:
            mat4_rotate_y((float_t)-M_PI_2, &dir_mat);
            break;
        case DIRECTION_X_AXIS:
            mat4_rotate_x((float_t)-M_PI_2, &dir_mat);
            break;
        case DIRECTION_Z_AXIS:
            mat4_rotate_z((float_t)-M_PI_2, &dir_mat);
            break;
        case DIRECTION_BILLBOARD_Y_AXIS:
            mat4_rotate_y(camera_data->rotation.y, &dir_mat);
            break;
        default:
            dir_mat = mat4_identity;
            break;
        }

        switch (rend_group->draw_type) {
        case DIRECTION_PREV_POSITION:
        case DIRECTION_EMIT_POSITION:
        case DIRECTION_PREV_POSITION_DUP:
            CalcDispQuadDirectionRotation(rend_group, &model_mat, &dir_mat);
            break;
        default:
            CalcDispQuadNormal(rend_group, &model_mat, &dir_mat);
            break;
        }
    }

    void RenderScene::CalcDispQuadDirectionRotation(
        RenderGroup* rend_group, mat4* model_mat, mat4* dir_mat) {
        mat4 inv_model_mat;
        mat4_invert(model_mat, &inv_model_mat);
        mat4_clear_trans(&inv_model_mat, &inv_model_mat);

        vec3 x_vec_base = { 1.0f, 0.0f, 0.0f };
        vec3 y_vec_base = { 0.0f, 1.0f, 0.0f };

        mat4_transform_vector(&inv_model_mat, &x_vec_base, &x_vec_base);
        mat4_transform_vector(&inv_model_mat, &y_vec_base, &y_vec_base);

        vec3 up_vec;
        mat4(*rotate_func)(RenderGroup*, RenderElement*, vec3*);
        if (rend_group->draw_type == DIRECTION_EMIT_POSITION) {
            up_vec = { 0.0f, 0.0f, 1.0f };
            rotate_func = RenderGroup::RotateToEmitPosition;
        }
        else {
            up_vec = { 0.0f, 1.0f, 0.0f };
            rotate_func = RenderGroup::RotateToPrevPosition;
        }

        vec3 scale;
        mat4_get_scale(model_mat, &scale);

        Buffer* buf = (Buffer*)rend_group->vbo.MapMemory();
        if (!buf)
            return;

        bool use_scale = rend_group->flags & PARTICLE_SCALE ? true : false;
        RenderElement* elem = rend_group->elements;
        vec2 split_uv = rend_group->split_uv;
        Pivot pivot = rend_group->pivot;
        size_t disp = 0;
        for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
            j_max = min_def(i, j_max);
            for (size_t j = j_max; j > 0; elem++) {
                if (!elem->alive)
                    continue;
                j--;

                if (elem->color.w < 0.01f)
                    continue;

                vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                    continue;

                vec3 pos = elem->translation;

                float_t v00;
                float_t v01;
                float_t v10;
                float_t v11;
                CalcDispQuadSetPivot(pivot,
                    scale_particle.x, scale_particle.y,
                    v00, v01, v10, v11);

                vec2 pos_add[4];
                vec2 uv_add[4];
                pos_add[0].x = v00;
                pos_add[0].y = v11;
                uv_add[0].x = 0.0f;
                uv_add[0].y = 0.0f;
                pos_add[1].x = v00;
                pos_add[1].y = v10;
                uv_add[1].x = 0.0f;
                uv_add[1].y = split_uv.y;
                pos_add[2].x = v01;
                pos_add[2].y = v10;
                uv_add[2].x = split_uv.x;
                uv_add[2].y = split_uv.y;
                pos_add[3].x = v01;
                pos_add[3].y = v11;
                uv_add[3].x = split_uv.x;
                uv_add[3].y = 0.0f;

                vec2 uv = elem->uv + elem->uv_scroll;

                mat4 mat = rotate_func(rend_group, elem, &up_vec);
                mat4_clear_trans(&mat, &mat);
                mat4_mul(dir_mat, &mat, &mat);
                mat4_clear_trans(&mat, &mat);

                vec3 x_vec;
                mat4_transform_vector(&mat, &x_vec_base, &x_vec);
                vec3 y_vec;
                mat4_transform_vector(&mat, &y_vec_base, &y_vec);

                if (use_scale) {
                    x_vec *= scale;
                    y_vec *= scale;
                }

                float_t rot_z_cos = elem->rot_z_cos;
                float_t rot_z_sin = elem->rot_z_sin;
                for (int32_t k = 0; k < 4; k++, buf++) {
                    vec3 x_vec_rot = x_vec * (pos_add[k].x * rot_z_cos - pos_add[k].y * rot_z_sin);
                    vec3 y_vec_rot = y_vec * (pos_add[k].x * rot_z_sin + pos_add[k].y * rot_z_cos);
                    buf->position = pos + (x_vec_rot + y_vec_rot);
                    buf->uv[0] = uv + uv_add[k];
                    buf->uv[1] = buf->uv[0];
                    buf->color = elem->color;
                }
                disp++;
            }
        }
        rend_group->disp = disp;

        rend_group->vbo.UnmapMemory();
    }

    void RenderScene::CalcDispQuadNormal(
        RenderGroup* rend_group, mat4* model_mat, mat4* dir_mat) {
        mat4 inv_model_mat;
        mat4_invert(model_mat, &inv_model_mat);
        mat4_clear_trans(&inv_model_mat, &inv_model_mat);

        vec3 x_vec = { 1.0f, 0.0f, 0.0f };
        vec3 y_vec = { 0.0f, 1.0f, 0.0f };
        bool use_z_offset = false;
        vec3 dist_to_cam = 0.0f;
        mat4 z_offset_inv_mat = mat4_identity;
        if (fabsf(rend_group->z_offset) > 0.000001f) {
            use_z_offset = true;
            mat4_get_translation(model_mat, &dist_to_cam);
            dist_to_cam = camera_data->view_point - dist_to_cam;
            if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
                mat4_normalize_rotation(model_mat, &z_offset_inv_mat);
                mat4_invert(&z_offset_inv_mat, &z_offset_inv_mat);
            }
        }

        bool has_scale = false;
        bool emitter_local = false;
        vec3 scale = 0.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            mat4_get_scale(model_mat, &scale);
            if (rend_group->flags & PARTICLE_SCALE) {
                x_vec.x = scale.x;
                y_vec.y = scale.y;
            }
            scale -= 1.0f;
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;

            mat4 mat_rot;
            mat4_transpose(&rend_group->mat_rot, &mat_rot);
            mat4_transform_vector(&mat_rot, &scale, &scale);
            emitter_local = true;
        }

        vec3 ext_anim_scale;
        float_t some_scale = 0.0f;
        bool (FASTCALL * RenderGroup_GetExtAnimScale)(RenderGroup * rend_group,
            vec3 * ext_anim_scale, float_t * some_scale) = (bool(FASTCALL*)(RenderGroup * rend_group,
                vec3 * ext_anim_scale, float_t * some_scale))0x00000001403A6800;
        if (RenderGroup_GetExtAnimScale(rend_group, &ext_anim_scale, &some_scale)) {
            if (some_scale <= 0.0f)
                some_scale = 1.0f;

            ext_anim_scale += some_scale;
            x_vec.x *= ext_anim_scale.x;
            y_vec.y *= ext_anim_scale.y;
        }

        if (rend_group->draw_type != DIRECTION_BILLBOARD) {
            mat4_transform_vector(dir_mat, &x_vec, &x_vec);
            mat4_transform_vector(dir_mat, &y_vec, &y_vec);
        }
        else
            mat4_mul(&inv_model_mat, dir_mat, &inv_model_mat);

        mat4_transform_vector(&inv_model_mat, &x_vec, &x_vec);
        mat4_transform_vector(&inv_model_mat, &y_vec, &y_vec);

        Buffer* buf = (Buffer*)rend_group->vbo.MapMemory();
        if (!buf)
            return;

        RenderElement* elem = rend_group->elements;
        vec2 split_uv = rend_group->split_uv;
        Pivot pivot = rend_group->pivot;
        float_t z_offset = rend_group->z_offset;
        size_t disp = 0;
        if (rend_group->draw_type == DIRECTION_PARTICLE_ROTATION)
            for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
                j_max = min_def(i, j_max);
                for (size_t j = j_max; j > 0; elem++) {
                    if (!elem->alive)
                        continue;
                    j--;

                    if (elem->color.w < 0.01f)
                        continue;

                    vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                    if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                        continue;

                    vec3 pos = elem->translation;
                    if (has_scale) {
                        vec3 pos_diff = (pos - elem->base_translation) * scale;
                        mat4_transform_vector(&inv_model_mat, &pos_diff, &pos_diff);
                        pos += pos_diff;
                    }

                    float_t v00;
                    float_t v01;
                    float_t v11;
                    float_t v10;
                    CalcDispQuadSetPivot(pivot,
                        scale_particle.x, scale_particle.y,
                        v00, v01, v10, v11);

                    vec2 pos_add[4];
                    vec2 uv_add[4];
                    pos_add[0].x = v00;
                    pos_add[0].y = v11;
                    uv_add[0].x = 0.0f;
                    uv_add[0].y = 0.0f;
                    pos_add[1].x = v00;
                    pos_add[1].y = v10;
                    uv_add[1].x = 0.0f;
                    uv_add[1].y = split_uv.y;
                    pos_add[2].x = v01;
                    pos_add[2].y = v10;
                    uv_add[2].x = split_uv.x;
                    uv_add[2].y = split_uv.y;
                    pos_add[3].x = v01;
                    pos_add[3].y = v11;
                    uv_add[3].x = split_uv.x;
                    uv_add[3].y = 0.0f;

                    vec2 uv = elem->uv + elem->uv_scroll;

                    if (use_z_offset) {
                        vec3 z_offset_dir = vec3::normalize(dist_to_cam - pos);
                        if (emitter_local)
                            mat4_transform_vector(&z_offset_inv_mat, &z_offset_dir, &z_offset_dir);
                        pos += z_offset_dir * z_offset;
                    }

                    mat3 ptc_rot;
                    mat3_rotate_zyx(elem->rotation.x, elem->rotation.y, elem->rotation.z, &ptc_rot);
                    for (int32_t k = 0; k < 4; k++, buf++) {
                        vec3 xy_vec_rot;
                        xy_vec_rot.x = x_vec.x * pos_add[k].x;
                        xy_vec_rot.y = y_vec.y * pos_add[k].y;
                        xy_vec_rot.z = 0.0f;
                        mat3_transform_vector(&ptc_rot, &xy_vec_rot, &xy_vec_rot);
                        buf->position = pos + xy_vec_rot;
                        buf->uv[0] = uv + uv_add[k];
                        buf->uv[1] = buf->uv[0];
                        buf->color = elem->color;
                    }
                    disp++;
                }
            }
        else
            for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
                j_max = min_def(i, j_max);
                for (size_t j = j_max; j > 0; elem++) {
                    if (!elem->alive)
                        continue;
                    j--;

                    if (elem->color.w < 0.01f)
                        continue;

                    vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                    if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                        continue;

                    vec3 pos = elem->translation;
                    if (has_scale) {
                        vec3 pos_diff = (pos - elem->base_translation) * scale;
                        mat4_transform_vector(&inv_model_mat, &pos_diff, &pos_diff);
                        pos += pos_diff;
                    }

                    float_t v00;
                    float_t v01;
                    float_t v11;
                    float_t v10;
                    CalcDispQuadSetPivot(pivot,
                        scale_particle.x, scale_particle.y,
                        v00, v01, v10, v11);

                    vec2 pos_add[4];
                    vec2 uv_add[4];
                    pos_add[0].x = v00;
                    pos_add[0].y = v11;
                    uv_add[0].x = 0.0f;
                    uv_add[0].y = 0.0f;
                    pos_add[1].x = v00;
                    pos_add[1].y = v10;
                    uv_add[1].x = 0.0f;
                    uv_add[1].y = split_uv.y;
                    pos_add[2].x = v01;
                    pos_add[2].y = v10;
                    uv_add[2].x = split_uv.x;
                    uv_add[2].y = split_uv.y;
                    pos_add[3].x = v01;
                    pos_add[3].y = v11;
                    uv_add[3].x = split_uv.x;
                    uv_add[3].y = 0.0f;

                    vec2 uv = elem->uv + elem->uv_scroll;

                    if (use_z_offset) {
                        vec3 z_offset_dir = vec3::normalize(dist_to_cam - pos);
                        if (emitter_local)
                            mat4_transform_vector(&z_offset_inv_mat, &z_offset_dir, &z_offset_dir);
                        pos += z_offset_dir * z_offset;
                    }

                    float_t rot_z_cos = elem->rot_z_cos;
                    float_t rot_z_sin = elem->rot_z_sin;
                    for (int32_t k = 0; k < 4; k++, buf++) {
                        vec3 x_vec_rot = x_vec * (rot_z_cos * pos_add[k].x - rot_z_sin * pos_add[k].y);
                        vec3 y_vec_rot = y_vec * (rot_z_sin * pos_add[k].x + rot_z_cos * pos_add[k].y);
                        buf->position = pos + (x_vec_rot + y_vec_rot);
                        buf->uv[0] = uv + uv_add[k];
                        buf->uv[1] = buf->uv[0];
                        buf->color = elem->color;
                    }
                    disp++;
                }
            }
        rend_group->disp = disp;

        rend_group->vbo.UnmapMemory();
    }

    void RenderScene::Disp(RenderScene* rend_sc, RenderGroup* rend_group) {
        switch (rend_group->type) {
        case PARTICLE_QUAD:
        case PARTICLE_LINE:
        case PARTICLE_LOCUS:
            break;
        default:
            return;
        }

        if (!rend_group->vao || rend_group->disp < 1)
            return;

        mat4 cam_view;
        mat4 cam_projection;
        mat4_transpose(&camera_data->view_matrix, &cam_view);
        mat4_transpose(&camera_data->projection_matrix, &cam_projection);

        mat4 mat;
        mat4_transpose(&rend_group->mat_draw, &mat);
        mat4_mul(&mat, &cam_view, &mat);
        mat4_mul(&mat, &cam_projection, &mat);

        float_t emission = 1.0f;
        if (rend_group->flags & PARTICLE_EMISSION || rend_group->blend_mode == PARTICLE_BLEND_TYPICAL)
            emission = rend_group->emission;

        glitter_batch_shader_data shader_data = {};
        mat4_transpose(&mat, &mat);
        shader_data.g_mvp[0] = mat.row0;
        shader_data.g_mvp[1] = mat.row1;
        shader_data.g_mvp[2] = mat.row2;
        shader_data.g_mvp[3] = mat.row3;
        shader_data.g_glitter_blend_color = 1.0f;
        shader_data.g_state_material_diffuse = 0.0f;
        shader_data.g_state_material_emission = { emission, emission, emission, 1.0f };
        rctx->glitter_batch_ubo.WriteMemory(shader_data);

        GLenum blend_src = GL_SRC_ALPHA;
        GLenum blend_dst = GL_ONE_MINUS_SRC_ALPHA;
        switch (rend_group->blend_mode) {
        case PARTICLE_BLEND_ADD:
            blend_src = GL_SRC_ALPHA;
            blend_dst = GL_ONE;
            break;
        case PARTICLE_BLEND_MULTIPLY:
            blend_src = GL_ZERO;
            blend_dst = GL_SRC_COLOR;
            break;
        }

        gl_state_enable_blend(true);
        gl_state_set_blend_func(blend_src, blend_dst, true);
        gl_state_set_blend_equation(GL_FUNC_ADD, true);

        GLuint texture = rctx->empty_texture_2d;
        GLuint mask_texture = rctx->empty_texture_2d;
        if (rend_group->type != PARTICLE_LINE && rend_group->texture) {
            texture = rend_group->texture;
            if (rend_group->mask_texture) {
                mask_texture = rend_group->mask_texture;

                uniform->arr[U_TEXTURE_COUNT] = 2;
                switch (rend_group->mask_blend_mode) {
                default:
                    uniform->arr[U_TEXTURE_BLEND] = 0;
                    break;
                case PARTICLE_BLEND_MULTIPLY:
                    uniform->arr[U_TEXTURE_BLEND] = 1;
                    break;
                case PARTICLE_BLEND_ADD:
                    uniform->arr[U_TEXTURE_BLEND] = 2;
                    break;
                }
            }
            else {
                uniform->arr[U_TEXTURE_COUNT] = 1;
                uniform->arr[U_TEXTURE_BLEND] = 0;
            }
        }
        else {
            uniform->arr[U_TEXTURE_COUNT] = 0;
            uniform->arr[U_TEXTURE_BLEND] = 0;
        }

        gl_state_active_bind_texture_2d(0, texture, true);
        gl_state_active_bind_texture_2d(1, mask_texture, true);

        switch (rend_group->type) {
        case PARTICLE_QUAD:
            switch (rend_group->fog_type) {
            default:
                uniform->arr[U_FOG_HEIGHT] = 0;
                break;
            case Glitter::FOG_DEPTH:
                uniform->arr[U_FOG_HEIGHT] = 1;
                break;
            case Glitter::FOG_HEIGHT:
                uniform->arr[U_FOG_HEIGHT] = 2;
                break;
            }

            if (rend_group->blend_mode == PARTICLE_BLEND_PUNCH_THROUGH) {
                uniform->arr[U_ALPHA_BLEND] = 1;
                gl_state_enable_depth_test(true);
                gl_state_set_depth_mask(GL_TRUE, true);
            }
            else {
                uniform->arr[U_ALPHA_BLEND] = rend_group->disp_type ? 2 : 0;
                gl_state_enable_depth_test(true);
                gl_state_set_depth_mask(GL_FALSE, true);
            }

            if (rend_group->draw_type == DIRECTION_BILLBOARD) {
                gl_state_enable_cull_face(true);
                gl_state_set_cull_face_mode(GL_BACK, true);
            }
            else
                gl_state_disable_cull_face(true);
            break;
        case PARTICLE_LINE:
            uniform->arr[U_FOG_HEIGHT] = 0;
            uniform->arr[U_ALPHA_BLEND] = 2;

            gl_state_enable_depth_test(true);
            gl_state_set_depth_mask(GL_FALSE, true);
            gl_state_enable_cull_face(true);
            gl_state_set_cull_face_mode(GL_BACK, true);
            break;
        case PARTICLE_LOCUS:
            uniform->arr[U_FOG_HEIGHT] = 0;
            uniform->arr[U_ALPHA_BLEND] = 2;

            gl_state_enable_depth_test(true);
            gl_state_set_depth_mask(GL_FALSE, true);
            gl_state_disable_cull_face(true);
            break;
        }

        shaders_ft.set(SHADER_FT_GLITTER_PT);
        rctx->glitter_batch_ubo.Bind(2);
        switch (rend_group->type) {
        case PARTICLE_QUAD:
            gl_state_bind_vertex_array(rend_group->vao, true);
            shaders_ft.enable_primitive_restart();
            shaders_ft.draw_elements(GL_TRIANGLE_STRIP, (GLsizei)(5 * rend_group->disp - 1), GL_UNSIGNED_INT, 0);
            shaders_ft.disable_primitive_restart();
            gl_state_bind_vertex_array(0);
            break;
        case PARTICLE_LINE:
        case PARTICLE_LOCUS:
            gl_state_bind_vertex_array(rend_group->vao, true);
            DrawListData& draw_list = *rend_group->draw_list;
            if (rend_group->type == PARTICLE_LINE)
                for (prj::pair<GLint, GLsizei>& i : draw_list)
                    shaders_ft.draw_arrays(GL_LINE_STRIP, i.first, i.second);
            else
                for (prj::pair<GLint, GLsizei>& i : draw_list)
                    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, i.first, i.second);
            gl_state_bind_vertex_array(0);
            break;
        }
        shaders_ft.set(-1);
    }

    void RenderScene::CalcDispLocusSetPivot(Pivot pivot,
        float_t w, float_t& v00, float_t& v01) {
        switch (pivot) {
        case PIVOT_TOP_LEFT:
        case PIVOT_MIDDLE_LEFT:
        case PIVOT_BOTTOM_LEFT:
            v00 = 0.0f;
            v01 = w;
            break;
        case PIVOT_TOP_CENTER:
        case PIVOT_MIDDLE_CENTER:
        case PIVOT_BOTTOM_CENTER:
        default:
            v00 = w * -0.5f;
            v01 = w * 0.5f;
            break;
        case PIVOT_TOP_RIGHT:
        case PIVOT_MIDDLE_RIGHT:
        case PIVOT_BOTTOM_RIGHT:
            v00 = -w;
            v01 = 0.0f;
            break;
        }
    }

    void RenderScene::CalcDispQuadSetPivot(Pivot pivot,
        float_t w, float_t h, float_t& v00, float_t& v01, float_t& v10, float_t& v11) {
        switch (pivot) {
        case PIVOT_TOP_LEFT:
            v00 = 0.0f;
            v01 = w;
            v10 = -h;
            v11 = 0.0f;
            break;
        case PIVOT_TOP_CENTER:
            v00 = w * -0.5f;
            v01 = w * 0.5f;
            v10 = -h;
            v11 = 0.0f;
            break;
        case PIVOT_TOP_RIGHT:
            v00 = -w;
            v01 = 0.0f;
            v10 = -h;
            v11 = 0.0f;
            break;
        case PIVOT_MIDDLE_LEFT:
            v00 = 0.0f;
            v01 = w;
            v10 = h * -0.5f;
            v11 = h * 0.5f;
            break;
        case PIVOT_MIDDLE_CENTER:
        default:
            v00 = w * -0.5f;
            v01 = w * 0.5f;
            v10 = h * -0.5f;
            v11 = h * 0.5f;
            break;
        case PIVOT_MIDDLE_RIGHT:
            v00 = -w;
            v01 = 0.0f;
            v10 = h * -0.5f;
            v11 = h * 0.5f;
            break;
        case PIVOT_BOTTOM_LEFT:
            v00 = 0.0f;
            v01 = w;
            v10 = 0.0f;
            v11 = h;
            break;
        case PIVOT_BOTTOM_CENTER:
            v00 = w * -0.5f;
            v01 = w * 0.5f;
            v10 = 0.0f;
            v11 = h;
            break;
        case PIVOT_BOTTOM_RIGHT:
            v00 = -w;
            v01 = 0.0f;
            v10 = 0.0f;
            v11 = h;
            break;
        }
    }
}
