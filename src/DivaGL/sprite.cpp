/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "sprite.hpp"
#include "render_context.hpp"
#include "shader_ft.hpp"
#include <Helpers.h>

struct sprite_draw_vertex {
    vec3 pos;
    vec2 uv[2];
    color4u8 color;
};

struct sprite_draw_param {
    bool blend;
    GLenum blend_src_rgb;
    GLenum blend_src_alpha;
    GLenum blend_dst_rgb;
    GLenum blend_dst_alpha;

    bool copy_texture;
    GLint copy_texture_x;
    GLint copy_texture_y;
    GLint copy_texture_width;
    GLint copy_texture_height;

    int32_t shader;
    int32_t tex_0_type;
    int32_t tex_1_type;
    int32_t combiner;

    GLuint texture[2];
    GLuint sampler;

    GLenum mode;
    union {
        GLint first;
        struct {
            GLuint start;
            GLuint end;
        };
    };
    GLsizei count;
    GLintptr offset;
};

namespace spr {
    struct TexCoord {
        struct UV {
            float_t u;
            float_t v;
        };

        TexCoord::UV uv[4];
    };

    struct TexParam {
        texture* texture;
        TexCoord texcoord;
        int32_t pad[2];
    };

    struct SpriteManager {
        struct RenderData {
            std::vector<sprite_draw_param> draw_param_buffer;
            std::vector<sprite_draw_vertex> vertex_buffer;
            std::vector<uint32_t> index_buffer;
            GLuint vao;
            GLArrayBuffer vbo;
            size_t vbo_vertex_count;
            GLElementArrayBuffer ebo;
            size_t ebo_index_count;

            RenderData();
            ~RenderData();

            void Clear();
            void Update();
        };

        prj::map<int32_t, SprSet> sets;
        prj::list<SprArgs> reqlist[4][2][SPR_PRIO_MAX];
        float_t aspect[2];
        prj::pair<resolution_mode, rectangle> field_1018[2];
        int32_t index;
        mat4 view_projection;
        mat4 mat;
        resolution_mode resolution_mode;

        void Draw(int32_t index, bool font, texture* tex);
    };

    static_assert(sizeof(spr::SpriteManager) == 0x10C8, "\"spr::SpriteManager\" struct should have a size of 0x10C8");

    static void calc_sprite_vertex(spr::SprArgs* args, vec3* vtx, mat4* mat, bool font);
    static int32_t calc_sprite_texture_param(SprArgs* args, spr::TexParam* param, vec3* vtx, bool font);

    static void draw_sprite(SprArgs& args, bool font, const mat4& mat,
        int32_t x_min, int32_t y_min, int32_t x_max, int32_t y_max,
        std::vector<sprite_draw_param>& draw_param_buffer,
        std::vector<sprite_draw_vertex>& vertex_buffer, std::vector<uint32_t>& index_buffer);
    static void draw_sprite_begin();
    static void draw_sprite_end();
    static void draw_sprite_scale(spr::SprArgs* args);
}

vec4& spr_color = *(vec4*)0x00000001411ACC30;

void (FASTCALL* sprite_manager_unload_set)(int32_t set_id)
    = (void (FASTCALL*)(int32_t set_id))0x000000014063F8D0;
size_t(FASTCALL* sprite_manager_get_reqlist_count)(int32_t index)
    = (size_t(FASTCALL*)(int32_t index))0x000000014063FA90;
void (FASTCALL* sprite_manager_read_file)(int32_t set_id, const prj::string mdata_dir)
    = (void (FASTCALL*)(int32_t set_id, const prj::string mdata_dir))0x0000000140640E10;
bool (FASTCALL* sprite_manager_load_file)(int32_t set_id)
    = (bool (FASTCALL*)(int32_t set_id))0x0000000140640FB0;

spr::SpriteManager* sprite_manager = (spr::SpriteManager*)0x000000014CC611C0;

static mat4 view_projection_aet;

static spr::SpriteManager::RenderData* sprite_manager_render_data;

static const GLenum spr_blend_param[6][4] = {
    { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE  },
    { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR, GL_ZERO, GL_ONE  },
    { GL_SRC_ALPHA, GL_ONE,                 GL_ZERO, GL_ONE  },
    { GL_DST_COLOR, GL_ZERO,                GL_ZERO, GL_ONE  },
    { GL_ONE,       GL_ZERO,                GL_ONE,  GL_ZERO },
    { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE  },
};

namespace spr {
    void(FASTCALL* put_rgb_cross)(mat4* mat) = (void(FASTCALL*)(mat4 * mat))0x00000001404BC8F0;
    spr::SprArgs* (FASTCALL* put_sprite)(const spr::SprArgs* args)
        = (spr::SprArgs * (FASTCALL*)(const spr::SprArgs * args))0x0000000140640740;

