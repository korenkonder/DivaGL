/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "draw_object.hpp"
#include "../../KKdLib/hash.hpp"
#include "../light_param/light.hpp"
#include "../shader_ft.hpp"
#include "../texture.hpp"

static bool draw_object_blend_set(
    const mdl::ObjSubMeshArgs* args, obj_material_shader_lighting_type lighting_type);
static bool draw_object_blend_set_check_use_default_blend(int32_t index);
static void draw_object_chara_color_fog_set(
    const mdl::ObjSubMeshArgs* args, bool disable_fog);
static void draw_object_material_reset_default(const obj_material_data* mat_data);
static void draw_object_material_reset_reflect();
static void draw_object_material_set_default(
    const mdl::ObjSubMeshArgs* args, bool use_shader);
static void draw_object_material_set_parameter(
    const obj_material_data* mat_data);
static void draw_object_material_set_reflect(const mdl::ObjSubMeshArgs* args);
static void draw_object_material_set_uniform(const obj_material_data* mat_data, bool disable_color_l);
static void draw_object_model_mat_load(const mat4& mat);
static void draw_object_vertex_attrib_reset_default(const mdl::ObjSubMeshArgs* args);
static void draw_object_vertex_attrib_reset_reflect(const mdl::ObjSubMeshArgs* args);
static void draw_object_vertex_attrib_set_default(const mdl::ObjSubMeshArgs* args);
static void draw_object_vertex_attrib_set_reflect(const mdl::ObjSubMeshArgs* args);

static bool(FASTCALL* use_shader_get)() = (bool(FASTCALL*)())0x0000000140440E30;

namespace mdl {
    void draw(obj_primitive_type primitive_type, uint32_t count,
        uint16_t start, uint16_t end, obj_index_format index_format, size_t indices) {
        GLenum mesh_draw_mode[] = {
            GL_ZERO, //GL_POINTS,
            GL_LINES,
            GL_LINE_STRIP,
            GL_LINE_LOOP,
            GL_TRIANGLES,
            GL_TRIANGLE_STRIP,
            GL_TRIANGLE_FAN,
            GL_ZERO, //GL_QUADS,
            GL_ZERO, //GL_QUAD_STRIP,
            GL_ZERO, //GL_POLYGON,
        };

        GLenum mesh_indices_type[] = {
            GL_ZERO,
            GL_UNSIGNED_SHORT,
            GL_UNSIGNED_INT,
        };

        rctx->obj_batch_ubo.WriteMemory(rctx->obj_batch);
        if (primitive_type == OBJ_PRIMITIVE_TRIANGLE_STRIP && index_format == OBJ_INDEX_U16)
            shaders_ft.draw_range_elements(GL_TRIANGLE_STRIP,
                start, end, count, GL_UNSIGNED_SHORT, (void*)indices);
        else
            shaders_ft.draw_elements(mesh_draw_mode[primitive_type],
                count, mesh_indices_type[index_format], (void*)indices);

        draw_state->stats.draw_count += count;
        if (primitive_type == OBJ_PRIMITIVE_TRIANGLES)
            draw_state->stats.draw_triangle_count += count / 3;
        else if (primitive_type == OBJ_PRIMITIVE_TRIANGLE_STRIP)
            draw_state->stats.draw_triangle_count += count - 2;
    }

    void draw_etc_obj(const mdl::EtcObj* etc, const mat4* mat) {
        switch (etc->type) {
        case mdl::ETC_OBJ_TEAPOT:
        case mdl::ETC_OBJ_GRID:
        case mdl::ETC_OBJ_CUBE:
        case mdl::ETC_OBJ_SPHERE:
        case mdl::ETC_OBJ_PLANE:
        case mdl::ETC_OBJ_CONE:
        case mdl::ETC_OBJ_LINE:
        case mdl::ETC_OBJ_CROSS:
            break;
        default:
            return;
        }

        if (!etc->count)
            return;

        GLuint vao = disp_manager->get_vertex_array(etc);
        if (!vao)
            return;

        draw_object_model_mat_load(*mat);

        vec4 g_material_state_diffuse = rctx->obj_batch.g_material_state_diffuse;
        vec4 g_material_state_ambient = rctx->obj_batch.g_material_state_ambient;
        vec4 g_material_state_specular = rctx->obj_batch.g_material_state_specular;
        vec4 g_material_state_emission = rctx->obj_batch.g_material_state_emission;
        vec4 g_material_state_shininess = rctx->obj_batch.g_material_state_shininess;

        vec4 color = etc->color;

        vec4 ambient;
        *(vec3*)&ambient = *(vec3*)&color * 0.5f;
        ambient.w = color.w;

        rctx->obj_batch.g_material_state_diffuse = color;
        rctx->obj_batch.g_material_state_ambient = ambient;
        rctx->obj_batch.g_material_state_specular = { 0.0f, 0.0f, 0.0f, 1.0f };
        rctx->obj_batch.g_material_state_emission = { 0.0f, 0.0f, 0.0f, 1.0f };
        rctx->obj_batch.g_material_state_shininess = { 0.0f, 0.0f, 0.0f, 1.0f };

        rctx->obj_batch.g_blend_color = color;

        gl_state_bind_vertex_array(vao);
        rctx->obj_batch_ubo.WriteMemory(rctx->obj_batch);

        shaders_ft.set(SHADER_FT_SIMPLE);
        switch (etc->type) {
        case mdl::ETC_OBJ_TEAPOT:
            shaders_ft.draw_elements(GL_TRIANGLES, etc->count, GL_UNSIGNED_INT, 0);
            break;
        case mdl::ETC_OBJ_GRID:
            shaders_ft.draw_arrays(GL_LINES, 0, etc->count);
            break;
        case mdl::ETC_OBJ_CUBE:
            if (etc->data.sphere.wire)
                shaders_ft.draw_elements(GL_LINES, etc->count, GL_UNSIGNED_INT, (void*)etc->offset);
            else
                shaders_ft.draw_elements(GL_TRIANGLES, etc->count, GL_UNSIGNED_INT, (void*)etc->offset);
            break;
        case mdl::ETC_OBJ_SPHERE:
            if (etc->data.sphere.wire)
                shaders_ft.draw_elements(GL_LINES, etc->count, GL_UNSIGNED_INT, (void*)etc->offset);
            else
                shaders_ft.draw_elements(GL_TRIANGLES, etc->count, GL_UNSIGNED_INT, (void*)etc->offset);
            break;
        case mdl::ETC_OBJ_PLANE:
            shaders_ft.draw_arrays(GL_TRIANGLES, 0, etc->count);
            break;
        case mdl::ETC_OBJ_CONE:
            if (etc->data.cone.wire)
                shaders_ft.draw_elements(GL_LINES, etc->count, GL_UNSIGNED_INT, (void*)etc->offset);
            else
                shaders_ft.draw_elements(GL_TRIANGLES, etc->count, GL_UNSIGNED_INT, (void*)etc->offset);
            break;
        case mdl::ETC_OBJ_LINE:
            shaders_ft.draw_arrays(GL_LINES, 0, etc->count);
            break;
        case mdl::ETC_OBJ_CROSS:
            shaders_ft.draw_arrays(GL_LINES, 0, etc->count);
            break;
        }
        shader::unbind();
        uniform_value_reset();

        rctx->obj_batch.g_material_state_diffuse = g_material_state_diffuse;
        rctx->obj_batch.g_material_state_ambient = g_material_state_ambient;
        rctx->obj_batch.g_material_state_specular = g_material_state_specular;
        rctx->obj_batch.g_material_state_emission = g_material_state_emission;
        rctx->obj_batch.g_material_state_shininess = g_material_state_shininess;

        rctx->obj_batch.g_blend_color = 1.0f;
    }

