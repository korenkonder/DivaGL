/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "dof.hpp"
#include "../rob/rob.hpp"
#include "../camera.hpp"
#include "../gl_state.hpp"
#include "../render_context.hpp"

struct dof_common_shader_data {
    vec4 g_depth_params; //x=(n-f)/(nf), y=1/n, z=coc_from_z_scale, w=coc_from_z_offset
    vec4 g_spread_scale; //x=scale_from_meter_to_pixel, y=scale_from_meter_to_sample,
        //  z=scale_from_pixel_to_sample, w=scale_from_sample_to_pixel
    vec4 g_depth_params2; //x=distance_to_focus_m, y=focus_range,
        // z=k/(fuzzing_range*fuzzing_range), w=max_coc_radius_in_pixel   //yzw=for_f2
};

dof_debug* dof_debug_data = (dof_debug*)0x00000001411AB650;
dof_pv* dof_pv_data = (dof_pv*)0x0000001411AB630;

namespace renderer {
    void DOF3::apply(RenderTexture* rt) {
        if (dof_debug_data->flags & DOF_DEBUG_USE_UI_PARAMS) {
            if (dof_debug_data->flags & DOF_DEBUG_ENABLE_DOF) {
                if (dof_debug_data->flags & DOF_DEBUG_ENABLE_PHYS_DOF) {
                    float_t focus = dof_debug_data->focus;
                    if (dof_debug_data->flags & DOF_DEBUG_AUTO_FOCUS) {
                        size_t rob_chara_smth = get_rob_chara_smth();

                        size_t rob_chr = 0;
                        for (int32_t i = 0; i < ROB_CHARA_COUNT; i++) {
                            size_t rob_chr = rob_chara_array_get(rob_chara_smth, i);
                            if (!rob_chr || !(*(uint8_t*)(rob_chara_smth + 0x440) & 1))
                                continue;

                            mat4 mat;
                            sub_1405163C0(rob_chr, 4, &mat);
                            mat4_transpose(&mat, &mat);

                            vec3 chara_trans = 0.0f;
                            mat4_get_translation(&mat, &chara_trans);

                            mat4 view;
                            mat4_transpose(&camera_data->view, &view);
                            focus = -vec3::dot(*(vec3*)&view.row2, chara_trans) - view.row2.w - 0.1f;
                            break;
                        }
                    }

                    focus = max_def(focus, camera_data->min_distance);
                    apply_physical(rt, rt->GetColorTex(), rt->GetDepthTex(),
                        camera_data->min_distance, camera_data->max_distance,
                        focus, dof_debug_data->focal_length,
                        camera_data->fov * DEG_TO_RAD_FLOAT,
                        dof_debug_data->f_number);
                }
                else {
                    float_t fuzzing_range = max_def(dof_debug_data->f2.fuzzing_range, 0.01f);
                    apply_f2(rt, rt->GetColorTex(), rt->GetDepthTex(),
                        camera_data->min_distance, camera_data->max_distance,
                        camera_data->fov * DEG_TO_RAD_FLOAT,
                        dof_debug_data->f2.focus, dof_debug_data->f2.focus_range,
                        fuzzing_range, dof_debug_data->f2.ratio);
                }
            }
        }
        else if (dof_pv_data->enable && dof_pv_data->f2.ratio > 0.0f) {
            float_t fuzzing_range = max_def(dof_pv_data->f2.fuzzing_range, 0.01f);
            if (dof_pv_data->f2.fuzzing_range >= 0.0099999998)
                apply_f2(rt, rt->GetColorTex(), rt->GetDepthTex(),
                    camera_data->min_distance, camera_data->max_distance,
                    camera_data->fov * DEG_TO_RAD_FLOAT,
                    dof_pv_data->f2.focus, dof_pv_data->f2.focus_range,
                    fuzzing_range, dof_pv_data->f2.ratio);
            enum_or(dof_debug_data->flags, DOF_DEBUG_ENABLE_DOF);
            dof_debug_data->f2 = dof_pv_data->f2;
        }
        else
            enum_and(dof_debug_data->flags, ~DOF_DEBUG_ENABLE_DOF);
    }