    SpriteManager::RenderData::RenderData() {
        glGenVertexArrays(1, &vao);
        gl_state_bind_vertex_array(vao, true);

        vbo_vertex_count = 4096;

        static const GLsizei buffer_size = sizeof(sprite_draw_vertex);

        vbo.Create(buffer_size * vbo_vertex_count);
        vbo.Bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buffer_size,
            (void*)offsetof(sprite_draw_vertex, pos));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, buffer_size,
            (void*)offsetof(sprite_draw_vertex, color));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, buffer_size,
            (void*)offsetof(sprite_draw_vertex, uv[0]));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, buffer_size,
            (void*)offsetof(sprite_draw_vertex, uv[1]));

        ebo_index_count = 4096;

        ebo.Create(sizeof(uint32_t) * ebo_index_count);
        ebo.Bind();

        gl_state_bind_array_buffer(0);
        gl_state_bind_vertex_array(0);
        gl_state_bind_element_array_buffer(0);
    }

    SpriteManager::RenderData::~RenderData() {
        ebo.Destroy();
        vbo.Destroy();

        if (vao) {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }
    }

    void SpriteManager::RenderData::Clear() {
        draw_param_buffer.clear();
        vertex_buffer.clear();
        index_buffer.clear();
    }

    void SpriteManager::RenderData::Update() {
        static const GLsizei buffer_size = sizeof(sprite_draw_vertex);

        if (vbo_vertex_count < vertex_buffer.size()) {
            while (vbo_vertex_count < vertex_buffer.size())
                vbo_vertex_count *= 2;

            vbo.Destroy();

            gl_state_bind_vertex_array(vao, true);

            vbo.Create(buffer_size * vbo_vertex_count);
            vbo.Bind();

            glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(buffer_size
                * vertex_buffer.size()), vertex_buffer.data());

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buffer_size,
                (void*)offsetof(sprite_draw_vertex, pos));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, buffer_size,
                (void*)offsetof(sprite_draw_vertex, color));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, buffer_size,
                (void*)offsetof(sprite_draw_vertex, uv[0]));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, buffer_size,
                (void*)offsetof(sprite_draw_vertex, uv[1]));

            gl_state_bind_array_buffer(0);
            gl_state_bind_vertex_array(0);
        }
        else
            vbo.WriteMemory(0, buffer_size * vertex_buffer.size(), vertex_buffer.data());

        if (ebo_index_count < index_buffer.size()) {
            while (ebo_index_count < index_buffer.size())
                ebo_index_count *= 2;

            ebo.Destroy();

            gl_state_bind_vertex_array(vao, true);

            ebo.Create(sizeof(uint32_t) * ebo_index_count);
            ebo.Bind();

            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (GLsizeiptr)(sizeof(uint32_t)
                * index_buffer.size()), index_buffer.data());

            gl_state_bind_vertex_array(0);
            gl_state_bind_element_array_buffer(0);
        }
        else
            ebo.WriteMemory(0, sizeof(uint32_t) * index_buffer.size(), index_buffer.data());
    }

    void SpriteManager::Draw(int32_t index, bool font, texture* tex) {
        draw_sprite_begin();

        size_t count = 0;
        for (auto& i : reqlist[index])
            for (auto& j : i)
                count += j.size();

        ::resolution_mode mode = res_window_get()->resolution_mode;
        if (index == 2 && resolution_mode != RESOLUTION_MODE_MAX)
            res_window_get()->resolution_mode = resolution_mode;

        GLint v43[4];
        glGetIntegervDLL(GL_VIEWPORT, v43);

        for (int32_t i = 0; i < 2; i++) {
            auto reqlist = this->reqlist[index][i];

            size_t count = 0;
            for (uint32_t j = SPR_PRIO_MAX, k = 0; j; j--, k++)
                count += reqlist[k].size();

            if (!count)
                continue;

            int32_t x_min;
            int32_t y_min;
            int32_t x_max;
            int32_t y_max;
            if (index == 0 || index == 3) {
                float_t sprite_half_width = (float_t)res_window_get()->width * 0.5f;
                float_t sprite_half_height = (float_t)res_window_get()->height * 0.5f;

                float_t aet_depth = camera_data->aet_depth;
                float_t aet_depth_1 = 1.0f / aet_depth;

                float_t v15a = sprite_half_height * aspect[i] * 0.2f * aet_depth_1;
                float_t v15b = sprite_half_height * 0.2f * aet_depth_1;

                mat4 proj;
                mat4_frustrum(-v15a, v15a, v15b, -v15b, 0.2f, 3000.0f, &proj);

                vec3 eye = { sprite_half_width, sprite_half_height, aet_depth };
                vec3 target = { sprite_half_width, sprite_half_height, 0.0f };
                vec3 up = { 0.0f, 1.0f, 0.0f };

                mat4 view;
                mat4_look_at(&eye, &target, &up, &view);
                mat4_mul(&view, &proj, &view_projection);
                mat4_transpose(&view_projection, &view_projection);

                vec2 min;
                vec2 max;
                resolution_mode_scale_pos(min, mode, field_1018[i].second.pos, field_1018[i].first);
                resolution_mode_scale_pos(max, mode,
                    field_1018[i].second.pos + field_1018[i].second.size, field_1018[i].first);
                x_min = (int32_t)min.x;
                y_min = (int32_t)min.y;
                x_max = (int32_t)(max.x - min.x);
                y_max = (int32_t)(max.y - min.y);
                glViewportDLL(x_min, y_min, x_max, y_max);
            }
            else {
                view_projection = view_projection_aet;

                x_min = v43[0];
                y_min = v43[1];
                x_max = v43[2];
                y_max = v43[3];
            }

            float_t v23 = (float_t)x_max * 0.5f;
            float_t v24 = (float_t)x_min + v23;
            float_t v25 = (float_t)y_max * 0.5f;
            float_t v26 = (float_t)y_min + v25;

            mat4 mat = view_projection;
            mat.row0.x = v23 * view_projection.row0.x + v24 * view_projection.row0.x;
            mat.row0.y = v23 * view_projection.row0.y + v24 * view_projection.row0.y;
            mat.row0.z = v23 * view_projection.row0.z + v24 * view_projection.row0.z;
            mat.row0.w = v23 * view_projection.row0.w + v24 * view_projection.row0.w;
            mat.row1.x = v25 * view_projection.row1.x + v26 * view_projection.row1.x;
            mat.row1.y = v25 * view_projection.row1.y + v26 * view_projection.row1.y;
            mat.row1.z = v25 * view_projection.row1.z + v26 * view_projection.row1.z;
            mat.row1.w = v25 * view_projection.row1.w + v26 * view_projection.row1.w;
            this->mat = mat;

            mat4_transpose(&mat, &mat);

            sprite_scene_shader_data shader_data = {};
            shader_data.g_framebuffer_size = {
                1.0f / (float_t)tex->width,
                1.0f / (float_t)tex->height, 0.0f, 0.0f
            };
            shader_data.g_transform = view_projection;
            rctx->sprite_scene_ubo.WriteMemory(shader_data);

            RenderData& render_data = *sprite_manager_render_data;

            render_data.Clear();

            for (uint32_t j = SPR_PRIO_MAX; j; j--, reqlist++)
                for (auto& k : *reqlist)
                    draw_sprite(k, font, mat, x_min, y_min, x_max, y_max,
                        render_data.draw_param_buffer,
                        render_data.vertex_buffer, render_data.index_buffer);

            render_data.Update();

            rctx->sprite_scene_ubo.Bind(0);

            gl_state_active_bind_texture_2d(7, tex->tex);

            gl_state_bind_vertex_array(render_data.vao);
            for (sprite_draw_param& j : render_data.draw_param_buffer) {
                if (j.blend) {
                    gl_state_enable_blend();
                    gl_state_set_blend_func_separate(
                        j.blend_src_rgb, j.blend_dst_rgb,
                        j.blend_src_alpha, j.blend_dst_alpha);

                    if (j.copy_texture) {
                        gl_state_active_texture(7);
                        glCopyTexSubImage2DDLL(GL_TEXTURE_2D, 0, j.copy_texture_x, j.copy_texture_y,
                            j.copy_texture_x, j.copy_texture_y, j.copy_texture_width, j.copy_texture_height);
                    }
                }
                else
                    gl_state_disable_blend();

                if (j.shader == SHADER_FT_SPRITE) {
                    uniform->arr[U_TEX_0_TYPE] = j.tex_0_type;
                    uniform->arr[U_TEX_1_TYPE] = j.tex_1_type;
                    uniform->arr[U_COMBINER] = j.combiner;
                }

                if (j.texture[0]) {
                    gl_state_active_bind_texture_2d(0, j.texture[0]);
                    gl_state_bind_sampler(0, j.sampler);

                    if (j.texture[1]) {
                        gl_state_active_bind_texture_2d(1, j.texture[1]);
                        gl_state_bind_sampler(1, j.sampler);
                    }
                    else if (!gl_state.texture_binding_2d[1])
                        gl_state_active_bind_texture_2d(1, rctx->empty_texture_2d);
                }
                else {
                    if (!gl_state.texture_binding_2d[0])
                        gl_state_active_bind_texture_2d(0, rctx->empty_texture_2d);
                    if (!gl_state.texture_binding_2d[1])
                        gl_state_active_bind_texture_2d(1, rctx->empty_texture_2d);
                }

                shaders_ft.set(j.shader);
                if (j.mode != GL_TRIANGLES)
                    shaders_ft.draw_arrays(j.mode, j.first, j.count);
                else
                    shaders_ft.draw_range_elements(j.mode, j.start, j.end,
                        j.count, GL_UNSIGNED_INT, (void*)j.offset);
            }
            gl_state_bind_vertex_array(0);
        }

        if (index == 2 && resolution_mode != RESOLUTION_MODE_MAX)
            res_window_get()->resolution_mode = mode;

        draw_sprite_end();
    }

    static void calc_sprite_vertex(spr::SprArgs* args, vec3* vtx, mat4* mat, bool font) {
        vtx[0].x = 0.0f;
        vtx[0].y = 0.0f;
        vtx[1].x = 0.0f;
        vtx[1].y = args->sprite_size.y;
        vtx[1].z = 0.0f;
        vtx[2].x = args->sprite_size.x;
        vtx[2].y = args->sprite_size.y;
        vtx[2].z = 0.0f;
        vtx[3].x = args->sprite_size.x;
        vtx[3].y = 0.0f;
        vtx[3].z = 0.0f;

        mat4 m;
        SprAttr attr = args->attr;
        if (!(attr & SPR_ATTR_MATRIX)) {
            if (attr & SPR_ATTR_CTR) {
                if (attr & SPR_ATTR_CTR_LT)
                    args->center = 0.0f;
                else if (attr & SPR_ATTR_CTR_LC) {
                    args->center.x = 0.0f;
                    args->center.y = args->sprite_size.y * 0.5f;
                    args->center.z = 0.0f;
                }
                else if (attr & SPR_ATTR_CTR_LB) {
                    args->center.x = 0.0f;
                    args->center.y = args->sprite_size.y;
                    args->center.z = 0.0f;
                }
                else if (attr & SPR_ATTR_CTR_CT) {
                    args->center.x = args->sprite_size.x * 0.5f;
                    args->center.y = 0.0f;
                    args->center.z = 0.0f;
                }
                else if (attr & SPR_ATTR_CTR_CC) {
                    args->center.x = args->sprite_size.x * 0.5f;
                    args->center.y = args->sprite_size.y * 0.5f;
                    args->center.z = 0.0f;
                }
                else if (attr & SPR_ATTR_CTR_CB) {
                    args->center.x = args->sprite_size.x * 0.5f;
                    args->center.y = args->sprite_size.y;
                    args->center.z = 0.0f;
                }
                else if (attr & SPR_ATTR_CTR_RT) {
                    args->center.x = args->sprite_size.x;
                    args->center.y = 0.0f;
                    args->center.z = 0.0f;
                }
                else if (attr & SPR_ATTR_CTR_RC) {
                    args->center.x = args->sprite_size.x;
                    args->center.y = args->sprite_size.y * 0.5f;
                    args->center.z = 0.0f;
                }
                else if (attr & SPR_ATTR_CTR_RB) {
                    args->center.x = args->sprite_size.x;
                    args->center.y = args->sprite_size.y;
                    args->center.z = 0.0f;
                }
            }

            if (font) {
                if (attr & SPR_ATTR_FLIP_H) {
                    args->scale.x = -args->scale.x;
                    args->rot.z = -args->rot.z;
                }
                if (attr & SPR_ATTR_FLIP_V) {
                    args->scale.y = -args->scale.y;
                    args->rot.z = -args->rot.z;
                }
            }

            if (fabsf(args->skew_angle.x) > 0.000001f) {
                float_t skew_width = tanf(args->skew_angle.x) * args->sprite_size.y * 0.5f;
                vtx[0].x = vtx[0].x + skew_width;
                vtx[1].x = vtx[1].x - skew_width;
                vtx[2].x = vtx[2].x - skew_width;
                vtx[3].x = vtx[3].x + skew_width;
            }

            if (fabsf(args->skew_angle.y) > 0.000001f) {
                float_t skew_height = tanf(args->skew_angle.y) * args->sprite_size.x * 0.5f;
                vtx[0].y = vtx[0].y - skew_height;
                vtx[1].y = vtx[1].y - skew_height;
                vtx[2].y = vtx[2].y + skew_height;
                vtx[3].y = vtx[3].y + skew_height;
            }

            mat4_translate(&args->trans, &m);
            if (fabsf(args->rot.x) > 0.000001f)
                mat4_mul_rotate_x(&m, args->rot.x, &m);
            if (fabsf(args->rot.y) > 0.000001f)
                mat4_mul_rotate_y(&m, args->rot.y, &m);
            if (fabsf(args->rot.z) > 0.000001f)
                mat4_mul_rotate_z(&m, args->rot.z, &m);
            mat4_scale_rot(&m, &args->scale, &m);
            const vec3 center = -args->center;
            mat4_mul_translate(&m, &center, &m);
        }
        else {
            mat4_translate(&args->trans, &m);
            mat4_scale_rot(&m, &args->scale, &m);
        }

        mat4 args_mat;
        mat4_transpose(&args->mat, &args_mat);
        mat4_mul(&args_mat, &m, &m);

        mat4_transform_point(&m, &vtx[0], &vtx[0]);
        mat4_transform_point(&m, &vtx[1], &vtx[1]);
        mat4_transform_point(&m, &vtx[2], &vtx[2]);
        mat4_transform_point(&m, &vtx[3], &vtx[3]);

        if (mat)
            *mat = m;
    }

    static int32_t calc_sprite_texture_param(SprArgs* args, spr::TexParam* param, vec3* vtx, bool font) {
        int32_t tex_param_count = 0;
        texture* tex = args->texture;
        while (args) {
            param->texture = args->texture;

            float_t width = (float_t)args->texture->width;
            float_t height = (float_t)args->texture->height;

            float_t u_scale = 1.0f / width;
            float_t v_scale = 1.0f / height;

            if (!args->num_vertex) {
                vec2 uv00;
                vec2 uv01;
                vec2 uv10;
                vec2 uv11;
                if (args->kind == SPR_KIND_LINE) {
                    vec3 v42[4];
                    mat4 mat;
                    calc_sprite_vertex(args, v42, &mat, font);
                    mat4_invert(&mat, &mat);
                    mat4_transform_point(&mat, &vtx[0], &v42[0]);
                    mat4_transform_point(&mat, &vtx[1], &v42[1]);
                    mat4_transform_point(&mat, &vtx[2], &v42[2]);
                    mat4_transform_point(&mat, &vtx[3], &v42[3]);
                    uv00 = *(vec2*)&v42[0].x;
                    uv01 = *(vec2*)&v42[1].x;
                    uv10 = *(vec2*)&v42[2].x;
                    uv11 = *(vec2*)&v42[3].x;
                }
                else {
                    uv00.x = 0.0f;
                    uv00.y = 0.0f;
                    uv01.x = 0.0f;
                    uv01.y = args->texture_size.y;
                    uv10.x = args->texture_size.x;
                    uv10.y = args->texture_size.y;
                    uv11.x = args->texture_size.x;
                    uv11.y = 0.0f;
                }

                vec2 texture_pos = args->texture_pos;
                param->texcoord.uv[0].u = (uv00.x + texture_pos.x) * u_scale;
                param->texcoord.uv[0].v = (height - (uv00.y + texture_pos.y)) * v_scale;
                param->texcoord.uv[1].u = (uv01.x + texture_pos.x) * u_scale;
                param->texcoord.uv[1].v = (height - (uv01.y + texture_pos.y)) * v_scale;
                param->texcoord.uv[2].u = (uv10.x + texture_pos.x) * u_scale;
                param->texcoord.uv[2].v = (height - (uv10.y + texture_pos.y)) * v_scale;
                param->texcoord.uv[3].u = (uv11.x + texture_pos.x) * u_scale;
                param->texcoord.uv[3].v = (height - (uv11.y + texture_pos.y)) * v_scale;
            }
            else if (font) {
                SpriteVertex* vtx = args->vertex_array;
                for (size_t i = args->num_vertex; i; i--, vtx++) {
                    vtx->uv.x = vtx->uv.x * u_scale;
                    vtx->uv.y = (height - vtx->uv.y) * v_scale;
                }
            }

            args = args->next;
            param++;
            tex_param_count++;
        }
        return tex_param_count;
    }

    static void draw_sprite(SprArgs& args, bool font,
        const mat4& mat, int32_t x_min, int32_t y_min, int32_t x_max, int32_t y_max,
        std::vector<sprite_draw_param>& draw_param_buffer,
        std::vector<sprite_draw_vertex>& vertex_buffer, std::vector<uint32_t>& index_buffer) {
        if (args.kind == SPR_KIND_LINE)
            return;

        // BGRA to RGBA
        const color4u8 color = { args.color.r, args.color.g, args.color.b, args.color.a };

        if (font)
            draw_sprite_scale(&args);

        vec3 vtx[4] = {};
        spr::TexParam tex_param[4] = {};

        if (args.flags & SprArgs::SPRITE_SIZE)
            calc_sprite_vertex(&args, vtx, 0, font);

        int32_t tex_param_count = 0;
        if ((args.flags & SprArgs::TEXTURE_POS_SIZE) && args.texture) {
            tex_param_count = calc_sprite_texture_param(&args, tex_param, vtx, font);
            tex_param_count = min_def(tex_param_count, 2);
        }

        mat4 mat_t;
        mat4_transpose(&mat, &mat_t);

        draw_param_buffer.push_back({});
        sprite_draw_param& draw_param = draw_param_buffer.back();

        if (!(args.attr & SPR_ATTR_NOBLEND)) {
            const GLenum* blend = spr_blend_param[args.blend];
            draw_param.blend = true;
            draw_param.blend_src_rgb = blend[0];
            draw_param.blend_dst_rgb = blend[1];
            draw_param.blend_src_alpha = blend[2];
            draw_param.blend_dst_alpha = blend[3];

            if (args.blend == 5 && !args.num_vertex && args.kind == SPR_KIND_NORMAL) {
                float_t v25 = 0.0f;
                float_t v26 = 0.0f;
                float_t v27 = 0.0f;
                float_t v28 = 0.0f;
                for (int32_t i = 0, j = 0; i < 4; i++) {
                    vec4 v;
                    *(vec3*)&v = vtx[i];
                    v.w = 1.0f;

                    float_t v31 = vec4::dot(mat_t.row3, v);
                    if (v31 * v31 <= 0.0001f)
                        continue;

                    float_t v33 = vec4::dot(mat_t.row0, v) * (1.0f / v31);
                    float_t v34 = vec4::dot(mat_t.row1, v) * (1.0f / v31);
                    if (j) {
                        if (v25 > v33)
                            v25 = v33;
                        else if (v27 < v33)
                            v27 = v33;

                        if (v26 > v34)
                            v26 = v34;
                        else if (v28 < v34)
                            v28 = v34;
                    }
                    else {
                        v25 = v33;
                        v27 = v33;
                        v26 = v34;
                        v28 = v34;
                    }
                    j++;
                }

                int32_t v36 = (int32_t)v27 + 1;
                int32_t v37 = (int32_t)v28 + 1;
                int32_t v38 = (int32_t)v25 - 1;
                int32_t v39 = (int32_t)v26 - 1;

                v36 = clamp_def(v36, x_min, x_min + x_max - 1);
                v37 = clamp_def(v37, y_min, y_min + y_max - 1);
                v38 = clamp_def(v38, x_min, x_min + x_max - 1);
                v39 = clamp_def(v39, y_min, y_min + y_max - 1);

                int32_t v42 = v36 - v38 + 1;
                int32_t v43 = v37 - v39 + 1;
                if (v42 * v43 > 0) {
                    draw_param.copy_texture = true;
                    draw_param.copy_texture_x = v38;
                    draw_param.copy_texture_y = v39;
                    draw_param.copy_texture_width = v42;
                    draw_param.copy_texture_height = v43;
                }
            }
        }

        draw_param.shader = args.shader;
        if (draw_param.shader == SHADER_FT_FFP) {
            draw_param.shader = SHADER_FT_SPRITE;
            draw_param.tex_0_type = 0;
            draw_param.tex_1_type = 0;

            if (tex_param_count == 1 || tex_param_count == 2) {
                if (tex_param[0].texture->internal_format == GL_COMPRESSED_RED_RGTC1_EXT)
                    draw_param.tex_0_type = 3;
                else if (tex_param[0].texture->internal_format == GL_COMPRESSED_RED_GREEN_RGTC2_EXT)
                    draw_param.tex_0_type = 2;
                else
                    draw_param.tex_0_type = 1;

                if (tex_param_count == 2) {
                    if (tex_param[1].texture->internal_format == GL_COMPRESSED_RED_RGTC1_EXT)
                        draw_param.tex_1_type = 3;
                    else if (tex_param[1].texture->internal_format == GL_COMPRESSED_RED_GREEN_RGTC2_EXT)
                        draw_param.tex_1_type = 2;
                    else
                        draw_param.tex_1_type = 1;
                }
            }

            if (args.attr & SPR_ATTR_NOBLEND)
                draw_param.combiner = 0;
            else if (args.blend != 5)
                draw_param.combiner = args.blend == 3 ? 1 : 0;
            else
                draw_param.combiner = 2;

        }

        switch (tex_param_count) {
        case 0:
            switch (args.kind) {
            case SPR_KIND_LINES: {
                if (vtx[0] == 0.0f && vtx[2] == 0.0f) {
                    draw_param_buffer.pop_back();
                    return;
                }

                sprite_draw_vertex spr_vtx[2] = {};
                spr_vtx[0].pos = vtx[0];
                spr_vtx[0].color = color;

                spr_vtx[1].pos = vtx[2];
                spr_vtx[1].color = color;

                draw_param.mode = GL_LINES;
                draw_param.first = (GLint)vertex_buffer.size();
                draw_param.count = 2;

                vertex_buffer.reserve(2);
                vertex_buffer.push_back(spr_vtx[0]);
                vertex_buffer.push_back(spr_vtx[1]);
            } break;
            case SPR_KIND_RECT: {
                if (vtx[0] == 0.0f && vtx[1] == 0.0f && vtx[2] == 0.0f && vtx[3] == 0.0f) {
                    draw_param_buffer.pop_back();
                    return;
                }

                sprite_draw_vertex spr_vtx[4] = {};
                spr_vtx[0].pos = vtx[0];
                spr_vtx[0].color = color;

                spr_vtx[1].pos = vtx[1];
                spr_vtx[1].color = color;

                spr_vtx[2].pos = vtx[2];
                spr_vtx[2].color = color;

                spr_vtx[3].pos = vtx[3];
                spr_vtx[3].color = color;

                draw_param.mode = GL_TRIANGLES;
                draw_param.start = (GLuint)vertex_buffer.size();
                draw_param.end = draw_param.start + 3;
                draw_param.count = 6;
                draw_param.offset = (GLintptr)(index_buffer.size() * sizeof(uint32_t));

                uint32_t start_vertex_index = (uint32_t)vertex_buffer.size();

                vertex_buffer.reserve(4);
                vertex_buffer.push_back(spr_vtx[0]); // LB
                vertex_buffer.push_back(spr_vtx[3]); // RB
                vertex_buffer.push_back(spr_vtx[2]); // RT
                vertex_buffer.push_back(spr_vtx[1]); // LT

                index_buffer.reserve(6);
                index_buffer.push_back(start_vertex_index + 0); // LB
                index_buffer.push_back(start_vertex_index + 3); // LT
                index_buffer.push_back(start_vertex_index + 1); // RB
                index_buffer.push_back(start_vertex_index + 1); // RB
                index_buffer.push_back(start_vertex_index + 3); // LT
                index_buffer.push_back(start_vertex_index + 2); // RT
            } break;
            case SPR_KIND_MULTI: {
                if (vtx[0] == 0.0f && vtx[1] == 0.0f && vtx[2] == 0.0f && vtx[3] == 0.0f) {
                    draw_param_buffer.pop_back();
                    return;
                }

                sprite_draw_vertex spr_vtx[4] = {};
                spr_vtx[0].pos = vtx[0];
                spr_vtx[0].color = color;

                spr_vtx[1].pos = vtx[1];
                spr_vtx[1].color = color;

                spr_vtx[2].pos = vtx[2];
                spr_vtx[2].color = color;

                spr_vtx[3].pos = vtx[3];
                spr_vtx[3].color = color;

                draw_param.mode = GL_LINE_LOOP;
                draw_param.first = (GLint)vertex_buffer.size();
                draw_param.count = 4;

                vertex_buffer.reserve(4);
                vertex_buffer.push_back(spr_vtx[0]);
                vertex_buffer.push_back(spr_vtx[1]);
                vertex_buffer.push_back(spr_vtx[2]);
                vertex_buffer.push_back(spr_vtx[3]);
            } break;
            case SPR_KIND_ARROW_B: {
                sprite_draw_vertex spr_vtx[2] = {};
                spr_vtx[0].color = color;
                spr_vtx[1].color = color;

                if (args.num_vertex) {
                    draw_param.mode = GL_LINES;
                    draw_param.first = (GLint)vertex_buffer.size();
                    draw_param.count = (uint32_t)(args.num_vertex / 2 * 2);

                    size_t vertex_buffer_size = vertex_buffer.size();
                    vertex_buffer.reserve(args.num_vertex / 2 * 2);

                    SpriteVertex* vtx = args.vertex_array;
                    for (size_t i = args.num_vertex / 2; i; i--, vtx += 2) {
                        if (vtx[0].pos == 0.0f && vtx[1].pos == 0.0f)
                            continue;

                        spr_vtx[0].pos = vtx[0].pos;
                        spr_vtx[1].pos = vtx[1].pos;
                        vertex_buffer.push_back(spr_vtx[0]);
                        vertex_buffer.push_back(spr_vtx[1]);
                    }

                    if (vertex_buffer_size == vertex_buffer.size()) {
                        draw_param_buffer.pop_back();
                        return;
                    }
                }
                else {
                    if (vtx[0] == 0.0f && vtx[2] == 0.0f) {
                        draw_param_buffer.pop_back();
                        return;
                    }

                    spr_vtx[0].pos = vtx[0];
                    spr_vtx[1].pos = vtx[2];

                    draw_param.mode = GL_LINES;
                    draw_param.first = (GLint)vertex_buffer.size();
                    draw_param.count = 2;

                    vertex_buffer.reserve(2);
                    vertex_buffer.push_back(spr_vtx[0]);
                    vertex_buffer.push_back(spr_vtx[1]);
                }
            } break;
            case SPR_KIND_ARROW_AB: {
                if (!args.num_vertex) {
                    draw_param_buffer.pop_back();
                    return;
                }

                draw_param.mode = GL_LINE_STRIP;
                draw_param.first = (GLint)vertex_buffer.size();
                draw_param.count = (GLsizei)args.num_vertex;

                vertex_buffer.reserve(args.num_vertex);

                sprite_draw_vertex spr_vtx = {};
                spr_vtx.color = color;

                SpriteVertex* vtx = args.vertex_array;
                for (size_t i = args.num_vertex; i; i--, vtx++) {
                    spr_vtx.pos = vtx->pos;
                    vertex_buffer.push_back(spr_vtx);
                }
            } break;
            case SPR_KIND_TRIANGLE: {
                if (!args.num_vertex) {
                    draw_param_buffer.pop_back();
                    return;
                }

                draw_param.mode = GL_TRIANGLE_STRIP;
                draw_param.first = (GLint)vertex_buffer.size();
                draw_param.count = (GLsizei)args.num_vertex;

                vertex_buffer.reserve(args.num_vertex);

                sprite_draw_vertex spr_vtx = {};
                spr_vtx.color = color;

                SpriteVertex* vtx = args.vertex_array;
                for (size_t i = args.num_vertex; i; i--, vtx++) {
                    spr_vtx.pos = vtx->pos;
                    vertex_buffer.push_back(spr_vtx);
                }
            } break;
            default:
                draw_param_buffer.pop_back();
                return;
            }
            break;
        case 1:
            draw_param.texture[0] = tex_param[0].texture ? tex_param[0].texture->tex : 0;
            draw_param.sampler = rctx->sprite_samplers[0];

            if (args.num_vertex) {
                if (args.kind == SPR_KIND_TRIANGLE)
                    draw_param.sampler = rctx->sprite_samplers[1];

                if (args.kind == SPR_KIND_TRIANGLE) {
                    draw_param.mode = GL_TRIANGLE_STRIP;
                    draw_param.first = (GLint)vertex_buffer.size();
                    draw_param.count = (GLsizei)args.num_vertex;

                    vertex_buffer.reserve(args.num_vertex);

                    sprite_draw_vertex spr_vtx = {};

                    SpriteVertex* vtx = args.vertex_array;
                    for (size_t i = args.num_vertex; i; i--, vtx++) {
                        spr_vtx.pos = vtx->pos;
                        spr_vtx.uv[0] = vtx->uv;
                        spr_vtx.color = vtx->color;
                        vertex_buffer.push_back(spr_vtx);
                    }
                }
                else {
                    size_t num_vertex = args.num_vertex / 4 * 6;

                    draw_param.mode = GL_TRIANGLES;
                    draw_param.start = (GLuint)vertex_buffer.size();
                    draw_param.end = draw_param.start + (GLuint)(args.num_vertex - 1);
                    draw_param.count = (GLsizei)num_vertex;
                    draw_param.offset = (GLintptr)(index_buffer.size() * sizeof(uint32_t));

                    uint32_t start_vertex_index = (uint32_t)vertex_buffer.size();

                    size_t vertex_buffer_size = vertex_buffer.size();
                    vertex_buffer.reserve(args.num_vertex);
                    index_buffer.reserve(num_vertex);

                    sprite_draw_vertex spr_vtx[4] = {};

                    SpriteVertex* vtx = args.vertex_array;
                    for (size_t i = num_vertex / 6, j = 0; i; i--, j += 4, vtx += 4) {
                        if (vtx[0].pos == 0.0f && vtx[1].pos == 0.0f
                            && vtx[2].pos == 0.0f && vtx[3].pos == 0.0f)
                            continue;

                        spr_vtx[0].pos = vtx[0].pos;
                        spr_vtx[0].uv[0] = vtx[0].uv;
                        spr_vtx[0].color = vtx[0].color;
                        spr_vtx[1].pos = vtx[1].pos;
                        spr_vtx[1].uv[0] = vtx[1].uv;
                        spr_vtx[1].color = vtx[1].color;
                        spr_vtx[2].pos = vtx[2].pos;
                        spr_vtx[2].uv[0] = vtx[2].uv;
                        spr_vtx[2].color = vtx[2].color;
                        spr_vtx[3].pos = vtx[3].pos;
                        spr_vtx[3].uv[0] = vtx[3].uv;
                        spr_vtx[3].color = vtx[3].color;

                        vertex_buffer.push_back(spr_vtx[0]); // LB
                        vertex_buffer.push_back(spr_vtx[3]); // RB
                        vertex_buffer.push_back(spr_vtx[2]); // RT
                        vertex_buffer.push_back(spr_vtx[1]); // LT

                        index_buffer.push_back(start_vertex_index + (uint32_t)j + 0); // LB
                        index_buffer.push_back(start_vertex_index + (uint32_t)j + 3); // LT
                        index_buffer.push_back(start_vertex_index + (uint32_t)j + 1); // RB
                        index_buffer.push_back(start_vertex_index + (uint32_t)j + 1); // RB
                        index_buffer.push_back(start_vertex_index + (uint32_t)j + 3); // LT
                        index_buffer.push_back(start_vertex_index + (uint32_t)j + 2); // RT
                    }

                    if (vertex_buffer_size == vertex_buffer.size()) {
                        draw_param_buffer.pop_back();
                        return;
                    }
                }
            }
            else {
                if (vtx[0] == 0.0f && vtx[1] == 0.0f && vtx[2] == 0.0f && vtx[3] == 0.0f) {
                    draw_param_buffer.pop_back();
                    return;
                }

                sprite_draw_vertex spr_vtx[4] = {};
                spr_vtx[0].pos = vtx[0];
                spr_vtx[0].uv[0].x = tex_param[0].texcoord.uv[0].u;
                spr_vtx[0].uv[0].y = tex_param[0].texcoord.uv[0].v;
                spr_vtx[0].color = color;

                spr_vtx[1].pos = vtx[1];
                spr_vtx[1].uv[0].x = tex_param[0].texcoord.uv[1].u;
                spr_vtx[1].uv[0].y = tex_param[0].texcoord.uv[1].v;
                spr_vtx[1].color = color;

                spr_vtx[2].pos = vtx[2];
                spr_vtx[2].uv[0].x = tex_param[0].texcoord.uv[2].u;
                spr_vtx[2].uv[0].y = tex_param[0].texcoord.uv[2].v;
                spr_vtx[2].color = color;

                spr_vtx[3].pos = vtx[3];
                spr_vtx[3].uv[0].x = tex_param[0].texcoord.uv[3].u;
                spr_vtx[3].uv[0].y = tex_param[0].texcoord.uv[3].v;
                spr_vtx[3].color = color;

                draw_param.mode = GL_TRIANGLES;
                draw_param.start = (GLuint)vertex_buffer.size();
                draw_param.end = draw_param.start + 3;
                draw_param.count = 6;
                draw_param.offset = (GLintptr)(index_buffer.size() * sizeof(uint32_t));

                uint32_t start_vertex_index = (uint32_t)vertex_buffer.size();

                vertex_buffer.reserve(4);
                vertex_buffer.push_back(spr_vtx[0]); // LB
                vertex_buffer.push_back(spr_vtx[3]); // RB
                vertex_buffer.push_back(spr_vtx[2]); // RT
                vertex_buffer.push_back(spr_vtx[1]); // LT

                index_buffer.reserve(6);
                index_buffer.push_back(start_vertex_index + 0); // LB
                index_buffer.push_back(start_vertex_index + 3); // LT
                index_buffer.push_back(start_vertex_index + 1); // RB
                index_buffer.push_back(start_vertex_index + 1); // RB
                index_buffer.push_back(start_vertex_index + 3); // LT
                index_buffer.push_back(start_vertex_index + 2); // RT
            }
            break;
        case 2: {
            if (vtx[0] == 0.0f && vtx[1] == 0.0f && vtx[2] == 0.0f && vtx[3] == 0.0f) {
                draw_param_buffer.pop_back();
                return;
            }

            draw_param.texture[0] = tex_param[0].texture ? tex_param[0].texture->tex : 0;
            draw_param.texture[1] = tex_param[1].texture ? tex_param[1].texture->tex : 0;
            draw_param.sampler = rctx->sprite_samplers[2];

            sprite_draw_vertex spr_vtx[4] = {};
            spr_vtx[0].pos = vtx[0];
            spr_vtx[0].uv[0].x = tex_param[0].texcoord.uv[0].u;
            spr_vtx[0].uv[0].y = tex_param[0].texcoord.uv[0].v;
            spr_vtx[0].uv[1].x = tex_param[1].texcoord.uv[0].u;
            spr_vtx[0].uv[1].y = tex_param[1].texcoord.uv[0].v;
            spr_vtx[0].color = color;

            spr_vtx[1].pos = vtx[1];
            spr_vtx[1].uv[0].x = tex_param[0].texcoord.uv[1].u;
            spr_vtx[1].uv[0].y = tex_param[0].texcoord.uv[1].v;
            spr_vtx[1].uv[1].x = tex_param[1].texcoord.uv[1].u;
            spr_vtx[1].uv[1].y = tex_param[1].texcoord.uv[1].v;
            spr_vtx[1].color = color;

            spr_vtx[2].pos = vtx[2];
            spr_vtx[2].uv[0].x = tex_param[0].texcoord.uv[2].u;
            spr_vtx[2].uv[0].y = tex_param[0].texcoord.uv[2].v;
            spr_vtx[2].uv[1].x = tex_param[1].texcoord.uv[2].u;
            spr_vtx[2].uv[1].y = tex_param[1].texcoord.uv[2].v;
            spr_vtx[2].color = color;

            spr_vtx[3].pos = vtx[3];
            spr_vtx[3].uv[0].x = tex_param[0].texcoord.uv[3].u;
            spr_vtx[3].uv[0].y = tex_param[0].texcoord.uv[3].v;
            spr_vtx[3].uv[1].x = tex_param[1].texcoord.uv[3].u;
            spr_vtx[3].uv[1].y = tex_param[1].texcoord.uv[3].v;
            spr_vtx[3].color = color;

            draw_param.mode = GL_TRIANGLES;
            draw_param.start = (GLuint)vertex_buffer.size();
            draw_param.end = draw_param.start + 3;
            draw_param.count = 6;
            draw_param.offset = (GLintptr)(index_buffer.size() * sizeof(uint32_t));

            uint32_t start_vertex_index = (uint32_t)vertex_buffer.size();

            vertex_buffer.reserve(4);
            vertex_buffer.push_back(spr_vtx[0]); // LB
            vertex_buffer.push_back(spr_vtx[3]); // RB
            vertex_buffer.push_back(spr_vtx[2]); // RT
            vertex_buffer.push_back(spr_vtx[1]); // LT

            index_buffer.reserve(6);
            index_buffer.push_back(start_vertex_index + 0); // LB
            index_buffer.push_back(start_vertex_index + 3); // LT
            index_buffer.push_back(start_vertex_index + 1); // RB
            index_buffer.push_back(start_vertex_index + 1); // RB
            index_buffer.push_back(start_vertex_index + 3); // LT
            index_buffer.push_back(start_vertex_index + 2); // RT
        } break;
        }

        if (draw_param_buffer.size() >= 2) {
            sprite_draw_param& draw_param_2 = draw_param_buffer.data()[draw_param_buffer.size() - 2];
            sprite_draw_param& draw_param_1 = draw_param_buffer.data()[draw_param_buffer.size() - 1];

            if (draw_param_1.mode == GL_TRIANGLES
                && draw_param_2.mode == draw_param_1.mode
                && draw_param_2.blend == draw_param_1.blend
                && draw_param_2.blend_src_rgb == draw_param_1.blend_src_rgb
                && draw_param_2.blend_src_alpha == draw_param_1.blend_src_alpha
                && draw_param_2.blend_dst_rgb == draw_param_1.blend_dst_rgb
                && draw_param_2.blend_dst_alpha == draw_param_1.blend_dst_alpha
                && !draw_param_2.copy_texture && !draw_param_1.copy_texture
                && draw_param_2.shader == draw_param_1.shader
                && draw_param_2.tex_0_type == draw_param_1.tex_0_type
                && draw_param_2.tex_1_type == draw_param_1.tex_1_type
                && draw_param_2.combiner == draw_param_1.combiner
                && draw_param_2.texture[0] == draw_param_1.texture[0]
                && draw_param_2.texture[1] == draw_param_1.texture[1]
                && draw_param_2.sampler == draw_param_1.sampler
                && draw_param_2.end + 1 == draw_param_1.start) {
                draw_param_2.end = draw_param_1.end;
                draw_param_2.count += draw_param_1.count;
                draw_param_buffer.pop_back();
            }
        }
    }

    static void draw_sprite_begin() {
        gl_state_disable_blend(true);
        gl_state_active_bind_texture_2d(0, rctx->empty_texture_2d, true);
        gl_state_bind_sampler(0, 0, true);
        gl_state_active_bind_texture_2d(1, rctx->empty_texture_2d, true);
        gl_state_bind_sampler(1, 0, true);
        gl_state_bind_sampler(7, 0, true);
        gl_state_set_blend_func_separate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO, true);
        gl_state_bind_vertex_array(0, true);
        gl_state_bind_uniform_buffer_base(0, 0, true);
    }

    static void draw_sprite_end() {
        gl_state_disable_blend();
        gl_state_active_bind_texture_2d(0, 0);
        gl_state_bind_sampler(0, 0);
        gl_state_active_bind_texture_2d(1, 0);
        gl_state_bind_sampler(1, 0);
        gl_state_active_bind_texture_2d(7, 0);
        gl_state_bind_sampler(7, 0);
        gl_state_set_blend_func_separate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
        shaders_ft.set(-1);
    }

    static void draw_sprite_scale(spr::SprArgs* args) {
        int32_t index = args->index;
        resolution_mode mode = res_window_get()->resolution_mode;
        while (args) {
            resolution_mode_scale_data data(args->resolution_mode_screen, mode);
            if (args->resolution_mode_screen != RESOLUTION_MODE_MAX
                && args->resolution_mode_screen != mode && (index <= 0 || index >= 3)) {
                float_t scale_x = data.scale.x;
                float_t scale_y = data.scale.y;
                float_t src_res_x = data.src_res.x;
                float_t src_res_y = data.src_res.y;
                float_t dst_res_x = data.dst_res.x;
                float_t dst_res_y = data.dst_res.y;
                args->trans.x = (args->trans.x - src_res_x) * scale_x + src_res_x;
                args->trans.y = (args->trans.y - src_res_y) * scale_y + src_res_y;
                args->trans.z = args->trans.z * scale_y;

                SpriteVertex* vtx = args->vertex_array;
                for (size_t i = args->num_vertex; i; i--, vtx++) {
                    vtx->pos.x = (vtx->pos.x - src_res_x) * scale_x + dst_res_x;
                    vtx->pos.y = (vtx->pos.y - src_res_y) * scale_y + dst_res_y;
                }
            }

            if (args->resolution_mode_sprite != RESOLUTION_MODE_MAX) {
                vec2 scale = resolution_mode_get_scale(mode, args->resolution_mode_sprite);
                args->scale.x = scale.x * args->scale.x;
                args->scale.y = scale.y * args->scale.y;
            }
            args = args->next;
        }
    }
}