    void draw_sub_mesh(const ObjSubMeshArgs* args, const mat4* mat,
        void(*func)(const ObjSubMeshArgs* args)) {
        GLuint vao = disp_manager->get_vertex_array(args);
        if (!vao)
            return;

        if (args->mats) {
            const mat4* mats = args->mats;
            rctx->obj_batch.set_g_joint(mats[0]);

            vec4* g_joint_transforms = rctx->obj_skinning.g_joint_transforms;

            mat4 mat;
            for (int32_t i = 0; i < args->mat_count; i++, mats++, g_joint_transforms += 3) {
                mat4_transpose(mats, &mat);
                g_joint_transforms[0] = mat.row0;
                g_joint_transforms[1] = mat.row1;
                g_joint_transforms[2] = mat.row2;
            }

            rctx->obj_skinning_ubo.WriteMemory(rctx->obj_skinning);

            rctx->obj_batch.set_transforms(mat4_identity, rctx->view_mat, rctx->proj_mat);

            uniform->arr[U_BONE_MAT] = 1;
            gl_state_bind_vertex_array(vao);
            func(args);
            uniform->arr[U_BONE_MAT] = 0;
        }
        else {
            mat4 _mat;
            if (args->mesh->attrib.m.billboard)
                model_mat_face_camera_view(&rctx->view_mat, mat, &_mat);
            else if (args->mesh->attrib.m.billboard_y_axis)
                model_mat_face_camera_position(&rctx->view_mat, mat, &_mat);
            else
                _mat = *mat;

            uniform->arr[U_BONE_MAT] = 0;
            gl_state_bind_vertex_array(vao);
            if (func != draw_sub_mesh_default || !args->instances_count) {
                draw_object_model_mat_load(_mat);
                func(args);
            }
            else
                draw_sub_mesh_default_instanced(args, &_mat);
            uniform->arr[U_BONE_MAT] = 0;
        }
    }

    /*void draw_sub_mesh_show_vector(
        const ObjSubMeshArgs* args, mat4* model, int32_t show_vector) {
        return;
    }*/

    void draw_sub_mesh_default(const ObjSubMeshArgs* args) {
        if (args->set_blend_color) {
            rctx->obj_batch.g_blend_color = args->blend_color;
            rctx->obj_batch.g_offset_color = 0.0f;
        }

        if (!args->func)
            draw_object_vertex_attrib_set_default(args);

        draw_object_material_set_default(args, use_shader_get());

        const obj_sub_mesh* sub_mesh = args->sub_mesh;
        draw(
            sub_mesh->primitive_type,
            sub_mesh->num_index,
            sub_mesh->first_index,
            sub_mesh->last_index,
            sub_mesh->index_format,
            sub_mesh->index_offset);

        draw_object_material_reset_default(args->material);
        if (!args->func)
            draw_object_vertex_attrib_reset_default(args);

        if (args->set_blend_color) {
            rctx->obj_batch.g_blend_color = 1.0f;
            rctx->obj_batch.g_offset_color = 0.0f;
        }

        draw_state->stats.object_draw_count++;
    }

    void draw_sub_mesh_default_instanced(
        const ObjSubMeshArgs* args, const mat4* mat) {
        if (args->set_blend_color) {
            rctx->obj_batch.g_blend_color = args->blend_color;
            rctx->obj_batch.g_offset_color = 0.0f;
        }

        if (!args->func)
            draw_object_vertex_attrib_set_default(args);

        draw_object_material_set_default(args, use_shader_get());

        const obj_sub_mesh* sub_mesh = args->sub_mesh;
        const mat4* instances_mat = args->instances_mat;
        mat4 _mat;
        for (int32_t i = args->instances_count; i >= 0; i--, instances_mat++) {
            mat4_mul(instances_mat, mat, &_mat);
            draw_object_model_mat_load(_mat);
            draw(
                sub_mesh->primitive_type,
                sub_mesh->num_index,
                sub_mesh->first_index,
                sub_mesh->last_index,
                sub_mesh->index_format,
                sub_mesh->index_offset);
        }

        draw_object_material_reset_default(args->material);
        if (!args->func)
            draw_object_vertex_attrib_reset_default(args);

        if (args->set_blend_color) {
            rctx->obj_batch.g_blend_color = 1.0f;
            rctx->obj_batch.g_offset_color = 0.0f;
        }

        draw_state->stats.object_draw_count++;
    }

