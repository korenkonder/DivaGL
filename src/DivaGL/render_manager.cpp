/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_manager.hpp"
#include "../KKdLib/mat.hpp"
#include "Glitter/glitter.hpp"
#include "light_param/fog.hpp"
#include "light_param/light.hpp"
#include "mdl/disp_manager.hpp"
#include "rob/rob.hpp"
#include "clear_color.hpp"
#include "effect.hpp"
#include "gl_state.hpp"
#include "light_param.hpp"
#include "render.hpp"
#include "render_context.hpp"
#include "render_texture.hpp"
#include "shader_ft.hpp"
#include "sprite.hpp"
#include "stage_param.hpp"
#include "texture.hpp"
#include <Helpers.h>

static void draw_pass_shadow_begin_make_shadowmap(Shadow* shad, int32_t index, int32_t a3);
static void draw_pass_shadow_end_make_shadowmap(Shadow* shad, int32_t index, int32_t a3);
static void draw_pass_shadow_filter(RenderTexture* a1, RenderTexture* a2,
    RenderTexture* a3, float_t sigma, float_t far_texel_offset, bool enable_lit_proj);
static void draw_pass_shadow_esm_filter(RenderTexture* dst, RenderTexture* buf, RenderTexture* src);
static bool draw_pass_shadow_litproj();
static bool draw_pass_shadow_litproj_set();
static bool draw_pass_shadow_litproj_set_mat(bool set_mat);
static void draw_pass_sss_contour(rndr::Render* rend);
static void draw_pass_sss_filter(sss_data* sss);
static int32_t draw_pass_3d_get_translucent_count();
static void draw_pass_3d_shadow_reset();
static void draw_pass_3d_shadow_set(Shadow* shad);
extern void draw_pass_3d_translucent(bool opaque_enable,
    bool transparent_enable, bool translucent_enable, mdl::ObjType opaque,
    mdl::ObjType transparent, mdl::ObjType translucent);
static int32_t draw_pass_3d_translucent_count_layers(int32_t* alpha_array,
    mdl::ObjType opaque, mdl::ObjType transparent, mdl::ObjType translucent);
static void draw_pass_3d_translucent_has_objects(bool* arr, mdl::ObjType type);

static void blur_filter_apply(RenderTexture* dst, RenderTexture* src,
    blur_filter_mode filter, const vec2 res_scale, const vec4 scale, const vec4 offset);

static void render_manager_free_render_textures();
static void render_manager_init_render_textures(int32_t multisample);

extern render_context* rctx;

RenderTexture* litproj_shadow = (RenderTexture*)0x0000000141194DA0;
RenderTexture* litproj_texture = (RenderTexture*)0x0000000141194E00;
rndr::RenderManager* render_manager = (rndr::RenderManager*)0x00000001411AD320;

namespace rndr {
    struct RenderTextureData {
        GLenum type;
        int32_t width;
        int32_t height;
        int32_t max_level;
        GLenum color_format;
        GLenum depth_format;
    };

    struct RenderTextureIndex {
        int32_t arr[3];
    };

    static const RenderTextureData* render_manager_render_texture_data_array
        = (const RenderTextureData*)0x0000000140A24420;

    static const RenderTextureIndex* render_manager_render_texture_index_array
        = (const RenderTextureIndex*)0x0000000140A244E0;

    void RenderManager::add_pre_process(int32_t type, void(*func)(void*), void* data) {
        pre_process.push_back({ type, func, data });
    }

    void RenderManager::clear_pre_process(int32_t type) {
        for (auto i = pre_process.begin(); i != pre_process.end(); i++)
            if (i->type == type) {
                pre_process.erase(i);
                break;
            }
    }

    RenderTexture& RenderManager::get_render_texture(int32_t index) {
        return render_textures[render_manager_render_texture_index_array[index].arr[tex_index[index]]];
    }

    void RenderManager::reset() {
        for (int32_t i = 0; i < RND_PASSID_NUM; i++) {
            pass_sw[i] = true;
            cpu_time[i] = 0.0;
            gpu_time[i] = 0.0;
        }

        set_pass_sw(RND_PASSID_2, 0);
        set_pass_sw(RND_PASSID_REFLECT, 0);
        set_pass_sw(RND_PASSID_REFRACT, 0);
        set_pass_sw(RND_PASSID_PRE_PROCESS, 0);
        set_pass_sw(RND_PASSID_SHOW_VECTOR, 0);

        shadow_ptr = 0;
        sync_gpu = false;
        time.get_timestamp();

        shadow = true;
        opaque_z_sort = true;
        alpha_z_sort = true;
        field_11F = false;
        field_120 = false;

        reflect_blur_num = 1;
        reflect_blur_filter = BLUR_FILTER_4;
        show_ref_map = false;

        reflect_type = STAGE_DATA_REFLECT_DISABLE;
        check_state = false;
        show_vector_flags = 0;
        show_vector_length = 0.05f;
        show_vector_z_offset = 0.0f;
        show_stage_shadow = false;
        effect_texture = 0;

        reflect = false;
        refract = false;
        npr_param = 0;
        field_31C = false;
        reflect_texture_mask = false;
        reflect_tone_curve = false;
        field_31F = false;
        light_stage_ambient = false;

        for (bool& i : draw_pass_3d)
            i = true;

        field_11E = false;
        clear = false;

        width = 0;
        height = 0;
        multisample_framebuffer = 0;
        multisample_renderbuffer = 0;
        multisample = true;

        npr = false;
    }

