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
    void RenderScene::CalcDisp() {
        disp_quad = 0;
        disp_line = 0;
        disp_locus = 0;

        bool (FASTCALL * RenderGroup_CannotDisp)(RenderGroup * rend_group)
            = (bool(FASTCALL*)(RenderGroup * rend_group))0x00000001403A7120;

        for (RenderGroup*& i : render_groups) {
            if (!i)
                continue;

            RenderGroup* rend_group = i;
            if (RenderGroup_CannotDisp(rend_group))
                continue;

            CalcDisp(rend_group);
        }
    }

    void RenderScene::CalcDisp(RenderGroup* rend_group) {
        rend_group->disp = 0;
        switch (rend_group->type) {
        case PARTICLE_QUAD:
            if (!rend_group->vao)
                return;

            CalcDispQuad(rend_group);
            disp_quad += (uint32_t)rend_group->disp;
            break;
        case PARTICLE_LINE:
            if (!rend_group->vao)
                return;

            CalcDispLine(rend_group);
            disp_line += (uint32_t)rend_group->disp;
            break;
        case PARTICLE_LOCUS:
            if (!rend_group->vao)
                return;

            CalcDispLocus(rend_group);
            disp_locus += (uint32_t)rend_group->disp;
            break;
        }
    }

    void RenderScene::Disp(Glitter::DispType disp_type) {
        bool (FASTCALL * RenderGroup_CannotDisp)(RenderGroup * rend_group)
            = (bool(FASTCALL*)(RenderGroup * rend_group))0x00000001403A7120;

        for (RenderGroup*& i : render_groups) {
            if (!i)
                continue;

            RenderGroup* rend_group = i;
            if (RenderGroup_CannotDisp(rend_group) || rend_group->disp_type != disp_type)
                continue;

            Disp(rend_group);
        }
    }

    void RenderScene::Disp(RenderGroup* rend_group) {
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
        mat4_transpose(&camera_data->view, &cam_view);
        mat4_transpose(&camera_data->projection, &cam_projection);

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

        gl_state_enable_blend();
        gl_state_set_blend_func(blend_src, blend_dst);
        gl_state_set_blend_equation(GL_FUNC_ADD);

        GLuint texture = rctx->empty_texture_2d->glid;
        GLuint mask_texture = rctx->empty_texture_2d->glid;
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

        gl_state_active_bind_texture_2d(0, texture);
        gl_state_active_bind_texture_2d(1, mask_texture);

        switch (rend_group->type) {
        case PARTICLE_QUAD:
            switch (rend_group->fog_type) {
            default:
                uniform->arr[U_FOG_STAGE] = 0;
                break;
            case Glitter::FOG_DEPTH:
                uniform->arr[U_FOG_STAGE] = 1;
                break;
            case Glitter::FOG_HEIGHT:
                uniform->arr[U_FOG_STAGE] = 2;
                break;
            }

            if (rend_group->blend_mode == PARTICLE_BLEND_PUNCH_THROUGH) {
                uniform->arr[U_ALPHA_BLEND] = 1;
                gl_state_enable_depth_test();
                gl_state_set_depth_mask(GL_TRUE);
            }
            else {
                uniform->arr[U_ALPHA_BLEND] = rend_group->disp_type ? 2 : 0;
                gl_state_enable_depth_test();
                gl_state_set_depth_mask(GL_FALSE);
            }

            if (rend_group->draw_type == DIRECTION_BILLBOARD) {
                gl_state_enable_cull_face();
                gl_state_set_cull_face_mode(GL_BACK);
            }
            else
                gl_state_disable_cull_face();
            break;
        case PARTICLE_LINE:
            uniform->arr[U_FOG_STAGE] = 0;
            uniform->arr[U_ALPHA_BLEND] = 2;

            gl_state_enable_depth_test();
            gl_state_set_depth_mask(GL_FALSE);
            gl_state_enable_cull_face();
            gl_state_set_cull_face_mode(GL_BACK);
            break;
        case PARTICLE_LOCUS:
            uniform->arr[U_FOG_STAGE] = 0;
            uniform->arr[U_ALPHA_BLEND] = 2;

            gl_state_enable_depth_test();
            gl_state_set_depth_mask(GL_FALSE);
            gl_state_disable_cull_face();
            break;
        }

        shaders_ft.set(SHADER_FT_GLITTER_PT);
        rctx->set_glitter_render_data();
        rctx->glitter_batch_ubo.Bind(3);
        switch (rend_group->type) {
        case PARTICLE_QUAD:
            gl_state_bind_vertex_array(rend_group->vao);
            shaders_ft.enable_primitive_restart();
            shaders_ft.draw_elements(GL_TRIANGLE_STRIP, (GLsizei)(5 * rend_group->disp - 1), GL_UNSIGNED_INT, 0);
            shaders_ft.disable_primitive_restart();
            break;
        case PARTICLE_LINE:
            gl_state_bind_vertex_array(rend_group->vao);
            for (std::pair<GLint, GLsizei>& i : *rend_group->draw_list)
                shaders_ft.draw_arrays(GL_LINE_STRIP, i.first, i.second);
            break;
        case PARTICLE_LOCUS:
            gl_state_bind_vertex_array(rend_group->vao);
            for (std::pair<GLint, GLsizei>& i : *rend_group->draw_list)
                shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, i.first, i.second);
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

        Buffer* buf = rend_group->buffer;
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

        rend_group->vbo.WriteMemory(0, (buf - rend_group->buffer) * sizeof(Buffer), rend_group->buffer);
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
        mat4_transpose(&camera_data->view, &cam_view);
        mat4_transpose(&camera_data->inv_view, &cam_inv_view);

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

        Buffer* buf = rend_group->buffer;
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

        rend_group->vbo.WriteMemory(0, (buf - rend_group->buffer) * sizeof(Buffer), rend_group->buffer);
    }

    void RenderScene::CalcDispQuad(RenderGroup* rend_group) {
        if (!rend_group->elements || rend_group->vbo.IsNull() || rend_group->ctrl < 1)
            return;

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data->view, &cam_view);
        mat4_transpose(&camera_data->inv_view, &cam_inv_view);

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

        Buffer* buf = rend_group->buffer;
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

        rend_group->vbo.WriteMemory(0, (buf - rend_group->buffer) * sizeof(Buffer), rend_group->buffer);
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
        float_t ext_scale = 0.0f;
        bool (FASTCALL * RenderGroup_GetExtAnimScale)(RenderGroup * rend_group,
            vec3 * ext_anim_scale, float_t * ext_scale) = (bool(FASTCALL*)(RenderGroup * rend_group,
                vec3 * ext_anim_scale, float_t * ext_scale))0x00000001403A6800;
        if (RenderGroup_GetExtAnimScale(rend_group, &ext_anim_scale, &ext_scale)) {
            if (ext_scale <= 0.0f)
                ext_scale = 1.0f;

            ext_anim_scale += ext_scale;
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

        Buffer* buf = rend_group->buffer;
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

        rend_group->vbo.WriteMemory(0, (buf - rend_group->buffer) * sizeof(Buffer), rend_group->buffer);
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

    RenderSceneX::RenderSceneX() : ctrl_quad(), ctrl_line(), ctrl_locus(),
        ctrl_mesh(), disp_quad(), disp_line(), disp_locus(), disp_mesh() {

    }

    RenderSceneX::~RenderSceneX() {

    }

    void RenderSceneX::Append(RenderGroupX* rend_group) {
        groups.push_back(rend_group);
    }

    void RenderSceneX::CalcDisp() {
        disp_quad = 0;
        disp_line = 0;
        disp_locus = 0;
        disp_mesh = 0;

        for (RenderGroupX*& i : groups) {
            if (!i)
                continue;

            RenderGroupX* rend_group = i;
            if (rend_group->CannotDisp())
                continue;

            CalcDisp(rend_group);
        }
    }

    void RenderSceneX::CalcDisp(RenderGroupX* rend_group) {
        rend_group->disp = 0;
        switch (rend_group->type) {
        case PARTICLE_QUAD:
            if (!rend_group->vao)
                return;

            CalcDispQuad(rend_group);
            disp_quad += rend_group->disp;
            break;
        case PARTICLE_LINE:
            if (!rend_group->vao)
                return;

            CalcDispLine(rend_group);
            disp_line += rend_group->disp;
            break;
        case PARTICLE_LOCUS:
            if (!rend_group->vao)
                return;

            CalcDispLocus(rend_group);
            disp_locus += rend_group->disp;
            break;
        case PARTICLE_MESH:
            CalcDispMesh(rend_group);
            disp_mesh += rend_group->disp;
            break;
        }
    }

    void RenderSceneX::CalcDispLine(RenderGroupX* rend_group) {
        if (!rend_group->elements || rend_group->vbo.IsNull() || rend_group->ctrl < 1)
            return;

        size_t count = 0;
        RenderElementX* elem = rend_group->elements;
        for (size_t i = rend_group->ctrl; i > 0; i--, elem++) {
            if (!elem->alive)
                continue;

            if (elem->locus_history) {
                size_t length = elem->locus_history->data.size();
                if (length > 1)
                    count += length;
            }
        }

        if (!count || count > rend_group->max_count)
            return;

        bool has_scale = false;
        vec3 scale = 0.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            rend_group->GetEmitterScale(scale);
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;
            mat4_normalize_rotation(&rend_group->mat, &rend_group->mat_draw);
        }
        else
            rend_group->mat_draw = mat4_identity;

        Buffer* buf = rend_group->buffer;
        if (!buf)
            return;

        elem = rend_group->elements;
        size_t disp = 0;
        rend_group->draw_list.clear();
        for (size_t i = rend_group->ctrl, index = 0; i > 0; elem++) {
            if (!elem->alive)
                continue;

            i--;
            LocusHistoryX* hist = elem->locus_history;
            if (!elem->disp || !hist || hist->data.size() < 2)
                continue;

            size_t j = 0;
            if (has_scale)
                for (LocusHistoryX::Data& hist_data : hist->data) {
                    vec3& pos = hist_data.translation;
                    buf->position = pos + (pos - elem->base_translation) * scale;
                    buf->uv[0] = 0.0f;
                    buf->uv[1] = 0.0f;
                    buf->color = hist_data.color;
                    j++;
                    buf++;
                }
            else
                for (LocusHistoryX::Data& hist_data : hist->data) {
                    buf->position = hist_data.translation;
                    buf->uv[0] = 0.0f;
                    buf->uv[1] = 0.0f;
                    buf->color = hist_data.color;
                    j++;
                    buf++;
                }

            if (j > 0) {
                disp += j;
                rend_group->draw_list.push_back({ (GLint)index, (GLsizei)j });
                index += j;
            }
        }
        rend_group->disp = disp;
        rend_group->vbo.WriteMemory(0, (buf - rend_group->buffer) * sizeof(Buffer), rend_group->buffer);
    }

    void RenderSceneX::CalcDispLocus(RenderGroupX* rend_group) {
        if (!rend_group->elements || rend_group->vbo.IsNull() || rend_group->ctrl < 1)
            return;

        size_t count = 0;
        RenderElementX* elem = rend_group->elements;
        for (size_t i = rend_group->ctrl; i > 0; i--, elem++) {
            if (!elem->alive)
                continue;

            if (elem->locus_history) {
                size_t length = elem->locus_history->data.size();
                if (length > 1)
                    count += 2 * length;
            }
        }

        if (!count || count > rend_group->max_count)
            return;

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data->view, &cam_view);
        mat4_transpose(&camera_data->inv_view, &cam_inv_view);

        vec3 x_vec = { 1.0f, 0.0f, 0.0f };
        if (rend_group->flags & PARTICLE_LOCAL) {
            mat4 mat;
            mat4_mul(&cam_inv_view, &rend_group->mat, &mat);
            mat4_mul(&cam_view, &mat, &mat);
            mat4_mul(&mat, &cam_inv_view, &rend_group->mat_draw);
        }
        else
            rend_group->mat_draw = mat4_identity;

        mat3 model_mat;
        bool has_scale = false;
        vec3 scale = 0.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            mat4_get_scale(&rend_group->mat, &scale);
            if (rend_group->flags & PARTICLE_SCALE)
                x_vec.x = scale.x;
            scale -= 1.0f;
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;

            mat4_to_mat3(&rend_group->mat, &model_mat);
            mat3_normalize_rotation(&model_mat, &model_mat);
        }
        else
            model_mat = mat3_identity;

        mat3 cam_inv_view_mat3;
        mat4_to_mat3(&cam_inv_view, &cam_inv_view_mat3);
        mat3_invert(&cam_inv_view_mat3, &cam_inv_view_mat3);
        mat3_transform_vector(&cam_inv_view_mat3, &x_vec, &x_vec);

        Buffer* buf = rend_group->buffer;
        if (!buf)
            return;

        elem = rend_group->elements;
        vec2 split_uv = rend_group->split_uv;
        Pivot pivot = rend_group->pivot;
        size_t disp = 0;
        rend_group->draw_list.clear();
        for (size_t i = rend_group->ctrl, index = 0; i > 0; elem++) {
            if (!elem->alive)
                continue;

            i--;
            LocusHistoryX* hist = elem->locus_history;
            if (!elem->disp || !hist || hist->data.size() < 2)
                continue;

            float_t uv_u = elem->uv.x + elem->uv_scroll.x;
            float_t uv_u_2nd = elem->uv.x + elem->uv_scroll.x + split_uv.x;
            float_t uv_v_2nd = elem->uv.y + elem->uv_scroll.y + split_uv.y;
            float_t uv_v_scale = split_uv.y / (float_t)(hist->data.size() - 1);

            uv_v_2nd = 1.0f - uv_v_2nd;

            size_t j = 0;
            if (has_scale)
                for (LocusHistoryX::Data& hist_data : hist->data) {
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
                for (LocusHistoryX::Data& hist_data : hist->data) {
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
                rend_group->draw_list.push_back({ (GLint)index, (GLsizei)(j * 2) });
                index += j * 2;
            }
        }
        rend_group->disp = disp;

        rend_group->vbo.WriteMemory(0, (buf - rend_group->buffer) * sizeof(Buffer), rend_group->buffer);
    }

    void RenderSceneX::CalcDispMesh(RenderGroupX* rend_group) {
        if (rend_group->object.is_null())
            return;

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data->view, &cam_view);
        mat4_transpose(&camera_data->inv_view, &cam_inv_view);

        bool has_scale = false;
        bool emitter_local = false;
        vec3 emit_scale = 0.0f;
        mat4 model_mat;
        mat4 view_mat;
        mat4 inv_view_mat;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            model_mat = rend_group->mat;
            mat4_normalize_rotation(&model_mat, &view_mat);
            mat4_mul(&view_mat, &cam_view, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);

            emitter_local = true;
            if (rend_group->flags & PARTICLE_SCALE) {
                if (rend_group->GetEmitterScale(emit_scale))
                    has_scale = true;
            }
        }
        else {
            model_mat = mat4_identity;
            view_mat = cam_view;
            inv_view_mat = cam_inv_view;
        }

        bool local = false;
        if (rend_group->flags & PARTICLE_LOCAL) {
            mat4_mul(&inv_view_mat, &rend_group->mat, &inv_view_mat);
            mat4_mul(&view_mat, &inv_view_mat, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);
            local = true;
        }
        mat4_mul(&view_mat, &cam_inv_view, &rend_group->mat_draw);

        mat4 dir_mat = mat4_identity;
        vec3 up_vec = { 0.0f, 0.0f, 1.0f };
        bool billboard = false;
        bool emitter_rotation = false;
        mat4(*rotate_func)(RenderGroupX*, RenderElementX*, vec3*, vec3*) = 0;
        switch (rend_group->draw_type) {
        case DIRECTION_BILLBOARD:
            mat4_clear_trans(&model_mat, &dir_mat);
            mat4_mul(&dir_mat, &inv_view_mat, &dir_mat);
            mat4_clear_trans(&dir_mat, &dir_mat);
            billboard = true;
            break;
        case DIRECTION_EMITTER_DIRECTION:
            dir_mat = rend_group->mat_rot;
            break;
        case DIRECTION_PREV_POSITION:
        case DIRECTION_PREV_POSITION_DUP:
            rotate_func = RenderGroupX::RotateMeshToPrevPosition;
            break;
        case DIRECTION_EMIT_POSITION:
            rotate_func = RenderGroupX::RotateMeshToEmitPosition;
            break;
        case DIRECTION_Y_AXIS:
            mat4_rotate_y((float_t)M_PI_2, &dir_mat);
            break;
        case DIRECTION_X_AXIS:
            mat4_rotate_x((float_t)-M_PI_2, &dir_mat);
            break;
        case DIRECTION_BILLBOARD_Y_AXIS:
            mat4_rotate_y(camera_data->rotation.y, &dir_mat);
            break;
        case DIRECTION_EMITTER_ROTATION:
            emitter_rotation = true;
            break;
        }

        ParticleX* particle = rend_group->particle->data.particle;

        obj* obj = objset_info_storage_get_obj(rend_group->object);
        if (!obj)
            return;

        texture_transform_struct tex_trans[2];
        int32_t tex_trans_count = 0;

        for (texture_transform_struct& i : tex_trans) {
            tex_trans->id = -1;
            tex_trans->mat = mat4_identity;
        }

        obj_material& material = obj->material_array[0].material;

        for (obj_material_texture_data& i : material.texdata) {
            if (i.tex_index == -1)
                continue;

            tex_trans[tex_trans_count].id = i.tex_index & 0xFFFFF;
            tex_trans[tex_trans_count].mat = mat4_identity;

            if (++tex_trans_count == 2)
                break;
        }

        vec3 ext_anim_scale;
        float_t ext_scale = 0.0f;
        if (rend_group->GetExtAnimScale(&ext_anim_scale, &ext_scale)) {
            if (!has_scale) {
                emit_scale = 1.0f;
                has_scale = true;
            }

            if (ext_scale >= 0.0f)
                emit_scale *= ext_anim_scale + ext_scale;
            else
                emit_scale += ext_anim_scale;
        }

        mdl::DispManager& disp_manager = *::disp_manager;
        disp_manager.set_texture_pattern(0, 0);

        RenderElementX* elem = rend_group->elements;
        size_t disp = 0;
        for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
            j_max = min_def(i, j_max);
            for (size_t j = j_max; j > 0; elem++) {
                if (!elem->alive)
                    continue;
                j--;

                if (!elem->disp)
                    continue;

                vec3 trans = elem->translation;
                vec3 rot = elem->rotation;
                vec3 scale = elem->scale * elem->scale_all;
                if (has_scale)
                    scale *= emit_scale;

                if (emitter_local)
                    mat4_transform_point(&model_mat, &trans, &trans);

                mat4 mat;
                if (billboard) {
                    if (local)
                        mat = mat4_identity;
                    else
                        mat = dir_mat;
                }
                else if (rotate_func) {
                    mat = rotate_func(rend_group, elem, &up_vec, &trans);
                }
                else if (emitter_rotation)
                    mat = elem->mat;
                else
                    mat = dir_mat;

                mat4_set_translation(&mat, &trans);
                mat4_mul_rotate_zyx(&mat, &rot, &mat);
                mat4_scale_rot(&mat, &scale, &mat);

                vec3 uv_scroll;
                vec3 uv_scroll_2nd;
                uv_scroll.x = elem->uv_scroll.x;
                uv_scroll.y = -elem->uv_scroll.y;
                uv_scroll.z = 0.0f;
                uv_scroll_2nd.x = elem->uv_scroll_2nd.x;
                uv_scroll_2nd.y = -elem->uv_scroll_2nd.y;
                uv_scroll_2nd.z = 0.0f;

                mat4_set_translation(&tex_trans[0].mat, &uv_scroll);
                mat4_set_translation(&tex_trans[1].mat, &uv_scroll_2nd);

                mat4_transpose(&tex_trans[0].mat, &tex_trans[0].mat);
                mat4_transpose(&tex_trans[1].mat, &tex_trans[1].mat);
                disp_manager.set_texture_transform(tex_trans_count, tex_trans);

                if (local)
                    mat4_mul(&mat, &cam_inv_view, &mat);
                elem->mat_draw = mat;

                mat4_transpose(&mat, &mat);
                if (disp_manager.entry_obj_by_object_info(&mat,
                    rend_group->object, &elem->color, 0, local))
                    disp++;

                disp_manager.set_texture_transform(0, 0);
            }
        }
        disp_manager.set_texture_pattern(0, 0);
        rend_group->disp = disp;
    }

    void RenderSceneX::CalcDispQuad(RenderGroupX* rend_group) {
        if (!rend_group->elements || rend_group->vbo.IsNull() || rend_group->ctrl < 1)
            return;

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data->view, &cam_view);
        mat4_transpose(&camera_data->inv_view, &cam_inv_view);

        mat4 model_mat;
        mat4 view_mat;
        mat4 inv_view_mat;

        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            if (rend_group->draw_type == DIRECTION_EMITTER_ROTATION
                || rend_group->draw_type == DIRECTION_PARTICLE_ROTATION)
                mat4_clear_rot(&rend_group->mat, &model_mat);
            else
                mat4_normalize_rotation(&rend_group->mat, &model_mat);
            mat4_mul(&model_mat, &cam_view, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);
        }
        else {
            model_mat = mat4_identity;
            view_mat = cam_view;
            inv_view_mat = cam_inv_view;
        }

        if (rend_group->flags & PARTICLE_LOCAL) {
            if (rend_group->flags & PARTICLE_EMITTER_LOCAL)
                mat4_mul(&inv_view_mat, &rend_group->mat, &inv_view_mat);
            mat4_mul(&view_mat, &inv_view_mat, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);
        }
        mat4_mul(&view_mat, &cam_inv_view, &rend_group->mat_draw);

        mat4 dir_mat;
        switch (rend_group->draw_type) {
        case DIRECTION_BILLBOARD:
            mat4_clear_trans(&model_mat, &dir_mat);
            mat4_mul(&dir_mat, &inv_view_mat, &dir_mat);
            mat4_clear_trans(&dir_mat, &dir_mat);
            break;
        case DIRECTION_EMITTER_DIRECTION:
        case DIRECTION_EMITTER_ROTATION:
        case DIRECTION_PARTICLE_ROTATION:
            dir_mat = rend_group->mat_rot;
            break;
        case DIRECTION_Y_AXIS:
            mat4_rotate_y((float_t)M_PI_2, &dir_mat);
            break;
        case DIRECTION_X_AXIS:
            mat4_rotate_x((float_t)-M_PI_2, &dir_mat);
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
            CalcDispQuadDirectionRotation(rend_group, &model_mat);
            break;
        default:
            CalcDispQuadNormal(rend_group, &model_mat, &dir_mat);
            break;
        }
    }

    void RenderSceneX::CalcDispQuadDirectionRotation(RenderGroupX* rend_group, mat4* model_mat) {
        vec3 up_vec;
        mat4(*rotate_func)(RenderGroupX*, RenderElementX*, vec3*);
        if (rend_group->draw_type == DIRECTION_EMIT_POSITION) {
            up_vec = { 0.0f, 0.0f, 1.0f };
            rotate_func = RenderGroupX::RotateToEmitPosition;
        }
        else {
            up_vec = { 0.0f, 1.0f, 0.0f };
            rotate_func = RenderGroupX::RotateToPrevPosition;
        }

        bool use_scale = false;
        vec3 scale = 1.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL
            && rend_group->GetEmitterScale(scale))
            use_scale = rend_group->flags & PARTICLE_SCALE ? true : false;

        vec3 x_vec_base = { 1.0f, 0.0f, 0.0f };
        vec3 y_vec_base = { 0.0f, 1.0f, 0.0f };

        if (use_scale) {
            x_vec_base.x *= scale.x;
            y_vec_base.y *= scale.y;
        }

        Buffer* buf = rend_group->buffer;
        if (!buf)
            return;

        RenderElementX* elem = rend_group->elements;
        vec2 split_uv = rend_group->split_uv;
        Pivot pivot = rend_group->pivot;
        size_t disp = 0;
        for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
            j_max = min_def(i, j_max);
            for (size_t j = j_max; j > 0; elem++) {
                if (!elem->alive)
                    continue;
                j--;

                if (!elem->disp)
                    continue;

                vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                    continue;

                vec3 pos = elem->translation;
                if (use_scale)
                    pos *= scale;

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

                vec3 x_vec = x_vec_base;
                vec3 y_vec = y_vec_base;

                if (fabsf(elem->rotation.z) > 0.000001f) {
                    mat3 ptc_rot;
                    mat3_rotate_z(elem->rot_z_sin, elem->rot_z_cos, &ptc_rot);
                    mat3_transform_vector(&ptc_rot, &x_vec, &x_vec);
                    mat3_transform_vector(&ptc_rot, &y_vec, &y_vec);
                }

                vec2 uv = elem->uv + elem->uv_scroll;

                mat4 mat = rotate_func(rend_group, elem, &up_vec);
                mat4_transform_vector(&mat, &x_vec, &x_vec);
                mat4_transform_vector(&mat, &y_vec, &y_vec);

                for (int32_t k = 0; k < 4; k++, buf++) {
                    vec3 x_vec_rot = pos_add[k].x * x_vec;
                    vec3 y_vec_rot = pos_add[k].y * y_vec;
                    buf->position = pos + (x_vec_rot + y_vec_rot);
                    buf->uv[0] = uv + uv_add[k];
                    buf->uv[1] = buf->uv[0];
                    buf->color = elem->color;
                }
                disp++;
            }
        }
        rend_group->disp = disp;

        rend_group->vbo.WriteMemory(0, (buf - rend_group->buffer) * sizeof(Buffer), rend_group->buffer);
    }

    void RenderSceneX::CalcDispQuadNormal(RenderGroupX* rend_group, mat4* model_mat, mat4* dir_mat) {
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
            rend_group->GetEmitterScale(scale);
            if (rend_group->flags & PARTICLE_SCALE) {
                x_vec.x = scale.x;
                y_vec.y = scale.y;
            }
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;
            emitter_local = true;
        }

        vec3 ext_anim_scale;
        float_t ext_scale = 0.0f;
        if (rend_group->GetExtAnimScale(&ext_anim_scale, &ext_scale)) {
            if (ext_scale <= 0.0f)
                ext_scale = 1.0f;

            ext_anim_scale += ext_scale;
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

        Buffer* buf = rend_group->buffer;
        if (!buf)
            return;

        RenderElementX* elem = rend_group->elements;
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

                    if (!elem->disp)
                        continue;

                    vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                    if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                        continue;

                    vec3 pos = elem->translation;
                    if (has_scale)
                        pos *= scale;

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

                    if (!elem->disp)
                        continue;

                    vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                    if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                        continue;

                    vec3 pos = elem->translation;
                    if (has_scale)
                        pos *= scale;

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

        rend_group->vbo.WriteMemory(0, (buf - rend_group->buffer) * sizeof(Buffer), rend_group->buffer);
    }

    bool RenderSceneX::CanDisp(DispType disp_type, bool a3) {
        for (RenderGroupX*& i : groups)
            if (!i->CannotDisp() && i->disp_type == disp_type && (!a3 || !i->HasEnded()))
                return true;
        return false;
    }

    void RenderSceneX::CheckUseCamera() {
        for (RenderGroupX*& i : groups)
            if (i)
                i->CheckUseCamera();
    }

    void RenderSceneX::Ctrl(float_t delta_frame, bool copy_mats) {
        ctrl_quad = 0;
        ctrl_line = 0;
        ctrl_locus = 0;
        ctrl_mesh = 0;

        for (RenderGroupX*& i : groups) {
            if (!i)
                continue;

            switch (i->type) {
            case PARTICLE_QUAD:
                ctrl_quad += i->ctrl;
                break;
            case PARTICLE_LINE:
                ctrl_line += i->ctrl;
                break;
            case PARTICLE_LOCUS:
                ctrl_locus += i->ctrl;
                break;
            case PARTICLE_MESH:
                ctrl_mesh += i->ctrl;
                break;
            }

            i->Ctrl(delta_frame, copy_mats);
        }
    }

    void RenderSceneX::Disp(DispType disp_type) {
        for (RenderGroupX* i : groups) {
            if (!i)
                continue;

            RenderGroupX* rend_group = i;
            if ((rend_group)->disp_type != disp_type || rend_group->CannotDisp())
                continue;

            Disp(rend_group);
        }
    }

    void RenderSceneX::Disp(RenderGroupX* rend_group) {
        if (rend_group->disp < 1)
            return;

        mat4 cam_view;
        mat4 cam_projection;
        mat4_transpose(&camera_data->view, &cam_view);
        mat4_transpose(&camera_data->projection, &cam_projection);

        mat4 mat;
        mat4_mul(&rend_group->mat_draw, &cam_view, &mat);
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

        gl_state_enable_blend();
        gl_state_set_blend_func(blend_src, blend_dst);
        gl_state_set_blend_equation(GL_FUNC_ADD);

        GLuint texture = rctx->empty_texture_2d->glid;
        GLuint mask_texture = rctx->empty_texture_2d->glid;
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

        gl_state_active_bind_texture_2d(0, texture);
        gl_state_active_bind_texture_2d(1, mask_texture);

        switch (rend_group->fog_type) {
        default:
            uniform->arr[U_FOG_STAGE] = 0;
            break;
        case Glitter::FOG_DEPTH:
            uniform->arr[U_FOG_STAGE] = 1;
            break;
        case Glitter::FOG_HEIGHT:
            uniform->arr[U_FOG_STAGE] = 2;
            break;
        }

        if (rend_group->blend_mode == PARTICLE_BLEND_PUNCH_THROUGH)
            uniform->arr[U_ALPHA_BLEND] = rend_group->disp_type ? 3 : 1;
        else
            uniform->arr[U_ALPHA_BLEND] = rend_group->disp_type ? 2 : 0;

        if (!(rend_group->flags & PARTICLE_DEPTH_TEST))
            gl_state_enable_depth_test();
        else
            gl_state_disable_depth_test();

        if (rend_group->blend_mode == PARTICLE_BLEND_PUNCH_THROUGH)
            gl_state_set_depth_mask(GL_TRUE);
        else
            gl_state_set_depth_mask(GL_FALSE);

        if (rend_group->draw_type == DIRECTION_BILLBOARD && !rend_group->use_culling) {
            gl_state_enable_cull_face();
            gl_state_set_cull_face_mode(GL_BACK);
        }
        else
            gl_state_disable_cull_face();

        shaders_ft.set(SHADER_FT_GLITTER_PT);
        rctx->set_glitter_render_data();
        rctx->glitter_batch_ubo.Bind(3);
        switch (rend_group->type) {
        case PARTICLE_QUAD:
            gl_state_bind_vertex_array(rend_group->vao);
            shaders_ft.enable_primitive_restart();
            shaders_ft.draw_elements(GL_TRIANGLE_STRIP, (GLsizei)(5 * rend_group->disp - 1), GL_UNSIGNED_INT, 0);
            shaders_ft.disable_primitive_restart();
            break;
        case PARTICLE_LINE:
            gl_state_bind_vertex_array(rend_group->vao);
            for (std::pair<GLint, GLsizei>& i : rend_group->draw_list)
                shaders_ft.draw_arrays(GL_LINE_STRIP, i.first, i.second);
            break;
        case PARTICLE_LOCUS:
            gl_state_bind_vertex_array(rend_group->vao);
            for (std::pair<GLint, GLsizei>& i : rend_group->draw_list)
                shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, i.first, i.second);
            break;
        }
    }

    size_t RenderSceneX::GetCtrlCount(ParticleType type) {
        switch (type) {
        case PARTICLE_QUAD:
            return ctrl_quad;
        case PARTICLE_LINE:
            return ctrl_line;
        case PARTICLE_LOCUS:
            return ctrl_locus;
        case PARTICLE_MESH:
            return ctrl_mesh;
        default:
            return 0;
        }
    }

    size_t RenderSceneX::GetDispCount(ParticleType type) {
        switch (type) {
        case PARTICLE_QUAD:
            return disp_quad;
        case PARTICLE_LINE:
            return disp_line;
        case PARTICLE_LOCUS:
            return disp_locus;
        case PARTICLE_MESH:
            return disp_mesh;
        default:
            return 0;
        }
    }

    void RenderSceneX::CalcDispLocusSetPivot(Pivot pivot,
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

    void RenderSceneX::CalcDispQuadSetPivot(Pivot pivot,
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