    void draw_sub_mesh_sss(const ObjSubMeshArgs* args) {
        obj_material_attrib_member attrib = args->material->material.attrib.m;
        uniform->arr[U_ALPHA_TEST] = (!attrib.flag_28 && (args->blend_color.w < 1.0f
            || (attrib.alpha_texture || attrib.alpha_material) && !attrib.punch_through
            || args->sub_mesh->attrib.m.transparent)
            || attrib.punch_through) ? 1 : 0;

        uniform->arr[U26] = 1;
        bool aniso = false;
        const obj_material_data* material = args->material;
        switch (material->material.shader.index) {
        case SHADER_FT_CLOTH:
            if (!render_manager->npr_param && material->material.color.ambient.w < 1.0f
                && material->material.shader_info.m.aniso_direction == OBJ_MATERIAL_ANISO_DIRECTION_NORMAL)
                aniso = true;
            break;
        case SHADER_FT_TIGHTS:
            if (!render_manager->npr_param)
                aniso = true;
            break;
        //case SHADER_FT_EYEBALL:
        case SHADER_FT_GLASEYE:
            uniform->arr[U26] = 0;
            aniso = true;
            break;
        case SHADER_FT_SKIN:
            aniso = true;
            break;
        }

        if (aniso) {
            rctx->obj_batch.g_sss_param = { 0.0f, 0.0f, 0.0f, 0.5f };
            uniform->arr[U37] = 1;
        }
        else {
            const vec4& sss_param = sss_data_get()->param;
            rctx->obj_batch.g_sss_param = { sss_param.x, sss_param.y, sss_param.z, 0.5f };
            uniform->arr[U37] = 0;
        }
        draw_sub_mesh_default(args);
    }

    void draw_sub_mesh_reflect(const ObjSubMeshArgs* args) {
        const obj_material_data* material = args->material;
        draw_object_vertex_attrib_set_reflect(args);
        obj_material_shader_lighting_type lighting_type
            = material->material.shader_info.get_lighting_type();
        bool disable_fog = draw_object_blend_set(args, lighting_type);
        draw_object_chara_color_fog_set(args, disable_fog);
        draw_object_material_set_reflect(args);

        const obj_sub_mesh* sub_mesh = args->sub_mesh;
        if (sub_mesh->index_format != OBJ_INDEX_U8)
            draw(
                sub_mesh->primitive_type,
                sub_mesh->num_index,
                sub_mesh->first_index,
                sub_mesh->last_index,
                sub_mesh->index_format,
                sub_mesh->index_offset);

        draw_object_material_reset_reflect();
        draw_object_vertex_attrib_reset_reflect(args);
        uniform_value_reset();

        draw_state->stats.object_reflect_draw_count++;
    }

    void draw_sub_mesh_reflect_reflect_map(const ObjSubMeshArgs* args) {
        draw_object_vertex_attrib_set_reflect(args);
        draw_object_material_set_reflect(args);

        const obj_sub_mesh* sub_mesh = args->sub_mesh;
        if (sub_mesh->index_format != OBJ_INDEX_U8)
            draw(
                sub_mesh->primitive_type,
                sub_mesh->num_index,
                sub_mesh->first_index,
                sub_mesh->last_index,
                sub_mesh->index_format,
                sub_mesh->index_offset);

        draw_object_material_reset_reflect();
        draw_object_vertex_attrib_reset_reflect(args);

        draw_state->stats.object_reflect_draw_count++;
    }

    void draw_sub_mesh_shadow(const ObjSubMeshArgs* args) {
        obj_material_attrib_member attrib = args->material->material.attrib.m;
        if (!attrib.flag_28 && (args->blend_color.w < 1.0f
            || (attrib.alpha_texture || attrib.alpha_material) && !attrib.punch_through
            || args->sub_mesh->attrib.m.transparent)
            || attrib.punch_through) {
            uniform->arr[U_ALPHA_TEST] = 1;
            draw_sub_mesh_translucent(args);
        }
        else {
            uniform->arr[U_ALPHA_TEST] = 0;
            draw_sub_mesh_translucent(args);
        }
    }