    void RenderManager::resize(int32_t width, int32_t height) {
        if (this->width == width && this->height == height)
            return;

        this->width = width;
        this->height = height;

        if (!multisample_framebuffer)
            glGenFramebuffers(1, &multisample_framebuffer);

        if (!multisample_renderbuffer)
            glGenRenderbuffers(1, &multisample_renderbuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, multisample_framebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, multisample_renderbuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGBA8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multisample_renderbuffer);
        glDrawBufferDLL(GL_COLOR_ATTACHMENT0);
        glReadBufferDLL(GL_COLOR_ATTACHMENT0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void RenderManager::set_clear(bool value) {
        clear = value;
    }
    
    void RenderManager::set_effect_texture(texture* value) {
        effect_texture = value;
    }

    void RenderManager::set_multisample(bool value) {
        multisample = value;
    }

    void RenderManager::set_npr_param(int32_t value) {
        npr_param = value;
    }

    void RenderManager::set_pass_sw(RenderPassID id, bool value) {
        pass_sw[id] = value;
    }

    void RenderManager::set_reflect(bool value) {
        reflect = value;
    }

    void RenderManager::set_reflect_blur(int32_t reflect_blur_num, blur_filter_mode reflect_blur_filter) {
        this->reflect_blur_num = reflect_blur_num;
        this->reflect_blur_filter = reflect_blur_filter;
    }

    void RenderManager::set_reflect_resolution_mode(reflect_refract_resolution_mode mode) {
        tex_index[0] = mode;
    }

    void RenderManager::set_reflect_type(stage_data_reflect_type type) {
        reflect_type = type;
    }

    void RenderManager::set_refract(bool value) {
        refract = value;
    }

    void RenderManager::set_refract_resolution_mode(reflect_refract_resolution_mode mode) {
        tex_index[1] = mode;
    }

    void RenderManager::set_shadow_false() {
        shadow = false;
    }

    void RenderManager::set_shadow_true() {
        shadow = true;
    }

    void RenderManager::render_all() {
        if (!rctx)
            return;

        light_param_data_storage_data_set_ibl();

        static const vec4 color_clear = 0.0f;

        gl_state_bind_framebuffer(0);
        glClearBufferfv(GL_COLOR, 0, (float_t*)&color_clear);

        rctx->set_scene_framebuffer_size(render->render_width[0],
            render->render_height[0], render->render_width[0], render->render_height[0]);

        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        gl_state_enable_depth_test();
        gl_state_set_depth_func(GL_LEQUAL);
        gl_state_set_depth_mask(GL_TRUE);

        gl_state_begin_event("rndpass_render_all_pass::Caller::execute_pre3d");
        for (int32_t i = RND_PASSID_SHADOW; i <= RND_PASSID_CLEAR; i++)
            RenderManager::render_single_pass((RenderPassID)i);
        gl_state_end_event();

        gl_state_begin_event("rndpass_render_all_pass::Caller::execute_3d");
        for (int32_t i = RND_PASSID_PRE_SPRITE; i <= RND_PASSID_3D; i++)
            RenderManager::render_single_pass((RenderPassID)i);
        gl_state_end_event();

        gl_state_begin_event("rndpass_render_all_pass::Caller::execute_2d");
        for (int32_t i = RND_PASSID_SHOW_VECTOR; i <= RND_PASSID_12; i++)
            RenderManager::render_single_pass((RenderPassID)i);
        gl_state_end_event();

        disp_manager->check_vertex_arrays();
        gl_state_bind_vertex_array(0);
        gl_state_disable_primitive_restart();
        gl_state_bind_uniform_buffer_base(0, 0);
        gl_state_bind_uniform_buffer_base(1, 0);
        gl_state_bind_uniform_buffer_base(2, 0);
        gl_state_bind_uniform_buffer_base(3, 0);
        gl_state_bind_uniform_buffer_base(4, 0);
    }

    void RenderManager::render_single_pass(RenderPassID id) {
        cpu_time[id] = 0.0;
        gpu_time[id] = 0.0;
        if (!pass_sw[id]) {
            gl_state_get_error();
            return;
        }

        render_pass_begin();
        switch (id) {
        case RND_PASSID_SHADOW:
            pass_shadow();
            break;
        case RND_PASSID_SS_SSS:
            pass_ss_sss();
            break;
        case RND_PASSID_REFLECT:
            pass_reflect();
            break;
        case RND_PASSID_REFRACT:
            pass_refract();
            break;
        case RND_PASSID_PRE_PROCESS:
            pass_pre_process();
            break;
        case RND_PASSID_CLEAR:
            pass_clear();
            break;
        case RND_PASSID_PRE_SPRITE:
            pass_pre_sprite();
            break;
        case RND_PASSID_3D:
            pass_3d();
            break;
        case RND_PASSID_SHOW_VECTOR:
            pass_show_vector();
            break;
        case RND_PASSID_POST_PROCESS:
            pass_post_process();
            break;
        case RND_PASSID_SPRITE:
            pass_sprite();
            break;
        }
        render_pass_end(id);
        gl_state_get_error();
    }

    void RenderManager::render_pass_begin() {
        if (sync_gpu)
            glFinishDLL();
        time.get_timestamp();
    }

    void RenderManager::render_pass_end(RenderPassID id) {
        cpu_time[id] = time.calc_time();
        if (sync_gpu) {
            time_struct t;
            glFinishDLL();
            gpu_time[id] = t.calc_time();
        }
        else
            gpu_time[id] = 0;
    }

    void RenderManager::pass_shadow() {
        gl_state_begin_event("pass_shadow");
        gl_state_begin_event("texproj");
        if (draw_pass_shadow_litproj_set()) {
            disp_manager->draw(mdl::OBJ_TYPE_OPAQUE);
            disp_manager->draw(mdl::OBJ_TYPE_TRANSPARENT);
            disp_manager->draw(mdl::OBJ_TYPE_TRANSLUCENT);

            draw_state.shader_index = -1;
            uniform->arr[U_DEPTH] = 0;
            draw_pass_shadow_filter(&litproj_shadow[0], &litproj_shadow[1], 0, 1.5f, 0.01f, true);

            if (draw_pass_shadow_litproj()) {
                draw_pass_set_camera();

                for (int32_t i = LIGHT_SET_MAIN; i < LIGHT_SET_MAX; i++)
                    light_set_data[i].data_set((light_set_id)i);

                disp_manager->draw(mdl::OBJ_TYPE_OPAQUE);
                disp_manager->draw(mdl::OBJ_TYPE_TRANSPARENT);
                disp_manager->obj_sort(&rctx->view_mat, mdl::OBJ_TYPE_TRANSLUCENT, 1);
                gl_state_set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                gl_state_enable_blend();
                disp_manager->draw(mdl::OBJ_TYPE_TRANSLUCENT);
                gl_state_disable_blend();
                draw_state.shader_index = -1;
                gl_state_bind_framebuffer(0);
            }
        }
        gl_state_end_event();

        bool v3 = false;
        int32_t v10[2];
        for (int32_t i = 0; i < 2; i++) {
            v10[i] = disp_manager->get_obj_count((mdl::ObjType)(mdl::OBJ_TYPE_SHADOW_CHARA + i));
            if (v10[i])
                v3 = true;
        }

        Shadow* shad = shadow_ptr;
        if (shadow && v3) {
            int32_t v11[2];
            v11[0] = shad->field_200[0];
            v11[1] = shad->field_200[1];
            shad->curr_render_textures[0] = &shad->render_textures[0];
            shad->curr_render_textures[1] = &shad->render_textures[1];
            shad->curr_render_textures[2] = &shad->render_textures[2];

            for (int32_t i = 0, j = 0; i < 2; i++) {
                if (!v10[i])
                    continue;

                draw_pass_shadow_begin_make_shadowmap(shad, v11[i], j);
                disp_manager->draw((mdl::ObjType)(mdl::OBJ_TYPE_SHADOW_CHARA + v11[i]));
                if (disp_manager->get_obj_count((mdl::ObjType)(mdl::OBJ_TYPE_SHADOW_OBJECT_CHARA + v11[i])) > 0) {
                    gl_state_set_color_mask(show_stage_shadow
                        ? GL_TRUE : GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
                    disp_manager->draw((mdl::ObjType)(mdl::OBJ_TYPE_SHADOW_OBJECT_CHARA + i));
                    gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                }
                draw_pass_shadow_end_make_shadowmap(shad, v11[i], j);
                j++;
            }
        }
        else {
            shad->curr_render_textures[0] = &shad->render_textures[0];
            shad->curr_render_textures[1] = &shad->render_textures[1];
            shad->curr_render_textures[2] = &shad->render_textures[2];

            for (int32_t i = 1; i < 3; i++) {
                shad->curr_render_textures[i]->Bind();
                glClearColorDLL(1.0f, 1.0f, 1.0f, 1.0f);
                glClearDLL(GL_COLOR_BUFFER_BIT);
            }
        }
        shader::unbind();
        gl_state_bind_framebuffer(0);
        gl_state_end_event();
    }

    void RenderManager::pass_ss_sss() {
        ::sss_data* sss = sss_data_get();
        if (!sss->init_data || !sss->enable)
            return;

        gl_state_begin_event("pass_ss_sss");
        rndr::Render* rend = render;
        //if (rend->render_width > 1280.0)
        //    sss->npr_contour = false;

        //if (sss->npr_contour) {
            rend->rend_texture[0].Bind();
            rend->rend_texture[0].SetViewport();
        //}
        //else {
        //    sss->textures[0].Bind();
        //    gl_state_set_viewport(0, 0, 640, 360);
        //}

        glClearColorDLL(sss->param.x, sss->param.y, sss->param.z, 0.0f);
        glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_pass_set_camera();
        for (int32_t i = LIGHT_SET_MAIN; i < LIGHT_SET_MAX; i++)
            light_set_data[i].data_set((light_set_id)i);

        if (shadow)
            draw_pass_3d_shadow_set(shadow_ptr);
        else
            draw_pass_3d_shadow_reset();

        draw_state.shader_index = SHADER_FT_SSS_SKIN;
        gl_state_enable_depth_test();
        gl_state_set_depth_func(GL_LEQUAL);
        gl_state_set_depth_mask(GL_TRUE);
        disp_manager->draw(mdl::OBJ_TYPE_SSS);
        gl_state_disable_depth_test();
        draw_state.shader_index = -1;
        draw_pass_3d_shadow_reset();
        uniform->arr[U_NPR] = 0;

        if (npr_param == 1) {
            if (sss->enable && sss->npr_contour) {
                uniform->arr[U_NPR] = 1;
                draw_pass_sss_contour(rend);
            }
            else if (npr) {
                rend->rend_texture[0].Bind();
                rend->rend_texture[0].SetViewport();
                glClearDLL(GL_DEPTH_BUFFER_BIT);
                draw_state.shader_index = SHADER_FT_SSS_SKIN;
                gl_state_enable_depth_test();
                gl_state_set_depth_mask(GL_TRUE);
                disp_manager->draw(mdl::OBJ_TYPE_SSS);
                gl_state_disable_depth_test();
                draw_state.shader_index = -1;
                gl_state_bind_framebuffer(0);
                uniform->arr[U_NPR] = 1;
                draw_pass_sss_contour(rend);
            }
        }

        if (!sss->npr_contour) {
            sss->textures[0].Bind();
            gl_state_set_viewport(0, 0, 640, 360);

            filter_scene_shader_data filter_scene = {};
            filter_scene.g_transform = { 1.0f, 1.0f, 0.0f, 0.0f };
            filter_scene.g_texcoord = { 1.0f, 1.0f, 0.0f, 0.0f };
            rctx->filter_scene_ubo.WriteMemory(filter_scene);

            imgfilter_batch_shader_data imgfilter_batch = {};
            imgfilter_batch.g_color_scale = 1.0f;
            imgfilter_batch.g_color_offset = 0.0f;
            imgfilter_batch.g_texture_lod = 0.0f;
            rctx->imgfilter_batch_ubo.WriteMemory(imgfilter_batch);

            uniform->arr[U_IMAGE_FILTER] = 5;
            shaders_ft.set(SHADER_FT_IMGFILT);
            rctx->filter_scene_ubo.Bind(0);
            rctx->imgfilter_batch_ubo.Bind(1);
            gl_state_active_bind_texture_2d(0, rend->rend_texture[0].GetColorTex());
            gl_state_bind_sampler(0, rctx->render_samplers[0]);
            gl_state_bind_vertex_array(rctx->common_vao);
            shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        draw_pass_sss_filter(sss);
        shader::unbind();
        gl_state_bind_framebuffer(0);
        gl_state_end_event();
    }

    void RenderManager::pass_reflect() {
        gl_state_begin_event("pass_reflect");
        RenderTexture& refl_tex = render_manager->get_render_texture(0);
        RenderTexture& refl_buf_tex = rctx->reflect_buffer;
        refl_tex.Bind();
        if (disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_OPAQUE)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_TRANSPARENT)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_TRANSLUCENT)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_CHARA_OPAQUE)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_CHARA_TRANSPARENT)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_CHARA_TRANSLUCENT)) {
            refl_tex.SetViewport();

            draw_pass_set_camera();

            for (int32_t i = LIGHT_SET_MAIN; i < LIGHT_SET_MAX; i++)
                light_set_data[i].data_set((light_set_id)i);
            for (int32_t i = FOG_DEPTH; i < FOG_BUMP; i++)
                fog_data[i].data_set((fog_id)i);

            glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
            glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            draw_state.shader_index = SHADER_FT_S_REFL;

            light_set* set = &light_set_data[LIGHT_SET_MAIN];
            light_clip_plane clip_plane;
            set->lights[LIGHT_SPOT].get_clip_plane(clip_plane);
            uniform->arr[U_REFLECT] = 2;
            uniform->arr[U_CLIP_PLANE] = clip_plane.data[1];

            gl_state_enable_depth_test();
            gl_state_set_depth_func(GL_LEQUAL);
            gl_state_set_depth_mask(GL_TRUE);
            disp_manager->draw(mdl::OBJ_TYPE_REFLECT_OPAQUE, 0, reflect_texture_mask);
            if (reflect_type == STAGE_DATA_REFLECT_REFLECT_MAP) {
                gl_state_enable_blend();
                gl_state_set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                gl_state_set_blend_equation(GL_FUNC_ADD);
                disp_manager->draw(mdl::OBJ_TYPE_REFLECT_TRANSLUCENT);
                gl_state_disable_blend();
            }

            if (reflect_type == STAGE_DATA_REFLECT_REFLECT_MAP) {
                uniform->arr[U_REFLECT] = reflect_tone_curve ? 1 : 0;
                uniform->arr[U_CLIP_PLANE] = clip_plane.data[0];
                disp_manager->draw(mdl::OBJ_TYPE_REFLECT_CHARA_OPAQUE);
            }
            gl_state_disable_depth_test();
            uniform->arr[U_REFLECT] = 0;
            draw_state.shader_index = -1;

            refl_buf_tex.Bind();
            for (int32_t i = reflect_blur_num, j = 0; i > 0; i--, j++) {
                blur_filter_apply(&refl_buf_tex, &refl_tex,
                    reflect_blur_filter, 1.0f, 1.0f, 0.0f);
                image_filter_scale(&refl_tex, refl_buf_tex.color_texture);
            }
        }
        else {
            vec4 clear_color;
            glGetFloatvDLL(GL_COLOR_CLEAR_VALUE, (GLfloat*)&clear_color);
            glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
            glClearDLL(GL_COLOR_BUFFER_BIT);
            glClearColorDLL(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        }
        shader::unbind();
        gl_state_bind_framebuffer(0);
        gl_state_end_event();
    }

    void RenderManager::pass_refract() {
        gl_state_begin_event("pass_refract");
        RenderTexture& refract_texture = render_manager->get_render_texture(1);
        refract_texture.Bind();
        if (disp_manager->get_obj_count(mdl::OBJ_TYPE_REFRACT_OPAQUE)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFRACT_TRANSPARENT)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFRACT_TRANSLUCENT)) {
            texture* refr_tex = refract_texture.color_texture;
            gl_state_set_viewport(0, 0, refr_tex->width, refr_tex->height);

            draw_pass_set_camera();

            for (int32_t i = LIGHT_SET_MAIN; i < LIGHT_SET_MAX; i++)
                light_set_data[i].data_set((light_set_id)i);
            for (int32_t i = FOG_DEPTH; i < FOG_BUMP; i++)
                fog_data[i].data_set((fog_id)i);

            vec4 clear_color;
            glGetFloatvDLL(GL_COLOR_CLEAR_VALUE, (GLfloat*)&clear_color);
            glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
            glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColorDLL(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

            draw_state.shader_index = SHADER_FT_S_REFR;
            gl_state_enable_depth_test();
            gl_state_set_depth_func(GL_LEQUAL);
            gl_state_set_depth_mask(GL_TRUE);
            disp_manager->draw(mdl::OBJ_TYPE_REFRACT_OPAQUE);
            disp_manager->draw(mdl::OBJ_TYPE_REFRACT_TRANSPARENT);
            disp_manager->draw(mdl::OBJ_TYPE_REFRACT_TRANSLUCENT);
            gl_state_disable_depth_test();
            draw_state.shader_index = -1;
        }
        else {
            vec4 clear_color;
            glGetFloatvDLL(GL_COLOR_CLEAR_VALUE, (GLfloat*)&clear_color);
            glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
            glClearDLL(GL_COLOR_BUFFER_BIT);
            glClearColorDLL(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        }
        shader::unbind();
        gl_state_bind_framebuffer(0);
        gl_state_end_event();
    }

    void RenderManager::pass_pre_process() {
        gl_state_begin_event("pass_pre_process");
        for (draw_pre_process& i : pre_process)
            if (i.func)
                i.func(i.data);
        shader::unbind();
        gl_state_bind_framebuffer(0);
        gl_state_end_event();
    }

    void RenderManager::pass_clear() {
        gl_state_begin_event("pass_clear");
        if (clear) {
            rctx->screen_buffer.Bind();
            clear_color_set_gl();
            //glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearDLL(GL_COLOR_BUFFER_BIT);
            gl_state_bind_framebuffer(0);
        }

        rndr::Render* rend = render;
        if (!sss_data_get()->enable || !sss_data_get()->npr_contour) {
            rend->bind_render_texture();
            if (sprite_manager_get_reqlist_count(2)) {
                glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
                glClearDepthf(1.0f);
                glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                gl_state_bind_framebuffer(0);
            }
            else {
                clear_color_set_gl();
                glClearDepthf(1.0f);
                if (clear)
                    glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                else
                    glClearDLL(GL_DEPTH_BUFFER_BIT);
                gl_state_bind_framebuffer(0);
            }
        }
        else {
            if (sprite_manager_get_reqlist_count(2)) {
                rend->bind_render_texture();
                glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
                glClearDLL(GL_COLOR_BUFFER_BIT);
                gl_state_bind_framebuffer(0);
            }
            else if (clear) {
                rend->bind_render_texture();
                clear_color_set_gl();
                glClearDLL(GL_COLOR_BUFFER_BIT);
                gl_state_bind_framebuffer(0);
            }
        }

        gl_state_get_error();
        gl_state_end_event();
    }

    void RenderManager::pass_pre_sprite() {
        if (!sprite_manager_get_reqlist_count(2))
            return;

        gl_state_begin_event("pass_pre_sprite");
        rndr::Render* rend = render;
        rend->bind_render_texture(true);
        glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDLL(GL_COLOR_BUFFER_BIT);
        sprite_manager_set_view_projection(true);
        gl_state_set_depth_mask(GL_FALSE);
        gl_state_disable_depth_test();
        gl_state_enable_blend();
        gl_state_disable_cull_face();
        sprite_manager_draw(2, true,
            rend->temp_buffer.color_texture);
        gl_state_enable_cull_face();
        gl_state_disable_blend();
        gl_state_enable_depth_test();
        gl_state_set_depth_mask(GL_TRUE);
        shader::unbind();
        gl_state_bind_framebuffer(0);
        gl_state_get_error();
        gl_state_end_event();
    }

    void RenderManager::pass_3d() {
        render->bind_render_texture();
        draw_pass_set_camera();
        if (!sss_data_get()->enable || !sss_data_get()->npr_contour
            || draw_pass_3d_get_translucent_count())
            glClearDLL(GL_DEPTH_BUFFER_BIT);

        gl_state_set_depth_func(GL_LEQUAL);

        for (int32_t i = LIGHT_SET_MAIN; i < LIGHT_SET_MAX; i++)
            light_set_data[i].data_set((light_set_id)i);
        for (int32_t i = FOG_DEPTH; i < FOG_MAX; i++)
            fog_data[i].data_set((fog_id)i);

        if (shadow)
            draw_pass_3d_shadow_set(shadow_ptr);
        else
            draw_pass_3d_shadow_reset();

        if (effect_texture)
            gl_state_active_bind_texture_2d(14, effect_texture->glid);
        else
            gl_state_active_bind_texture_2d(14, rctx->empty_texture_2d->glid);

        if (pass_sw[RND_PASSID_REFLECT] && reflect) {
            RenderTexture& refl_tex = get_render_texture(0);
            gl_state_active_bind_texture_2d(15, refl_tex.GetColorTex());
            uniform->arr[U_WATER_REFLECT] = 1;
        }
        else {
            gl_state_active_bind_texture_2d(15, rctx->empty_texture_2d->glid);
            uniform->arr[U_WATER_REFLECT] = 0;
        }

        sss_data_get()->set_texture(1);

        gl_state_bind_sampler(14, rctx->render_samplers[0]);
        gl_state_bind_sampler(15, rctx->render_samplers[0]);
        gl_state_bind_sampler(16, rctx->render_samplers[0]);

        uniform->arr[U_STAGE_AMBIENT] = light_stage_ambient ? 1 : 0;

        if (alpha_z_sort) {
            disp_manager->obj_sort(&rctx->view_mat, mdl::OBJ_TYPE_TRANSLUCENT, 1, field_31F);
            disp_manager->obj_sort(&rctx->view_mat, mdl::OBJ_TYPE_TRANSLUCENT_SORT_BY_RADIUS, 2);
            disp_manager->obj_sort(&rctx->view_mat, mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_1, 1, field_31F);
            disp_manager->obj_sort(&rctx->view_mat, mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_2, 1, field_31F);
            disp_manager->obj_sort(&rctx->view_mat, mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_3, 1, field_31F);
            disp_manager->obj_sort(&rctx->view_mat, mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_2_LOCAL, 1, field_31F);
        }

        if (opaque_z_sort)
            disp_manager->obj_sort(&rctx->view_mat, mdl::OBJ_TYPE_OPAQUE, 0);

        if (draw_pass_3d[DRAW_PASS_3D_OPAQUE]) {
            gl_state_enable_depth_test();
            gl_state_set_depth_mask(GL_TRUE);
            disp_manager->draw(mdl::OBJ_TYPE_OPAQUE);
            gl_state_disable_depth_test();
        }

        Glitter::glt_particle_manager->DispScenes(Glitter::DISP_OPAQUE);
        Glitter::glt_particle_manager_x->DispScenes(Glitter::DISP_OPAQUE);

        gl_state_enable_depth_test();
        gl_state_set_depth_mask(GL_TRUE);
        if (draw_pass_3d[DRAW_PASS_3D_TRANSPARENT])
            disp_manager->draw(mdl::OBJ_TYPE_TRANSPARENT);
        if (render_manager->field_120)
            disp_manager->draw(mdl::OBJ_TYPE_TYPE_7);

        render->calc_exposure_chara_data();

        if (npr_param == 1)
            pass_3d_contour();

        render->draw_lens_flare();
        star_catalog_draw();

        draw_pass_3d_translucent(
            draw_pass_3d[DRAW_PASS_3D_OPAQUE],
            draw_pass_3d[DRAW_PASS_3D_TRANSPARENT],
            draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT],
            mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_3,
            mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_3,
            mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_3);

        snow_particle_draw();
        rain_particle_draw();
        leaf_particle_draw();
        particle_draw();
        gl_state_disable_depth_test();

        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
        Glitter::glt_particle_manager->DispScenes(Glitter::DISP_ALPHA);
        Glitter::glt_particle_manager_x->DispScenes(Glitter::DISP_ALPHA);
        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        /*if (npr_param == 1) {
            gl_state_set_color_mask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
            glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
            glClearDLL(GL_COLOR_BUFFER_BIT);
            gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        }*/

        gl_state_enable_depth_test();

        if (draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT]) {
            gl_state_enable_blend();
            gl_state_set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            gl_state_set_depth_mask(GL_FALSE);
            disp_manager->draw(mdl::OBJ_TYPE_TRANSLUCENT_SORT_BY_RADIUS);
            disp_manager->draw(mdl::OBJ_TYPE_TRANSLUCENT);
            gl_state_disable_blend();
        }

        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE); // X
        Glitter::glt_particle_manager_x->DispScenes(Glitter::DISP_TYPE_2);
        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        gl_state_set_depth_mask(GL_TRUE);
        draw_pass_3d_translucent(
            draw_pass_3d[DRAW_PASS_3D_OPAQUE],
            draw_pass_3d[DRAW_PASS_3D_TRANSPARENT],
            draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT],
            mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_2,
            mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_2,
            mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_2);
        gl_state_disable_depth_test();

        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
        Glitter::glt_particle_manager->DispScenes(Glitter::DISP_NORMAL);
        Glitter::glt_particle_manager_x->DispScenes(Glitter::DISP_NORMAL);
        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        gl_state_enable_depth_test();
        gl_state_set_depth_mask(GL_TRUE);
        draw_pass_3d_translucent(
            draw_pass_3d[DRAW_PASS_3D_OPAQUE],
            draw_pass_3d[DRAW_PASS_3D_TRANSPARENT],
            draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT],
            mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_1,
            mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_1,
            mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_1);

        if (Glitter::glt_particle_manager_x->CheckHasLocalEffect()) { // X
            float_t fov = camera_data->fov;
            camera_data->fov = 32.2673416137695f;
            draw_pass_set_camera();

            disp_manager->draw(mdl::OBJ_TYPE_OPAQUE_LOCAL);
            disp_manager->draw(mdl::OBJ_TYPE_TRANSPARENT_LOCAL);
            gl_state_enable_blend();
            disp_manager->draw(mdl::OBJ_TYPE_TRANSLUCENT_LOCAL);
            gl_state_disable_blend();

            draw_pass_3d_translucent(
                draw_pass_3d[DRAW_PASS_3D_OPAQUE],
                draw_pass_3d[DRAW_PASS_3D_TRANSPARENT],
                draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT],
                mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_2_LOCAL,
                mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_2_LOCAL,
                mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_2_LOCAL);

            gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
            Glitter::glt_particle_manager_x->DispScenes(Glitter::DISP_LOCAL);
            gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            camera_data->fov = fov;
            draw_pass_set_camera();
        }

        gl_state_active_bind_texture_2d(14, 0);
        gl_state_active_bind_texture_2d(15, 0);

        gl_state_disable_depth_test();

        if (shadow)
            draw_pass_3d_shadow_reset();
        pass_sprite_surf();
        shader::unbind();
        gl_state_bind_framebuffer(0);
    }

    void RenderManager::pass_show_vector() {
        return;
        /*if (!show_vector_flags)
            return;

        gl_state_begin_event("pass_show_vector");

        render->bind_render_texture();
        draw_pass_set_camera();

        rctx->obj_scene_ubo.WriteMemory(rctx->obj_scene);

        //shaders_ft.env_vert_set(20, show_vector_length, show_vector_z_offset, 0.0f, 0.0f);
        for (int32_t i = 1; i < 4; i++) {
            if (!(show_vector_flags & (1 << (i - 1))))
                continue;

            if (draw_pass_3d[DRAW_PASS_3D_OPAQUE])
                draw_task_draw_objects_by_type_show_vector(mdl::OBJ_TYPE_OPAQUE, i);
            if (draw_pass_3d[DRAW_PASS_3D_TRANSPARENT])
                draw_task_draw_objects_by_type_show_vector(mdl::OBJ_TYPE_TRANSPARENT, i);
            if (draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT]) {
                draw_task_draw_objects_by_type_show_vector(mdl::OBJ_TYPE_TRANSLUCENT_SORT_BY_RADIUS, i);
                draw_task_draw_objects_by_type_show_vector(mdl::OBJ_TYPE_TRANSLUCENT, i);
            }
        }
        shader::unbind();
        gl_state_bind_framebuffer(0);
        gl_state_end_event();*/
    }

    void RenderManager::pass_post_process() {
        gl_state_begin_event("pass_post_process");

        static texture* (FASTCALL * litproj_texture_get)() = (texture * (FASTCALL*)())0x0000000140350600;
        render->apply_post_process(litproj_texture_get(), npr_param);
        gl_state_end_event();
    }

    void RenderManager::pass_sprite() {
        gl_state_begin_event("pass_sprite");
        if (sprite_manager_get_reqlist_count(0)) {
            rndr::Render* rend = render;
            gl_state_set_viewport(0, 0, rend->screen_width, rend->screen_height);

            if (multisample && multisample_framebuffer) {
                gl_state_bind_framebuffer(multisample_framebuffer);
                gl_state_enable_multisample();
                glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
                glClearDLL(GL_COLOR_BUFFER_BIT);
            }
            else
                rctx->screen_buffer.Bind();

            sprite_manager_set_view_projection(false);
            gl_state_disable_depth_test();
            gl_state_enable_blend();
            gl_state_disable_cull_face();
            sprite_manager_set_res((double_t)rctx->screen_width / (double_t)rctx->screen_height,
                rctx->screen_width, rctx->screen_height);
            sprite_manager_draw(0, true,
                rend->temp_buffer.color_texture);
            gl_state_enable_cull_face();
            gl_state_disable_blend();
            gl_state_enable_depth_test();

            if (multisample && multisample_framebuffer) {
                gl_state_bind_framebuffer(0);
                gl_state_disable_multisample();
                fbo_blit(multisample_framebuffer, rctx->screen_buffer.fbos[0],
                    0, 0, rctx->screen_width, rctx->screen_height,
                    0, 0, rctx->screen_width, rctx->screen_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            }
            shader::unbind();

            gl_state_get_error();
        }

        fbo_blit(rctx->screen_buffer.fbos[0], 0,
            0, 0, rctx->screen_width, rctx->screen_height,
            0, 0, rctx->screen_width, rctx->screen_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        gl_state_bind_framebuffer(0);
        gl_state_end_event();
    }

    void RenderManager::pass_3d_contour() {
        RenderTexture* rt = &render->rend_texture[0];
        RenderTexture* contour_rt = render->sss_contour_texture;

        gl_state_begin_event("`anonymous-namespace'::draw_npr_frame");
        gl_state_enable_blend();
        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
        gl_state_set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gl_state_set_blend_equation(GL_FUNC_ADD);
        gl_state_disable_depth_test();

        quad_shader_data quad_data = {};
        quad_data.g_texcoord_modifier = { 0.5f, 0.5f, 0.5f, 0.5f };
        rctx->quad_ubo.WriteMemory(quad_data);

        contour_params_shader_data contour_params_data = {};
        contour_params_data.g_near_far = rctx->g_near_far;
        rctx->contour_params_ubo.WriteMemory(contour_params_data);

        shaders_ft.set(SHADER_FT_CONTOUR_NPR);
        gl_state_active_bind_texture_2d(14, rt->GetDepthTex());
        gl_state_active_bind_texture_2d(16, contour_rt->GetColorTex());
        gl_state_active_bind_texture_2d(17, contour_rt->GetDepthTex());
        gl_state_bind_sampler(14, rctx->render_samplers[1]);
        gl_state_bind_sampler(16, rctx->render_samplers[1]);
        gl_state_bind_sampler(17, rctx->render_samplers[1]);
        rctx->quad_ubo.Bind(0);
        rctx->contour_params_ubo.Bind(2);
        gl_state_bind_vertex_array(rctx->common_vao);
        shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);

        if (effect_texture)
            gl_state_active_bind_texture_2d(14, effect_texture->glid);
        else
            gl_state_active_bind_texture_2d(14, rctx->empty_texture_2d->glid);
        gl_state_bind_sampler(14, rctx->render_samplers[0]);
        gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        gl_state_enable_depth_test();
        gl_state_disable_blend();
        gl_state_end_event();
    }

    void RenderManager::pass_sprite_surf() {
        if (!sprite_manager_get_reqlist_count(1))
            return;

        rndr::Render* rend = render;

        sprite_manager_set_view_projection(false);
        gl_state_set_depth_mask(GL_FALSE);
        gl_state_disable_depth_test();
        gl_state_disable_depth_test();
        gl_state_enable_blend();
        gl_state_disable_cull_face();
        sprite_manager_draw(1, true,
            rend->temp_buffer.color_texture);
    }
}