    void DOF3::resize(int32_t width, int32_t height) {
        if (textures[0]) {
            glDeleteTextures(6, textures);
            textures[0] = 0;
        }

        void(FASTCALL * renderer__FBO__init)(void* fbo, int32_t width, int32_t height,
            GLuint * color_textures, int32_t count, GLuint depth_texture)
            = (void(FASTCALL*)(void* fbo, int32_t width, int32_t height,
                GLuint * color_textures, int32_t count, GLuint depth_texture))0x00000001404AEF40;

        this->width = width;
        this->height = height;
        int32_t w20 = max_def(width / 20, 1);
        int32_t h20 = max_def(height / 20, 1);
        int32_t w2 = max_def(width / 2, 1);
        int32_t h2 = max_def(height / 2, 1);
        glGenTextures(6, textures);
        gl_state_bind_texture_2d(textures[0]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG16F, w20, h20);
        renderer__FBO__init(&fbo[0], w20, h20, &textures[0], 1, 0);
        gl_state_bind_texture_2d(textures[1]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG16F, w20, h20);
        renderer__FBO__init(&fbo[1], w20, h20, &textures[1], 1, 0);
        gl_state_bind_texture_2d(textures[2]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R11F_G11F_B10F, w2, h2);
        gl_state_bind_texture_2d(textures[3]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R11F_G11F_B10F, w2, h2);
        renderer__FBO__init(&fbo[2], w2, h2, &textures[2], 2, 0);
        gl_state_bind_texture_2d(textures[4]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R11F_G11F_B10F, w2, h2);
        gl_state_bind_texture_2d(textures[5]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, w2, h2);
        renderer__FBO__init(&fbo[3], w2, h2, &textures[4], 2, 0);
    }

    void DOF3::apply_f2(RenderTexture* rt, GLuint color_texture,
        GLuint depth_texture, float_t min_distance, float_t max_distance, float_t fov,
        float_t focus, float_t focus_range, float_t fuzzing_range, float_t ratio) {
        gl_state_begin_event("renderer::DOF3::apply_f2");
        gl_state_disable_blend();
        gl_state_set_depth_mask(GL_FALSE);
        gl_state_set_depth_func(GL_ALWAYS);
        update_data(min_distance, max_distance,
            fov, focus, 0.0f, 1.0f, focus_range, fuzzing_range, ratio);

        gl_state_bind_vertex_array(vao);
        render_tiles(depth_texture, true);
        downsample(color_texture, depth_texture, true);
        main_filter(true);
        upsample(rt, color_texture, depth_texture, true);

        gl_state_use_program(0);
        for (int32_t i = 0; i < 8; i++) {
            gl_state_bind_sampler(i, 0);
            gl_state_active_bind_texture_2d(i, 0);
        }
        gl_state_bind_vertex_array(0);
        gl_state_end_event();
    }

    void DOF3::apply_physical(RenderTexture* rt, GLuint color_texture,
        GLuint depth_texture, float_t min_distance, float_t max_distance,
        float_t focus, float_t focal_length, float_t fov, float_t f_number) {
        gl_state_begin_event("renderer::DOF3::apply_physical");
        gl_state_disable_blend();
        gl_state_set_depth_mask(GL_FALSE);
        gl_state_set_depth_func(GL_ALWAYS);
        update_data(min_distance, max_distance,
            fov, focus, focal_length, f_number, 0.0f, 0.1f, 0.0f);

        gl_state_bind_vertex_array(vao);
        render_tiles(depth_texture, false);
        downsample(color_texture, depth_texture, false);
        main_filter(false);
        upsample(rt, color_texture, depth_texture, false);

        gl_state_use_program(0);
        for (int32_t i = 0; i < 8; i++) {
            gl_state_bind_sampler(i, 0);
            gl_state_active_bind_texture_2d(i, 0);
        }
        gl_state_bind_vertex_array(0);
        gl_state_end_event();
    }

    void DOF3::render_tiles(GLuint depth_texture, bool f2) {
        gl_state_begin_event("renderer::DOF3::render_tiles");
        gl_state_bind_framebuffer(fbo[0].buffer);
        glViewportDLL(0, 0, fbo[0].width, fbo[0].height);
        gl_state_use_program(program[f2 ? 5 : 1]);
        common_ubo.Bind(0);
        gl_state_active_bind_texture_2d(0, depth_texture);
        gl_state_bind_sampler(0, samplers[1]);
        glDrawArraysDLL(GL_TRIANGLE_STRIP, 0, 4);

        gl_state_bind_framebuffer(fbo[1].buffer);
        glViewportDLL(0, 0, fbo[1].width, fbo[1].height);
        gl_state_use_program(program[0]);
        gl_state_active_bind_texture_2d(0, textures[0]);
        gl_state_bind_sampler(0, samplers[1]);
        glDrawArraysDLL(GL_TRIANGLE_STRIP, 0, 4);
        gl_state_end_event();

    }