    void draw_sub_mesh_translucent(const ObjSubMeshArgs* args) {
        const obj_material_data* material = args->material;
        const prj::vector<GLuint>* textures = args->textures;
        if (draw_state->shader_index != -1) {
            rctx->obj_batch.g_material_state_emission = material->material.color.emission;
            draw_object_material_set_uniform(material, false);
            if (material->material.attrib.m.alpha_texture)
                uniform->arr[U_TEXTURE_COUNT] = 0;
            rctx->obj_batch.g_texture_blend = { (float_t)uniform->arr[U_TEXTURE_BLEND], 0.0f, 0.0f, 0.0f };
            shaders_ft.set(draw_state->shader_index);
        }

        draw_object_vertex_attrib_set_default(args);
        if (args->material->material.attrib.m.double_sided)
            gl_state_disable_cull_face();

        GLuint tex_id = -1;
        int32_t tex_index = 0;
        if (material->material.attrib.m.alpha_texture) {
            const obj_material_texture_data* texdata = material->material.texdata;
            uint32_t texture_id = -1;
            if (!material->material.shader_compo.m.transparency) {
                for (int32_t i = 0; i < 8; i++, texdata++)
                    if (texdata->shader_info.m.tex_type == OBJ_MATERIAL_TEXTURE_COLOR) {
                        texture_id = texdata->tex_index;
                        break;
                    }
                tex_index = 0;
            }
            else {
                for (int32_t i = 0; i < 8; i++, texdata++)
                    if (texdata->shader_info.m.tex_type == OBJ_MATERIAL_TEXTURE_TRANSPARENCY) {
                        if (texdata->attrib.m.flag_29)
                            texture_id = texdata->tex_index;
                        break;
                    }
                tex_index = 3;
            }

            if (texture_id != -1) {
                texture_id &= 0xFFFFF;
                for (int32_t j = 0; j < args->texture_pattern_count; j++)
                    if (args->texture_pattern_array[j].src == tex_index) {
                        texture* tex = texture_handler_get_texture(args->texture_pattern_array[j].dst);
                        if (tex)
                            tex_id = tex->tex;
                        break;
                    }

                if (tex_id == -1)
                    tex_id = (*textures)[texdata->tex_index >> 20];

                if (tex_id == -1)
                    tex_id = 0;

                gl_state_active_bind_texture_2d(tex_index, tex_id);

                int32_t wrap_s;
                if (texdata->attrib.m.mirror_u)
                    wrap_s = 2;
                else if (texdata->attrib.m.repeat_u)
                    wrap_s = 1;
                else
                    wrap_s = 0;

                int32_t wrap_t;
                if (texdata->attrib.m.mirror_v)
                    wrap_t = 2;
                else if (texdata->attrib.m.repeat_v)
                    wrap_t = 1;
                else
                    wrap_t = 0;

                texture* tex = texture_handler_get_texture(texture_id);
                gl_state_bind_sampler(tex_index, rctx->samplers[(wrap_t * 3 + wrap_s) * 2
                    + (tex->max_mipmap_level > 0 ? 1 : 0)]);
            }
        }

        const obj_sub_mesh* sub_mesh = args->sub_mesh;
        if (sub_mesh->index_format != OBJ_INDEX_U8)
            draw(
                sub_mesh->primitive_type,
                sub_mesh->num_index,
                sub_mesh->first_index,
                sub_mesh->last_index,
                sub_mesh->index_format,
                sub_mesh->index_offset);

        if (tex_id != -1)
            gl_state_active_bind_texture_2d(tex_index, rctx->empty_texture_2d);

        gl_state_enable_cull_face();
        draw_object_vertex_attrib_reset_default(args);

        if (draw_state->shader_index != -1)
            uniform_value_reset();

        draw_state->stats.object_translucent_draw_count++;
    }
}

inline void model_mat_face_camera_position(const mat4* view, const mat4* src, mat4* dst) {
    mat4 mat;
    mat4_invert_fast(view, &mat);

    vec3 dir = vec3::normalize(*(vec3*)&mat.row3 - *(vec3*)&src->row3);

    vec3 x_rot;
    vec3 y_rot;
    vec3 z_rot;

    y_rot = *(vec3*)&src->row1;
    x_rot = vec3::normalize(vec3::cross(y_rot, dir));
    z_rot = vec3::cross(x_rot, y_rot);

    *(vec3*)&dst->row0 = x_rot;
    *(vec3*)&dst->row1 = y_rot;
    *(vec3*)&dst->row2 = z_rot;
    *(vec3*)&dst->row3 = *(vec3*)&src->row3;
    dst->row0.w = 0.0f;
    dst->row1.w = 0.0f;
    dst->row2.w = 0.0f;
    dst->row3.w = 1.0f;
}

inline void model_mat_face_camera_view(const mat4* view, const mat4* src, mat4* dst) {
    mat3 mat;
    mat4_to_mat3(view, &mat);
    mat3_invert(&mat, &mat);
    mat4_from_mat3(&mat, dst);
    mat4_mul(dst, src, dst);
}

static bool draw_object_blend_set(
    const mdl::ObjSubMeshArgs* args, obj_material_shader_lighting_type lighting_type) {
    static const GLenum blend_factor_table[] = {
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA_SATURATE,
        GL_ZERO,
        GL_ZERO,
        GL_ZERO,
        GL_ZERO,
        GL_ZERO,
    };

    const obj_material_data* material = args->material;
    obj_material_attrib_member attrib = material->material.attrib.m;
    if ((!attrib.alpha_texture && !attrib.alpha_material) || attrib.punch_through)
        return false;

    GLenum src_blend_factor = blend_factor_table[attrib.src_blend_factor];
    GLenum dst_blend_factor = blend_factor_table[attrib.dst_blend_factor];
    if (args->chara_color) {
        light_set* set = &light_set_data[LIGHT_SET_MAIN];
        light_data* chara_color = &set->lights[LIGHT_CHARA_COLOR];
        vec4 specular;
        chara_color->get_specular(specular);
        if (specular.w >= 4.0f && chara_color->get_type() == LIGHT_PARALLEL
            && (src_blend_factor != GL_ONE || dst_blend_factor)) {
            int32_t shader_index = draw_state->shader_index;
            if (shader_index == -1) {
                shader_index = material->material.shader.index;
                if (shader_index == SHADER_FT_BLINN) {
                    if (lighting_type == OBJ_MATERIAL_SHADER_LIGHTING_CONSTANT)
                        shader_index = SHADER_FT_CONSTANT;
                    if (lighting_type == OBJ_MATERIAL_SHADER_LIGHTING_LAMBERT)
                        shader_index = SHADER_FT_LAMBERT;
                }
            }

            if (draw_object_blend_set_check_use_default_blend(shader_index)) {
                src_blend_factor = GL_SRC_ALPHA;
                dst_blend_factor = GL_ONE_MINUS_SRC_ALPHA;
            }
        }
    }
    gl_state_set_blend_func(src_blend_factor, dst_blend_factor);
    return dst_blend_factor == GL_ONE;
}