void image_filter_scale(RenderTexture* dst, texture* src, const vec4& scale) {
    if (!dst || !dst->color_texture->glid || !dst->color_texture || !src || !src->glid)
        return;

    gl_state_begin_event("`anonymous-namespace'::Impl::apply_no_filter_sub");
    gl_state_set_viewport(0, 0, dst->color_texture->width, dst->color_texture->height);

    dst->Bind();

    filter_scene_shader_data filter_scene = {};
    filter_scene.g_transform = { 1.0f, 1.0f, 0.0f, 0.0f };
    filter_scene.g_texcoord = { 1.0f, 1.0f, 0.0f, 0.0f };
    rctx->filter_scene_ubo.WriteMemory(filter_scene);

    imgfilter_batch_shader_data imgfilter_batch = {};
    imgfilter_batch.g_color_scale = scale;
    imgfilter_batch.g_color_offset = 0.0f;
    imgfilter_batch.g_texture_lod = 0.0f;
    rctx->imgfilter_batch_ubo.WriteMemory(imgfilter_batch);

    uniform->arr[U_IMAGE_FILTER] = 5;
    shaders_ft.set(SHADER_FT_IMGFILT);
    rctx->filter_scene_ubo.Bind(0);
    rctx->imgfilter_batch_ubo.Bind(1);
    gl_state_active_bind_texture_2d(0, src->glid);
    gl_state_bind_sampler(0, rctx->render_samplers[0]);
    gl_state_bind_vertex_array(rctx->common_vao);
    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
    gl_state_end_event();
}