int32_t sprite_database_get_spr_set_id_by_name(const prj::string& name) {
    int32_t(FASTCALL * sprite_database_struct__get_spr_set_id_by_name)
        (size_t _this, const prj::string & name)
        = (int32_t(FASTCALL*)(size_t _this, const prj::string & name))0x00000001401121A0;
    return sprite_database_struct__get_spr_set_id_by_name(0x000000014CC62290, name);
}

void sprite_manager_init() {
    if (!sprite_manager_render_data)
        sprite_manager_render_data = new spr::SpriteManager::RenderData;
}

void sprite_manager_draw(int32_t index, bool font, texture* tex) {
    sprite_manager->Draw(index, font, tex);
}

void sprite_manager_set_res(double_t aspect, int32_t width, int32_t height) {
    sprite_manager->aspect[0] = (float_t)aspect;
    sprite_manager->field_1018[0].second = { { 0.0f, 0.0f }, { (float_t)width, (float_t)height } };
    sprite_manager->aspect[1] = (float_t)aspect;
    sprite_manager->field_1018[1].second = { { 0.0f, 0.0f }, { (float_t)width, (float_t)height } };
}

void sprite_manager_set_view_projection(bool aet_3d) {
    view_projection_aet = aet_3d
        ? camera_data->view_projection_aet_3d
        : camera_data->view_projection_aet_2d;
}

void sprite_manager_free() {
    if (sprite_manager_render_data) {
        delete sprite_manager_render_data;
        sprite_manager_render_data = 0;
    }
}

HOOK(void, FASTCALL, sprite_manager_draw, 0x000000014063F870, int32_t index, bool font, texture* tex) {
    sprite_manager_draw(index, font, tex);
}


HOOK(void, FASTCALL, sprite_manager_set_view_projection, 0x00000001401F9590, bool aet_3d) {
    sprite_manager_set_view_projection(aet_3d);
}

void sprite_patch() {
    INSTALL_HOOK(sprite_manager_draw);
    INSTALL_HOOK(sprite_manager_set_view_projection);
}