static bool draw_object_blend_set_check_use_default_blend(int32_t index) {
    bool use_default_blend[] = {
        false, false,  true, false,  true, false, false,  true,  true,  true, false, false,
        false,  true, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false
    };

    if (index >= 0 && index < SHADER_FT_MAX)
        return use_default_blend[index];
    return false;
}

static void draw_object_chara_color_fog_set(const mdl::ObjSubMeshArgs* args, bool disable_fog) {
    uniform->arr[U_CHARA_COLOR] = 0;
    if (args->chara_color) {
        light_set* set = &light_set_data[LIGHT_SET_MAIN];
        light_data* chara_color = &set->lights[LIGHT_CHARA_COLOR];
        vec4 specular;
        chara_color->get_specular(specular);
        if (specular.w > 0.0f && chara_color->get_type() == LIGHT_PARALLEL)
            uniform->arr[U_CHARA_COLOR] = 1;
    }

    obj_material_attrib_member attrib = args->material->material.attrib.m;
    if (!attrib.no_fog && !disable_fog) {
        if (attrib.has_fog_height)
            uniform->arr[U_FOG_HEIGHT] = 2 + attrib.fog_height;
        else
            uniform->arr[U_FOG_HEIGHT] = 1;
        uniform->arr[U_FOG] = draw_state->fog_height ? 2 : 1;
    }
}