void draw_pass_set_camera() {
    camera_struct::update_data();
    mat4_transpose(&camera_data->view, &rctx->view_mat);
    mat4_transpose(&camera_data->projection, &rctx->proj_mat);
    mat4_transpose(&camera_data->view_projection, &rctx->vp_mat);
    vec3 view_point;
    camera_data->get_view_point(view_point);
    rctx->set_scene_projection_view(rctx->view_mat, rctx->proj_mat, view_point);

    float_t max_distance = camera_data->max_distance;
    float_t min_distance = camera_data->min_distance;
    rctx->g_near_far = {
        (float_t)(max_distance / (max_distance - min_distance)),
        (float_t)(-(max_distance * min_distance) / (max_distance - min_distance)),
        0.0f, 0.0f
    };
}

HOOK(void, FASTCALL, render_manager_init_render_textures, 0x0000000140502560, int32_t multisample) {
    render_manager_init_render_textures(multisample);
}

HOOK(void, FASTCALL, render_manager_free_data, 0x0000000140502770) {
    render_manager_free_render_textures();
    render_manager->render->free();

    shadow_ptr_free();

    sss_data_get()->free();
}

HOOK(void, FASTCALL, render_manager_free_render_textures, 0x00000001405027A0) {
    render_manager_free_render_textures();
}

