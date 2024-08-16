/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render.hpp"
#include "rob/rob.hpp"
#include "gl_state.hpp"
#include "render_context.hpp"
#include "render_context.hpp"
#include "shader_ft.hpp"
#include "texture.hpp"

extern bool task_stage_is_modern;

extern render_context* rctx;

rndr::Render* (FASTCALL* render_get)() = (rndr::Render * (FASTCALL*)())0x00000001404AE8A0;

#define MLAA_MAX_EDGE_DETECTION_LEN (3)
#define MLAA_GRID_SIDE_LEN (2 * MLAA_MAX_EDGE_DETECTION_LEN + 1)
#define MLAA_SIDE_LEN (5 * MLAA_GRID_SIDE_LEN)

#define TONE_MAP_SAT_GAMMA_SAMPLES 32

namespace rndr {
    static void calculate_mlaa_area_texture_data(uint8_t* data, int32_t cross1, int32_t cross2);
    static void calculate_mlaa_area_texture_data_inner(float_t* val_left,
        float_t* val_right, int32_t cross1, int32_t cross2, int32_t dleft, int32_t dright);

    void Render::ExposureCharaData::reset() {
        for (GLuint& i : query_data)
            i = -1;
        spot_weight = 0.0f;
    }

    void Render::apply_post_process(texture* light_proj_tex, int32_t npr_param) {
        for (int32_t i = 0; i < 8; i++)
            gl_state_bind_sampler(i, 0);

        gl_state_disable_depth_test();
        gl_state_disable_blend();

        dof->apply(&rend_texture[0]);

        draw_lens_ghost();

        downsample();

        reduce_tex_draw = 0;
        get_blur();

        calc_exposure();
        apply_tone_map(light_proj_tex, npr_param);

        apply_mlaa(taa_texture_selector, 2, mlaa);

        for (int32_t i = 0; i < 16; i++) {
            if (!render_textures_data[i])
                continue;

            render_textures[i].Bind();

            texture* t = render_textures_data[i];
            if (render_width[0] > t->width * 2 || render_height[0] > t->height * 2)
                uniform->arr[U_REDUCE] = 1;
            else
                uniform->arr[U_REDUCE] = 0;

            gl_state_set_viewport(0, 0, t->width, t->height);
            gl_state_active_bind_texture_2d(0, taa_tex[taa_texture_selector]->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[0]);
            shaders_ft.set(SHADER_FT_REDUCE);
            draw_quad(render_post_width[0], render_post_height[0],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        }

        copy_to_frame_texture(rend_texture[0].GetColorTex(),
            render_width[0], render_height[0], taa_tex[taa_texture_selector]->glid);

        if (taa) {
            if (taa_blend >= 0.0f)
                calc_taa_blend();
            else
                taa_blend = 1.0f;
        }

        if (taa_blend < 0.0f || taa_blend >= 1.0f)
            taa_texture = taa_texture_selector;
        else {
            bool blur;
            GLuint sampler;
            if (taa_blend > 0.99f) {
                blur = true;
                sampler = rctx->render_samplers[0];
            }
            else {
                blur = false;
                sampler = rctx->render_samplers[1];
            }

            taa_texture = 2;
            taa_buffer[2].Bind();
            gl_state_set_viewport(0, 0, render_width[0], render_height[0]);
            gl_state_active_bind_texture_2d(0, taa_tex[taa_texture_selector]->glid);
            gl_state_bind_sampler(0, sampler);

            if (blur) {
                gl_state_active_bind_texture_2d(2, rend_texture[0].depth_texture->glid);
                gl_state_bind_sampler(2, sampler);

                mat4 mat;
                mat4_invert(&cam_view_projection, &mat);
                mat4_mul(&cam_view_projection_prev, &mat, &mat);
                uniform->arr[U_REDUCE] = 6;

                camera_blur_shader_data shader_data = {};
                shader_data.g_transform[0] = mat.row0;
                shader_data.g_transform[1] = mat.row1;
                shader_data.g_transform[2] = mat.row2;
                shader_data.g_transform[3] = mat.row3;
                rctx->camera_blur_ubo.WriteMemory(shader_data);
                rctx->camera_blur_ubo.Bind(1);
            }
            else {
                int32_t taa_texture_selector = this->taa_texture_selector + 1;
                if (taa_texture_selector > taa)
                    taa_texture_selector = 0;
                gl_state_active_bind_texture_2d(1, taa_tex[taa_texture_selector]->glid);
                gl_state_bind_sampler(1, sampler);
                uniform->arr[U_REDUCE] = 5;
            }

            shaders_ft.set(SHADER_FT_REDUCE);

            draw_quad(render_post_width[0], render_post_height[0],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, taa_blend);
        }

        if (taa) {
            int32_t taa_texture_selector = this->taa_texture_selector + 1;
            if (taa_texture_selector > taa)
                taa_texture_selector = 0;
            this->taa_texture_selector = taa_texture_selector;
        }

        rctx->screen_buffer.Bind();
        gl_state_set_viewport(0, 0, rctx->screen_width, rctx->screen_height);
        if (ssaa) {
            gl_state_active_bind_texture_2d(0, taa_tex[taa_texture]->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[0]);
            uniform->arr[U_ALPHA_MASK] = ss_alpha_mask ? 1 : 0;
            uniform->arr[U_REDUCE] = 0;
            shaders_ft.set(SHADER_FT_REDUCE);
            draw_quad(render_post_width[0], render_post_height[0],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            uniform->arr[U_ALPHA_MASK] = 0;
        }
        else {
            gl_state_active_bind_texture_2d(0, taa_tex[taa_texture]->glid);
            if (mag_filter == MAG_FILTER_NEAREST)
                gl_state_bind_sampler(0, rctx->render_samplers[1]);
            else
                gl_state_bind_sampler(0, rctx->render_samplers[0]);

            switch (mag_filter) {
            case MAG_FILTER_NEAREST:
            case MAG_FILTER_BILINEAR:
            default:
                uniform->arr[U_MAGNIFY] = 0;
                break;
            case MAG_FILTER_SHARPEN_5_TAP:
                uniform->arr[U_MAGNIFY] = 2;
                break;
            case MAG_FILTER_SHARPEN_4_TAP:
                uniform->arr[U_MAGNIFY] = 3;
                break;
            case MAG_FILTER_CONE_4_TAP:
                uniform->arr[U_MAGNIFY] = 4;
                break;
            case MAG_FILTER_CONE_2_TAP:
                uniform->arr[U_MAGNIFY] = 5;
                break;
            }

            shaders_ft.set(SHADER_FT_MAGNIFY);
            draw_quad(render_post_width[0], render_post_height[0],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        }
        shader::unbind();

        for (int32_t i = 0; i < 8; i++)
            gl_state_bind_sampler(i, 0);

        fbo_blit(rctx->screen_buffer.fbos[0], 0,
            0, 0, rctx->screen_width, rctx->screen_height,
            0, 0, rctx->screen_width, rctx->screen_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        gl_state_bind_framebuffer(0);
    }

    void Render::bind_render_texture(bool aet_back) {
        if (aet_back) {
            aet_back_texture.Bind();
            this->aet_back = 1;
        }
        else
            rend_texture[0].Bind();
        gl_state_set_viewport(0, 0, render_width[0], render_height[0]);
    }

    void Render::calc_exposure_chara_data() {
        shader::unbind();

        gl_state_set_color_mask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        gl_state_set_depth_mask(GL_FALSE);
        gl_state_disable_cull_face();

        shaders_ft.set(SHADER_FT_SUN_NO_TEXTURED);
        gl_state_bind_vertex_array(rctx->common_vao);
        rctx->sun_quad_ubo.Bind(0);

        mat4 view;
        mat4_transpose(&camera_data->view, &view);
        mat4 projection;
        mat4_transpose(&camera_data->projection, &projection);

        ExposureCharaData* chara_data = exposure_chara_data;
        int32_t query_index = (this->exposure_query_index + 1) % 3;
        this->exposure_query_index = query_index;
        int32_t next_query_index = (query_index + 2) % 3;
        size_t rob_chara_smth = get_rob_chara_smth();
        for (int32_t i = 0; i < ROB_CHARA_COUNT; i++, chara_data++) {
            rob_chara* rob_chr = rob_chara_array_get(rob_chara_smth, i);
            if (!rob_chr || !rob_chr->is_visible())
                continue;

            float_t max_face_depth = rob_chara_get_max_face_depth(rob_chr);

            mat4 mat = mat4_identity;
            sub_1405163C0(rob_chr, 4, &mat);
            mat4_mul(&mat, &view, &mat);
            mat4_mul_translate(&mat, max_face_depth + 0.1f, 0.0f, -0.06f, &mat);
            mat4_clear_rot(&mat, &mat);
            mat4_scale_rot(&mat, 0.0035f, &mat);
            mat4_mul(&mat, &projection, &mat);

            sun_quad_shader_data shader_data = {};
            mat4_transpose(&mat, &mat);
            shader_data.g_transform[0] = mat.row0;
            shader_data.g_transform[1] = mat.row1;
            shader_data.g_transform[2] = mat.row2;
            shader_data.g_transform[3] = mat.row3;
            shader_data.g_emission = 0.0f;
            rctx->sun_quad_ubo.WriteMemory(shader_data);

            glBeginQuery(GL_SAMPLES_PASSED, chara_data->query[next_query_index]);
            shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
            glEndQuery(GL_SAMPLES_PASSED);

            if (chara_data->query_data[next_query_index] == -1)
                chara_data->query_data[next_query_index] = 0;

            if (chara_data->query_data[query_index] != -1) {
                int32_t res = 0;
                glGetQueryObjectiv(chara_data->query[query_index],
                    GL_QUERY_RESULT_AVAILABLE, &res);
                if (res)
                    glGetQueryObjectuiv(chara_data->query[query_index],
                        GL_QUERY_RESULT, chara_data->query_data);
                else
                    chara_data->query_data[query_index] = 0;
            }
        }

        gl_state_bind_vertex_array(0);

        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        gl_state_set_depth_mask(GL_TRUE);
        gl_state_enable_cull_face();
    }

    void Render::calc_projection_matrix(mat4* mat, float_t fov, float_t aspect, float_t z_near, float_t z_far,
        float_t left_offset, float_t right_offset, float_t bottom_offset, float_t top_offset) {
        float_t tan_fov = (float_t)tan(fov * M_PI * (1.0 / 360.0)) * z_near;
        float_t left = -tan_fov * aspect + tan_fov * aspect * left_offset;
        float_t right = tan_fov * aspect + tan_fov * aspect * right_offset;
        float_t bottom = -tan_fov + tan_fov * bottom_offset;
        float_t top = tan_fov + tan_fov * top_offset;

        if (taa) {
            float_t offset = taa_texture_selector == 1 ? -0.25f : 0.25f;
            float_t left_right_offset = (right - left) * offset / (float_t)render_width[0];
            float_t bottom_top_offset = (top - bottom) * offset / (float_t)render_height[0];
            left += left_right_offset;
            right += left_right_offset;
            bottom += bottom_top_offset;
            top += bottom_top_offset;
        }

        mat4_frustrum(left, right, bottom, top, z_near, z_far, mat);
        mat4_transpose(mat, mat);
    }

    void Render::ctrl() {
        static int32_t(FASTCALL * task_stage_get_current_stage_index)()
            = (int32_t(FASTCALL*)())0x000000014064ADA0;

        view_point_prev = view_point;
        interest_prev = interest;
        view_point = camera_data->view_point;
        interest = camera_data->interest;

        stage_index_prev = stage_index;
        stage_index = task_stage_get_current_stage_index();

        cam_view_projection_prev = cam_view_projection;
        cam_view_projection = camera_data->view_projection;

        reset_exposure = camera_data->fast_change_hist1 && !camera_data->fast_change_hist0;
        if (reset_exposure) {
            float_t view_point_dist = vec3::distance(view_point, view_point_prev);

            vec3 dir = vec3::normalize(interest - view_point);
            vec3 dir_prev = vec3::normalize(interest_prev - view_point_prev);

            float_t dir_diff_angle = vec3::dot(dir, dir_prev);
            if (dir_diff_angle < 0.5f)
                dir_diff_angle = 0.0f;
            if (view_point_dist < dir_diff_angle * 0.4f)
                reset_exposure = false;
        }
        else if (stage_index != stage_index_prev)
            reset_exposure = true;

        if (movie_textures_data[0]) {
            static size_t(FASTCALL * task_movie_get)(int32_t index)
                = (size_t(FASTCALL*)(int32_t index))0x000000014041ED30;
            size_t task_movie = task_movie_get(0);
            if (task_movie) {
                static texture*(FASTCALL * sub_14041ED80)(size_t task_movie)
                    = (texture*(FASTCALL*)(size_t task_movie))0x000000014041ED80;
                texture* movie_texture = sub_14041ED80(task_movie);
                if (movie_texture) {
                    movie_textures[0].Bind();
                    gl_state_set_viewport(0, 0, movie_textures_data[0]->width, movie_textures_data[0]->height);
                    gl_state_active_bind_texture_2d(0, movie_texture->glid);
                    uniform->arr[U_REDUCE] = 0;
                    shaders_ft.set(SHADER_FT_REDUCE);
                    draw_quad(movie_texture->width, movie_texture->height,
                        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                    gl_state_bind_framebuffer(0);
                }
            }
        }
    }

    void Render::draw_quad(int32_t width, int32_t height, float_t s0, float_t t0, float_t s1, float_t t1,
        float_t scale, float_t param_x, float_t param_y, float_t param_z, float_t param_w) {
        s0 -= s1;
        t0 -= t1;

        float_t w = (float_t)max_def(width, 1);
        float_t h = (float_t)max_def(height, 1);
        quad_shader_data quad = {};
        quad.g_texcoord_modifier = { 0.5f * s0, 0.5f * t0, 0.5f * s0 + s1, 0.5f * t0 + t1 }; // x * 0.5 * y0 + 0.5 * y0 + y1
        quad.g_texel_size = { scale / w, scale / h, w, h };
        quad.g_color = { param_x, param_y, param_z, param_w };
        quad.g_texture_lod = 0.0f;

        rctx->quad_ubo.WriteMemory(quad);
        rctx->quad_ubo.Bind(0);
        gl_state_bind_vertex_array(rctx->common_vao);
        shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void Render::draw_lens_flare() {
        static float_t flt_1411ACB84 = sinf(0.01365909819f);
        static float_t flt_1411ACB8C = tanf(0.01365909819f) * 2.0f * 0.94f;

        if (!lens_flare_texture)
            return;

        GLuint tex = objset_info_storage_get_set_texture(5, 4549);
        if (!tex)
            return;

        mat4 view;
        mat4_transpose(&camera_data->view, &view);
        mat4 projection;
        mat4_transpose(&camera_data->projection, &projection);

        mat4 view_projection;
        mat4 inverse_view_projection_matrix;
        mat4_transpose(&camera_data->view_projection, &view_projection);
        mat4_invert(&view_projection, &inverse_view_projection_matrix);

        float_t v5 = tanf(camera_data->fov * 0.5f * DEG_TO_RAD_FLOAT);
        light_set* set = &light_set_data[LIGHT_SET_MAIN];
        light_data* data = &set->lights[LIGHT_SUN];

        vec3 position;
        vec4 emission;
        if (data->get_type() == LIGHT_PARALLEL) {
            data->get_position(position);
            data->get_diffuse(emission);
        }
        else {
            set->lights[LIGHT_STAGE].get_position(position);
            position = vec3::normalize(position) * 500.0f;
            set->lights[LIGHT_STAGE].get_ibl_color0(emission);
            data->set_type(LIGHT_PARALLEL);
            data->set_position(position);
            data->set_diffuse(emission);
        }

        vec4 v44;
        *(vec3*)&v44 = position;
        v44.w = 1.0f;

        mat4_transform_vector(&view_projection, &v44, &v44);

        float_t v13 = 1.0f / v44.w;
        float_t v14 = v44.x * v13;
        float_t v15 = v44.y * v13;
        float_t v16 = (float_t)width * 0.01f / (float_t)height;
        v14 = clamp_def(v14, -0.99f, 0.99f);
        v15 = clamp_def(v15, v16 - 1.0f, 1.0f - v16);
        v44.x = v14 * v44.w;
        v44.y = v15 * v44.w;

        mat4_transform_vector(&inverse_view_projection_matrix, &v44, &v44);

        void (FASTCALL * pos_scale__get_screen_pos_scale)(vec3 * data, vec3 * trans, __int64 r8, bool apply_offset)
            = (void(FASTCALL*)(vec3 * data, vec3 * trans, __int64 r8, bool apply_offset))0x00000001401F7EE0;
        pos_scale__get_screen_pos_scale(&lens_flare_pos, &position, 0, false);

        float_t v17 = lens_flare_pos.x - (float_t)width * 0.5f;
        float_t v19 = v5 / (float_t)height * 0.5f;
        float_t v20 = lens_flare_pos.y - (float_t)height * 0.5f;

        float_t v22 = sqrtf(v17 * v19 * v17 * v19 + v20 * v19 * v20 * v19 + 1.0f);
        float_t v23 = ((float_t)render_width[0] * (flt_1411ACB8C / v5)) * (v22 * v22 * 0.5f)
            * ((float_t)render_height[0] * (flt_1411ACB8C / v5));
        float_t v24 = vec3::distance(position, view_point) * flt_1411ACB84;
        emission *= 1.0f / (float_t)(1.0f - cosf((float_t)(3.0 * DEG_TO_RAD)));

        sun_quad_shader_data shader_data = {};
        shader_data.g_emission = emission;

        gl_state_active_bind_texture_2d(0, tex);
        shaders_ft.set(SHADER_FT_SUN);
        gl_state_bind_vertex_array(rctx->common_vao);
        rctx->sun_quad_ubo.Bind(0);

        int32_t query_index = (lens_flare_query_index + 1) % 3;
        lens_flare_query_index = query_index;
        int32_t next_query_index = (query_index + 2) % 3;

        if (lens_shaft_query_data[next_query_index] == -1)
            lens_shaft_query_data[next_query_index] = 0;

        if (lens_flare_query_data[next_query_index] == -1)
            lens_flare_query_data[next_query_index] = 0;

        gl_state_set_color_mask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        gl_state_set_depth_mask(GL_FALSE);
        gl_state_disable_cull_face();

        mat4 mat;
        mat4_mul_translate(&view, &position, &mat);
        mat4_clear_rot(&mat, &mat);
        mat4_scale_rot(&mat, v24 * 0.2f, &mat);
        mat4_mul(&mat, &projection, &mat);

        mat4_transpose(&mat, &mat);
        shader_data.g_transform[0] = mat.row0;
        shader_data.g_transform[1] = mat.row1;
        shader_data.g_transform[2] = mat.row2;
        shader_data.g_transform[3] = mat.row3;
        rctx->sun_quad_ubo.WriteMemory(shader_data);

        glBeginQuery(GL_SAMPLES_PASSED, lens_shaft_query[next_query_index]);
        shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
        glEndQuery(GL_SAMPLES_PASSED);

        mat4_mul_translate(&view, (vec3*)&v44, &mat);
        mat4_clear_rot(&mat, &mat);
        mat4_scale_rot(&mat, v24 * 2.0f, &mat);
        mat4_mul(&mat, &projection, &mat);

        mat4_transpose(&mat, &mat);
        shader_data.g_transform[0] = mat.row0;
        shader_data.g_transform[1] = mat.row1;
        shader_data.g_transform[2] = mat.row2;
        shader_data.g_transform[3] = mat.row3;
        rctx->sun_quad_ubo.WriteMemory(shader_data);

        glBeginQuery(GL_SAMPLES_PASSED, lens_flare_query[next_query_index]);
        shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
        glEndQuery(GL_SAMPLES_PASSED);

        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        gl_state_set_depth_mask(GL_TRUE);
        gl_state_enable_cull_face();

        if (lens_shaft_query_data[query_index] != -1) {
            int32_t res = 0;
            glGetQueryObjectiv(lens_shaft_query[query_index], GL_QUERY_RESULT_AVAILABLE, &res);
            if (res)
                glGetQueryObjectuiv(lens_shaft_query[query_index], GL_QUERY_RESULT, &lens_shaft_query_data[query_index]);
        }

        if (lens_flare_query_data[query_index] != -1) {
            int32_t res = 0;
            glGetQueryObjectiv(lens_flare_query[query_index], GL_QUERY_RESULT_AVAILABLE, &res);
            if (res)
                glGetQueryObjectuiv(lens_flare_query[query_index], GL_QUERY_RESULT, &lens_flare_query_data[query_index]);
        }

        if (emission.x + emission.y + emission.z > 0.0f) {
            gl_state_enable_blend();
            gl_state_set_blend_func(GL_ONE, GL_ONE);
            gl_state_set_depth_mask(GL_FALSE);

            mat4_mul_translate(&view, &position, &mat);
            mat4_clear_rot(&mat, &mat);
            mat4_scale_rot(&mat, v24 * 1.1f, &mat);
            mat4_mul(&mat, &projection, &mat);

            mat4_transpose(&mat, &mat);
            shader_data.g_transform[0] = mat.row0;
            shader_data.g_transform[1] = mat.row1;
            shader_data.g_transform[2] = mat.row2;
            shader_data.g_transform[3] = mat.row3;
            rctx->sun_quad_ubo.WriteMemory(shader_data);

            shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);

            gl_state_disable_blend();
            gl_state_set_blend_func(GL_ONE, GL_ZERO);
            gl_state_set_depth_mask(GL_TRUE);
        }

        gl_state_active_bind_texture_2d(0, 0);
        gl_state_bind_vertex_array(0);

        if (lens_flare_appear_power <= 0.01f)
            lens_flare_appear_power = 0.0f;
        else
            lens_flare_appear_power *= 0.93f;
        lens_shaft_scale = 100.0f;
        lens_flare_power = 0.0f;

        float_t v34 = lens_flare_pos.x;
        float_t v35 = lens_flare_pos.y;
        float_t v32 = lens_flare_pos.z;

        float_t v33 = 0.0f;
        if (v32 >= 0.0f
            || v34 < (float_t)-width || v34 >(float_t)(2 * width)
            || v35 < (float_t)-height || v35 >(float_t)(2 * height))
            lens_flare_texture = 0;
        else {
            for (GLuint& i : lens_flare_query_data)
                if (i != -1)
                    v33 += min_def((float_t)i, v23);
            v33 /= (v23 * 3.0f);
            lens_flare_power = powf(v33, 0.7f);

            if (v33 > 0.0f && lens_shaft_query_data[query_index]) {
                float_t v41 = max_def(v33 - 0.4f, 0.005f);
                lens_shaft_scale = ((0.6f / v41) * lens_shaft_inv_scale) * (v5 * 3.4f);
            }
        }

        if (v32 >= 0.0f
            || v34 < -12.0f || v34 >(float_t)(width + 12)
            || v35 < -12.0f || v35 >(float_t)(height + 12)) {
            field_A10 = 0.0f;
        }
        else if (v34 > -2.0f && v34 < (float_t)(width + 2)
            && v35 > -2.0f && v35 < (float_t)(height + 2)) {
            if (v33 > 0.4f && field_A10 == 0.0f && lens_flare_appear_power < 0.02f && !reset_exposure)
                lens_flare_appear_power = 8.0f;
            field_A10 = 1.0f;
        }
    }

    void Render::free() {
        if (transparency) {
            delete transparency;
            transparency = 0;
        }

        for (int32_t i = 0; i < 1; i++)
            if (transparency_tex[i]) {
                texture_release(transparency_tex[i]);
                transparency_tex[i] = 0;
                texture_counter--;
            }

        if (dof) {
            delete dof;
            dof = 0;
        }

        if (!texture_counter)
            return;

        for (int32_t i = 0; i < downsample_max_count; i++)
            rend_texture[i].Free();

        for (int32_t i = 0; i < 3; i++) {
            taa_buffer[i].Free();

            if (taa_tex[i]) {
                texture_release(taa_tex[i]);
                taa_tex[i] = 0;
                texture_counter--;
            }
        }

        aet_back_texture.Free();

        if (aet_back_tex) {
            texture_release(aet_back_tex);
            aet_back_tex = 0;
            texture_counter--;
        }

        mlaa_buffer.Free();
        temp_buffer.Free();

        sss_contour_texture = 0;

        downsample_texture.Free();
        field_3E8.Free();

        for (int32_t i = 0; i < 5; i++)
            reduce_texture->Free();

        for (int32_t i = 0; i < 5; i++)
            if (reduce_tex[i]) {
                texture_release(reduce_tex[i]);
                reduce_tex[i] = 0;
                texture_counter--;
            }

        if (exposure_history) {
            texture_release(exposure_history);
            exposure_history = 0;
            texture_counter--;
        }

        exposure_texture.Free();

        if (exposure_tex) {
            glDeleteTextures(1, &exposure_tex);
            exposure_tex = 0;
        }

        if (tonemap_lut_texture) {
            glDeleteTextures(1, &tonemap_lut_texture);
            tonemap_lut_texture = 0;
        }
        if (mlaa_area_texture) {
            glDeleteTextures(1, &mlaa_area_texture);
            mlaa_area_texture = 0;
        }

        glDeleteQueries(3, lens_shaft_query);
        glDeleteQueries(3, lens_flare_query);

        for (ExposureCharaData& i : exposure_chara_data) {
            glDeleteQueries(3, i.query);
            i.query[0] = 0;
        }
    }

    void Render::init_render_buffers(int32_t width, int32_t height,
        int32_t ssaa, int32_t hd_res, int32_t ss_alpha_mask) {
        this->ssaa = ssaa;
        this->hd_res = hd_res;
        this->ss_alpha_mask = ss_alpha_mask;

        mlaa = 1;
        taa = ssaa || ss_alpha_mask ? 0 : 1;
        mag_filter = hd_res ? MAG_FILTER_SHARPEN_4_TAP : MAG_FILTER_BILINEAR;

        this->width = width;
        this->height = height;

        if (ssaa) {
            inner_width = 2 * width;
            inner_height = 2 * height;
        }
        else {
            inner_width = width;
            inner_height = height;
        }

        screen_x_offset = 0;
        screen_y_offset = 0;

        screen_width = width;
        screen_height = height;
        reduce_width[0] = 256;
        reduce_height[0] = 144;
        reduce_width[1] = 128;
        reduce_height[1] = 72;
        reduce_width[2] = 64;
        reduce_height[2] = 36;
        reduce_width[3] = 32;
        reduce_height[3] = 18;
        reduce_width[4] = 8;
        reduce_height[4] = 8;

        update_res(true, 1);

        rend_texture[0].Init(render_post_width[0], render_post_height[0], 0, GL_RGBA16F, GL_DEPTH_COMPONENT24);

        gl_state_bind_texture_2d(rend_texture[0].GetColorTex());
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        gl_state_bind_texture_2d(rend_texture[0].GetDepthTex());
        GLint swizzle[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);

        for (int32_t i = 0; i < 3; i++) {
            taa_tex[i] = texture_load_tex_2d(texture_id(0x25, texture_counter++),
                GL_RGBA8, render_post_width[0], render_post_height[0], 0, 0, 0);
            taa_buffer[i].SetColorDepthTextures(taa_tex[i]->glid, 0, rend_texture[0].GetDepthTex());
        }

        aet_back_tex = texture_load_tex_2d(texture_id(0x25, texture_counter++),
            GL_RGBA8, render_post_width[0], render_post_height[0], 0, 0, 0);
        aet_back_texture.SetColorDepthTextures(aet_back_tex->glid, 0, rend_texture[0].GetDepthTex());

        mlaa_buffer.Init(render_post_width[0], render_post_height[0], 0, GL_RGBA8, GL_DEPTH_COMPONENT24);
        temp_buffer.Init(render_post_width[0], render_post_height[0], 0, GL_RGBA8, GL_ZERO);
        sss_contour_texture = &mlaa_buffer;

        gl_state_bind_texture_2d(mlaa_buffer.GetDepthTex());
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);

        for (int32_t i = 1; i < downsample_count; i++) {
            rend_texture[i].Init(render_post_width[i], render_post_height[i], 0, GL_RGBA16F, GL_ZERO);

            gl_state_bind_texture_2d(rend_texture[i].GetColorTex());
            glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        downsample_texture.Init(reduce_width[0], reduce_height[0], 0, GL_RGBA16F, GL_ZERO);

        gl_state_bind_texture_2d(downsample_texture.GetColorTex());

        const vec4 border_color = 0.0f;
        glTexParameterfvDLL(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        field_3E8.Init(512, 256, 0, GL_RGBA8, GL_ZERO);

        dof = new renderer::DOF3(render_post_width[0], render_post_height[0]);

        for (int32_t i = 0; i < 1; i++)
            transparency_tex[i] = texture_load_tex_2d(texture_id(0x25, texture_counter++),
                GL_RGBA16F, render_post_width[0], render_post_height[0], 0, 0, 0);

        transparency = new renderer::Transparency(transparency_tex[0]->glid,
            rend_texture[0].GetDepthTex(), render_post_width[0], render_post_height[0]);
    }

    void Render::init_tone_map_buffers() {
        for (int32_t i = 0; i < 5; i++) {
            reduce_tex[i] = texture_load_tex_2d(texture_id(0x25, texture_counter++),
                GL_RGBA16F, reduce_width[i], reduce_height[i], 0, 0, 0);
            gl_state_bind_texture_2d(reduce_tex[i]->glid);

            const vec4 border_color = 0.0f;
            glTexParameterfvDLL(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);
            glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        }

        reduce_texture[0].SetColorDepthTextures(reduce_tex[0]->glid);

        exposure_history = texture_load_tex_2d(texture_id(0x25, texture_counter++),
            GL_RGBA16F, 32, 2, 0, 0, 0);

        glGenTextures(1, &exposure_tex);
        gl_state_bind_texture_2d(exposure_tex);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_RGBA32F, 2, 2, 0, GL_RGBA, GL_FLOAT, 0);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        gl_state_bind_texture_2d(0);

        exposure_texture.SetColorDepthTextures(exposure_tex);

        glGenTextures(1, &tonemap_lut_texture);
        gl_state_bind_texture_2d(tonemap_lut_texture);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_RG16F, 16 * TONE_MAP_SAT_GAMMA_SAMPLES, 1, 0, GL_RG, GL_HALF_FLOAT, 0);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        const GLint swizzle[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        gl_state_bind_texture_2d(0);

        generate_mlaa_area_texture();

        glGenQueries(3, lens_shaft_query);
        glGenQueries(3, lens_flare_query);

        for (GLuint& i : lens_shaft_query_data)
            i = -1;

        for (GLuint& i : lens_flare_query_data)
            i = -1;

        for (ExposureCharaData& i : exposure_chara_data) {
            glGenQueries(3, i.query);
            for (GLuint& j : i.query_data)
                j = -1;
        }
    }

    int32_t Render::movie_texture_set(texture* movie_texture) {
        if (!movie_texture)
            return -1;

        int32_t index = 0;
        while (movie_textures_data[index])
            if (++index >= 1)
                return -1;

        movie_textures_data[index] = movie_texture;
        movie_textures[index].SetColorDepthTextures(movie_texture->glid);
        return index;
    }

    void Render::movie_texture_free(texture* movie_texture) {
        if (!movie_texture)
            return;

        int32_t index = 0;
        while (movie_textures_data[index] != movie_texture)
            if (++index >= 1)
                return;

        movie_textures_data[index] = 0;
        movie_textures[index].Free();
    }

    int32_t Render::render_texture_set(texture* render_texture, bool task_photo) {
        if (!render_texture)
            return -1;

        int32_t index = 0;
        if (task_photo)
            index = 15;
        else
            while (render_textures_data[index])
                if (++index >= 15)
                    return -1;

        render_textures_data[index] = render_texture;
        render_textures[index].SetColorDepthTextures(render_texture->glid);
        return index;
    }

    void Render::render_texture_free(texture* render_texture, bool task_photo) {
        if (!render_texture)
            return;

        int32_t index = 0;
        if (task_photo)
            index = 15;
        else
            while (render_textures_data[index] != render_texture)
                if (++index >= 15)
                    return;

        render_textures_data[index] = 0;
        render_textures[index].Free();
    }

    void Render::resize(int32_t width, int32_t height) {
        this->width = width;
        this->height = height;

        if (ssaa) {
            inner_width = 2 * width;
            inner_height = 2 * height;
        }
        else {
            inner_width = width;
            inner_height = height;
        }

        screen_x_offset = 0;
        screen_y_offset = 0;

        screen_width = width;
        screen_height = height;

        update_res(true, 1);

        rend_texture[0].Init(render_post_width[0], render_post_height[0], 0, GL_RGBA16F, GL_DEPTH_COMPONENT24);

        gl_state_bind_texture_2d(rend_texture[0].GetColorTex());
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        gl_state_bind_texture_2d(rend_texture[0].GetDepthTex());
        GLint swizzle[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);

        for (int32_t i = 0; i < 3; i++) {
            texture_id id = taa_tex[i]->id;
            texture_release(taa_tex[i]);
            taa_tex[i] = texture_load_tex_2d(id, GL_RGBA8,
                render_post_width[0], render_post_height[0], 0, 0, 0);
            taa_buffer[i].SetColorDepthTextures(taa_tex[i]->glid, 0, rend_texture[0].GetDepthTex());
        }

        texture_id aet_back_tex_id = aet_back_tex->id;
        texture_release(aet_back_tex);
        aet_back_tex = texture_load_tex_2d(aet_back_tex_id, GL_RGBA8,
            render_post_width[0], render_post_height[0], 0, 0, 0);
        aet_back_texture.SetColorDepthTextures(aet_back_tex->glid, 0, rend_texture[0].GetDepthTex());

        mlaa_buffer.Init(render_post_width[0], render_post_height[0], 0, GL_RGBA8, GL_DEPTH_COMPONENT24);
        temp_buffer.Init(render_post_width[0], render_post_height[0], 0, GL_RGBA8, GL_ZERO);
        sss_contour_texture = &mlaa_buffer;

        gl_state_bind_texture_2d(mlaa_buffer.GetDepthTex());
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);

        for (int32_t i = 1; i < downsample_count; i++) {
            rend_texture[i].Init(render_post_width[i], render_post_height[i], 0, GL_RGBA16F, GL_ZERO);

            gl_state_bind_texture_2d(rend_texture[i].GetColorTex());
            glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        dof->resize(render_post_width[0], render_post_height[0]);

        for (int32_t i = 0; i < 1; i++) {
            texture_id id = taa_tex[i]->id;
            texture_release(transparency_tex[i]);
            transparency_tex[i] = texture_load_tex_2d(id, GL_RGBA16F,
                render_post_width[0], render_post_height[0], 0, 0, 0);
        }

        transparency->resize(transparency_tex[0]->glid, rend_texture[0].GetDepthTex(),
            render_post_width[0], render_post_height[0]);
    }

    void Render::downsample() {
        uniform->arr[U_REDUCE] = 1;
        shaders_ft.set(SHADER_FT_REDUCE);
        for (int32_t i = 1; i < downsample_count - 1; i++) {
            rend_texture[i].Bind();
            gl_state_active_bind_texture_2d(0, rend_texture[i - 1].GetColorTex());
            gl_state_bind_sampler(0, rctx->render_samplers[0]);
            gl_state_set_viewport(0, 0, render_width[i], render_height[i]);
            draw_quad(
                render_post_width[i - 1], render_post_height[i - 1],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        }

        uniform->arr[U_REDUCE] = 3;
        shaders_ft.set(SHADER_FT_REDUCE);

        int32_t downsample = max_def(downsample_count - 2, 0);
        gl_state_set_viewport(0, 0, reduce_width[0], reduce_height[0]);
        reduce_texture[0].Bind();
        gl_state_active_bind_texture_2d(0, rend_texture[downsample].GetColorTex());
        gl_state_bind_sampler(0, rctx->render_samplers[0]);
        draw_quad(
            render_post_width[downsample], render_post_height[downsample],
            render_post_width_scale, render_post_height_scale,
            0.0f, 0.0f, 1.0f, 1.1f, 1.1f, 1.1f, 0.0f);

        downsample_texture.Bind();
        gl_state_active_bind_texture_2d(0, reduce_tex[0]->glid);
        gl_state_bind_sampler(0, rctx->render_samplers[2]);
        for (int32_t i = 1; i < 5; i++) {
            float_t scale;
            if (i == 4) {
                scale = 1.0f;
                uniform->arr[U_EXPOSURE] = 0;
                shaders_ft.set(SHADER_FT_EXPOSURE);
            }
            else
            {
                scale = 0.75;
                uniform->arr[U_REDUCE] = 1;
                shaders_ft.set(SHADER_FT_REDUCE);
            }

            gl_state_set_viewport(0, 0, reduce_width[i], reduce_height[i]);
            draw_quad(reduce_width[i - 1], reduce_height[i - 1],
                1.0f, 1.0f, 0.0f, 0.0f, scale, 1.0f, 1.0f, 1.0f, 1.0f);
            gl_state_active_bind_texture_2d(0, reduce_tex[i]->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[2]);
            glCopyTexSubImage2DDLL(GL_TEXTURE_2D, 0, 0, 0, 0, 0, reduce_width[i], reduce_height[i]);
        }
    }

    void Render::set_screen_res(int32_t x_offset, int32_t y_offset, int32_t width, int32_t height) {
        screen_x_offset = x_offset;
        screen_y_offset = y_offset;
        screen_width = min_def(width, this->width);
        screen_height = min_def(height, this->height);

        update_res(false, -1);
        update = 1;
    }

    void Render::take_ss(texture* tex, bool vertical, float_t horizontal_offset) {
        int32_t index = render_texture_set(tex, true);
        if (index < 0)
            return;

        float_t x_min = 0.0f;
        float_t x_max = 1.0f;
        if (vertical) {
            const float_t center = 0.5f + horizontal_offset / (float_t)width;
            const float_t area = (float_t)height * (float_t)(9.0 / 16.0) / (float_t)width * 0.5f;
            x_min = center - area;
            x_max = center + area;
        }

        render_textures[index].Bind();
        texture* rend_tex = render_textures_data[index];
        uniform->arr[U_REDUCE] = 0;

        gl_state_set_viewport(0, 0, rend_tex->width, rend_tex->height);
        gl_state_active_bind_texture_2d(0, rctx->screen_buffer.GetColorTex());
        gl_state_bind_sampler(0, rctx->render_samplers[0]);
        shaders_ft.set(SHADER_FT_REDUCE);
        draw_quad(rend_tex->width, rend_tex->height,
            x_max, 1.0f, x_min, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        shader::unbind();
        gl_state_bind_framebuffer(0);
        gl_state_active_bind_texture_2d(0, 0);
        gl_state_bind_sampler(0, 0);

        render_texture_free(tex, true);
    }

    void Render::transparency_combine(float_t alpha) {
        transparency->combine(rend_texture, alpha);
    }

    void Render::transparency_copy() {
        transparency->copy(rend_texture[0].GetColorTex());
    }

    void Render::update_res(bool set, int32_t base_downsample) {
        int32_t hd_res_widths[4];
        int32_t hd_res_heights[4];
        hd_res_widths[0] = 1280;
        hd_res_heights[0] = 720;
        hd_res_widths[1] = 1024;
        hd_res_heights[1] = 576;
        hd_res_widths[2] = 1024;
        hd_res_heights[2] = 576;
        hd_res_widths[3] = 960;
        hd_res_heights[3] = 544;

        base_downsample = min_def(base_downsample, 3);

        if (set)
            this->base_downsample = base_downsample;
        else
            base_downsample = max_def(base_downsample, this->base_downsample);

        base_downsample = clamp_def(base_downsample, 0, 3);

        int32_t hd_res_width = hd_res_widths[base_downsample];
        int32_t hd_res_height = hd_res_heights[base_downsample];
        if (hd_res && (screen_width > hd_res_width || screen_height > hd_res_height)) {
            render_width[0] = hd_res_width;
            render_height[0] = hd_res_height;

            if (set) {
                inner_width = render_width[0];
                inner_height = render_height[0];
            }
        }
        else {
            /*float_t round_val = ssaa ? 1.0f : 16.0f;
            render_width[0] = (int32_t)(prj::ceilf((float_t)screen_width
                * (float_t)inner_width / (float_t)width * (1.0f / round_val)) * round_val);
            render_height[0] = (int32_t)(prj::ceilf((float_t)screen_height
                * (float_t)inner_height / (float_t)height * (1.0f / round_val)) * round_val);*/
            
            render_width[0] = (int32_t)prj::ceilf((float_t)screen_width
                * (float_t)inner_width / (float_t)width);
            render_height[0] = (int32_t)prj::ceilf((float_t)screen_height
                * (float_t)inner_height / (float_t)height);
        }

        int32_t downsample = 1;
        for (int32_t i = 0; i < 4; i++) {
            int32_t width = (render_width[i] + 1) / 2;
            int32_t height = (render_height[i] + 1) / 2;
            if (width <= reduce_width[0] && height <= reduce_height[0])
                break;

            render_width[i + 1] = width;
            render_height[i + 1] = height;
            downsample++;
        }
        this->downsample_count = downsample;

        if (set) {
            downsample_max_count = downsample;
            for (int32_t i = 0; i < downsample; i++) {
                render_post_width[i] = render_width[i];
                render_post_height[i] = render_height[i];
            }
        }

        render_post_width_scale = (float_t)render_width[0] / (float_t)render_post_width[0];
        render_post_height_scale = (float_t)render_height[0] / (float_t)render_post_height[0];

        taa_blend = -1.0f;
    }

    void Render::apply_mlaa(int32_t destination, int32_t source, int32_t mlaa) {
        gl_state_begin_event("PostProcess::mlaa");
        if (mlaa) {
            mlaa_buffer.Bind();
            gl_state_active_bind_texture_2d(0, taa_tex[source]->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[1]);
            uniform->arr[U_MLAA] = 0;
            shaders_ft.set(SHADER_FT_MLAA);
            draw_quad(render_post_width[0], render_post_height[0],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

            temp_buffer.Bind();
            gl_state_active_bind_texture_2d(0, mlaa_buffer.GetColorTex());
            gl_state_active_bind_texture_2d(1, mlaa_area_texture);
            gl_state_bind_sampler(0, rctx->render_samplers[0]);
            gl_state_bind_sampler(1, rctx->render_samplers[3]);
            uniform->arr[U_MLAA] = 1;
            uniform->arr[U_MLAA_SEARCH] = 2;
            shaders_ft.set(SHADER_FT_MLAA);
            draw_quad(render_post_width[0], render_post_height[0],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

            taa_buffer[destination].Bind();
            gl_state_active_bind_texture_2d(0, taa_tex[source]->glid);
            gl_state_active_bind_texture_2d(1, temp_buffer.GetColorTex());
            gl_state_bind_sampler(0, rctx->render_samplers[1]);
            gl_state_bind_sampler(1, rctx->render_samplers[1]);
            uniform->arr[U_MLAA] = 2;
            uniform->arr[U_ALPHA_MASK] = ss_alpha_mask ? 1 : 0;
            shaders_ft.set(SHADER_FT_MLAA);
            draw_quad(render_post_width[0], render_post_height[0],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            uniform->arr[U_ALPHA_MASK] = 0;
            gl_state_active_bind_texture_2d(0, 0);
            gl_state_active_bind_texture_2d(1, 0);
        }
        else {
            taa_buffer[destination].Bind();
            gl_state_active_bind_texture_2d(0, taa_tex[source]->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[1]);
            uniform->arr[U_REDUCE] = 0;
            shaders_ft.set(SHADER_FT_REDUCE);
            draw_quad(render_post_width[0], render_post_height[0],
                render_post_width_scale, render_post_height_scale,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        }
        gl_state_end_event();
    }

    void Render::apply_tone_map(texture* light_proj_tex, int32_t npr_param) {
        if (!this)
            return;
        
        gl_state_begin_event("PostProcess::tone_map");

        update_tone_map_lut();

        uniform->arr[U_TONE_MAP] = (int32_t)tone_map;
        uniform->arr[U_FLARE] = 0;
        uniform->arr[U_AET_BACK] = 0;
        uniform->arr[U_LIGHT_PROJ] = 0;

        int32_t scene_fade_blend_func = this->scene_fade_blend_func[scene_fade_index];

        tone_map_shader_data shader_data;
        shader_data.g_exposure.x = exposure * exposure_rate;
        shader_data.g_exposure.y = 0.0625f;
        shader_data.g_exposure.z = exposure * exposure_rate * 0.5f;
        shader_data.g_exposure.w = auto_exposure ? 1.0f : 0.0f;
        shader_data.g_flare_coef.x = (lens_flare * 2.0f) * (lens_flare_appear_power + lens_flare_power);
        shader_data.g_flare_coef.y = lens_shaft * 2.0f;
        shader_data.g_flare_coef.z = 0.0f;
        shader_data.g_flare_coef.w = 0.0f;
        *(vec3*)&shader_data.g_fade_color = scene_fade_color[scene_fade_index];
        shader_data.g_fade_color.w = scene_fade_alpha[scene_fade_index];
        if (scene_fade_blend_func == 1 || scene_fade_blend_func == 2)
            *(vec3*)&shader_data.g_fade_color *= shader_data.g_fade_color.w;
        *(vec3*)&shader_data.g_tone_scale = tone_trans_scale[tone_trans_index];
        *(vec3*)&shader_data.g_tone_offset = tone_trans_offset[tone_trans_index];
        shader_data.g_tone_scale.w = (float_t)scene_fade_blend_func;
        shader_data.g_tone_offset.w = gamma > 0.0f ? 2.0f / (gamma * 3.0f) : 0.0f;

        shader_data.g_texcoord_transforms[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
        shader_data.g_texcoord_transforms[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
        shader_data.g_texcoord_transforms[2] = { 1.0f, 0.0f, 0.0f, 0.0f };
        shader_data.g_texcoord_transforms[3] = { 0.0f, 1.0f, 0.0f, 0.0f };

        gl_state_active_bind_texture_2d(0, rend_texture[0].GetColorTex());
        gl_state_bind_sampler(0, rctx->render_samplers[1]);
        gl_state_active_bind_texture_2d(1, reduce_tex_draw);
        gl_state_bind_sampler(1, rctx->render_samplers[2]);
        gl_state_active_bind_texture_2d(2, tonemap_lut_texture);
        gl_state_bind_sampler(2, rctx->render_samplers[0]);
        gl_state_active_bind_texture_2d(3, exposure_tex);
        gl_state_bind_sampler(3, rctx->render_samplers[3]);

        if (lens_flare_texture) {
            const float_t aspect = (float_t)height / (float_t)width;

            uniform->arr[U_FLARE] = 1;
            gl_state_active_bind_texture_2d(4, lens_flare_texture);
            gl_state_bind_sampler(4, rctx->render_samplers[2]);

            mat4 mat;
            mat4_translate(0.5f, 0.5f, 0.0f, &mat);
            mat4_scale_rot(&mat, 0.75f, 0.75f, 1.0f, &mat);
            mat4_mul_rotate_z(&mat, (lens_flare_pos.x / (float_t)width)
                * 25.0f * DEG_TO_RAD_FLOAT, &mat);
            mat4_mul_translate(&mat, -((1.0f / (float_t)width) * lens_flare_pos.x),
                (lens_flare_pos.y - (float_t)height) * (1.0f / (float_t)width), 0.0f, &mat);
            mat4_scale_rot(&mat, 1.0f, aspect, 1.0f, &mat);
            mat4_transpose(&mat, &mat);
            shader_data.g_texcoord_transforms[4] = mat.row0;
            shader_data.g_texcoord_transforms[5] = mat.row1;

            if (lens_shaft_scale < 50.0f) {
                uniform->arr[U_FLARE] = 2;
                gl_state_active_bind_texture_2d(5, lens_shaft_texture);
                gl_state_bind_sampler(5, rctx->render_samplers[2]);

                mat4_translate(0.5f, 0.5f, 0.0f, &mat);
                mat4_scale_rot(&mat, lens_shaft_scale, lens_shaft_scale, 1.0f, &mat);
                mat4_mul_rotate_z(&mat, (lens_flare_pos.x / (float_t)width)
                    * 60.0f * DEG_TO_RAD_FLOAT, &mat);
                mat4_mul_translate(&mat, -((1.0f / (float_t)width) * lens_flare_pos.x),
                    (lens_flare_pos.y - (float_t)height) * (1.0f / (float_t)width), 0.0f, &mat);
                mat4_scale_rot(&mat, 1.0f, aspect, 1.0f, &mat);
                mat4_transpose(&mat, &mat);
                shader_data.g_texcoord_transforms[6] = mat.row0;
                shader_data.g_texcoord_transforms[7] = mat.row1;
            }
            else {
                gl_state_active_bind_texture_2d(5, rctx->empty_texture_2d->glid);
                gl_state_bind_sampler(5, rctx->render_samplers[2]);

                shader_data.g_texcoord_transforms[6] = { 1.0f, 0.0f, 0.0f, 0.0f };
                shader_data.g_texcoord_transforms[7] = { 0.0f, 1.0f, 0.0f, 0.0f };
            }
        }
        else {
            gl_state_active_bind_texture_2d(4, rctx->empty_texture_2d->glid);
            gl_state_bind_sampler(4, rctx->render_samplers[2]);
            gl_state_active_bind_texture_2d(5, rctx->empty_texture_2d->glid);
            gl_state_bind_sampler(5, rctx->render_samplers[2]);

            shader_data.g_texcoord_transforms[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
            shader_data.g_texcoord_transforms[5] = { 0.0f, 1.0f, 0.0f, 0.0f };
            shader_data.g_texcoord_transforms[6] = { 1.0f, 0.0f, 0.0f, 0.0f };
            shader_data.g_texcoord_transforms[7] = { 0.0f, 1.0f, 0.0f, 0.0f };
        }

        if (aet_back) {
            gl_state_active_bind_texture_2d(6, aet_back_tex->glid);
            gl_state_bind_sampler(6, rctx->render_samplers[2]);
            uniform->arr[U_AET_BACK] = 1;
        }

        if (light_proj_tex) {
            uniform->arr[U_LIGHT_PROJ] = 1;
            gl_state_active_bind_texture_2d(7, light_proj_tex->glid);
            gl_state_bind_sampler(7, rctx->render_samplers[2]);

            shader_data.g_flare_coef.z = 1.0f;
        }
        else {
            gl_state_active_bind_texture_2d(7, rctx->empty_texture_2d->glid);
            gl_state_bind_sampler(7, rctx->render_samplers[2]);
        }

        if (aet_back && npr_param == 1) {
            gl_state_active_bind_texture_2d(14, rend_texture[0].GetDepthTex());
            gl_state_bind_sampler(14, rctx->render_samplers[1]);
        }

        rctx->tone_map_ubo.WriteMemory(shader_data);

        gl_state_set_viewport(0, 0, render_width[0], render_height[0]);
        taa_buffer[2].Bind();
        shaders_ft.set(SHADER_FT_TONEMAP);
        rctx->tone_map_ubo.Bind(1);
        gl_state_bind_vertex_array(rctx->common_vao);
        shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
        gl_state_active_bind_texture_2d(2, 0);
        gl_state_end_event();
    }


    void Render::calc_exposure() {
        bool v2 = false;
        if (!reset_exposure) {
            ExposureCharaData* chara_data = exposure_chara_data;
            size_t rob_chara_smth = get_rob_chara_smth();
            for (int32_t i = 0; i < ROB_CHARA_COUNT; i++, chara_data++) {
                rob_chara* rob_chr = rob_chara_array_get(rob_chara_smth, i);
                if (!rob_chr || !(*(uint8_t*)(rob_chr + 0x440) & 1))
                    continue;

                vec4 v34 = { 0.05f, 0.0f, -0.04f, 1.0f };
                vec4 v33 = { 1.0f, 0.0f, 0.0f, 0.0f };

                mat4 v44 = mat4_identity;
                sub_1405163C0(rob_chr, 4, &v44);

                mat4 v42;
                mat4 v43;
                mat4_mul(&camera_data->view, &v44, &v42);
                mat4_mul(&camera_data->projection, &v42, &v43);
                mat4_transpose(&v42, &v42);
                mat4_transpose(&v43, &v43);

                vec4 v41;
                vec4 v40;
                mat4_transform_vector(&v42, &v34, &v41);
                mat4_transform_vector(&v43, &v34, &v40);

                float_t v12 = 1.0f / v40.w;
                float_t v13 = v40.x * v12;
                float_t v14 = v40.y * v12;
                if (v41.z >= 0.0f || fabsf(v13) >= 1.0f || fabsf(v14) >= 1.0f)
                    continue;

                float_t v31 = 0.5f - (fabsf(v14) - 1.0f) * 2.5f;
                float_t v32 = 0.5f - (fabsf(v13) - 1.0f) * 2.5f;

                float_t v16 = min_def(min_def(v31, v32), 1.0f);

                vec4 v39;
                mat4_transform_vector(&v42, &v33, &v39);

                float_t v17 = (v39.z + 1.0f) * 0.5f;
                if (v17 > 0.8f)
                    v17 = 0.8f;
                else if (v17 < 0.2f)
                    v17 = 0.0f;

                float_t v18 = (float_t)height / (float_t)width;
                float_t v20 = tanf(camera_data->fov * 0.5f * DEG_TO_RAD_FLOAT);
                float_t v21 = 0.25f / sqrtf(powf(v20 * 3.4f, 2.0f) * (v41.z * v41.z));
                float_t v22;
                if (v21 < 0.055f)
                    v22 = max_def((v21 - 0.035f) * 50.0f, 0.0f);
                else if (v21 > 0.5f)
                    v22 = max_def(1.0f - (v21 - 0.5f) * 3.3333333f, 0.0f);
                else
                    v22 = 1.0f;

                chara_data->spot_coefficients[0].x = (v13 + v21 * 0.0f * v18 + 1.0f) * 0.5f;
                chara_data->spot_coefficients[0].y = (v14 + v21 * 0.1f + 1.0f) * 0.5f;
                chara_data->spot_coefficients[0].z = 0.0f;
                chara_data->spot_coefficients[0].w = 4.0f;
                chara_data->spot_coefficients[1].x = (v13 + v21 * 0.0f * v18 + 1.0f) * 0.5f;
                chara_data->spot_coefficients[1].y = (v14 - v21 * 0.3f + 1.0f) * 0.5f;
                chara_data->spot_coefficients[1].z = 0.0f;
                chara_data->spot_coefficients[1].w = 4.0f;
                chara_data->spot_coefficients[2].x = (v13 + v21 * -0.5f * v18 + 1.0f) * 0.5f;
                chara_data->spot_coefficients[2].y = (v14 + v21 * -0.5f + 1.0f) * 0.5f;
                chara_data->spot_coefficients[2].z = 0.0f;
                chara_data->spot_coefficients[2].w = 3.0f;
                chara_data->spot_coefficients[3].x = (v13 - v21 * 0.6f * v18 + 1.0f) * 0.5f;
                chara_data->spot_coefficients[3].y = (v14 - v21 * 0.1f + 1.0f) * 0.5f;
                chara_data->spot_coefficients[3].z = 0.0f;
                chara_data->spot_coefficients[3].w = 2.0f;
                chara_data->spot_coefficients[4].x = (v13 + v21 * 0.6f * v18 + 1.0f) * 0.5f;
                chara_data->spot_coefficients[4].y = (v14 - v21 * 0.1f + 1.0f) * 0.5f;
                chara_data->spot_coefficients[4].z = 0.0f;
                chara_data->spot_coefficients[4].w = 2.0f;
                chara_data->spot_coefficients[5].x = (v13 + v21 * 0.5f * v18 + 1.0f) * 0.5f;
                chara_data->spot_coefficients[5].y = (v14 + v21 * -0.5f + 1.0f) * 0.5f;
                chara_data->spot_coefficients[5].z = 0.0f;
                chara_data->spot_coefficients[5].w = 3.0f;
                chara_data->spot_coefficients[6].x = (v13 + v21 * 0.0f * v18 + 1.0f) * 0.5f;
                chara_data->spot_coefficients[6].y = (v14 - v21 * 0.8f + 1.0f) * 0.5f;
                chara_data->spot_coefficients[6].z = 0.0f;
                chara_data->spot_coefficients[6].w = 3.0f;
                chara_data->spot_weight = v16 * 1.6f * v17 * v22;
                if (chara_data->spot_weight > 0.0f && !chara_data->query_data[exposure_query_index])
                    v2 = true;
            }
        }

        if (!v2 || reset_exposure) {
            exposure_measure_shader_data exposure_measure = {};
            exposure_measure.g_spot_weight = {
                exposure_chara_data[0].spot_weight,
                exposure_chara_data[1].spot_weight,
                exposure_chara_data[2].spot_weight,
                exposure_chara_data[3].spot_weight
            };

            ExposureCharaData* chara_data = exposure_chara_data;
            for (int32_t i = 0; i < 4 && i < ROB_CHARA_COUNT; i++, chara_data++)
                for (int32_t j = 0; j < 8; j++)
                    exposure_measure.g_spot_coefficients[i * 8 + j] = chara_data->spot_coefficients[j];
            rctx->exposure_measure_ubo.WriteMemory(exposure_measure);

            gl_state_set_viewport(0, 0, 32, 1);
            downsample_texture.Bind();

            uniform->arr[U_EXPOSURE] = 1;
            shaders_ft.set(SHADER_FT_EXPOSURE);
            rctx->exposure_measure_ubo.Bind(1);
            gl_state_active_bind_texture_2d(0, reduce_tex[4]->glid);
            gl_state_active_bind_texture_2d(1, reduce_tex[2]->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[2]);
            gl_state_bind_sampler(1, rctx->render_samplers[2]);
            draw_quad(1, 1, 1.0f, 1.0f,
                0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

            gl_state_active_bind_texture_2d(0, exposure_history->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[2]);
            if (reset_exposure)
                glCopyTexSubImage2DDLL(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 32, 1);
            else
                glCopyTexSubImage2DDLL(GL_TEXTURE_2D, 0, exposure_history_counter, 0, 0, 0, 1, 1);
            exposure_history_counter = (exposure_history_counter + 1) % 32;
        }

        gl_state_set_viewport(0, 0, 1, 1);
        exposure_texture.Bind();
        uniform->arr[U_EXPOSURE] = 2;
        shaders_ft.set(SHADER_FT_EXPOSURE);
        gl_state_active_bind_texture_2d(0, exposure_history->glid);
        gl_state_bind_sampler(0, rctx->render_samplers[0]);
        draw_quad(1, 1, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    void Render::calc_gaussian_blur(float_t start, float_t step,
        int32_t kernel_size, float_t radius_scale, float_t intensity_scale) {
        auto calculate_gaussian_kernel = [&](float_t* gaussian_kernel,
            float_t radius, int32_t stride, int32_t offset) {
            float_t first_val = (start - step * 0.5f) * 2.0f;
            for (int32_t i = 0; i < kernel_size; i++)
                gaussian_kernel[i * stride + offset] = 0.0f;

            float_t temp_gaussian_kernel[20];
            float_t s = radius * radius_scale;
            s = 1.0f / (2.0f * s * s);

            double_t sum = first_val;
            temp_gaussian_kernel[0] = first_val;
            float_t val = start;
            for (size_t i = 1; i < kernel_size; i++) {
                sum += temp_gaussian_kernel[i] = expf(-(val * val * s)) * step;
                val += step;
            }

            sum = 1.0f / sum;
            for (size_t i = 0; i < kernel_size; i++)
                gaussian_kernel[i * stride + offset] = (float_t)(temp_gaussian_kernel[i] * sum);
        };

        vec3 gauss[20];
        calculate_gaussian_kernel((float_t*)gauss, radius.x, 3, 0);
        calculate_gaussian_kernel((float_t*)gauss, radius.y, 3, 1);
        calculate_gaussian_kernel((float_t*)gauss, radius.z, 3, 2);

        vec3 intensity = this->intensity * (intensity_scale * 0.5f);

        gaussian_coef_shader_data gaussian_coef = {};
        for (int32_t i = 0; i < kernel_size && i < 8; i++) {
            *(vec3*)&gaussian_coef.g_coef[i] = gauss[i] * intensity;
            gaussian_coef.g_coef[i].w = 0.0f;
        }
        rctx->gaussian_coef_ubo.WriteMemory(gaussian_coef);
    }

    static void make_ghost_quad(uint8_t flags, float_t opacity, mat4* mat, float_t*& data) {
        const float_t x0 = flags & 0x01 ? 0.5f : 0.0f;
        const float_t x1 = flags & 0x01 ? 1.0f : 0.5f;
        const float_t y0 = flags & 0x02 ? 0.5f : 0.0f;
        const float_t y1 = flags & 0x02 ? 1.0f : 0.5f;

        vec4 p0 = { -0.5f, -0.5f, x0, y0 };
        vec4 p1 = { 0.5f, -0.5f, x1, y0 };
        vec4 p2 = { -0.5f,  0.5f, x0, y1 };
        vec4 p3 = { 0.5f,  0.5f, x1, y1 };
        mat4_transform_point(mat, (vec2*)&p0, (vec2*)&p0);
        mat4_transform_point(mat, (vec2*)&p1, (vec2*)&p1);
        mat4_transform_point(mat, (vec2*)&p2, (vec2*)&p2);
        mat4_transform_point(mat, (vec2*)&p3, (vec2*)&p3);

        *(vec4*)&data[0] = p0;
        data[4] = opacity;
        *(vec4*)&data[5] = p1;
        data[9] = opacity;
        *(vec4*)&data[10] = p2;
        data[14] = opacity;
        *(vec4*)&data[15] = p2;
        data[19] = opacity;
        *(vec4*)&data[20] = p1;
        data[24] = opacity;
        *(vec4*)&data[25] = p3;
        data[29] = opacity;
        data += 30;
    }

    void Render::calc_taa_blend() {
        float_t view_point_dist = vec3::distance(view_point, view_point_prev);
        float_t interest_dist = vec3::distance(interest, interest_prev);
        float_t dist = max_def(view_point_dist, interest_dist);

        if (!reset_exposure) {
            if (dist < 0.0001f)
                taa_blend = 0.5f;
            else if (dist < 0.005f)
                taa_blend = 0.75f;
            else if (dist < 0.03f)
                taa_blend = 0.875f;
            else if (dist <= 0.05f && cam_blur)
                taa_blend = 0.999f;
        }
    }

    void Render::copy_to_frame_texture(GLuint pre_pp_tex, int32_t wight, int32_t height, GLuint post_pp_tex) {
        for (Render::FrameTexture& i : frame_texture) {
            if (!i.capture)
                continue;

            for (auto& j : i.data) {
                if (!j.texture || j.render_texture.Bind() < 0)
                    continue;

                texture* dst_tex = j.texture;

                gl_state_active_bind_texture_2d(0, j.type == FRAME_TEXTURE_PRE_PP ? pre_pp_tex : post_pp_tex);
                gl_state_bind_sampler(0, rctx->render_samplers[0]);
                gl_state_set_viewport(0, 0, dst_tex->width, dst_tex->height);
                uniform->arr[U_REDUCE] = 0;
                shaders_ft.set(SHADER_FT_REDUCE);
                draw_quad(dst_tex->width, dst_tex->height,
                    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                gl_state_bind_framebuffer(0);
            }

            i.capture = false;
        }
    }

    void Render::draw_lens_ghost() {
        static const float_t v13[16] = {
            -0.70f, -0.30f,  0.35f,  0.50f,
            -0.45f, -0.80f,  0.20f,  0.41f,
             0.17f, -0.10f,  0.06f,  0.10f,
             0.14f,  0.04f, -0.13f, -0.22f,
        };

        static const float_t v14[16] = {
            0.8f, 1.0f, 1.0f, 1.0f,
            0.4f, 0.5f, 0.8f, 0.8f,
            0.6f, 0.7f, 0.8f, 0.7f,
            0.8f, 0.7f, 0.6f, 0.8f,
        };

        static const float_t v15[16] = {
             1.3f, 1.5f, 1.00f, 1.1f,
             2.5f, 0.8f, 0.50f, 0.5f,
             0.7f, 0.4f, 0.35f, 0.5f,
             0.4f, 0.3f, 0.60f, 0.4f,
        };

        if (!lens_flare_texture || lens_ghost_count <= 0)
            return;

        if (lens_ghost_count > 16)
            lens_ghost_count = 16;

        const float_t aspect = (float_t)render_width[0] / (float_t)render_height[0];

        const float_t v7 = (lens_flare_pos.x - (float_t)(width / 2)) / (float_t)width;
        const float_t v8 = -((lens_flare_pos.y - (float_t)(height / 2)) / (float_t)height);
        const float_t v9 = (float_t)((1.1 - sqrtf(v8 * v8 + v7 * v7)) * lens_flare_power);
        if (v9 < 0.001f)
            return;

        const float_t v9a = v9 * v9;

        float_t angle = atan2f(v8, v7) - (float_t)M_PI_2;
        const float_t angle_sin = sinf(angle);
        const float_t angle_cos = cosf(angle);

        float_t* data = (float_t*)rctx->lens_ghost_vbo.MapMemory();
        if (!data)
            return;

        const float_t lens_ghost = this->lens_ghost;
        const int32_t lens_ghost_count = this->lens_ghost_count;
        for (int32_t i = 0; i < lens_ghost_count; i++) {
            float_t opacity = v9 * v14[i] * lens_ghost;

            float_t scale = (v9a * 0.03f + 0.02f) * v15[i];

            mat4 mat;
            mat4_translate(v13[i] * v7 + 0.5f, v13[i] * v8 + 0.5f, 0.0f, &mat);
            mat4_scale_rot(&mat, scale, scale * aspect, 1.0f, &mat);
            mat4_mul_rotate_z(&mat, angle_sin, angle_cos, &mat);
            make_ghost_quad((uint8_t)(i & 0x03), opacity, &mat, data);
        }

        rctx->lens_ghost_vbo.UnmapMemory();

        gl_state_set_viewport(0, 0, render_width[0], render_height[0]);
        rend_texture[0].Bind();
        gl_state_enable_blend();
        gl_state_set_blend_func(GL_ONE, GL_ONE);

        uniform->arr[U_REDUCE] = 4;
        shaders_ft.set(SHADER_FT_REDUCE);
        gl_state_active_bind_texture_2d(0, lens_ghost_texture);
        gl_state_bind_sampler(0, rctx->render_samplers[0]);
        gl_state_bind_vertex_array(rctx->lens_ghost_vao);
        shaders_ft.draw_arrays(GL_TRIANGLES, 0, (GLsizei)(lens_ghost_count * 6LL));
        gl_state_bind_vertex_array(0);

        gl_state_disable_blend();
        gl_state_set_blend_func(GL_ONE, GL_ZERO);
    }

    void Render::generate_mlaa_area_texture() {
        uint8_t* data = force_malloc<uint8_t>(MLAA_SIDE_LEN * MLAA_SIDE_LEN * 2);
        if (!data)
            return;

        for (int32_t cross2 = 0; cross2 < 5; cross2++)
            for (int32_t cross1 = 0; cross1 < 5; cross1++)
                calculate_mlaa_area_texture_data(data, cross1, cross2);

        glGenTextures(1, &mlaa_area_texture);
        gl_state_bind_texture_2d(mlaa_area_texture);
        glPixelStoreiDLL(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_RG8, MLAA_SIDE_LEN, MLAA_SIDE_LEN, 0, GL_RG, GL_UNSIGNED_BYTE, data);
        gl_state_bind_texture_2d(0);
        free_def(data);
    }

    void Render::get_blur() {
        uniform->arr[U_ALPHA_MASK] = 0;
        uniform->arr[U_GAUSS] = 0;
        shaders_ft.set(SHADER_FT_GAUSS);
        calc_gaussian_blur(1.0f, 1.0f, 7, 0.8f, 1.0f);
        rctx->gaussian_coef_ubo.Bind(1);

        downsample_texture.Bind();
        for (int32_t i = 1; i < 4; i++) {
            gl_state_set_viewport(0, 0, reduce_width[i], reduce_height[i]);
            gl_state_active_bind_texture_2d(0, reduce_tex[i]->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[2]);
            draw_quad(reduce_width[i], reduce_height[i],
                1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
            glCopyTexSubImage2DDLL(GL_TEXTURE_2D, 0, 0, 0, 0, 0, reduce_width[i], reduce_height[i]);
        }

        for (int32_t i = 1; i < 4; i++) {
            gl_state_set_viewport(0, 0, reduce_width[i], reduce_height[i]);
            gl_state_active_bind_texture_2d(0, reduce_tex[i]->glid);
            gl_state_bind_sampler(0, rctx->render_samplers[2]);
            draw_quad(reduce_width[i], reduce_height[i],
                1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
            glCopyTexSubImage2DDLL(GL_TEXTURE_2D, 0, 0, 0, 0, 0, reduce_width[i], reduce_height[i]);
        }

        uniform->arr[U_GAUSS] = 1;
        shaders_ft.set(SHADER_FT_GAUSS);

        gl_state_set_viewport(0, 0, reduce_width[0], reduce_height[0]);
        gl_state_active_bind_texture_2d(0, reduce_tex[0]->glid);
        gl_state_bind_sampler(0, rctx->render_samplers[2]);
        draw_quad(reduce_width[0], reduce_height[0],
            1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            intensity.x * 0.5f, intensity.y * 0.5f, intensity.z * 0.5f, 1.0f);

        uniform->arr[U_REDUCE] = 7;
        shaders_ft.set(SHADER_FT_REDUCE);

        gl_state_set_viewport(0, 0, reduce_width[0], reduce_height[0]);
        reduce_texture[0].Bind();
        gl_state_active_bind_texture_2d(0, downsample_texture.GetColorTex());
        gl_state_active_bind_texture_2d(1, reduce_tex[1]->glid);
        gl_state_active_bind_texture_2d(2, reduce_tex[2]->glid);
        gl_state_active_bind_texture_2d(3, reduce_tex[3]->glid);
        gl_state_bind_sampler(0, rctx->render_samplers[2]);
        gl_state_bind_sampler(1, rctx->render_samplers[2]);
        gl_state_bind_sampler(2, rctx->render_samplers[2]);
        gl_state_bind_sampler(3, rctx->render_samplers[2]);
        draw_quad(reduce_width[3], reduce_height[3],
            1.0f, 1.0f, 0.0f, 0.0f, 0.25f, 0.15f, 0.25f, 0.25f, 0.25f);
        reduce_tex_draw = reduce_tex[0]->glid;
    }

    void Render::update_tone_map_lut() {
        if (!update_lut)
            return;

        update_lut = false;
        saturate_lock = false;

        const float_t tone_map_scale = (float_t)(1.0 / (double_t)TONE_MAP_SAT_GAMMA_SAMPLES);
        const int32_t tone_map_size = 16 * TONE_MAP_SAT_GAMMA_SAMPLES;

        vec2 tex_data[16 * TONE_MAP_SAT_GAMMA_SAMPLES];
        float_t gamma_power = gamma * gamma_rate * 1.5f;
        int32_t saturate_power = this->saturate_power;
        float_t saturate_coeff = this->saturate_coeff[saturate_index];

        tex_data[0].x = 0.0f;
        tex_data[0].y = 0.0f;
        for (int32_t i = 1; i < tone_map_size; i++) {
            float_t gamma = powf(1.0f - expf((float_t)-i * tone_map_scale), gamma_power);
            float_t saturation = gamma * 2.0f - 1.0f;
            for (int32_t j = 0; j < saturate_power; j++) {
                saturation *= saturation;
                saturation *= saturation;
                saturation *= saturation;
                saturation *= saturation;
            }

            tex_data[i].x = gamma;
            tex_data[i].y = gamma * saturate_coeff
                * ((float_t)TONE_MAP_SAT_GAMMA_SAMPLES / (float_t)i) * (1.0f - saturation);
        }

        if (GL_VERSION_4_5)
            glTextureSubImage2D(tonemap_lut_texture, 0, 0, 0,
                16 * TONE_MAP_SAT_GAMMA_SAMPLES, 1, GL_RG, GL_FLOAT, tex_data);
        else {
            gl_state_bind_texture_2d(tonemap_lut_texture);
            glTexSubImage2DDLL(GL_TEXTURE_2D, 0, 0, 0,
                16 * TONE_MAP_SAT_GAMMA_SAMPLES, 1, GL_RG, GL_FLOAT, tex_data);
        }
    }

    static void calculate_mlaa_area_texture_data(uint8_t* data, int32_t cross1, int32_t cross2) {
        uint8_t* _data = &data[(MLAA_SIDE_LEN * MLAA_GRID_SIDE_LEN * cross2 + MLAA_GRID_SIDE_LEN * cross1) * 2];
        for (int32_t dright = 0; dright < MLAA_GRID_SIDE_LEN; dright++) {
            for (int32_t dleft = 0; dleft < MLAA_GRID_SIDE_LEN; dleft++) {
                float_t val_left = 0.0f;
                float_t val_right = 0.0f;
                calculate_mlaa_area_texture_data_inner(&val_left, &val_right, cross1, cross2, dleft, dright);
                _data[0] = (uint8_t)(val_left * 255.9f);
                _data[1] = (uint8_t)(val_right * 255.9f);
                _data += 2;
            }
            _data += MLAA_GRID_SIDE_LEN * 4 * 2;
        }
    }
    
    static float_t calc_area_tex_val(int32_t a1, int32_t a2) {
        float_t v2 = (float_t)(-0.5 / ((float_t)a2 - 0.5));
        float_t v3 = (float_t)a1 * v2 + 0.5f;
        if (a1 >= a2 - 1)
            return (v3 * 0.5f) * 0.5f;
        else
            return ((float_t)(a1 + 1) * v2 + 0.5f + v3) * 0.5f;
    };

    static void calculate_mlaa_area_texture_data_inner(float_t* val_left,
        float_t* val_right, int32_t cross1, int32_t cross2, int32_t dleft, int32_t dright) {
        int32_t d = dleft + dright + 1;
        float_t _val_left = 0.0f;
        float_t _val_right = 0.0f;
        switch (cross2) {
        case 0:
            switch (cross1) {
            case 1:
                _val_right = calc_area_tex_val(dleft, min_def(d, MLAA_GRID_SIDE_LEN));
                break;
            case 3:
                _val_left = calc_area_tex_val(dleft, min_def(d, MLAA_GRID_SIDE_LEN));
                break;
            }
            break;
        case 1:
            switch (cross1) {
            case 0:
                _val_right = calc_area_tex_val(dright, min_def(d, MLAA_GRID_SIDE_LEN));
                break;
            case 1:
                if (d % 2) {
                    int32_t v12 = d / 2 + 1;
                    int32_t v15 = d / 2;
                    if (dleft < v15)
                        _val_right = calc_area_tex_val(dleft, v12);
                    else if (dright < v15)
                        _val_right = calc_area_tex_val(dright, v12);
                    else
                        _val_right = calc_area_tex_val(v15, v12) * 2.0f;
                }
                else {
                    int32_t v12 = d / 2;
                    float_t v14 = -0.5f / (float_t)v12;
                    if (dleft >= v12)
                        _val_right = ((float_t)(dright * 2 + 1) * v14 + 1.0f) * 0.5f;
                    else
                        _val_right = ((float_t)(dleft * 2 + 1) * v14 + 1.0f) * 0.5f;
                }
                break;
            case 3:
                if (d % 2) {
                    int32_t v12 = d / 2 + 1;
                    int32_t v15 = d / 2;
                    if (dleft < v15)
                        _val_left = calc_area_tex_val(dleft, v12);
                    else if (dright < v15)
                        _val_right = calc_area_tex_val(dright, v12);
                    else
                        _val_left = _val_right = calc_area_tex_val(v15, v12);
                }
                else {
                    int32_t v12 = d / 2;
                    float_t v14 = -0.5f / (float_t)v12;
                    if (dleft < v12)
                        _val_left = ((float_t)(dleft * 2 + 1) * v14 + 1.0f) * 0.5f;
                    else
                        _val_right = ((float_t)(dright * 2 + 1) * v14 + 1.0f) * 0.5f;
                }
                break;
            case 4:
                if (d % 2) {
                    int32_t v12 = d / 2 + 1;
                    int32_t v15 = d / 2;
                    if (dleft < v15)
                        _val_left = calc_area_tex_val(dleft, v12);
                    else if (dright < v15)
                        _val_right = calc_area_tex_val(dright, v12);
                    else
                        _val_left = _val_right = calc_area_tex_val(v15, v12);
                }
                else {
                    int32_t v12 = d / 2;
                    float_t v14 = -0.5f / (float_t)v12;
                    if (dleft < v12)
                        _val_left = ((float_t)(dleft * 2 + 1) * v14 + 1.0f) * 0.5f;
                    else
                        _val_right = ((float_t)(dright * 2 + 1) * v14 + 1.0f) * 0.5f;
                }
                break;
            }
            break;
        case 3:
            switch (cross1) {
            case 0:
                _val_left = calc_area_tex_val(dright, min_def(d, MLAA_GRID_SIDE_LEN));
                break;
            case 1:
                if (d % 2) {
                    int32_t v12 = d / 2 + 1;
                    int32_t v15 = d / 2;
                    if (dleft < v15)
                        _val_right = calc_area_tex_val(dleft, v12);
                    else if (dright < v15)
                        _val_left = calc_area_tex_val(dright, v12);
                    else
                        _val_left = _val_right = calc_area_tex_val(v15, v12);
                }
                else {
                    int32_t v12 = d / 2;
                    float_t v14 = -0.5f / (float_t)v12;
                    if (dleft < v12)
                        _val_right = ((float_t)(dleft * 2 + 1) * v14 + 1.0f) * 0.5f;
                    else
                        _val_left = ((float_t)(dright * 2 + 1) * v14 + 1.0f) * 0.5f;
                }
                break;
            case 3:
                if (d % 2) {
                    int32_t v12 = d / 2 + 1;
                    int32_t v15 = d / 2;
                    if (dleft < v15)
                        _val_left = calc_area_tex_val(dleft, v12);
                    else if (dright < v15)
                        _val_left = calc_area_tex_val(dright, v12);
                    else
                        _val_left = calc_area_tex_val(v15, v12) * 2.0f;
                }
                else {
                    int32_t v12 = d / 2;
                    float_t v14 = -0.5f / (float_t)v12;
                    if (dleft < v12)
                        _val_left = ((float_t)(dleft * 2 + 1) * v14 + 1.0f) * 0.5f;
                    else
                        _val_left = ((float_t)(dright * 2 + 1) * v14 + 1.0f) * 0.5f;
                }
                break;
            case 4:
                if (d % 2) {
                    int32_t v12 = d / 2 + 1;
                    int32_t v15 = d / 2;
                    if (dleft < v15)
                        _val_right = calc_area_tex_val(dleft, v12);
                    else if (dright < v15)
                        _val_left = calc_area_tex_val(dright, v12);
                    else
                        _val_left = _val_right = calc_area_tex_val(v15, v12);
                }
                else {
                    int32_t v12 = d / 2;
                    float_t v14 = -0.5f / (float_t)v12;
                    if (dleft < v12)
                        _val_right = ((float_t)(dleft * 2 + 1) * v14 + 1.0f) * 0.5f;
                    else
                        _val_left = ((float_t)(dright * 2 + 1) * v14 + 1.0f) * 0.5f;
                }
                break;
            }
            break;
        case 4:
            switch (cross1) {
            case 1:
                if (d % 2) {
                    int32_t v12 = d / 2 + 1;
                    int32_t v15 = d / 2;
                    if (dleft < v15)
                        _val_right = calc_area_tex_val(dleft, v12);
                    else if (dright < v15)
                        _val_left = calc_area_tex_val(dright, v12);
                    else
                        _val_left = _val_right = calc_area_tex_val(v15, v12);
                }
                else {
                    int32_t v12 = d / 2;
                    float_t v14 = -0.5f / (float_t)v12;
                    if (dleft < v12)
                        _val_right = ((float_t)(dleft * 2 + 1) * v14 + 1.0f) * 0.5f;
                    else
                        _val_left = ((float_t)(dright * 2 + 1) * v14 + 1.0f) * 0.5f;
                }
                break;
            case 3:
                if (d % 2) {
                    int32_t v12 = d / 2 + 1;
                    int32_t v15 = d / 2;
                    if (dleft < v15)
                        _val_left = calc_area_tex_val(dleft, v12);
                    else if (dright < v15)
                        _val_right = calc_area_tex_val(dright, v12);
                    else
                        _val_left = _val_right = calc_area_tex_val(v15, v12);
                }
                else {
                    int32_t v12 = d / 2;
                    float_t v14 = -0.5f / (float_t)v12;
                    if (dleft < v12)
                        _val_left = ((float_t)(dleft * 2 + 1) * v14 + 1.0f) * 0.5f;
                    else
                        _val_right = ((float_t)(dright * 2 + 1) * v14 + 1.0f) * 0.5f;
                }
                break;
            }
            break;
        }

        *val_left = _val_left;
        *val_right = _val_right;
        return;
    }
}