    void DOF3::downsample(GLuint color_texture, GLuint depth_texture, bool f2) {
        gl_state_begin_event("renderer::DOF3::downsample");
        gl_state_bind_framebuffer(fbo[2].buffer);
        glViewportDLL(0, 0, fbo[2].width, fbo[2].height);
        gl_state_use_program(program[f2 ? 6 : 2]);
        common_ubo.Bind(0);
        gl_state_active_bind_texture_2d(0, depth_texture);
        gl_state_bind_sampler(0, samplers[1]);
        gl_state_active_bind_texture_2d(1, color_texture);
        gl_state_bind_sampler(1, samplers[0]);
        gl_state_active_bind_texture_2d(2, textures[1]);
        gl_state_bind_sampler(2, samplers[1]);
        glDrawArraysDLL(GL_TRIANGLE_STRIP, 0, 4);
        gl_state_end_event();
    }

    void DOF3::main_filter(bool f2) {
        gl_state_begin_event("renderer::DOF3::main_filter");
        gl_state_bind_framebuffer(fbo[3].buffer);
        glViewportDLL(0, 0, fbo[3].width, fbo[3].height);
        gl_state_use_program(program[f2 ? 7 : 3]);
        common_ubo.Bind(0);
        texcoords_ubo.Bind(1);
        gl_state_active_bind_texture_2d(0, textures[3]);
        gl_state_bind_sampler(0, samplers[1]);
        gl_state_active_bind_texture_2d(1, textures[2]);
        gl_state_bind_sampler(1, samplers[1]);
        gl_state_active_bind_texture_2d(2, textures[1]);
        gl_state_bind_sampler(2, samplers[1]);
        glDrawArraysDLL(GL_TRIANGLE_STRIP, 0, 4);
        gl_state_end_event();
    }

    void DOF3::upsample(RenderTexture* rt, GLuint color_texture, GLuint depth_texture, bool f2) {
        gl_state_begin_event("renderer::DOF3::upsample");
        rctx->render_buffer.Bind();
        glViewportDLL(0, 0, width, height);
        gl_state_use_program(program[f2 ? 8 : 4]);
        common_ubo.Bind(0);
        gl_state_active_bind_texture_2d(0, textures[4]);
        gl_state_bind_sampler(0, samplers[1]);
        gl_state_active_bind_texture_2d(1, textures[5]);
        gl_state_bind_sampler(1, samplers[1]);
        gl_state_active_bind_texture_2d(2, textures[1]);
        gl_state_bind_sampler(2, samplers[1]);
        gl_state_active_bind_texture_2d(3, color_texture);
        gl_state_bind_sampler(3, samplers[1]);
        gl_state_active_bind_texture_2d(4, depth_texture);
        gl_state_bind_sampler(4, samplers[1]);
        glDrawArraysDLL(GL_TRIANGLE_STRIP, 0, 4);

        glCopyImageSubData(
            rctx->render_buffer.GetColorTex(), GL_TEXTURE_2D, 0, 0, 0, 0,
            rt->GetColorTex(), GL_TEXTURE_2D, 0, 0, 0, 0, width, height, 1);
        gl_state_end_event();
    }

    void DOF3::update_data(float_t min_dist, float_t max_dist, float_t fov, float_t dist, float_t focal_length,
        float_t f_number, float_t focus_range, float_t fuzzing_range, float_t ratio) {
        float_t fl = focal_length;
        if (dist <= focal_length)
            fl = dist + 0.1f;
        fl = fl / (dist - fl) * fl / f_number;

        dof_common_shader_data shader_data = {};
        shader_data.g_depth_params.x = 1.0f / (min_dist * max_dist) * (min_dist - max_dist);
        shader_data.g_depth_params.y = 1.0f / min_dist;
        shader_data.g_depth_params.z = -((fl * dist * (min_dist - max_dist)) * (1.0f / (min_dist * max_dist)));
        shader_data.g_depth_params.w = (1.0f - 1.0f / min_dist * dist) * fl;
        shader_data.g_spread_scale.x = 720.0f / (tanf(fov * 0.5f) * (min_dist * 2.0f));
        shader_data.g_spread_scale.y = shader_data.g_spread_scale.x * (float_t)(1.0 / 3.0);
        shader_data.g_spread_scale.z = (float_t)(1.0 / 3.0);
        shader_data.g_spread_scale.w = 3.0f;
        shader_data.g_depth_params2.x = dist;
        shader_data.g_depth_params2.y = focus_range;
        shader_data.g_depth_params2.z = -4.5f / (fuzzing_range * fuzzing_range);
        shader_data.g_depth_params2.w = ratio * 8.0f;
        common_ubo.WriteMemory(shader_data);
    }

}