HOOK(void, FASTCALL, render_manager_init_data, 0x0000000140502A2A, int32_t ssaa, int32_t hd_res, int32_t ss_alpha_mask, bool npr) {
    rndr::RenderManager& render_manager = *::render_manager;

    render_manager.reset();
    render_manager.npr = npr;
    render_texture_counter_reset();

    resolution_struct* res_wind = res_window_get();
    resolution_struct* res_wind_int = res_window_internal_get();
    render_manager.render = render_get();
    render_manager.render->init_render_buffers(res_wind_int->width, res_wind_int->height, ssaa, hd_res, ss_alpha_mask);
    render_manager.render->set_screen_res(res_wind_int->x_offset,
        res_wind_int->y_offset, res_wind_int->width, res_wind_int->height);
    render_manager.width = res_wind->width;
    render_manager.height = res_wind->height;
    render_manager_init_render_textures(1);
    render_manager.render->init_tone_map_buffers();

    shadow_ptr_init();
    render_manager.shadow_ptr = shadow_ptr_get();
    render_manager.shadow_ptr->init();

    sss_data_get()->init();

    void(FASTCALL * sub_1403B6CC0)() = (void(FASTCALL*)())0x00000001403B6CC0;
    sub_1403B6CC0();

    glGetErrorDLL();
}

void render_manager_patch() {
    WRITE_NOP_6(0x0000000140502A24);
    WRITE_MEMORY(0x0000000140502A3C, uint8_t, 0xC3);

    INSTALL_HOOK(render_manager_free_data);
    INSTALL_HOOK(render_manager_init_render_textures);
    INSTALL_HOOK(render_manager_free_render_textures);
    INSTALL_HOOK(render_manager_init_data);
}

static void draw_pass_shadow_begin_make_shadowmap(Shadow* shad, int32_t index, int32_t a3) {
    shad->curr_render_textures[0]->Bind();
    shad->curr_render_textures[0]->SetViewport();
    texture* tex = shad->curr_render_textures[0]->color_texture;
    gl_state_set_scissor(0, 0, tex->width, tex->height);
    gl_state_enable_depth_test();
    gl_state_enable_scissor_test();
    glClearColorDLL(1.0, 1.0, 1.0, 1.0);
    if (!a3)
        glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else if (shad->separate)
        glClearDLL(GL_COLOR_BUFFER_BIT);
    gl_state_set_scissor(1, 1, tex->width - 2, tex->height - 2);

    float_t shadow_range = shad->get_shadow_range();
    float_t offset;
    vec3* interest;
    vec3* view_point;
    if (shad->separate) {
        offset = index ? 0.5f : -0.5f;
        interest = &shad->interest[index];
        view_point = &shad->view_point[index];
    }
    else {
        offset = 0.0f;
        interest = &shad->interest_shared;
        view_point = &shad->view_point_shared;
    }

    mat4 temp;
    mat4_translate(offset, 0.0f, 0.0f, &temp);

    mat4 proj;
    mat4_ortho(-shadow_range, shadow_range,
        -shadow_range, shadow_range, shad->z_near, shad->z_far, &proj);
    mat4_mul(&proj, &temp, &rctx->proj_mat);
    mat4_look_at(view_point, interest, &rctx->view_mat);
    mat4_mul(&rctx->view_mat, &rctx->proj_mat, &rctx->vp_mat);
    rctx->set_scene_projection_view(rctx->view_mat, rctx->proj_mat, *view_point);

    draw_state.shader_index = SHADER_FT_SIL;
    uniform->arr[U_DEPTH] = 0;
}

static void draw_pass_shadow_end_make_shadowmap(Shadow* shad, int32_t index, int32_t a3) {
    gl_state_disable_depth_test();
    gl_state_disable_scissor_test();

    draw_state.shader_index = -1;
    if (a3 == shad->num_shadow - 1) {
        draw_pass_shadow_filter(&shad->render_textures[3],
            &shad->render_textures[4], shad->curr_render_textures[0],
            shad->field_2DC, shad->field_2E0 / (shad->field_208 * 2.0f), false);
        draw_pass_shadow_esm_filter(&shad->render_textures[5],
            &shad->render_textures[6], &shad->render_textures[3]);
    }

    RenderTexture* rend_tex = shad->curr_render_textures[1 + index];
    RenderTexture& rend_buf_tex = rctx->shadow_buffer;

    image_filter_scale(rend_tex,
        shad->curr_render_textures[0]->color_texture);

    rend_buf_tex.Bind();
    if (shad->blur_filter_enable[index]) {
        for (int32_t i = shad->near_blur, j = 0; i > 0; i--, j++) {
            blur_filter_apply(&rend_buf_tex, rend_tex,
                shad->blur_filter, 1.0f, 1.0f, 0.0f);
            image_filter_scale(rend_tex, rend_buf_tex.color_texture);
        }
    }
    else {
        glClearColorDLL(1.0f, 1.0f, 1.0f, 1.0f);
        glClearDLL(GL_COLOR_BUFFER_BIT);
    }

    gl_state_bind_framebuffer(0);
    gl_state_bind_texture_2d(rend_tex->GetColorTex());
    glGenerateMipmap(GL_TEXTURE_2D);
    gl_state_bind_texture_2d(0);
}