static void draw_object_material_reset_default(const obj_material_data* mat_data) {
    if (mat_data) {
        gl_state_enable_cull_face();
        obj_material_attrib_member attrib = mat_data->material.attrib.m;
        if ((attrib.alpha_texture || attrib.alpha_material) && !attrib.punch_through)
            gl_state_set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    uniform_value_reset();
}

static void draw_object_material_reset_reflect() {
    gl_state_active_bind_texture_2d(0, rctx->empty_texture_2d);
    gl_state_enable_cull_face();
    uniform_value_reset();
}

static void draw_object_material_set_default(const mdl::ObjSubMeshArgs* args, bool use_shader) {
    const prj::vector<GLuint>* textures = args->textures;
    const obj_material_data* material = args->material;
    obj_material_shader_lighting_type lighting_type =
        material->material.shader_info.get_lighting_type();
    bool disable_fog = draw_object_blend_set(args, lighting_type);
    draw_object_material_set_uniform(material, false);
    if (!draw_state->light)
        uniform->arr[U_LIGHT_0] = 0;
    else if (args->self_shadow)
        uniform->arr[U_LIGHT_0] = 1;
    else
        uniform->arr[U_LIGHT_0] = args->sub_mesh->attrib.m.recieve_shadow ? 1 : 0;
    uniform->arr[U_SHADOW] = 0;
    uniform->arr[U_SELF_SHADOW] = draw_state->self_shadow ? 1 : 0;

    const obj_material_texture_data* texdata = material->material.texdata;
    uniform->arr[U_SHADOW] = args->shadow > SHADOW_CHARA;
    for (int32_t i = 0, j = 0; i < 8; i++, texdata++) {
        if (texdata->tex_index == -1)
            continue;

        GLuint tex_id = -1;
        uint32_t texture_id = texdata->tex_index & 0xFFFFF;
        for (int32_t j = 0; j < args->texture_pattern_count; j++)
            if (args->texture_pattern_array[j].src == texture_id) {
                texture* tex = texture_handler_get_texture(args->texture_pattern_array[j].dst);
                if (tex)
                    tex_id = tex->tex;
                break;
            }

        if (tex_id == -1)
            tex_id = (*textures)[texdata->tex_index >> 20];

        if (tex_id == -1)
            continue;

        int32_t tex_index = obj_material_texture_type_get_texture_index(texdata->shader_info.m.tex_type, j);
        if (tex_index < 0)
            continue;

        obj_material_texture_type tex_type = texdata->shader_info.m.tex_type;
        if (tex_type == OBJ_MATERIAL_TEXTURE_COLOR)
            j++;

        if (texdata->attrib.m.flag_29) {
            switch (tex_type) {
            case OBJ_MATERIAL_TEXTURE_NORMAL:
                uniform->arr[U_NORMAL] = 0;
                break;
            case OBJ_MATERIAL_TEXTURE_SPECULAR:
                uniform->arr[U_SPECULAR] = 0;
                break;
            case OBJ_MATERIAL_TEXTURE_TRANSLUCENCY:
                uniform->arr[U_TRANSLUCENCY] = 0;
                break;
            case OBJ_MATERIAL_TEXTURE_TRANSPARENCY:
                uniform->arr[U_TRANSPARENCY] = 0;
                break;
            }
            continue;
        }

        if (tex_type == OBJ_MATERIAL_TEXTURE_ENVIRONMENT_CUBE) {
            gl_state_active_bind_texture_cube_map(tex_index, tex_id);
            gl_state_bind_sampler(tex_index, 0);
        }
        else {
            gl_state_active_bind_texture_2d(tex_index, tex_id);

            int32_t wrap_s;
            if (texdata->attrib.m.mirror_u)
                wrap_s = 2;
            else if (texdata->attrib.m.repeat_u)
                wrap_s = 1;
            else
                wrap_s = 0;

            int32_t wrap_t;
            if (texdata->attrib.m.mirror_v)
                wrap_t = 2;
            else if (texdata->attrib.m.repeat_v)
                wrap_t = 1;
            else
                wrap_t = 0;

            texture* tex = texture_handler_get_texture(texture_id);
            gl_state_bind_sampler(tex_index, rctx->samplers[(wrap_t * 3 + wrap_s) * 2
                + (tex->max_mipmap_level > 0 ? 1 : 0)]);
        }

        if (material->material.shader.index == SHADER_FT_SKY) {
            uniform_name uni_type = U_TEX_0_TYPE;
            if (tex_index == 1)
                uni_type = U_TEX_1_TYPE;

            texture* tex = texture_handler_get_texture(texture_id);
            if (!tex)
                uniform->arr[uni_type] = 1;
            else if (tex->internal_format == GL_COMPRESSED_RED_RGTC1_EXT)
                uniform->arr[uni_type] = 3;
            else if (tex->internal_format == GL_COMPRESSED_RED_GREEN_RGTC2_EXT)
                uniform->arr[uni_type] = 2;
            else
                uniform->arr[uni_type] = 1;
        }

        if (tex_index >= 0 && tex_index <= 1)
            uniform->arr[U_TEXTURE_BLEND] = texdata->attrib.get_blend();
    }

    if (material->material.attrib.m.double_sided) {
        gl_state_disable_cull_face();
        if (!material->material.attrib.m.normal_dir_light)
            uniform->arr[U0B] = 1;
    }

    draw_object_chara_color_fog_set(args, disable_fog);

    rctx->obj_batch.g_material_state_ambient = material->material.color.ambient;
    if (!sss_data_get()->enable)
        rctx->obj_batch.g_material_state_ambient.w = 1.0f;
    rctx->obj_batch.g_material_state_diffuse = material->material.color.diffuse;
    rctx->obj_batch.g_material_state_emission = material->material.color.emission;

    float_t line_light;
    if (lighting_type == OBJ_MATERIAL_SHADER_LIGHTING_PHONG) {
        const vec4& specular = material->material.color.specular;
        rctx->obj_batch.g_material_state_specular = specular;

        float_t luma = vec3::dot(*(vec3*)&specular, { 0.30f, 0.59f, 0.11f });
        if (luma >= 0.01f || args->texture_color_coefficients.w >= 0.1f)
            uniform->arr[U_SPECULAR_IBL] = 1;
        else
            uniform->arr[U_SPECULAR_IBL] = 0;

        if (!material->material.shader_info.m.fresnel_type)
            uniform->arr[U_TRANSLUCENCY] = 0;

        line_light = (float_t)material->material.shader_info.m.line_light * (float_t)(1.0 / 9.0);
    }
    else
        line_light = 0.0;

    if (!use_shader)
        shaders_ft.set(SHADER_FT_SIMPLE);
    else if (draw_state->shader_index != -1)
        shaders_ft.set(draw_state->shader_index);
    else if (material->material.shader.index != -1) {
        if (material->material.shader.index != SHADER_FT_BLINN)
            shaders_ft.set(material->material.shader.index);
        else if (lighting_type == OBJ_MATERIAL_SHADER_LIGHTING_LAMBERT)
            shaders_ft.set(SHADER_FT_LAMBERT);
        else if (lighting_type == OBJ_MATERIAL_SHADER_LIGHTING_PHONG)
            shaders_ft.set(SHADER_FT_BLINN);
        else
            shaders_ft.set(SHADER_FT_CONSTANT);
    }
    else
        shaders_ft.set(SHADER_FT_CONSTANT);

    rctx->obj_batch.g_texture_blend = { (float_t)uniform->arr[U_TEXTURE_BLEND], 0.0f, 0.0f, 0.0f };
    if (lighting_type != OBJ_MATERIAL_SHADER_LIGHTING_CONSTANT) {
        float_t material_shininess;
        if (material->material.shader.index == SHADER_FT_GLASEYE/*SHADER_FT_EYEBALL*/)
            material_shininess = 10.0f;
        else {
            material_shininess = (material->material.color.shininess - 16.0f) * (float_t)(1.0 / 112.0);
            material_shininess = max_def(material_shininess, 0.0f);
        }
        rctx->obj_batch.g_material_state_shininess = { material_shininess, 0.0f, 0.0f, 1.0f };

        float_t fresnel = (float_t)material->material.shader_info.m.fresnel_type;
        if (fresnel > 9.0f)
            fresnel = 9.0f;
        else if (fresnel == 0.0f)
            fresnel = draw_state->fresnel;
        fresnel = (fresnel - 1.0f) * 0.12f * 0.82f;

        rctx->obj_batch.g_fresnel_coefficients = { fresnel, 0.18f, line_light, 0.0f };

        float_t shininess = max_def(material->material.color.shininess, 1.0f);
        rctx->obj_batch.g_shininess = { shininess, 0.0f, 0.0f, 0.0f };

        switch (material->material.shader.index) {
        case SHADER_FT_SKIN: {
            vec4 texture_color_coefficients = args->texture_color_coefficients;
            vec4 texture_color_offset = args->texture_color_offset;
            vec4 texture_specular_coefficients = args->texture_specular_coefficients;
            vec4 texture_specular_offset = args->texture_specular_offset;

            texture_color_coefficients.w *= 0.015f;
            texture_specular_coefficients.w *= 0.015f;

            rctx->obj_batch.g_texture_color_coefficients = texture_color_coefficients;
            rctx->obj_batch.g_texture_color_offset = texture_color_offset;
            rctx->obj_batch.g_texture_specular_coefficients = texture_specular_coefficients;
            rctx->obj_batch.g_texture_specular_offset = texture_specular_offset;
        } break;
        case SHADER_FT_HAIR:
        case SHADER_FT_CLOTH:
        case SHADER_FT_TIGHTS:
            rctx->obj_batch.g_texture_color_coefficients = {
                1.0f - args->texture_color_coefficients.w * 0.4f,
                0.0f, 0.0f, args->texture_color_coefficients.w * 0.02f };
            break;
        }
    }

    draw_object_material_set_parameter(material);
}

static void draw_object_material_set_parameter(const obj_material_data* mat_data) {
    float_t bump_depth;
    float_t intensity;
    float_t reflect_uv_scale;
    float_t refract_uv_scale;
    float_t inv_bump_depth;
    if (draw_state->use_global_material) {
        bump_depth = draw_state->bump_depth;
        intensity = draw_state->intensity;
        reflect_uv_scale = draw_state->reflect_uv_scale;
        refract_uv_scale = draw_state->refract_uv_scale;
        inv_bump_depth = (1.0f - draw_state->bump_depth) * 64.0f + 1.0f;

        vec4 specular = mat_data->material.color.specular;
        specular.w = draw_state->reflectivity;
        rctx->obj_batch.g_material_state_specular = specular;
    }
    else {
        bump_depth = mat_data->material.bump_depth;
        reflect_uv_scale = 0.1f;
        intensity = mat_data->material.color.intensity;
        refract_uv_scale = 0.1f;
        inv_bump_depth = (1.0f - bump_depth) * 256.0f + 1.0f;
    }

    rctx->obj_batch.g_bump_depth = { inv_bump_depth, bump_depth, 0.0f, 0.0f };
    rctx->obj_batch.g_intensity = { intensity, max_def(intensity, 1.0f), intensity * 25.5f, 1.0f };
    rctx->obj_batch.g_reflect_uv_scale = {
        reflect_uv_scale, reflect_uv_scale, refract_uv_scale, refract_uv_scale };
}

static void draw_object_material_set_reflect(const mdl::ObjSubMeshArgs* args) {
    const obj_material_data* material = args->material;
    const prj::vector<GLuint>* textures = args->textures;
    if (material->material.attrib.m.double_sided)
        gl_state_disable_cull_face();

    const obj_material_texture_data* texdata = material->material.texdata;
    for (int32_t i = 0; i < 1; i++, texdata++) {
        uint32_t texture_id = texdata->tex_index & 0xFFFFF;
        if (texdata->tex_index == -1)
            break;

        GLuint tex_id = -1;
        for (int32_t j = 0; j < args->texture_pattern_count; j++)
            if (args->texture_pattern_array[j].src == texture_id) {
                texture* tex = texture_handler_get_texture(args->texture_pattern_array[j].dst);
                if (tex)
                    tex_id = tex->tex;
                break;
            }

        if (tex_id == -1)
            tex_id = (*textures)[texdata->tex_index >> 20];

        if (tex_id == -1)
            tex_id = 0;

        gl_state_active_bind_texture_2d(i, tex_id);

        int32_t wrap_s;
        if (texdata->attrib.m.mirror_u)
            wrap_s = 2;
        else if (texdata->attrib.m.repeat_u)
            wrap_s = 1;
        else
            wrap_s = 0;

        int32_t wrap_t;
        if (texdata->attrib.m.mirror_v)
            wrap_t = 2;
        else if (texdata->attrib.m.repeat_v)
            wrap_t = 1;
        else
            wrap_t = 0;

        texture* tex = texture_handler_get_texture(texture_id);
        gl_state_bind_sampler(i, rctx->samplers[(wrap_t * 3 + wrap_s) * 2
            + (tex->max_mipmap_level > 0 ? 1 : 0)]);
    }

    vec4 ambient;
    vec4 diffuse;
    vec4 emission;
    vec4 specular;
    if (material->material.shader.index == -1) {
        ambient = 1.0f;
        diffuse = material->material.color.diffuse;
        emission = 1.0f;
        specular = material->material.color.specular;
    }
    else {
        ambient = material->material.color.ambient;
        diffuse = material->material.color.diffuse;
        emission = material->material.color.emission;
        specular = material->material.color.specular;
    }

    draw_object_material_set_uniform(material, true);
    rctx->obj_batch.g_texture_blend = { (float_t)uniform->arr[U_TEXTURE_BLEND], 0.0f, 0.0f, 0.0f };
    shaders_ft.set(draw_state->shader_index);
    rctx->obj_batch.g_material_state_ambient = ambient;
    rctx->obj_batch.g_material_state_diffuse = diffuse;
    rctx->obj_batch.g_material_state_emission = emission;
    rctx->obj_batch.g_material_state_specular = specular;
    draw_object_material_set_parameter(args->material);
}

static void draw_object_material_set_uniform(const obj_material_data* mat_data, bool disable_color_l) {
    obj_shader_compo_member shader_compo = mat_data->material.shader_compo.m;
    obj_material_shader_attrib_member shader_info = mat_data->material.shader_info.m;

    if (disable_color_l) {
        if (shader_compo.color)
            shader_compo.color_l1 = 0;
        shader_compo.color_l2 = 0;
    }

    int32_t v4 = 0;
    if (shader_compo.color) {
        if (!shader_compo.color_l2)
            if (shader_compo.color_l1) {
                uniform->arr[U_TEXTURE_COUNT] = 2;
                v4 = 3;
            }
            else {
                uniform->arr[U_TEXTURE_COUNT] = 1;
                v4 = 1;
            }
    }
    else if (shader_compo.color_l1 && !shader_compo.color_l2) {
        uniform->arr[U_TEXTURE_COUNT] = 1;
        v4 = 1;
    }
    else
        uniform->arr[U_TEXTURE_COUNT] = 0;

    if (shader_compo.normal_01)
        uniform->arr[U_NORMAL] = 1;
    if (shader_compo.specular)
        uniform->arr[U_SPECULAR] = 1;
    if (shader_compo.transparency) {
        uniform->arr[U_TRANSPARENCY] = 1;
        v4 |= 2;
    }
    if (shader_compo.translucency) {
        uniform->arr[U_TRANSLUCENCY] = 1;
        v4 |= 2;
    }
    if (shader_compo.override_ibl)
        uniform->arr[U_ENV_MAP] = 1;

    if (shader_info.aniso_direction != OBJ_MATERIAL_ANISO_DIRECTION_NORMAL)
        uniform->arr[U_ANISO] = shader_info.aniso_direction;

    if (v4 == 1)
        uniform->arr[U45] = 0;
    else if (v4 == 2 || v4 == 3)
        uniform->arr[U45] = 1;
}

static void draw_object_model_mat_load(const mat4& mat) {
    rctx->obj_batch.set_transforms(mat, rctx->view_mat, rctx->proj_mat);
    rctx->obj_batch.set_g_joint(mat);
}

static void draw_object_vertex_attrib_reset_default(const mdl::ObjSubMeshArgs* args) {
    const obj_mesh* mesh = args->mesh;
    obj_vertex_format vertex_format = mesh->vertex_format;

    if ((vertex_format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA)
        uniform->arr[U_BONE_MAT] = 0;

    if (args->morph_vertex_buffer) {
        uniform->arr[U_MORPH] = 0;
        uniform->arr[U_MORPH_COLOR] = 0;
    }

    /*if (args->instances_count)
        uniform->arr[U_INSTANCE] = 0;*/

    gl_state_active_texture(0);
}

static void draw_object_vertex_attrib_reset_reflect(const mdl::ObjSubMeshArgs* args) {
    const obj_mesh* mesh = args->mesh;
    obj_vertex_format vertex_format = mesh->vertex_format;

    if ((vertex_format & OBJ_VERTEX_BONE_DATA) == OBJ_VERTEX_BONE_DATA)
        uniform->arr[U_BONE_MAT] = 0;

    if (args->morph_vertex_buffer) {
        uniform->arr[U_MORPH] = 0;
        uniform->arr[U_MORPH_COLOR] = 0;
    }

    gl_state_active_texture(0);
}

static void draw_object_vertex_attrib_set_default(const mdl::ObjSubMeshArgs* args) {
    const obj_mesh* mesh = args->mesh;
    obj_vertex_format vertex_format = mesh->vertex_format;

    bool texcoord_mat_set[4] = { false };
    const obj_material_data* material = args->material;
    const obj_material_texture_data* texdata = material->material.texdata;
    for (int32_t i = 0, j = 0, l = 0; i < 4; i++, texdata++) {
        if (texdata->tex_index == -1)
            continue;

        int32_t texcoord_index = obj_material_texture_type_get_texcoord_index(
            texdata->shader_info.m.tex_type, j);
        if (texcoord_index < 0)
            continue;

        if (texdata->shader_info.m.tex_type == OBJ_MATERIAL_TEXTURE_COLOR)
            j++;

        l++;

        if (texcoord_mat_set[texcoord_index])
            continue;

        uint32_t texture_id = texdata->tex_index & 0xFFFFF;

        rctx->obj_batch.set_g_texcoord_transforms(texcoord_index,
            texdata->tex_coord_mat);
        if (texdata->shader_info.m.tex_type == OBJ_MATERIAL_TEXTURE_COLOR)
            for (int32_t k = 0; k < args->texture_transform_count; k++)
                if (args->texture_transform_array[k].id == texture_id) {
                    rctx->obj_batch.set_g_texcoord_transforms(texcoord_index,
                        args->texture_transform_array[k].mat);
                    texcoord_mat_set[texcoord_index] = true;
                    break;
                }
    }

    if (vertex_format & OBJ_VERTEX_BONE_DATA)
        uniform->arr[U_BONE_MAT] = 1;
    else
        uniform->arr[U_BONE_MAT] = 0;

    if (args->morph_vertex_buffer) {
        uniform->arr[U_MORPH] = 1;

        if (vertex_format & OBJ_VERTEX_COLOR0)
            uniform->arr[U_MORPH_COLOR] = 1;
        else
            uniform->arr[U_MORPH_COLOR] = 0;

        rctx->obj_batch.g_morph_weight = { args->morph_weight, 1.0f - args->morph_weight, 0.0f, 0.0f };
    }
    else {
        uniform->arr[U_MORPH] = 0;
        uniform->arr[U_MORPH_COLOR] = 0;
    }

    /*if (args->instances_count)
        uniform->arr[U_INSTANCE] = 1;
    else
        uniform->arr[U_INSTANCE] = 0;*/
}

static void draw_object_vertex_attrib_set_reflect(const mdl::ObjSubMeshArgs* args) {
    const obj_mesh* mesh = args->mesh;
    obj_vertex_format vertex_format = mesh->vertex_format;

    const obj_material_data* material = args->material;
    if (material->material.texdata[0].tex_index != -1) {
        uint32_t texture_id = material->material.texdata[0].tex_index & 0xFFFFF;

        rctx->obj_batch.set_g_texcoord_transforms(0,
            material->material.texdata[0].tex_coord_mat);
        if (material->material.texdata[0].shader_info.m.tex_type == OBJ_MATERIAL_TEXTURE_COLOR)
            for (int32_t j = 0; j < args->texture_transform_count; j++)
                if (args->texture_transform_array[j].id == texture_id) {
                    rctx->obj_batch.set_g_texcoord_transforms(0,
                        args->texture_transform_array[j].mat);
                    break;
                }
    }

    if (vertex_format & OBJ_VERTEX_BONE_DATA)
        uniform->arr[U_BONE_MAT] = 1;
    else
        uniform->arr[U_BONE_MAT] = 0;

    if (args->morph_vertex_buffer) {
        uniform->arr[U_MORPH] = 1;

        if (vertex_format & OBJ_VERTEX_COLOR0)
            uniform->arr[U_MORPH_COLOR] = 1;
        else
            uniform->arr[U_MORPH_COLOR] = 0;

        rctx->obj_batch.g_morph_weight = { args->morph_weight, 1.0f - args->morph_weight, 0.0f, 0.0f };
    }
    else {
        uniform->arr[U_MORPH] = 0;
        uniform->arr[U_MORPH_COLOR] = 0;
    }
}