static void draw_pass_shadow_filter(RenderTexture* a1, RenderTexture* a2,
    RenderTexture* a3, float_t sigma, float_t far_texel_offset, bool enable_lit_proj) {
    texture* v7 = a1->color_texture;
    texture* v9 = a2->color_texture;
    texture* v11 = v7;
    if (a3)
        v11 = a3->depth_texture;

    if (!v7 || !v9 || !v11 || v7->width != v9->width || v7->height != v9->height)
        return;

    gl_state_begin_event("`anonymous-namespace'::Impl::apply_esm_filter");
    filter_scene_shader_data filter_scene = {};
    filter_scene.g_transform = 0.0f;
    filter_scene.g_texcoord = { 1.0f, 1.0f, 0.0f, 0.0f };
    rctx->filter_scene_ubo.WriteMemory(filter_scene);

    esm_filter_batch_shader_data esm_filter_batch = {};
    esm_filter_batch.g_params = { 1.0f / (float_t)v7->width, 0.0f, far_texel_offset, far_texel_offset };
    double_t v6 = 1.0 / (sqrt(M_PI * 2.0) * sigma);
    double_t v8 = -1.0 / (2.0 * sigma * sigma);
    for (int32_t i = 0; i < 8; i++)
        ((float_t*)esm_filter_batch.g_gauss)[i] = (float_t)(exp((double_t)((ssize_t)i * i) * v8) * v6);

    uniform->arr[U_LIGHT_PROJ] = enable_lit_proj ? 1 : 0;
    shaders_ft.set(SHADER_FT_ESMGAUSS);
    rctx->filter_scene_ubo.Bind(0);
    rctx->esm_filter_batch_ubo.Bind(1);

    gl_state_set_viewport(0, 0, v7->width, v7->height);

    a2->Bind();
    esm_filter_batch.g_params = { 1.0f / (float_t)v7->width, 0.0f, far_texel_offset, far_texel_offset };
    rctx->esm_filter_batch_ubo.WriteMemory(esm_filter_batch);

    gl_state_active_bind_texture_2d(0, v11->glid);
    if (a3) {
        GLint swizzle[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
    }
    else {
        GLint swizzle[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
    }
    gl_state_bind_sampler(0, rctx->render_samplers[0]);
    gl_state_bind_vertex_array(rctx->common_vao);
    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);

    a1->Bind();

    esm_filter_batch.g_params = { 0.0f, 1.0f / (float_t)v7->height, far_texel_offset, far_texel_offset };
    rctx->esm_filter_batch_ubo.WriteMemory(esm_filter_batch);

    gl_state_active_bind_texture_2d(0, v9->glid);
    gl_state_bind_sampler(0, rctx->render_samplers[0]);
    gl_state_bind_vertex_array(rctx->common_vao);
    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
    shader::unbind();
    gl_state_end_event();
}

static void draw_pass_shadow_esm_filter(RenderTexture* dst, RenderTexture* buf, RenderTexture* src) {
    texture* dst_tex = dst->color_texture;
    texture* buf_tex = buf->color_texture;
    texture* src_tex = src->color_texture;
    gl_state_begin_event("`anonymous-namespace'::Impl::apply_esm_min_filter");
    if (!dst_tex || !dst_tex->glid || !buf_tex || !buf_tex->glid || !src_tex || !src_tex->glid) {
        gl_state_end_event();
        return;
    }

    gl_state_begin_event("minimize");
    filter_scene_shader_data filter_scene = {};
    filter_scene.g_transform = 0.0f;
    filter_scene.g_texcoord = { 1.0f, 1.0f, 0.0f, 0.0f };
    rctx->filter_scene_ubo.WriteMemory(filter_scene);

    esm_filter_batch_shader_data esm_filter_batch = {};
    esm_filter_batch.g_gauss[0] = 0.0f;
    esm_filter_batch.g_gauss[1] = 0.0f;

    rctx->filter_scene_ubo.Bind(0);
    rctx->esm_filter_batch_ubo.Bind(1);

    gl_state_set_viewport(0, 0, dst_tex->width, dst_tex->height);

    buf->Bind();
    esm_filter_batch.g_params = { 1.0f / (float_t)src_tex->width,
        1.0f / (float_t)src_tex->height, 0.0f, 0.0f };
    rctx->esm_filter_batch_ubo.WriteMemory(esm_filter_batch);

    uniform->arr[U_ESM_FILTER] = 0;
    shaders_ft.set(SHADER_FT_ESMFILT);
    gl_state_active_bind_texture_2d(0, src_tex->glid);
    gl_state_bind_sampler(0, rctx->render_samplers[0]);
    gl_state_bind_vertex_array(rctx->common_vao);
    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
    gl_state_end_event();

    gl_state_begin_event("erosion");
    dst->Bind();
    esm_filter_batch.g_params = { 0.75f / (float_t)buf_tex->width,
        0.75f / (float_t)buf_tex->height, 0.0f, 0.0f };
    rctx->esm_filter_batch_ubo.WriteMemory(esm_filter_batch);

    uniform->arr[U_ESM_FILTER] = 1;
    shaders_ft.set(SHADER_FT_ESMFILT);
    gl_state_active_bind_texture_2d(0, buf_tex->glid);
    gl_state_bind_sampler(0, rctx->render_samplers[0]);
    gl_state_bind_vertex_array(rctx->common_vao);
    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
    gl_state_end_event();
    gl_state_end_event();
}

static bool draw_pass_shadow_litproj() {
    if (!stage_param_data_litproj_current)
        return false;

    texture* tex = texture_manager_get_texture(stage_param_data_litproj_current->tex_id);
    if (!tex)
        return false;

    litproj_texture->Bind();
    litproj_texture->SetViewport();
    gl_state_enable_depth_test();
    glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0f);
    glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!draw_pass_shadow_litproj_set_mat(true)) {
        gl_state_bind_framebuffer(0);
        return false;
    }

    draw_state.shader_index = SHADER_FT_LITPROJ;
    gl_state_active_bind_texture_2d(17, tex->glid);
    gl_state_bind_sampler(17, rctx->render_samplers[2]);
    gl_state_active_bind_texture_2d(18, litproj_shadow[0].GetColorTex());
    gl_state_bind_sampler(18, rctx->render_samplers[2]);
    gl_state_active_texture(0);
    return true;
}

static bool draw_pass_shadow_litproj_set() {
    if (!stage_param_data_litproj_current)
        return 0;

    static const vec4 color_clear = 0.0f;
    static const GLfloat depth_clear = 1.0f;

    litproj_shadow[0].Bind();
    gl_state_set_viewport(0, 0, 2048, 512);
    gl_state_enable_depth_test();
    gl_state_set_depth_mask(GL_TRUE);
    glClearBufferfv(GL_COLOR, 0, (float_t*)&color_clear);
    glClearBufferfv(GL_DEPTH, 0, &depth_clear);

    if (draw_pass_shadow_litproj_set_mat(false)) {
        draw_state.shader_index = SHADER_FT_SIL;
        uniform->arr[U_DEPTH] = 1;
        return true;
    }
    else {
        litproj_texture->Bind();
        litproj_texture->SetViewport();
        gl_state_enable_depth_test();
        gl_state_set_depth_mask(GL_TRUE);
        glClearBufferfv(GL_COLOR, 0, (float_t*)&color_clear);
        glClearBufferfv(GL_DEPTH, 0, &depth_clear);
    }
    return false;
}

static bool draw_pass_shadow_litproj_set_mat(bool set_mat) {
    light_set* set = &light_set_data[LIGHT_SET_MAIN];
    light_data* data = &set->lights[LIGHT_PROJECTION];
    if (data->get_type() != LIGHT_SPOT)
        return false;

    vec3 position;
    vec3 spot_direction;
    data->get_position(position);
    data->get_spot_direction(spot_direction);

    if (vec3::length_squared(spot_direction) <= 0.000001f)
        return false;

    float_t spot_cutoff = data->get_spot_cutoff();
    float_t fov = atanf(tanf(spot_cutoff * DEG_TO_RAD_FLOAT) * 0.25f) * 2.0f;

    vec3 interest = position + spot_direction;
    if (set_mat) {
        mat4 temp;
        mat4_translate(0.5f, 0.5f, 0.5f, &temp);
        mat4_scale_rot(&temp, 0.5f, 0.5f, 0.5f, &temp);

        mat4 proj;
        mat4_persp(fov, 4.0f, 0.1f, 10.0f, &proj);
        mat4_mul(&proj, &temp, &proj);

        mat4 view;
        vec3 up = { 0.0f, 1.0f, 0.0f };
        mat4_look_at(&position, &interest, &up, &view);

        mat4 mat;
        mat4_mul(&view, &proj, &mat);
        rctx->set_scene_light_projection(mat);
    }
    else {
        mat4_persp(fov, 4.0f, 0.1f, 10.0f, &rctx->proj_mat);

        vec3 up = { 0.0f, 1.0f, 0.0f };
        mat4_look_at(&position, &interest, &up, &rctx->view_mat);
        rctx->set_scene_projection_view(rctx->view_mat, rctx->proj_mat, position);
    }
    return true;
}

static void draw_pass_sss_contour(rndr::Render* rend) {
    rend->sss_contour_texture->Bind();
    rend->sss_contour_texture->SetViewport();
    gl_state_enable_depth_test();
    gl_state_set_depth_func(GL_ALWAYS);
    gl_state_set_depth_mask(GL_TRUE);
    gl_state_set_viewport(0, 0, rend->render_width[0], rend->render_height[0]);
    gl_state_active_bind_texture_2d(0, rend->rend_texture[0].GetColorTex());
    gl_state_bind_sampler(0, rctx->render_samplers[1]);
    gl_state_active_bind_texture_2d(1, rend->rend_texture[0].GetDepthTex());
    gl_state_bind_sampler(1, rctx->render_samplers[1]);
    gl_state_active_texture(0);

    float_t v3 = 1.0f / tanf(camera_data->fov * 0.5f * DEG_TO_RAD_FLOAT);

    vec3 direction = camera_data->interest - camera_data->view_point;
    float_t length = vec3::length(direction);
    float_t v7 = direction.y;
    if (length != 0.0f)
        v7 /= length;

    float_t v9 = fabsf(v7) - 0.1f;
    if (v9 < 0.0f)
        v9 = 0.0f;

    contour_coef_shader_data shader_data = {};
    shader_data.g_contour = { v9 * 0.004f + 0.0027f, 0.003f, v3 * 0.35f, 0.0008f };
    shader_data.g_near_far = rctx->g_near_far;
    rctx->contour_coef_ubo.WriteMemory(shader_data);

    shaders_ft.set(SHADER_FT_CONTOUR);
    rctx->contour_coef_ubo.Bind(2);
    render_get()->draw_quad(
        rend->render_post_width[0], rend->render_post_height[0],
        rend->render_post_width_scale, rend->render_post_height_scale,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

static void draw_pass_sss_filter_calc_coef(double_t step, int32_t size, double_t a3, int32_t iterations,
    const double_t* a5, const double_t* a6, const double_t* a7, const double_t* a8, vec4 a9[64]) {
    if (size > 8)
        return;

    for (int32_t i = 0; i < iterations; i++) {
        const double_t v18 = a5[i];
        double_t v56[3];
        v56[0] = a6[i];
        v56[1] = a7[i];
        v56[2] = a8[i];

        for (int32_t j = 0; j < 3; j++) {
            double_t v22 = 0.0;
            double_t v23 = 0.0;
            double_t v54[8];
            double_t v24 = 1.0 / (a3 * v56[j]);
            for (int32_t k = 0; k < size; k++) {
                double_t v25 = exp(-0.5 * (v24 * v23) * (v24 * v23));
                v54[k] = v25;
                v23 += step;
                v22 += v25;
            }

            double_t v27 = 1.0 / v22;
            for (int32_t k = 0; k < size; k++)
                v54[k] *= v27;

            float_t* v35 = (float_t*)a9 + j;
            for (int32_t k = 0; k < size; k++)
                for (size_t l = 0; l < size; l++) {
                    *v35 = (float_t)(v54[k] * v18 * v54[l] + *v35);
                    v35 += 4;
                }
        }
    }
}

static void draw_pass_sss_filter(sss_data* sss) {
    const int32_t sss_count = 6;
    vec3 interest = camera_data->interest;
    vec3 view_point = camera_data->view_point;

    vec3 chara_position[2];
    chara_position[0] = 0.0f;
    chara_position[1] = 0.0f;
    float_t chara_distance[2];
    size_t rob_chara_smth = get_rob_chara_smth();
    for (int32_t i = 0; i < 2; i++) {
        chara_position[i] = interest;
        chara_distance[i] = 999999.0f;
        size_t rob_bone_data = rob_chara_array_get_bone_data(rob_chara_smth, i);
        if (rob_chara_pv_data_array_check_chara_id(rob_chara_smth, i)
            && rob_chara_array_check_visibility(rob_chara_smth, i)) {
            const mat4* mat = rob_chara_bone_data_get_mats_mat(rob_bone_data, 0);
            if (mat) {
                mat4 temp;
                mat4_transpose(mat, &temp);
                mat4_get_translation(&temp, &chara_position[i]);
                chara_distance[i] = vec3::distance(view_point, chara_position[i]);
            }
        }
    }

    vec3 closest_chara_position = chara_distance[0] <= chara_distance[1]
        ? chara_position[0] : chara_position[1];

    float_t length = vec3::distance(interest, closest_chara_position);
    if (length > 1.25f)
        interest = chara_position[0];

    float_t v29 = max_def(vec3::distance(view_point, interest), 0.25f);
    float_t v31 = max_def(tanf(camera_data->fov * 0.5f * DEG_TO_RAD_FLOAT) * 5.0f, 0.25f);
    float_t v33 = 0.6f;
    float_t v34 = (float_t)(1.0 / clamp_def(v31 * v29, 0.25f, 100.0f));
    if (v34 < 0.145f)
        v33 = max_def(v34 - 0.02f, 0.0f) * 8.0f * 0.6f;
    rctx->set_batch_sss_param({ v33, 0.0f, 0.0f, 0.0f });

    gl_state_active_texture(0);
    if (sss->npr_contour) {
        sss->textures[0].Bind();
        gl_state_set_viewport(0, 0, 640, 360);
        rndr::Render* rend = render_get();
        uniform->arr[U_REDUCE] = 0;
        shaders_ft.set(SHADER_FT_REDUCE);
        gl_state_bind_texture_2d(rend->rend_texture[0].GetColorTex());
        gl_state_bind_sampler(0, rctx->render_samplers[1]);
        render_get()->draw_quad(640, 360, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
    }
    sss->textures[2].Bind();
    gl_state_set_viewport(0, 0, 320, 180);
    uniform->arr[U_SSS_FILTER] = 0;
    shaders_ft.set(SHADER_FT_SSS_FILT);
    gl_state_bind_texture_2d(sss->textures[0].GetColorTex());
    gl_state_bind_sampler(0, rctx->render_samplers[0]);
    render_get()->draw_quad(640, 360, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);

    sss_filter_gaussian_coef_shader_data shader_data = {};
    shader_data.g_param = { (float_t)(sss_count - 1), 0.0f, 1.0f, 1.0f };

    const double_t a5[] = { 0.4, 0.3, 0.3 };
    const double_t a6[] = { 1.0, 2.0, 5.0 };
    const double_t a7[] = { 0.2, 0.4, 1.2 };
    const double_t a8[] = { 0.3, 0.7, 2.0 };
    draw_pass_sss_filter_calc_coef(1.0, sss_count, v34, 3, a5, a6, a7, a8, shader_data.g_coef);

    rctx->sss_filter_gaussian_coef_ubo.WriteMemory(shader_data);
    sss->textures[1].Bind();
    gl_state_set_viewport(0, 0, 320, 180);
    uniform->arr[U_SSS_FILTER] = 3;
    shaders_ft.set(SHADER_FT_SSS_FILT);
    gl_state_bind_texture_2d(sss->textures[2].GetColorTex());
    gl_state_bind_sampler(0, rctx->render_samplers[0]);
    rctx->sss_filter_gaussian_coef_ubo.Bind(1);
    render_get()->draw_quad(320, 180, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.96f, 1.0f, 0.0f);
    gl_state_bind_texture_2d(0);
}

static int32_t draw_pass_3d_get_translucent_count() {
    int32_t count = 0;
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_1);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_1);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_1);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_2);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_2);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_2);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_3);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_3);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_3);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_2_LOCAL);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_2_LOCAL);
    count += disp_manager->get_obj_count(mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_2_LOCAL);
    return count;
}

static void draw_pass_3d_shadow_reset() {
    gl_state_active_bind_texture_2d(6, 0);
    gl_state_active_bind_texture_2d(7, 0);
    draw_state.self_shadow = false;
    draw_state.shadow = false;
}

static void draw_pass_3d_shadow_set(Shadow* shad) {
    float_t esm_param;
    if (shad->self_shadow && shad->num_shadow > 0) {
        gl_state_active_bind_texture_2d(19, shad->render_textures[3].GetColorTex());
        gl_state_active_bind_texture_2d(20, shad->render_textures[5].GetColorTex());
        gl_state_active_texture(0);
        esm_param = (shad->field_2D8 * shad->field_208 * 2.0f) * 1.442695f;
        draw_state.self_shadow = true;
    }
    else {
        gl_state_active_bind_texture_2d(19, rctx->empty_texture_2d->glid);
        gl_state_active_bind_texture_2d(20, rctx->empty_texture_2d->glid);
        gl_state_active_texture(0);
        esm_param = 0.0f;
        draw_state.self_shadow = false;
    }

    gl_state_bind_sampler(19, rctx->render_samplers[0]);
    gl_state_bind_sampler(20, rctx->render_samplers[0]);

    mat4 mats[2];
    mats[0] = mat4_identity;
    mats[1] = mat4_identity;
    vec4 shadow_ambient;
    vec4 shadow_ambient1;
    if (shad->num_shadow > 0) {
        draw_state.shadow = true;
        uniform->arr[U_STAGE_SHADOW2] = shad->num_shadow > 1 ? 1 : 0;

        float_t shadow_range = shad->get_shadow_range();
        for (int32_t i = 0; i < 2; i++) {
            float_t offset;
            vec3* interest;
            vec3* view_point;
            if (shad->separate) {
                offset = i ? 0.5f : -0.5f;

                interest = &shad->interest[i];
                view_point = &shad->view_point[i];
            }
            else {
                offset = 0.0f;
                interest = &shad->interest_shared;
                view_point = &shad->view_point_shared;
            }

            mat4 temp;
            mat4_translate(0.5f, 0.5f, 0.5f, &temp);
            mat4_scale_rot(&temp, 0.5f, 0.5f, 0.5f, &temp);
            mat4_mul_translate(&temp, offset, 0.0f, 0.0f, &temp);

            mat4 proj;
            mat4_ortho(-shadow_range, shadow_range,
                -shadow_range, shadow_range, shad->z_near, shad->z_far, &proj);
            mat4_mul(&proj, &temp, &proj);

            mat4 view;
            mat4_look_at(view_point, interest, &view);
            mat4_mul(&view, &proj, &mats[i]);
        }

        int32_t j = 0;
        for (int32_t i = 0; i < 2; i++)
            if (shad->shadow_enable[i]) {
                gl_state_active_bind_texture_2d(6 + j, shad->curr_render_textures[1 + i]->GetColorTex());
                glTexParameterfDLL(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
                j++;
            }

        for (; j < 2; j++)
            gl_state_active_bind_texture_2d(6 + j, rctx->empty_texture_2d->glid);
        gl_state_active_texture(0);

        light_set* set = &light_set_data[LIGHT_SET_MAIN];
        light_data* data = &set->lights[LIGHT_SHADOW];

        vec4 ambient;
        if (data->get_type() == LIGHT_PARALLEL)
            data->get_ambient(ambient);
        else
            *(vec3*)&ambient.x = shad->shadow_ambient;

        shadow_ambient = { ambient.x, ambient.y, ambient.z, 1.0f };
        shadow_ambient1 = { 1.0f - ambient.x, 1.0f - ambient.y, 1.0f - ambient.z, 0.0f };
    }
    else {
        draw_state.shadow = false;

        for (int32_t i = 0; i < 2; i++)
            gl_state_active_bind_texture_2d(6 + i, shad->curr_render_textures[1 + i]->GetColorTex());
        gl_state_active_texture(0);

        shadow_ambient = 1.0f;
        shadow_ambient1 = 0.0f;
    }

    rctx->set_scene_shadow_params(esm_param, mats, shadow_ambient, shadow_ambient1);

    gl_state_bind_sampler(6, 0);
    gl_state_bind_sampler(7, 0);
}

static void draw_pass_3d_translucent(bool opaque_enable,
    bool transparent_enable, bool translucent_enable, mdl::ObjType opaque,
    mdl::ObjType transparent, mdl::ObjType translucent) {
    if (disp_manager->get_obj_count(opaque) < 1
        && disp_manager->get_obj_count(transparent) < 1
        && disp_manager->get_obj_count(translucent) < 1)
        return;

    rndr::Render* rend = render_get();

    int32_t alpha_array[256];
    int32_t count = draw_pass_3d_translucent_count_layers(
        alpha_array, opaque, transparent, translucent);
    for (int32_t i = 0; i < count; i++) {
        int32_t alpha = alpha_array[i];
        rend->transparency_copy();
        if (opaque_enable && disp_manager->get_obj_count(opaque))
            disp_manager->draw(opaque, 0, true, alpha);
        if (transparent_enable && disp_manager->get_obj_count(transparent))
            disp_manager->draw(transparent, 0, true, alpha);
        if (translucent_enable && disp_manager->get_obj_count(translucent)) {
            gl_state_enable_blend();
            disp_manager->draw(translucent, 0, true, alpha);
            gl_state_disable_blend();
        }
        rend->transparency_combine((float_t)alpha * (float_t)(1.0 / 255.0));
    }
}

static int32_t draw_pass_3d_translucent_count_layers(int32_t* alpha_array,
    mdl::ObjType opaque, mdl::ObjType transparent, mdl::ObjType translucent) {
    bool arr[256] = { 0 };

    draw_pass_3d_translucent_has_objects(arr, opaque);
    draw_pass_3d_translucent_has_objects(arr, transparent);
    draw_pass_3d_translucent_has_objects(arr, translucent);

    int32_t count = 0;
    for (int32_t i = 0xFF; i >= 1; i--)
        if (arr[i]) {
            count++;
            *alpha_array++ = i;
        }
    return count;
}

static void draw_pass_3d_translucent_has_objects(bool* arr, mdl::ObjType type) {
    disp_manager->calc_obj_radius(&rctx->view_mat, type);
    for (mdl::ObjData*& i : disp_manager->get_obj_list(type))
        switch (i->kind) {
        case mdl::OBJ_KIND_NORMAL: {
            int32_t alpha = (int32_t)(i->args.sub_mesh.blend_color.w * 255.0f);
            alpha = clamp_def(alpha, 0, 255);
            arr[alpha] = true;
        } break;
        case mdl::OBJ_KIND_TRANSLUCENT: {
            for (int32_t j = 0; j < i->args.translucent.count; j++) {
                int32_t alpha = (int32_t)(i->args.translucent.sub_mesh[j]->blend_color.w * 255.0f);
                alpha = clamp_def(alpha, 0, 255);
                arr[alpha] = true;
            }
        } break;
        }
}

static void blur_filter_apply(RenderTexture* dst, RenderTexture* src,
    blur_filter_mode filter, const vec2 res_scale, const vec4 scale, const vec4 offset) {
    if (!dst || !src)
        return;

    gl_state_begin_event("`anonymous-namespace'::Impl::apply_blur_filter_sub");
    filter_scene_shader_data filter_scene = {};
    float_t w = res_scale.x / (float_t)src->GetWidth();
    float_t h = res_scale.y / (float_t)src->GetHeight();
    filter_scene.g_transform = { w, h, 0.0f, 0.0f };
    filter_scene.g_texcoord = { 1.0f, 1.0f, 0.0f, 0.0f };
    rctx->filter_scene_ubo.WriteMemory(filter_scene);

    imgfilter_batch_shader_data imgfilter_batch = {};
    if (filter == BLUR_FILTER_32)
        imgfilter_batch.g_color_scale = scale * (float_t)(1.0 / 8.0);
    else
        imgfilter_batch.g_color_scale = scale * (float_t)(1.0 / 4.0);
    imgfilter_batch.g_color_offset = offset;
    imgfilter_batch.g_texture_lod = 0.0f;
    rctx->imgfilter_batch_ubo.WriteMemory(imgfilter_batch);

    uniform->arr[U_IMAGE_FILTER] = filter == BLUR_FILTER_32 ? 1 : 0;
    gl_state_bind_vertex_array(rctx->box_vao);
    shaders_ft.set(SHADER_FT_IMGFILT);
    rctx->filter_scene_ubo.Bind(0);
    rctx->imgfilter_batch_ubo.Bind(1);
    gl_state_active_bind_texture_2d(0, src->GetColorTex());
    gl_state_bind_sampler(0, rctx->render_samplers[0]);
    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, (GLint)(filter * 4LL), 4);
    gl_state_end_event();
}

static void render_manager_free_render_textures() {
    rndr::RenderManager& render_manager = *::render_manager;

    if (render_manager.multisample_renderbuffer) {
        glDeleteRenderbuffers(1, &render_manager.multisample_renderbuffer);
        render_manager.multisample_renderbuffer = 0;
    }

    if (render_manager.multisample_framebuffer) {
        glDeleteFramebuffers(1, &render_manager.multisample_framebuffer);
        render_manager.multisample_framebuffer = 0;
    }

    for (RenderTexture& i : render_manager.render_textures)
        i.Free();
}

static void render_manager_init_render_textures(int32_t multisample) {
    rndr::RenderManager& render_manager = *::render_manager;

    if (multisample) {
        glGenFramebuffers(1, &render_manager.multisample_framebuffer);
        glGenRenderbuffers(1, &render_manager.multisample_renderbuffer);

        gl_state_bind_framebuffer(render_manager.multisample_framebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, render_manager.multisample_renderbuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8,
            GL_RGBA8, render_manager.width, render_manager.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_RENDERBUFFER, render_manager.multisample_renderbuffer);
        glDrawBufferDLL(GL_COLOR_ATTACHMENT0);
        glDrawBufferDLL(GL_COLOR_ATTACHMENT0);
        gl_state_bind_framebuffer(0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        if (glGetErrorDLL()) {
            glDeleteRenderbuffers(1, &render_manager.multisample_renderbuffer);
            render_manager.multisample_renderbuffer = 0;
            glDeleteFramebuffers(1, &render_manager.multisample_framebuffer);
            render_manager.multisample_framebuffer = 0;
        }
    }

    for (int32_t i = 0; i < 9; i++) {
        const rndr::RenderTextureData* tex_data = &rndr::render_manager_render_texture_data_array[i];
        if (tex_data->type != GL_TEXTURE_2D)
            continue;

        RenderTexture& rt = render_manager.render_textures[i];
        rt.Init(tex_data->width, tex_data->height, tex_data->max_level,
            tex_data->color_format, tex_data->depth_format);
        rt.Bind();
        glClearColorDLL(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    gl_state_bind_framebuffer(0);
}
