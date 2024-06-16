/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "effect.hpp"
#include "../KKdLib/prj/algorithm.hpp"
#include "../KKdLib/prj/vector_pair.hpp"
#include "../KKdLib/hash.hpp"
#include "auth_3d.hpp"
#include "file_handler.hpp"
#include "frame_rate_control.hpp"
#include "gl_state.hpp"
#include "gl_element_array_buffer.hpp"
#include "render_context.hpp"
#include "render_manager.hpp"
#include "shader_ft.hpp"
#include "stage_param.hpp"
#include "texture.hpp"
#include <Helpers.h>

struct fog_ring_data {
    vec3 position;
    vec3 direction;
    vec3 field_18;
    float_t size;
    float_t density;
};

static_assert(sizeof(fog_ring_data) == 0x2C, "\"point_particle_data\" struct should have a size of 0x2C");

struct point_particle_data {
    vec3 position;
    vec4 color;
    float_t size;
};

static_assert(sizeof(point_particle_data) == 0x20, "\"point_particle_data\" struct should have a size of 0x20");

struct struc_573 {
    int32_t chara_index;
    int32_t bone_index;
    vec3 position;
};

static_assert(sizeof(struc_573) == 0x14, "\"struc_573\" struct should have a size of 0x14");

struct struc_371 {
    int32_t field_0;
    vec3 position;
    float_t field_10;
    float_t field_14;
    vec3 direction;
    float_t field_24;
};

static_assert(sizeof(struc_371) == 0x28, "\"struc_371\" struct should have a size of 0x28");

struct EffectFogRing {
    bool enable;
    float_t delta_frame;
    bool field_8;
    float_t ring_size;
    vec3 wind_dir;
    int32_t tex_id;
    vec4 color;
    float_t ptcl_size;
    int32_t max_ptcls;
    int32_t num_ptcls;
    float_t density;
    float_t density_offset;
    fog_ring_data* ptcl_data;
    point_particle_data* ptcl_vtx_data;
    int32_t num_vtx;
    struc_573 field_5C[2][5];
    int32_t field_124;
    struc_371 field_128[10];
    char field_2B8;
    char field_2B9;
    bool display;
    int32_t current_stage_index;
    prj::vector<int32_t> stage_indices;
    FrameRateControl* frame_rate_control;

    void draw();
};

static_assert(sizeof(EffectFogRing) == 0x2E0, "\"EffectFogRing\" struct should have a size of 0x2E0");

struct ripple_struct {
    int32_t ripple_uniform;
    int32_t ripple_emit_uniform;
    int32_t count;
    vec3* position;
    color4u8* color;
    float_t size;
};

static_assert(sizeof(ripple_struct) == 0x28, "\"ripple_struct\" struct should have a size of 0x28");

struct struc_192 {
    int32_t index;
    vec3 trans;
};

static_assert(sizeof(struc_192) == 0x10, "\"struc_192\" struct should have a size of 0x10");

struct struc_207 {
    struc_192 field_0[18];
};

static_assert(sizeof(struc_207) == 0x120, "\"struc_207\" struct should have a size of 0x120");

struct EffectRipple {
    struct Params {
        float_t wake_attn;
        float_t speed;
        float_t field_8;
        float_t field_C;
    };

    static_assert(sizeof(EffectRipple::Params) == 0x10, "\"EffectRipple::Params\" struct should have a size of 0x10");

    struct DrawData {
        ripple_struct data;
        vec3 position[16];
        color4u8 color[16];
    };

    static_assert(sizeof(EffectRipple::DrawData) == 0x128, "\"EffectRipple::DrawData\" struct should have a size of 0x128");

    float_t delta_frame;
    bool update;
    int32_t rain_ripple_num;
    float_t rain_ripple_min_value;
    float_t rain_ripple_max_value;
    int32_t field_14;
    float_t ground_y;
    float_t emit_pos_scale;
    float_t emit_pos_ofs_x;
    float_t emit_pos_ofs_z;
    int32_t ripple_tex_id;
    bool use_float_ripplemap;
    int32_t field_30;
    float_t rob_emitter_size;
    size_t emitter_num;
    const vec3* emitter_list;
    float_t emitter_size;
    DrawData field_50;
    DrawData field_178;
    DrawData field_2A0;
    DrawData field_3C8;
    int32_t field_4F0;
    struc_207 field_4F4[6];
    RenderTexture field_BB8;
    int32_t counter;
    bool field_BEC;
    EffectRipple::Params params;
    bool stage_set;
    int32_t current_stage_index;
    prj::vector<int32_t> stage_indices;

    void draw();

    void sub_1403584A0(RenderTexture* rt);
};

static_assert(sizeof(EffectRipple) == 0xC20, "\"EffectRipple\" struct should have a size of 0xC20");

struct for_ring_vertex_data {
    vec2 position;
    vec4 color;
    float_t size;
};

struct __declspec(align(16)) leaf_particle_data {
    vec4 position;
    vec4 direction;
    vec4 normal;
    vec4 rotation;
    vec4 rotation_add;
    float_t size;
    int32_t type;
    int32_t field_58;
    int32_t field_5C;
};

static_assert(sizeof(leaf_particle_data) == 0x60, "\"particle_data\" struct should have a size of 0x60");

struct leaf_particle_vertex_data {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
};

struct __declspec(align(16)) particle_data {
    vec4 position;
    float_t size;
    float_t alpha;
    int32_t life_time;
    int32_t pad_1C;
    vec4 velocity;
    vec4 direction;
};

static_assert(sizeof(particle_data) == 0x40, "\"particle_data\" struct should have a size of 0x40");

struct __declspec(align(16)) particle_rot_data {
    vec4 position;
    vec4 direction;
    vec4 normal;
    vec4 rotation;
    vec4 rotation_add;
    vec4 color;
    bool alive;
    float_t type;
    float_t life_time;
    float_t size;
};

static_assert(sizeof(particle_rot_data) == 0x70, "\"particle_rot_data\" struct should have a size of 0x70");

struct particle_vertex_data {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texcoord;
};

struct star_catalog_milky_way {
    GLArrayBuffer vbo;
    GLElementArrayBuffer ebo;
    GLuint vao;
    uint16_t restart_index;
    int32_t idx_count;
    int32_t longitude_degs_10;
    int32_t latitude_degs_10;
    float_t longitude_offset_degs_10;
    float_t latitude_offset_degs_10;
    float_t latitude;
    float_t longitude;
    float_t uv_rec_scale_u;
    float_t uv_rec_scale_v;

    void create_buffers(int32_t subdivs, float_t uv_rec_scale_u, float_t uv_rec_scale_v,
        int32_t longitude_degs_10, int32_t latitude_degs_10,
        float_t longitude_offset_degs_10, float_t latitude_offset_degs_10);
    void create_default_sphere();
    void delete_buffers();
    void draw(const mat4& vp, const mat4& model, texture* tex, GLUniformBuffer& scene_ubo);
};

static_assert(sizeof(star_catalog_milky_way) == 0x34, "\"star_catalog_milky_way\" struct should have a size of 0x34");

struct star_catalog {
    GLuint vbo;
    GLuint vao;
    bool random;
    bool enable;
    stage_param_data_star* stage_param_data_ptr;
    stage_param_data_star stage_param_data;
    star_catalog_milky_way milky_way;
    int32_t stars_count;
    p_file_handler file_handler;
    uint32_t star_tex;
    uint32_t star_b_tex;
    uint32_t milky_way_tex_id;

    void draw();
    void free();
    bool init();
};

static_assert(sizeof(star_catalog) == 0xB8, "\"star_catalog\" struct should have a size of 0xB8");

struct splash_particle_data {
    vec3 position;
    vec3 direction;
    float_t size;
    float_t life_time;
    float_t field_20;
    int32_t index;
    uint32_t flags;
    float_t alpha;
};
static_assert(sizeof(splash_particle_data) == 0x30, "\"splash_particle_data\" struct should have a size of 0x30");

struct splash_particle {
    int32_t count;
    int32_t alive;
    prj::vector<splash_particle_data> data;
    int32_t dead;
    prj::vector<int32_t> available;
};

static_assert(sizeof(splash_particle) == 0x40, "\"splash_particle\" struct should have a size of 0x40");

struct water_particle {
    splash_particle* splash;
    vec4 color;
    float_t particle_size;
    int32_t splash_count;
    prj::vector<point_particle_data> ptcl_data;
    int32_t count;
    int32_t splash_tex_id;
    bool blink;
    prj::vector<vec3> position_data;
    prj::vector<color4u8> color_data;
    ripple_struct ripple;
    float_t ripple_emission;

    void draw(mat4* mat);
};

static_assert(sizeof(water_particle) == 0xA8, "\"water_particle\" struct should have a size of 0xA8");

struct water_particle_vertex_data {
    vec3 position;
    float_t size;
    vec4 color;

    inline water_particle_vertex_data() : size() {

    }
};

struct leaf_particle_scene_shader_data {
    vec4 g_transform[4];
    vec4 g_view_pos;
    vec4 g_color;
    vec4 g_light_env_stage_diffuse;
    vec4 g_light_env_stage_specular;
    vec4 g_lit_dir;
    vec4 g_lit_luce;
};

struct particle_scene_shader_data {
    vec4 g_transform[4];
    vec4 g_view_pos;
    vec4 g_light_env_chara_diffuse;
    vec4 g_light_env_chara_specular;
};

struct rain_particle_scene_shader_data {
    vec4 g_view[4];
    vec4 g_proj[4];
    vec4 g_range_scale;
    vec4 g_range_offset;
};

struct rain_particle_batch_shader_data {
    vec4 g_pos_offset;
    vec4 g_tangent;
    vec4 g_color;
};

struct ripple_batch_shader_data {
    vec4 g_params;
};

struct ripple_scene_shader_data {
    vec4 g_transform;
    vec4 g_texcoord;
};

struct ripple_emit_scene_shader_data {
    vec4 g_size_in_projection;
    vec4 g_transform;
    vec4 g_framebuffer_size;
};

struct snow_particle_vertex_data {
    vec3 position;
    float_t size;
    float_t alpha;
};

struct snow_particle_gpu_vertex_data {
    vec3 position;
    float_t size;
};

struct snow_particle_scene_shader_data {
    vec4 g_transform[4];
    vec4 g_view_world_row2;
    vec4 g_size_in_projection;
    vec4 g_state_point_attenuation;
    vec4 g_range_scale;
    vec4 g_range_offset;
    vec4 g_framebuffer_size;
    vec4 g_near_far;
};

struct snow_particle_batch_shader_data {
    vec4 g_pos_offset;
    vec4 g_color;
    vec4i start_vertex_location;
};

struct star_catalog_scene_shader_data {
    vec4 g_transform[4];
};

struct star_catalog_batch_shader_data {
    vec4 g_size_in_projection;
    vec4 g_modifiers;
    vec4 g_thresholds;
};

struct water_particle_scene_shader_data {
    vec4 g_transform[4];
    vec4 g_view_world_row2;
    vec4 g_size_in_projection;
    vec4 g_state_point_attenuation;
};

static prj::vector_pair<EffectFogRing*, GLShaderStorageBuffer> fog_ring_ssbo;

static GLuint leaf_ptcl_vao;
static GLArrayBuffer leaf_ptcl_vbo;
static GLElementArrayBuffer leaf_ptcl_ebo;
static GLUniformBuffer leaf_particle_scene_ubo;
static const size_t leaf_ptcl_count = 0x800;

static GLuint ptcl_vao;
static GLArrayBuffer ptcl_vbo;
static GLUniformBuffer particle_scene_ubo;
static const size_t ptcl_count = 0x400;

static GLShaderStorageBuffer rain_ssbo;
static GLUniformBuffer rain_particle_scene_ubo;
static GLUniformBuffer rain_particle_batch_ubo;
static const size_t rain_ptcl_count = 0x8000;

static GLUniformBuffer ripple_batch_ubo;
static GLUniformBuffer ripple_scene_ubo;

static GLShaderStorageBuffer ripple_emit_ssbo;
static GLUniformBuffer ripple_emit_scene_ubo;
static const size_t ripple_emit_count = 5000;

static GLShaderStorageBuffer snow_ssbo;
static GLShaderStorageBuffer snow_gpu_ssbo;
static GLShaderStorageBuffer snow_fallen_ssbo;
static GLUniformBuffer snow_particle_scene_ubo;
static GLUniformBuffer snow_particle_batch_ubo;
static const size_t snow_ptcl_count = 0x8000;
static const size_t snow_ptcl_fallen_count = 0x2000;

static GLShaderStorageBuffer stars_ssbo;
static GLUniformBuffer stars_scene_ubo;
static GLUniformBuffer stars_batch_ubo;
static int32_t star_count;
static int32_t star_b_count;
static GLuint star_sampler;

static prj::vector_pair<water_particle*, GLShaderStorageBuffer> water_particle_ssbo;
static GLUniformBuffer water_particle_scene_ubo;

static float_t& snow_particle_delta_frame = *(float_t*)0x0000000140C9A4E0;

static stage_param_data_leaf*& stage_param_data_leaf_current = *(stage_param_data_leaf**)0x0000000141194D40;
static bool& leaf_particle_enable = *(bool*)0x0000000141194D48;
static bool& stage_param_data_leaf_set = *(bool*)0x0000000141194D49;
static float_t& leaf_particle_emit_timer = *(float_t*)0x0000000141194D4C;
static float_t& leaf_particle_emit_interval = *(float_t*)0x0000000141194D50;
static int32_t& leaf_particle_num_ptcls = *(int32_t*)0x0000000141194D54;
static leaf_particle_data*& leaf_ptcl_data = *(leaf_particle_data**)0x0000000141194D58;
static leaf_particle_vertex_data*& leaf_ptcl_vertex_data = *(leaf_particle_vertex_data**)0x0000000141194D60;

static bool& particle_enable = *(bool*)0x0000000141194E30;
static particle_rot_data*& ptcl_data = *(particle_rot_data**)0x0000000141194E38;
static int32_t& particle_index = *(int32_t*)0x0000000141194E48;
static int32_t& particle_count = *(int32_t*)0x0000000141194E4C;
static vec3& particle_wind = *(vec3*)0x0000000141194E50;

static particle_data* rain_ptcl_data = (particle_data*)0x0000000141194E70;
static stage_param_data_rain*& stage_param_data_rain_current = *(stage_param_data_rain**)0x0000000141195070;
static bool& rain_particle_enable = *(bool*)0x0000000141195078;
static bool& stage_param_data_rain_set = *(bool*)0x0000000141195079;

static float_t& snow_particle_size_mid = *(float_t*)0x0000000141195CF0;
static float_t& snow_particle_size_min = *(float_t*)0x0000000141195CF4;
static float_t& snow_particle_size_max = *(float_t*)0x0000000141195CF8;
static bool& snow_particle_enable = *(bool*)0x0000000141195CFC;
static bool& stage_param_data_snow_set = *(bool*)0x0000000141195CFD;
static int32_t& snow_particle_fallen_count = *(int32_t*)0x0000000141195D04;
static particle_data*& snow_ptcl_data = *(particle_data**)0x0000000141195D08;
static particle_data* snow_ptcl_gpu = (particle_data*)0x0000000141195D10;
static particle_data*& snow_ptcl_fallen_data = *(particle_data**)0x0000000141195E10;
static int32_t snow_particle_fallen_index = *(int32_t*)0x0000000141195E18;
static stage_param_data_snow*& stage_param_data_snow_current = *(stage_param_data_snow**)0x0000000141195E20;

static star_catalog& star_catalog_data = *(star_catalog*)0x0000000141195E90;

static float_t(FASTCALL* rand_state_array_get_float)(int32_t id)
    = (float_t(FASTCALL*)(int32_t id))0x00000001404FFE60;
static int32_t(FASTCALL* rand_state_array_get_int)(uint32_t min, uint32_t max, int32_t id)
    = (int32_t(FASTCALL*)(uint32_t min, uint32_t max, int32_t id))0x00000001404FFD80;

static EffectRipple* (FASTCALL* effect_ripple_data_get)() = (EffectRipple * (FASTCALL*)())0x00000001403595B0;

static void (FASTCALL* snow_particle_data_init)() = (void (FASTCALL*)())0x000000014035C930;
static void (FASTCALL* snow_particle_data_free)() = (void (FASTCALL*)())0x000000014035DB90;

extern render_context* rctx;

static void draw_fog_particle(EffectFogRing* data, mat4* mat);

static void draw_ripple_particles(ripple_struct* data, mat4* mat);

static void draw_water_particle(water_particle* data, mat4* mat);

static int32_t leaf_particle_disp();

static int32_t particle_disp(particle_vertex_data* vtx_data, particle_rot_data* data, int32_t count);

static void rain_particle_free();

static void ripple_emit_init();
static void ripple_emit_free();

static void snow_particle_data_emit_fallen(particle_data* data);
static void snow_particle_data_kill_fallen(particle_data* data, bool kill);
static void snow_particle_data_reset(particle_data* data);
static particle_data* snow_particle_emit_fallen();
static vec3 snow_particle_get_random_velocity();
static void snow_particle_free();

void leaf_particle_draw() {
    if (!stage_param_data_leaf_current || !leaf_ptcl_data
        || !leaf_particle_enable || !stage_param_data_leaf_set)
        return;

    texture* tex = texture_manager_get_texture(stage_param_data_leaf_current->tex_id);
    if (!tex)
        return;

    int32_t count = leaf_particle_disp();
    if (!count)
        return;

    const light_data& light_stage = light_set_data[LIGHT_SET_MAIN].lights[LIGHT_STAGE];

    leaf_particle_scene_shader_data shader_data = {};
    mat4 temp;
    mat4_transpose(&rctx->vp_mat, &temp);
    shader_data.g_transform[0] = temp.row0;
    shader_data.g_transform[1] = temp.row1;
    shader_data.g_transform[2] = temp.row2;
    shader_data.g_transform[3] = temp.row3;
    camera_data->get_view_point(shader_data.g_view_pos);
    shader_data.g_color = stage_param_data_leaf_current->color;
    light_stage.get_diffuse(shader_data.g_light_env_stage_diffuse);
    light_stage.get_specular(shader_data.g_light_env_stage_specular);
    shader_data.g_lit_dir = rctx->obj_scene.g_light_chara_dir;
    shader_data.g_lit_luce = rctx->obj_scene.g_light_chara_luce;
    leaf_particle_scene_ubo.WriteMemory(shader_data);

    gl_state_active_bind_texture_2d(0, tex->glid);
    shaders_ft.set(SHADER_FT_LEAF_PT);
    leaf_particle_scene_ubo.Bind(0);
    gl_state_bind_vertex_array(leaf_ptcl_vao);
    shaders_ft.draw_elements(GL_TRIANGLES, count / 4 * 6, GL_UNSIGNED_INT, 0);
    gl_state_bind_vertex_array(0);
}

void rain_particle_draw() {
    if (!stage_param_data_rain_current || !rain_particle_enable || !stage_param_data_rain_set)
        return;

    texture* tex = texture_manager_get_texture(stage_param_data_rain_current->tex_id);
    if (!tex)
        return;

    stage_param_data_rain* rain = stage_param_data_rain_current;
    vec3 range = rain->range;
    vec3 range_scale = rain->range;
    vec3 range_offset = rain->offset;
    range_offset.x -= range.x * 0.5f;
    range_offset.z -= range.z * 0.5f;

    rain_particle_scene_shader_data scene_shader_data = {};
    mat4 temp;
    mat4_transpose(&rctx->view_mat, &temp);
    scene_shader_data.g_view[0] = temp.row0;
    scene_shader_data.g_view[1] = temp.row1;
    scene_shader_data.g_view[2] = temp.row2;
    scene_shader_data.g_view[3] = temp.row3;
    mat4_transpose(&rctx->proj_mat, &temp);
    scene_shader_data.g_proj[0] = temp.row0;
    scene_shader_data.g_proj[1] = temp.row1;
    scene_shader_data.g_proj[2] = temp.row2;
    scene_shader_data.g_proj[3] = temp.row3;
    scene_shader_data.g_range_scale = { range_scale.x, range_scale.y, range_scale.z, 0.0f };
    scene_shader_data.g_range_offset = { range_offset.x, range_offset.y, range_offset.z, 0.0f };
    rain_particle_scene_ubo.WriteMemory(scene_shader_data);

    gl_state_enable_blend();
    gl_state_set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl_state_enable_depth_test();
    gl_state_set_depth_mask(GL_FALSE);
    gl_state_active_bind_texture_2d(0, tex->glid);
    shaders_ft.set(SHADER_FT_RAIN);

    float_t tangent_sign = -rain->psize.x;
    float_t tangent_size = -rain->psize.y * (float_t)(1.0 / 30.0);

    int32_t first = 0;
    int32_t count = min_def(rain->num_rain, (int32_t)rain_ptcl_count) / 2 / 4 * 6;

    vec4 color = rain->color;
    float_t color_a = color.w;
    rain_particle_scene_ubo.Bind(0);
    rain_particle_batch_ubo.Bind(1);
    rain_ssbo.Bind(0);
    gl_state_bind_vertex_array(rctx->common_vao);
    for (int32_t i = 0; i < 8; i++, first += count) {
        particle_data& data = rain_ptcl_data[i];
        vec3 pos_offset = *(vec3*)&data.position / range;
        vec3 tangent = *(vec3*)&data.velocity * tangent_size;
        color.w = color_a * data.alpha;

        rain_particle_batch_shader_data batch_shader_data = {};
        batch_shader_data.g_pos_offset = { pos_offset.x, pos_offset.y, pos_offset.z, 0.075f };
        batch_shader_data.g_tangent = { tangent.x, tangent.y, tangent.z, tangent_sign };
        batch_shader_data.g_color = color;
        rain_particle_batch_ubo.WriteMemory(batch_shader_data);
        shaders_ft.draw_arrays(GL_TRIANGLES, first, count);
    }
    gl_state_bind_vertex_array(0);
    gl_state_active_bind_texture_2d(0, 0);
    gl_state_set_depth_mask(GL_TRUE);
    gl_state_disable_blend();
}

void particle_draw() {
    if (!ptcl_data)
        return;

    particle_vertex_data* vtx_data = (particle_vertex_data*)ptcl_vbo.MapMemory();
    if (!vtx_data)
        return;

    int32_t count = particle_disp(vtx_data, ptcl_data, ptcl_count);

    ptcl_vbo.UnmapMemory();

    if (!count)
        return;

    const light_data& light_chara = light_set_data[LIGHT_SET_MAIN].lights[LIGHT_CHARA];

    particle_scene_shader_data shader_data = {};
    mat4 temp;
    mat4_transpose(&rctx->vp_mat, &temp);
    shader_data.g_transform[0] = temp.row0;
    shader_data.g_transform[1] = temp.row1;
    shader_data.g_transform[2] = temp.row2;
    shader_data.g_transform[3] = temp.row3;
    camera_data->get_view_point(shader_data.g_view_pos);
    light_chara.get_diffuse(shader_data.g_light_env_chara_diffuse);
    light_chara.get_specular(shader_data.g_light_env_chara_specular);
    particle_scene_ubo.WriteMemory(shader_data);

    shaders_ft.set(SHADER_FT_PARTICL);
    particle_scene_ubo.Bind(0);
    gl_state_bind_vertex_array(ptcl_vao);
    shaders_ft.draw_arrays(GL_TRIANGLES, 0, count);
    gl_state_bind_vertex_array(0);
}

void snow_particle_draw() {
    if (!stage_param_data_snow_current || !snow_particle_enable || !stage_param_data_snow_set)
        return;

    texture* tex = texture_manager_get_texture(stage_param_data_snow_current->tex_id);
    if (!tex)
        return;

    stage_param_data_snow* snow = stage_param_data_snow_current;
    draw_pass_set_camera();

    gl_state_enable_blend();
    gl_state_set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl_state_enable_depth_test();
    gl_state_set_depth_mask(GL_FALSE);

    float_t point_attenuation = powf(tanf((float_t)camera_data->fov
        * 0.5f * DEG_TO_RAD_FLOAT) * 3.4f, 2.0f) * 0.1f;

    snow_particle_scene_shader_data snow_scene = {};
    mat4 temp;
    mat4_transpose(&rctx->vp_mat, &temp);
    snow_scene.g_transform[0] = temp.row0;
    snow_scene.g_transform[1] = temp.row1;
    snow_scene.g_transform[2] = temp.row2;
    snow_scene.g_transform[3] = temp.row3;

    mat4_transpose(&rctx->view_mat, &temp);
    snow_scene.g_view_world_row2 = temp.row2;
    snow_scene.g_size_in_projection.x = 1.0f / (float_t)render_get()->render_width[0];
    snow_scene.g_size_in_projection.y = 1.0f / (float_t)render_get()->render_height[0];
    snow_scene.g_size_in_projection.z = snow_particle_size_min;
    snow_scene.g_size_in_projection.w = snow_particle_size_max;
    snow_scene.g_state_point_attenuation = { 0.0f, 0.0f, point_attenuation, 0.0f };
    snow_scene.g_range_scale.x = snow->range_gpu.x;
    snow_scene.g_range_scale.y = snow->range_gpu.y;
    snow_scene.g_range_scale.z = snow->range_gpu.z;
    snow_scene.g_range_offset.x = snow->offset_gpu.x - snow->range_gpu.x * 0.5f;
    snow_scene.g_range_offset.y = snow->offset_gpu.y;
    snow_scene.g_range_offset.z = snow->offset_gpu.z - snow->range_gpu.z * 0.5f;
    snow_particle_scene_ubo.WriteMemory(snow_scene);

    snow_particle_batch_shader_data snow_batch = {};
    snow_batch.g_color = snow->color;
    snow_batch.start_vertex_location.x = 0;
    snow_particle_batch_ubo.WriteMemory(snow_batch);

    gl_state_active_bind_texture_2d(0, tex->glid);
    gl_state_active_bind_texture_2d(1, render_get()->rend_texture[0].GetDepthTex());
    gl_state_bind_vertex_array(rctx->common_vao);

    uniform->arr[U_SNOW_PARTICLE] = 0;
    shaders_ft.set(SHADER_FT_SNOW_PT);
    snow_particle_scene_ubo.Bind(0);
    snow_particle_batch_ubo.Bind(1);

    snow_ssbo.Bind(0);
    shaders_ft.draw_arrays(GL_TRIANGLES, 0, snow->num_snow * 6);

    snow_fallen_ssbo.Bind(0);
    shaders_ft.draw_arrays(GL_TRIANGLES, 0, (GLsizei)(snow_ptcl_fallen_count * 6));

    uniform->arr[U_SNOW_PARTICLE] = 1;
    shaders_ft.set(SHADER_FT_SNOW_PT);
    snow_particle_scene_ubo.Bind(0);
    snow_particle_batch_ubo.Bind(1);

    point_attenuation = powf(tanf((float_t)camera_data->fov
        * 0.5f * DEG_TO_RAD_FLOAT) * 3.4f, 2.0f) * 0.06f;

    snow_scene.g_state_point_attenuation = { 0.0f, 0.0f, point_attenuation, 0.0f };
    snow_particle_scene_ubo.WriteMemory(snow_scene);

    snow_gpu_ssbo.Bind(0);

    int32_t count = snow->num_snow_gpu / 4;
    count = min_def(count, (int32_t)snow_ptcl_count / 4) * 6;

    int32_t first = 0;
    for (int32_t i = 0; i < 4; i++) {
        particle_data& j = snow_ptcl_gpu[i];
        vec3 pos_offset = *(vec3*)&j.position / snow->range_gpu;
        vec4 color = snow->color;
        color.w *= j.alpha;

        snow_batch = {};
        snow_batch.g_pos_offset = { pos_offset.x, pos_offset.y, pos_offset.z, 0.0f };
        snow_batch.g_color = color;
        snow_batch.start_vertex_location.x = first;
        snow_particle_batch_ubo.WriteMemory(snow_batch);

        shaders_ft.draw_arrays(GL_TRIANGLES, 0, count);
        first += count;
    }

    gl_state_active_bind_texture_2d(1, 0);
    gl_state_active_bind_texture_2d(0, 0);

    gl_state_disable_depth_test();
    gl_state_set_depth_mask(GL_TRUE);
    gl_state_disable_blend();
}

void star_catalog_draw() {
    star_catalog_data.draw();
}

HOOK(void, FASTCALL, EffectFogRing__dest, 0x0000000140348090, EffectFogRing* fog_ring) {
    auto elem = fog_ring_ssbo.find(fog_ring);
    if (elem != fog_ring_ssbo.end()) {
        elem->second.Destroy();
        fog_ring_ssbo.erase(elem);
        fog_ring_ssbo.sort();
    }

    originalEffectFogRing__dest(fog_ring);
}

HOOK(void, FASTCALL, EffectFogRing__disp, 0x0000000140348120, EffectFogRing* fog_ring) {
    if (fog_ring->enable && fog_ring->display)
        disp_manager->entry_obj_user(&mat4_identity,
            (mdl::UserArgsFunc)draw_fog_particle, fog_ring, mdl::OBJ_TYPE_USER);
}

HOOK(void, FASTCALL, EffectFogRing__set_stage_indices, 0x0000000140348790,
    EffectFogRing* fog_ring, prj::vector<int32_t>* stage_indices) {
    originalEffectFogRing__set_stage_indices(fog_ring, stage_indices);

    auto elem = fog_ring_ssbo.find(fog_ring);
    if (elem == fog_ring_ssbo.end()) {
        fog_ring_ssbo.push_back({ fog_ring, {} });
        fog_ring_ssbo.sort();
        elem = fog_ring_ssbo.find(fog_ring);
    }
    else
        elem->second.Destroy();
    elem->second.Create(sizeof(for_ring_vertex_data) * fog_ring->max_ptcls);
}

HOOK(void, FASTCALL, EffectFogRing__calc_vert, 0x000000001403495B0, EffectFogRing* fog_ring) {
    auto elem = fog_ring_ssbo.find(fog_ring);
    if (elem == fog_ring_ssbo.end())
        return;

    GLShaderStorageBuffer& ssbo = elem->second;

    float_t density = fog_ring->density;
    fog_ring_data* ptcl_data = fog_ring->ptcl_data;

    for_ring_vertex_data* ptcl_vtx_data = (for_ring_vertex_data*)ssbo.MapMemory();
    if (!ptcl_vtx_data) {
        fog_ring->num_vtx = 0;
        return;
    }

    vec4 color = fog_ring->color;
    for (int32_t i = fog_ring->num_ptcls; i > 0; i--, ptcl_data++, ptcl_vtx_data++) {
        float_t size = ptcl_data->size * (float_t)(1.0 / 256.0);
        color.w = ptcl_data->density * density;

        vec2 position;
        position.x = ptcl_data->position.x * (float_t)(1.0 / 8.0);
        position.y = ptcl_data->position.z * (float_t)(1.0 / 8.0);
        ptcl_vtx_data->position = position;
        ptcl_vtx_data->color = color;
        ptcl_vtx_data->size = size;
    }

    ssbo.UnmapMemory();

    fog_ring->num_vtx = (int32_t)(fog_ring->num_ptcls * 6LL);
}

HOOK(void, FASTCALL, EffectFogRing__draw_static, 0x0000000140349940, void* data) {
    ((EffectFogRing*)data)->draw();
}

HOOK(void, FASTCALL, leaf_particle_free, 0x000000014034BB30) {
    if (leaf_ptcl_data) {
        free(leaf_ptcl_data);
        leaf_ptcl_data = 0;
    }

    if (leaf_ptcl_vao) {
        glDeleteVertexArrays(1, &leaf_ptcl_vao);
        leaf_ptcl_vao = 0;
    }

    leaf_ptcl_vbo.Destroy();
    leaf_ptcl_ebo.Destroy();

    leaf_particle_scene_ubo.Destroy();
}

HOOK(void, FASTCALL, leaf_particle_init, 0x000000014034C530, bool change_stage) {
    leaf_particle_emit_timer = 0.0f;
    leaf_particle_emit_interval = stage_param_data_leaf_current->emit_interval;
    if (change_stage)
        return;

    implOfleaf_particle_free();

    const size_t leaf_ptcl_vtx_count = leaf_ptcl_count * 0x08;

    leaf_ptcl_data = force_malloc<leaf_particle_data>(leaf_ptcl_count);

    leaf_particle_num_ptcls = stage_param_data_leaf_current->num_initial_ptcls;

    static void (FASTCALL * leaf_particle_data__init)(leaf_particle_data * leaf_ptcl_data)
        = (void (FASTCALL*)(leaf_particle_data * leaf_ptcl_data))0x000000014034BDD0;

    leaf_particle_data* data = leaf_ptcl_data;
    int32_t i = 0;
    for (; i < leaf_particle_num_ptcls; i++, data++)
        leaf_particle_data__init(data);

    for (; i < leaf_ptcl_count; i++, data++)
        data->type = 0;

    if (!leaf_ptcl_vao)
        glGenVertexArrays(1, &leaf_ptcl_vao);

    static const GLsizei buffer_size = sizeof(leaf_particle_vertex_data);

    leaf_ptcl_vbo.Create(buffer_size * leaf_ptcl_vtx_count);
    leaf_ptcl_vbo.Bind();

    gl_state_bind_vertex_array(leaf_ptcl_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(leaf_particle_vertex_data, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(leaf_particle_vertex_data, texcoord));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(leaf_particle_vertex_data, normal));

    size_t ebo_count = leaf_ptcl_vtx_count / 4 * 6;
    uint32_t* ebo_data = force_malloc<uint32_t>(ebo_count);
    for (size_t i = 0, j = 0; i < ebo_count; i += 6, j += 4) {
        ebo_data[i + 0] = (uint32_t)(j + 0);
        ebo_data[i + 1] = (uint32_t)(j + 1);
        ebo_data[i + 2] = (uint32_t)(j + 2);
        ebo_data[i + 3] = (uint32_t)(j + 0);
        ebo_data[i + 4] = (uint32_t)(j + 2);
        ebo_data[i + 5] = (uint32_t)(j + 3);
    }

    leaf_ptcl_ebo.Create(sizeof(uint32_t) * ebo_count, ebo_data);
    leaf_ptcl_ebo.Bind();
    free_def(ebo_data);

    gl_state_bind_vertex_array(0);
    gl_state_bind_array_buffer(0);
    gl_state_bind_element_array_buffer(0);

    leaf_particle_scene_ubo.Create(sizeof(leaf_particle_scene_shader_data));
}

HOOK(bool, FASTCALL, TaskEffectRain__dest, 0x0000000140353DA0, size_t task_eff_rain) {
    if (stage_param_data_rain_current) {
        implOfleaf_particle_free();
        stage_param_data_rain_current = 0;
        void (FASTCALL * stage_param_data_rain_storage_clear)()
            = (void (FASTCALL*)())0x0000000140353CD0;
        stage_param_data_rain_storage_clear();
        stage_param_data_rain_set = 0;
        *(int32_t*)(task_eff_rain + 0x70) = -1;
        *(size_t*)(task_eff_rain + 0x80) = *(size_t*)(task_eff_rain + 0x78);
    }
    return true;
}

HOOK(void, FASTCALL, rain_particle_init, 0x00000001403546A0, bool change_stage) {
    vec3 velocity = stage_param_data_rain_current->velocity;
    vec3 vel_range = stage_param_data_rain_current->vel_range;

    for (int32_t i = 0; i < 8; i++) {
        particle_data& data = rain_ptcl_data[i];
        data.position.x = 0.0f;
        data.position.y = ((float_t)i * -12.5f) - 5.0f;
        data.position.z = 0.0f;
        data.size = 1.0f;
        data.alpha = 1.0f;
        data.life_time = 1;
        vec3 _velocity;
        _velocity.x = rand_state_array_get_float(4);
        _velocity.y = rand_state_array_get_float(4);
        _velocity.z = rand_state_array_get_float(4);
        *(vec3*)&data.velocity = (_velocity - 0.5f) * vel_range + velocity;
    }

    if (change_stage)
        return;

    rain_particle_free();

    vec3* vtx_data = force_malloc<vec3>(rain_ptcl_count);
    for (int32_t i = 0; i < rain_ptcl_count; i++) {
        vec3 position;
        position.x = rand_state_array_get_float(4);
        position.y = rand_state_array_get_float(4);
        position.z = rand_state_array_get_float(4);
        *vtx_data++ = position;
    }

    vtx_data -= rain_ptcl_count;

    rain_ssbo.Create(sizeof(vec3) * rain_ptcl_count, vtx_data);

    free_def(vtx_data);

    rain_particle_scene_ubo.Create(sizeof(rain_particle_scene_shader_data));
    rain_particle_batch_ubo.Create(sizeof(rain_particle_batch_shader_data));
}

HOOK(void, FASTCALL, particle_free, 0x0000000140351600) {
    if (ptcl_data)
        delete[] ptcl_data;

    ptcl_data = 0;
    particle_index = 0;
    particle_count = 0;

    if (ptcl_vao) {
        glDeleteVertexArrays(1, &ptcl_vao);
        ptcl_vao = 0;
    }

    ptcl_vbo.Destroy();

    particle_scene_ubo.Destroy();
}

HOOK(void, FASTCALL, particle_init, 0x0000000140351C50, vec3* offset) {
    implOfparticle_free();

    const size_t ptcl_vtx_count = ptcl_count * 0x06;

    ptcl_data = force_malloc<particle_rot_data>(ptcl_count);

    particle_rot_data* data = ptcl_data;
    for (size_t i = 0; i < ptcl_count; i++, data++)
        data->alive = false;

    particle_index = 0;
    particle_count = 0;

    if (offset)
        particle_wind = *offset;
    else
        particle_wind = 0.0f;

    if (!ptcl_vao)
        glGenVertexArrays(1, &ptcl_vao);

    static const GLsizei buffer_size = sizeof(particle_vertex_data);

    ptcl_vbo.Create(buffer_size * ptcl_vtx_count);
    ptcl_vbo.Bind();

    gl_state_bind_vertex_array(ptcl_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(particle_vertex_data, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(particle_vertex_data, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(particle_vertex_data, texcoord));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(particle_vertex_data, normal));

    gl_state_bind_vertex_array(0);
    gl_state_bind_array_buffer(0);

    particle_scene_ubo.Create(sizeof(particle_scene_shader_data));
}

HOOK(bool, FASTCALL, TaskEffectRipple__dest, 0x0000000140358600, size_t task_effect_ripple) {
    bool ret = originalTaskEffectRipple__dest(task_effect_ripple);
    ripple_emit_free();
    return ret;
}

HOOK(bool, FASTCALL, TaskEffectRipple__init, 0x00000001403596B0, size_t task_effect_ripple) {
    ripple_emit_init();
    return originalTaskEffectRipple__init(task_effect_ripple);
}

HOOK(void, FASTCALL, EffectRipple__draw_static, 0x000000014035AA50, void* data) {
    ((EffectRipple*)data)->draw();
}

HOOK(void, FASTCALL, EffectRipple__disp_particles, 0x000000014035AD80,
    EffectRipple* effect_ripple, ripple_struct* data) {
    if (data->count > 0)
        disp_manager->entry_obj_user(&mat4_identity,
            (mdl::UserArgsFunc)draw_ripple_particles, data, mdl::OBJ_TYPE_USER);
}

HOOK(bool, FASTCALL, TaskEffectSnow__dest, 0x000000014035CEF0, size_t task_eff_snow) {
    snow_particle_data_free();
    snow_particle_free();

    stage_param_data_snow_current = 0;
    void (FASTCALL * stage_param_data_snow_storage_clear)()
        = (void (FASTCALL*)())0x000000014035CE20;
    stage_param_data_snow_storage_clear();
    stage_param_data_snow_set = 0;
    *(int32_t*)(task_eff_snow + 0x70) = -1;
    *(size_t*)(task_eff_snow + 0x80) = *(size_t*)(task_eff_snow + 0x78);
    return true;
}

HOOK(void, FASTCALL, snow_particle_ctrl, 0x000000014035CA20) {
    if (!stage_param_data_snow_current)
        return;

    stage_param_data_snow* snow = stage_param_data_snow_current;
    float_t colli_ground_y = snow->colli_ground.y + 0.03f;
    float_t offset_y = snow->offset.y;

    float_t delta_time = (float_t)(snow_particle_delta_frame * (1.0 / 60.0));

    vec4 _colli_ground_xz = {
        -snow->colli_ground.min_x,
        snow->colli_ground.max_x,
        -snow->colli_ground.min_z,
        snow->colli_ground.max_z
    };

    __m128 colli_ground_xz = vec4::load_xmm(_colli_ground_xz);

    if (snow->num_snow > 0) {
        particle_data* snow_ptcl = snow_ptcl_data;
        for (int32_t i = ((snow->num_snow - 1) >> 1) + 1; i; i--, snow_ptcl += 2) {
            vec3 direction;
            if (--snow_ptcl[0].life_time > 0)
                direction = *(vec3*)&snow_ptcl[0].direction;
            else {
                snow_ptcl[0].life_time = rand_state_array_get_int(15, 35, 4);
                direction = snow_particle_get_random_velocity();
                *(vec3*)&snow_ptcl[0].direction = direction;
            }

            vec3 velocity = (direction * delta_time + *(vec3*)&snow_ptcl[0].velocity) * 0.98f;
            vec3 position = velocity * delta_time + *(vec3*)&snow_ptcl[0].position;
            *(vec3*)&snow_ptcl[0].velocity = velocity;
            *(vec3*)&snow_ptcl[0].position = position;

            if (position.y < offset_y)
                snow_particle_data_reset(snow_ptcl);
            else if (colli_ground_y > position.y) {
                __m128 pos = vec3::load_xmm(position);
                if (!_mm_movemask_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(pos, pos, 0xA0),
                    vec4::load_xmm({ 1.0f, -1.0f, 1.0f, -1.0f })), colli_ground_xz))) {
                    snow_ptcl[0].position.y = colli_ground_y;
                    snow_particle_data_emit_fallen(snow_ptcl);
                    snow_particle_data_reset(snow_ptcl);
                }
            }

            snow_ptcl[1].position = snow_ptcl[0].position + snow_ptcl[1].velocity;
        }
    }

    for (int32_t i = 0; i < 4; i++) {
        particle_data& j = snow_ptcl_gpu[i];

        vec3 direction;
        if (--j.life_time > 0)
            direction = *(vec3*)&j.direction;
        else {
            j.life_time = rand_state_array_get_int(0xFu, 0x23u, 4);
            direction = snow_particle_get_random_velocity();
            *(vec3*)&j.direction = direction;
        }

        vec3 velocity = (direction * delta_time + *(vec3*)&j.velocity) * 0.98f;
        vec3 position = velocity * delta_time + *(vec3*)&j.position;
        *(vec3*)&j.velocity = velocity;
        j.alpha = 1.0f;
        *(vec3*)&j.position = position;
        if (position.y > -1.0f)
            j.alpha = max_def(-position.y, 0.0f);
        else if (position.y < -99.0f) {
            j.alpha = max_def(position.y + 100.0f, 0.0f);
            if (position.y < -100.0f)
                j.position = 0.0f;
        }
    }

    particle_data* snow_ptcl_fallen = snow_ptcl_fallen_data;
    for (size_t i = snow->num_snow; i; i--, snow_ptcl_fallen++)
        if (snow_ptcl_fallen->size != 0.0f) {
            snow_ptcl_fallen->alpha *= 0.95f;
            if (snow_ptcl_fallen->alpha < 0.02f)
                snow_particle_data_kill_fallen(snow_ptcl_fallen, true);
        }

    snow_particle_vertex_data* vtx_data = (snow_particle_vertex_data*)snow_ssbo.MapMemory();
    if (vtx_data) {
        particle_data* snow_ptcl = snow_ptcl_data;
        for (int32_t i = snow->num_snow; i; i--, snow_ptcl++, vtx_data++) {
            vtx_data->position = *(vec3*)&snow_ptcl->position;
            vtx_data->size = snow_ptcl->size;
            vtx_data->alpha = snow_ptcl->alpha;
        }
    }
    snow_ssbo.UnmapMemory();

    snow_particle_vertex_data* fallen_vtx_data = (snow_particle_vertex_data*)snow_fallen_ssbo.MapMemory();
    if (fallen_vtx_data) {
        particle_data* snow_ptcl_fallen = snow_ptcl_fallen_data;
        for (size_t i = snow_ptcl_fallen_count; i; i--, snow_ptcl_fallen++, fallen_vtx_data++) {
            fallen_vtx_data->position = *(vec3*)&snow_ptcl_fallen->position;
            fallen_vtx_data->size = snow_ptcl_fallen->size;
            fallen_vtx_data->alpha = snow_ptcl_fallen->alpha;
        }
    }
    snow_fallen_ssbo.UnmapMemory();
}

HOOK(void, FASTCALL, snow_particle_init, 0x000000014035DD30, bool change_stage) {
    stage_param_data_snow* snow = stage_param_data_snow_current;

    float_t colli_ground_y = stage_param_data_snow_current->colli_ground.y;

    snow_particle_fallen_count = 0;
    snow_particle_fallen_index = 0;

    int32_t render_height = render_get()->render_height[0];
    float_t snow_particle_size = (float_t)render_height * (float_t)(1.0 / 720.0);
    snow_particle_size_min = snow_particle_size;
    snow_particle_size_mid = snow_particle_size * 31.0f;
    snow_particle_size_max = snow_particle_size * 63.0f;

    snow_particle_data_free();
    snow_particle_data_init();

    vec3 velocity = snow->velocity;
    vec3 vel_range = snow->vel_range;
    vec3 range = snow->range;
    vec3 offset = snow->offset;
    range.y = offset.y + range.y - max_def(offset.y, colli_ground_y);

    if (stage_param_data_snow_current->num_snow > 0) {
        particle_data* snow_ptcl = snow_ptcl_data;
        for (int32_t i = ((stage_param_data_snow_current->num_snow - 1) >> 1) + 1; i; i--, snow_ptcl += 2) {
            vec3 position;
            position.x = rand_state_array_get_float(4) - 0.5f;
            position.y = rand_state_array_get_float(4);
            position.z = rand_state_array_get_float(4) - 0.5f;
            *(vec3*)&snow_ptcl[0].position = position * range + offset;

            snow_ptcl[0].size = rand_state_array_get_float(4) * 0.4f + 0.3f;
            snow_ptcl[0].alpha = rand_state_array_get_float(4) * 0.5f + 0.4f;
            snow_ptcl[0].life_time = 1;

            vec3 _velocity;
            _velocity.x = rand_state_array_get_float(4);
            _velocity.y = rand_state_array_get_float(4);
            _velocity.z = rand_state_array_get_float(4);
            *(vec3*)&snow_ptcl[0].velocity = (_velocity - 0.5f) * vel_range + velocity;

            snow_ptcl[0].direction = 0.0f;

            snow_ptcl[1].size = ((rand_state_array_get_float(4) * 0.5f) + 0.3f) * snow_ptcl[0].size;
            snow_ptcl[1].alpha = (rand_state_array_get_float(4) + 1.0f) * 0.3f;

            _velocity.x = rand_state_array_get_float(4);
            _velocity.y = rand_state_array_get_float(4);
            _velocity.z = rand_state_array_get_float(4);
            *(vec3*)&snow_ptcl[1].velocity = (_velocity - 0.5f) * 0.035f * (snow_ptcl[1].size + snow_ptcl[0].size);

            snow_ptcl[0].size = snow_particle_size_mid * snow_ptcl[0].size;
            snow_ptcl[1].size = snow_particle_size_mid * snow_ptcl[1].size;
        }
    }

    for (int32_t i = 0; i < snow_ptcl_fallen_count; i++)
        snow_particle_data_kill_fallen(&snow_ptcl_fallen_data[i], false);

    for (int32_t i = 0; i < 4; i++) {
        particle_data& j = snow_ptcl_gpu[i];
        j.position.x = 0.0f;
        j.position.y = (float_t)i * -25.0f - 5.0f;
        j.position.z = 0.0f;
        j.alpha = 1.0;
        j.life_time = 1;

        vec3 _velocity;
        _velocity.x = rand_state_array_get_float(4);
        _velocity.y = rand_state_array_get_float(4);
        _velocity.z = rand_state_array_get_float(4);
        *(vec3*)&j.velocity = (_velocity - 0.5f) * vel_range + velocity;
        j.direction = 0.0f;
    }

    if (change_stage)
        return;

    snow_particle_free();

    snow_ssbo.Create(sizeof(snow_particle_vertex_data) * snow->num_snow);

    snow_particle_gpu_vertex_data* vtx_data = force_malloc<snow_particle_gpu_vertex_data>(snow_ptcl_count);

    for (int32_t i = 0; i < snow_ptcl_count; i++, vtx_data++) {
        vtx_data->position.x = rand_state_array_get_float(4);
        vtx_data->position.y = rand_state_array_get_float(4);
        vtx_data->position.z = rand_state_array_get_float(4);
        vtx_data->size = (rand_state_array_get_float(4) * 0.5f + 0.4f) * snow_particle_size_mid;
    }

    vtx_data -= snow_ptcl_count;

    snow_gpu_ssbo.Create(sizeof(snow_particle_gpu_vertex_data) * snow_ptcl_count, vtx_data);

    free_def(vtx_data);

    snow_fallen_ssbo.Create(sizeof(snow_particle_vertex_data) * snow_ptcl_fallen_count);

    snow_particle_scene_ubo.Create(sizeof(snow_particle_scene_shader_data));
    snow_particle_batch_ubo.Create(sizeof(snow_particle_batch_shader_data));
}

HOOK(void, FASTCALL, water_particle__free, 0x0000000140363F00, water_particle* water_ptcl) {
    auto elem = water_particle_ssbo.find(water_ptcl);
    if (elem != water_particle_ssbo.end()) {
        elem->second.Destroy();
        water_particle_ssbo.erase(elem);
        water_particle_ssbo.sort();
    }

    originalwater_particle__free(water_ptcl);
}

HOOK(bool, FASTCALL, TaskEffectSplash__dest, 0x0000000140364040, size_t task_effect_splash) {
    bool ret = originalTaskEffectSplash__dest(task_effect_splash);
    water_particle_scene_ubo.Destroy();
    return ret;
}

HOOK(void, FASTCALL, water_particle__disp, 0x0000000140364090, water_particle* water_ptcl) {
    if (!water_ptcl->splash)
        return;

    disp_manager->entry_obj_user(&mat4_identity,
        (mdl::UserArgsFunc)draw_water_particle, water_ptcl, mdl::OBJ_TYPE_TRANSLUCENT);

    EffectRipple* effect_ripple = effect_ripple_data_get();
    if (effect_ripple && !effect_ripple->use_float_ripplemap) {
        water_ptcl->ripple.ripple_uniform = 0;
        water_ptcl->ripple.ripple_emit_uniform = 0;
        implOfEffectRipple__disp_particles(effect_ripple, &water_ptcl->ripple);
    }
}

HOOK(void, FASTCALL, water_particle__set, 0x0000000140364F20,
    water_particle* water_ptcl, splash_particle* splash, int32_t splash_tex_id) {
    originalwater_particle__set(water_ptcl, splash, splash_tex_id);
    
    auto elem = water_particle_ssbo.find(water_ptcl);
    if (elem == water_particle_ssbo.end()) {
        water_particle_ssbo.push_back({ water_ptcl, {} });
        water_particle_ssbo.sort();
        elem = water_particle_ssbo.find(water_ptcl);
    }
    else
        elem->second.Destroy();
    elem->second.Create(sizeof(water_particle_vertex_data) * water_ptcl->splash_count);
}

HOOK(bool, FASTCALL, TaskEffectSplash__init, 0x00000001403655A0, size_t task_effect_splash) {
    water_particle_scene_ubo.Create(sizeof(water_particle_scene_shader_data));
    return originalTaskEffectSplash__init(task_effect_splash);
}

HOOK(bool, FASTCALL, TaskEffectStar__dest, 0x000000014036A830, size_t tast_effect_star) {
    star_catalog_data.free();
    void (FASTCALL * stage_param_data_star_storage_clear)()
        = (void (FASTCALL*)())0x000000014036A0E0;
    stage_param_data_star_storage_clear();
    return true;
}

HOOK(bool, FASTCALL, star_catalog__init, 0x000000014036CD50, star_catalog* star) {
    return star->init();
}

void effect_patch() {
    INSTALL_HOOK(EffectFogRing__dest);
    INSTALL_HOOK(EffectFogRing__disp);
    INSTALL_HOOK(EffectFogRing__set_stage_indices);
    INSTALL_HOOK(EffectFogRing__calc_vert);
    INSTALL_HOOK(EffectFogRing__draw_static);
    INSTALL_HOOK(leaf_particle_free);
    INSTALL_HOOK(leaf_particle_init);
    INSTALL_HOOK(TaskEffectRain__dest);
    INSTALL_HOOK(rain_particle_init);
    INSTALL_HOOK(particle_free);
    INSTALL_HOOK(particle_init);
    INSTALL_HOOK(TaskEffectRipple__dest);
    INSTALL_HOOK(TaskEffectRipple__init);
    INSTALL_HOOK(EffectRipple__draw_static);
    INSTALL_HOOK(EffectRipple__disp_particles);
    INSTALL_HOOK(TaskEffectSnow__dest);
    INSTALL_HOOK(snow_particle_ctrl);
    INSTALL_HOOK(snow_particle_init);
    INSTALL_HOOK(water_particle__free);
    INSTALL_HOOK(TaskEffectSplash__dest);
    INSTALL_HOOK(water_particle__disp);
    INSTALL_HOOK(water_particle__set);
    INSTALL_HOOK(TaskEffectSplash__init);
    INSTALL_HOOK(TaskEffectStar__dest);
    INSTALL_HOOK(star_catalog__init);
}

void EffectFogRing::draw() {
    draw_state.set_fog_height(false);
    if (!enable || !display)
        return;

    draw_state.set_fog_height(true);
    RenderTexture& rt = render_manager->get_render_texture(8);
    rt.Bind();
    glViewportDLL(0, 0,
        rt.color_texture->get_width_align_mip_level(),
        rt.color_texture->get_height_align_mip_level());
    glClearColorDLL(density_offset, density_offset, density_offset, 1.0f);
    glClearDLL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (disp_manager->get_obj_count(mdl::OBJ_TYPE_USER))
        disp_manager->draw(mdl::OBJ_TYPE_USER);
    gl_state_bind_framebuffer(0);
    render_manager->set_effect_texture(rt.color_texture);
    gl_state_get_error();
}

static void sub_1403B6F60(texture* a1, texture* a2, texture* a3, EffectRipple::Params& params) {
    if (!a1 || !a1->glid || !a2 || !a2->glid || !a3 || !a3->glid)
        return;

    int32_t width = a2->width;
    int32_t height = a2->height;

    GLint v43[4];
    glGetIntegervDLL(GL_VIEWPORT, v43);
    glViewportDLL(1, 1, width - 2, height - 2);

    ripple_scene_shader_data ripple_scene = {};
    ripple_scene.g_transform = {
        params.speed / (float_t)width, params.speed / (float_t)height,
        (float_t)width / (float_t)(width - 2), (float_t)height / (float_t)(height - 2)
    };
    ripple_scene.g_texcoord = { 1.0f, 0.0f, 0.0f, 0.0f };
    ripple_scene_ubo.WriteMemory(ripple_scene);

    ripple_batch_shader_data ripple_batch = {};
    ripple_batch.g_params = { params.wake_attn, params.speed, params.field_8, params.field_C };
    ripple_batch_ubo.WriteMemory(ripple_batch);

    gl_state_bind_vertex_array(rctx->common_vao);
    shaders_ft.set(SHADER_FT_RIPPLE);
    ripple_scene_ubo.Bind(0);
    ripple_batch_ubo.Bind(1);
    gl_state_active_bind_texture_2d(0, a2->glid);
    gl_state_active_bind_texture_2d(1, a3->glid);
    shaders_ft.draw_arrays(GL_TRIANGLE_STRIP, 0, 4);
    gl_state_active_bind_texture_2d(1, 0);
    gl_state_active_bind_texture_2d(0, 0);
    gl_state_bind_vertex_array(0);

    glViewportDLL(v43[0], v43[1], v43[2], v43[3]);
}

static void sub_1403B6ED0(RenderTexture* a1, RenderTexture* a2, RenderTexture* a3, EffectRipple::Params& params) {
    a1->Bind();
    if (a1->color_texture->internal_format == GL_RGBA32F
        || a1->color_texture->internal_format == GL_RGBA16F)
        uniform->arr[U_RIPPLE] = 1;
    else
        uniform->arr[U_RIPPLE] = 0;
    sub_1403B6F60(a1->color_texture, a2->color_texture, a3->color_texture, params);
    gl_state_bind_framebuffer(0);
}

void EffectRipple::draw() {
    if (!stage_set)
        return;

    gl_state_disable_cull_face();

    RenderTexture* rt[3];
    for (int32_t i = 0, j = 2; i < 3; i++, j++)
        rt[i] = &render_manager->get_render_texture(
            use_float_ripplemap ? j : (j + 3));

    if (update) {
        int32_t counter = this->counter + 1;
        if (counter >= 3)
            counter = 0;

        rt[(counter + 1) % 3]->Bind();

        GLint v43[4];
        glGetIntegervDLL(GL_VIEWPORT, v43);

        int32_t width = rt[0]->GetWidth();
        int32_t height = rt[0]->GetHeight();

        glViewportDLL(1, 1, width - 2, height - 2);

        draw_pass_set_camera();
        glClearDLL(GL_DEPTH_BUFFER_BIT);
        if (disp_manager->get_obj_count(mdl::OBJ_TYPE_USER)) {
            gl_state_active_bind_texture_2d(7, rt[counter % 3]->GetColorTex());
            disp_manager->draw(mdl::OBJ_TYPE_USER);
            gl_state_active_bind_texture_2d(7, 0);
        }

        gl_state_bind_framebuffer(0);

        params.field_8 = 0.0005f;
        params.field_C = 0.97f;
        if (field_30 > 0) {
            params.field_8 = 0.00005f;
            params.field_C = 0.999f;
        }

        sub_1403B6ED0(rt[(counter + 2) % 3], rt[(counter + 1) % 3], rt[counter % 3], params);

        glViewportDLL(v43[0], v43[1], v43[2], v43[3]);

        sub_1403584A0(rt[(counter + 2) % 3]);

        this->counter = counter;
    }

    int32_t v11 = (counter + 2) % 3 + 2;
    if (!use_float_ripplemap)
        v11 += 3;

    render_manager->set_effect_texture(
        render_manager->get_render_texture(v11).color_texture);

    update = false;

    gl_state_enable_cull_face();
}

void EffectRipple::sub_1403584A0(RenderTexture* rt) {
    if (ripple_tex_id == -1)
        return;

    texture* ripple_tex = texture_manager_get_texture(ripple_tex_id);
    if (!ripple_tex)
        return;

    field_BB8.SetColorDepthTextures(ripple_tex->glid);
    field_BB8.Bind();

    image_filter_scale(&field_BB8, rt->color_texture);
    gl_state_bind_framebuffer(0);
}

void star_catalog_milky_way::create_buffers(int32_t subdivs, float_t uv_rec_scale_u, float_t uv_rec_scale_v,
    int32_t longitude_degs_10, int32_t latitude_degs_10,
    float_t longitude_offset_degs_10, float_t latitude_offset_degs_10) {
    delete_buffers();

    const int32_t sectors_count = 2 * subdivs + 1;
    const int32_t vtx_count = (subdivs - 1) * sectors_count + 2;
    restart_index = (uint16_t)0xFFFFFFFF;

    gl_state_bind_vertex_array(0);
    gl_state_bind_array_buffer(0);
    gl_state_bind_element_array_buffer(0);

    const float_t rec_longitude_degs_10 = 1.0f / (float_t)longitude_degs_10;
    const float_t rec_latitude_degs_10 = 1.0f / (float_t)latitude_degs_10;

    struct star_catalog_vertex {
        vec3 position;
        vec2 texcoord;
    };

    star_catalog_vertex* vtx_data = force_malloc<star_catalog_vertex>(vtx_count);
    vtx_data[0].position = { 0.0f, 1.0f, 0.0f };
    vtx_data[0].texcoord.x = 0.5f;
    vtx_data[0].texcoord.y = (latitude_offset_degs_10 + (1.0f / uv_rec_scale_v) * 90.0f) * rec_latitude_degs_10;
    size_t vtx = 1;
    if (subdivs - 1 > 0) {
        float_t rec_stack_step = 1.0f / (float_t)subdivs;
        float_t rec_sector_step = 1.0f / (float_t)(2 * subdivs);
        for (uint32_t i = subdivs - 1, j = 1; i; i--, j++) {
            float_t stack_angle = (float_t)M_PI_2 - (float_t)j * (float_t)M_PI * rec_stack_step;
            float_t xz = cosf(stack_angle);
            float_t y = sinf(stack_angle);
            float_t texcoord_y = (latitude_offset_degs_10 + stack_angle * (float_t)(1.0 / M_PI)
                * 180.0f * (float_t)(1.0 / uv_rec_scale_v)) * rec_latitude_degs_10;
            for (uint32_t k = sectors_count, l = 0; k; k--, l++, vtx++) {
                float_t sector_angle = (float_t)l * (float_t)(2.0 * M_PI) * rec_sector_step;
                vtx_data[vtx].position.x = sinf(sector_angle) * xz;
                vtx_data[vtx].position.y = y;
                vtx_data[vtx].position.z = cosf(sector_angle) * xz;
                vtx_data[vtx].texcoord.x = (longitude_offset_degs_10 + (float_t)l * rec_sector_step
                    * 360.0f * (1.0f / uv_rec_scale_u)) * rec_longitude_degs_10;
                vtx_data[vtx].texcoord.y = texcoord_y;
            }
        }
    }
    vtx_data[vtx].position = { 0.0f, -1.0f, 0.0f };
    vtx_data[vtx].texcoord.x = 1.0f;
    vtx_data[vtx].texcoord.y = (latitude_offset_degs_10 - (1.0f / uv_rec_scale_v) * 90.0f) * rec_latitude_degs_10;

    if (!vao)
        glGenVertexArrays(1, &vao);

    vbo.Create(sizeof(star_catalog_vertex) * vtx_count, vtx_data);
    vbo.Bind();
    free_def(vtx_data);

    static const GLsizei buffer_size = sizeof(star_catalog_vertex);

    gl_state_bind_vertex_array(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(star_catalog_vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, buffer_size,
        (void*)offsetof(star_catalog_vertex, texcoord));

    const uint16_t first_vertex = 0;
    const uint16_t last_vertex = (uint16_t)(vtx_count - 1);
    uint16_t restart_index = this->restart_index;

    const int32_t ebo_count = (subdivs - 2) * (2 * sectors_count + 1) + 8 * sectors_count - 8;
    idx_count = ebo_count;
    uint16_t* ebo_data = force_malloc<uint16_t>(ebo_count);
    size_t idx = 0;
    for (uint32_t i = sectors_count - 1, j = 1; i; i--, j++, idx += 4) {
        ebo_data[idx + 0] = first_vertex;
        ebo_data[idx + 1] = (uint16_t)(j + 1);
        ebo_data[idx + 2] = (uint16_t)j;
        ebo_data[idx + 3] = restart_index;
    }

    uint16_t v37 = 1;
    for (uint32_t i = subdivs - 2; i; i--, idx++) {
        for (uint32_t j = sectors_count; j; j--, idx += 2, v37++) {
            ebo_data[idx + 0] = (uint16_t)(v37 + sectors_count);
            ebo_data[idx + 1] = (uint16_t)(v37);
        }
        ebo_data[idx] = restart_index;
    }

    for (uint32_t i = sectors_count - 1; i; i--, idx += 4, v37++) {
        ebo_data[idx + 0] = last_vertex;
        ebo_data[idx + 1] = (uint16_t)v37;
        ebo_data[idx + 2] = (uint16_t)(v37 + 1);
        ebo_data[idx + 3] = restart_index;
    }

    ebo.Create(sizeof(uint16_t) * ebo_count, ebo_data);
    ebo.Bind();
    free_def(ebo_data);

    if (!star_sampler)
        glGenSamplers(1, &star_sampler);
    glSamplerParameteri(star_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(star_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(star_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(star_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    gl_state_bind_vertex_array(0);
    gl_state_bind_array_buffer(0);
    gl_state_bind_element_array_buffer(0);
}

void star_catalog_milky_way::create_default_sphere() {
    create_buffers(16, uv_rec_scale_u, uv_rec_scale_v, longitude_degs_10, latitude_degs_10,
        longitude_offset_degs_10, latitude_offset_degs_10);
}

void star_catalog_milky_way::delete_buffers() {
    if (star_sampler) {
        glDeleteSamplers(1, &star_sampler);
        star_sampler = 0;
    }

    vbo.Destroy();
    ebo.Destroy();

    if (vao) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}

void star_catalog_milky_way::draw(const mat4& vp, const mat4& mat, texture* tex, GLUniformBuffer& scene_ubo) {
    if (!vao)
        return;

    mat4 latitude_mat;
    mat4 longitude_mat;
    mat4_rotate_x(latitude * DEG_TO_RAD_FLOAT, &latitude_mat);
    mat4_rotate_y(longitude * DEG_TO_RAD_FLOAT, &longitude_mat);

    mat4 model;
    mat4_mul(&latitude_mat, &longitude_mat, &model);

    const float_t pitch_forward = -0.50503153f;
    const float_t yaw_forward = 4.6496463f;
    const float_t pitch_up = 0.47347879f;
    const float_t yaw_up = 3.3660336f;

    vec3 forward;
    vec3 up;
    forward.x = sinf(yaw_forward) * cosf(pitch_forward);
    forward.y = sinf(pitch_forward);
    forward.z = cosf(yaw_forward) * cosf(pitch_forward);
    up.x = sinf(yaw_up) * cosf(pitch_up);
    up.y = sinf(pitch_up);
    up.z = cosf(yaw_up) * cosf(pitch_up);

    mat4 view = mat4_identity;
    *(vec3*)&view.row0 = vec3::cross(up, forward);
    *(vec3*)&view.row1 = up;
    *(vec3*)&view.row2 = forward;

    mat4_mul(&model, &view, &model);
    mat4_mul(&model, &mat, &model);

    star_catalog_scene_shader_data scene_shader_data = {};
    mat4 temp;
    mat4_mul(&model, &vp, &temp);
    mat4_transpose(&temp, &temp);
    scene_shader_data.g_transform[0] = temp.row0;
    scene_shader_data.g_transform[1] = temp.row1;
    scene_shader_data.g_transform[2] = temp.row2;
    scene_shader_data.g_transform[3] = temp.row3;
    scene_ubo.WriteMemory(scene_shader_data);

    gl_state_enable_cull_face();
    gl_state_disable_blend();
    gl_state_set_depth_mask(GL_FALSE);
    gl_state_active_bind_texture_2d(0, tex->glid);
    gl_state_bind_sampler(0, star_sampler);
    gl_state_enable_primitive_restart();
    gl_state_set_primitive_restart_index(restart_index);
    uniform->arr[U_STAR] = 1;
    shaders_ft.set(SHADER_FT_STAR);

    gl_state_bind_vertex_array(vao);
    scene_ubo.Bind(0);
    shaders_ft.draw_elements(GL_TRIANGLE_STRIP, idx_count, GL_UNSIGNED_SHORT, 0);
    gl_state_bind_vertex_array(0);

    gl_state_bind_sampler(0, 0);
    gl_state_active_bind_texture_2d(0, 0);
    gl_state_disable_primitive_restart();
    gl_state_set_depth_mask(GL_TRUE);
    gl_state_enable_blend();
    gl_state_disable_cull_face();


}

void star_catalog::draw() {
    if (!stage_param_data_ptr || !enable)
        return;

    texture* star_tex = texture_manager_get_texture(this->star_tex);
    texture* star_b_tex = texture_manager_get_texture(this->star_b_tex);
    if (!star_tex || !star_b_tex)
        return;

    mat4 model;
    stage_param_data_star::get_mat(model,
        stage_param_data.observer_north_latitude_deg * DEG_TO_RAD_FLOAT,
        stage_param_data.observer_east_longitude_deg * DEG_TO_RAD_FLOAT,
        stage_param_data.utc,
        stage_param_data.rotation_y_deg * DEG_TO_RAD_FLOAT);

    const mat4& view = rctx->view_mat;
    model.row3.x = vec3::dot(-*(vec3*)&view.row0, *(vec3*)&view.row3) + model.row3.x;
    model.row3.y = vec3::dot(-*(vec3*)&view.row1, *(vec3*)&view.row3) + model.row3.y;
    model.row3.z = vec3::dot(-*(vec3*)&view.row2, *(vec3*)&view.row3) + model.row3.z;

    mat4 proj = rctx->proj_mat;
    proj.row0.z = proj.row0.w;
    proj.row1.z = proj.row1.w;
    proj.row2.z = proj.row2.w;
    proj.row3.z = proj.row3.w;

    mat4 vp;
    mat4_mul(&rctx->view_mat, &proj, &vp);

    texture* milky_way_tex = texture_manager_get_texture(milky_way_tex_id);
    if (milky_way_tex)
        milky_way.draw(vp, model, milky_way_tex, stars_scene_ubo);

    star_catalog_scene_shader_data scene_shader_data = {};
    mat4 temp;
    mat4_mul(&model, &vp, &temp);
    mat4_transpose(&temp, &temp);
    scene_shader_data.g_transform[0] = temp.row0;
    scene_shader_data.g_transform[1] = temp.row1;
    scene_shader_data.g_transform[2] = temp.row2;
    scene_shader_data.g_transform[3] = temp.row3;
    stars_scene_ubo.WriteMemory(scene_shader_data);

    uniform->arr[U_STAR] = 0;
    shaders_ft.set(SHADER_FT_STAR);
    gl_state_enable_blend();
    gl_state_set_blend_func(GL_ONE, GL_ONE);
    gl_state_enable_depth_test();
    gl_state_set_depth_mask(false);
    gl_state_bind_vertex_array(rctx->common_vao);
    for (int32_t i = 0; i < 2; i++) {
        stage_param_data_star_modifiers& modifiers = stage_param_data.modifiers[i];

        star_catalog_batch_shader_data batch_shader_data = {};
        batch_shader_data.g_size_in_projection = {
            (float_t)(1.0 / 1280.0), (float_t)(1.0 / 720.0), 0.0f, modifiers.size_max
        };
        batch_shader_data.g_modifiers = {
            modifiers.color_scale, modifiers.color_scale * modifiers.offset_scale,
            modifiers.pos_scale, modifiers.pos_scale * modifiers.offset_scale
        };
        batch_shader_data.g_thresholds = { modifiers.threshold * modifiers.pos_scale, 0.0f, 0.0f, 0.0f };
        stars_batch_ubo.WriteMemory(batch_shader_data);

        stars_scene_ubo.Bind(0);
        stars_batch_ubo.Bind(1);
        stars_ssbo.Bind(0);
        if (i) {
            gl_state_active_bind_texture_2d(0, star_b_tex->glid);
            shaders_ft.draw_arrays(GL_TRIANGLES, 0, star_b_count * 6);
        }
        else {
            gl_state_active_bind_texture_2d(0, star_tex->glid);
            shaders_ft.draw_arrays(GL_TRIANGLES, 0, star_count * 6);
        }
    }
    gl_state_active_bind_texture_2d(0, 0);
    gl_state_bind_vertex_array(0);

}

void star_catalog::free() {
    stars_ssbo.Destroy();

    stars_batch_ubo.Destroy();
    stars_scene_ubo.Destroy();

    star_catalog_data.file_handler.reset();
    star_catalog_data.milky_way.delete_buffers();

    star_catalog_data.stage_param_data_ptr = 0;
}

bool star_catalog::init() {
    if (file_handler.check_not_ready())
        return false;

    if (file_handler.get_data()) {
        struct stars_buffer_data {
            vec3 position;
            float_t size;
            vec4 color;

            inline stars_buffer_data() : size() {

            }
        };

        prj::vector<stars_buffer_data> vec;
        file_handler.get_size();
        const void* data = file_handler.get_data();
        void (FASTCALL* star_catalog__parse_data)(prj::vector<stars_buffer_data>* vec, size_t data)
            = (void (FASTCALL*)(prj::vector<stars_buffer_data>*vec, size_t data))0x000000014036C6A0;
        star_catalog__parse_data(&vec, (size_t)data);

        for (stars_buffer_data& i : vec) {
            float_t size = i.color.w;
            i.color = *(vec4*)&i.size;
            i.size = size;
        }

        std::sort(vec.begin(), vec.end(), [](stars_buffer_data a, stars_buffer_data b) {
            return a.size > b.size;
        });

        stars_buffer_data* stars_data = vec.data();
        size_t length = vec.size();
        size_t temp;
        while (length > 0)
            if (stars_data[temp = length / 2].size <= 0.0f)
                length = temp;
            else {
                stars_data += temp + 1;
                length -= temp + 1;
            }

        if (stars_data != vec.data() + vec.size())
            vec.resize(stars_data - vec.data());

        stars_data = vec.data();
        length = vec.size();
        while (length > 0)
            if (stars_data[temp = length / 2].size <= 2.0f)
                length = temp;
            else {
                stars_data += temp + 1;
                length -= temp + 1;
            }

        if (stars_data != vec.data() + vec.size())
            star_b_count = (int32_t)(stars_data - vec.data());
        else
            star_b_count = (int32_t)vec.size();

        file_handler.reset();

        star_count = (int32_t)vec.size();

        stars_ssbo.Create(sizeof(stars_buffer_data) * vec.size(), vec.data());
    }

    milky_way.create_default_sphere();

    stars_scene_ubo.Create(sizeof(star_catalog_scene_shader_data));
    stars_batch_ubo.Create(sizeof(star_catalog_batch_shader_data));
    return true;
}

void water_particle::draw(mat4* mat) {
    if (count <= 0)
        return;

    auto elem = water_particle_ssbo.find(this);
    if (elem == water_particle_ssbo.end())
        return;

    GLShaderStorageBuffer& ssbo = elem->second;

    water_particle_vertex_data* vtx_data = (water_particle_vertex_data*)ssbo.MapMemory();
    if (!vtx_data)
        return;

    point_particle_data* ptcl_data = this->ptcl_data.data();
    for (int32_t i = count; i > 0; i--, vtx_data++, ptcl_data++) {
        vtx_data->position = ptcl_data->position;
        vtx_data->color = ptcl_data->color;
        vtx_data->size = ptcl_data->size;
    }
    ssbo.UnmapMemory();

    water_particle_scene_shader_data scene_shader_data = {};
    mat4 temp;
    mat4_mul(mat, &rctx->vp_mat, &temp);
    mat4_transpose(&temp, &temp);
    scene_shader_data.g_transform[0] = temp.row0;
    scene_shader_data.g_transform[1] = temp.row1;
    scene_shader_data.g_transform[2] = temp.row2;
    scene_shader_data.g_transform[3] = temp.row3;

    mat4_mul(mat, &rctx->view_mat, &temp);
    mat4_transpose(&temp, &temp);

    scene_shader_data.g_view_world_row2 = temp.row2;
    scene_shader_data.g_state_point_attenuation = { 0.7f, 0.4f, 0.0f, 0.0f };
    scene_shader_data.g_size_in_projection.x = (float_t)(1.0 / 1280.0);
    scene_shader_data.g_size_in_projection.y = (float_t)(1.0 / 720.0);
    scene_shader_data.g_size_in_projection.z = 1.0;
    scene_shader_data.g_size_in_projection.w = 60.0;
    water_particle_scene_ubo.WriteMemory(scene_shader_data);

    gl_state_disable_cull_face();
    shaders_ft.set(SHADER_FT_W_PTCL);

    gl_state_bind_vertex_array(rctx->common_vao);
    water_particle_scene_ubo.Bind(4);
    ssbo.Bind(0);
    texture* tex = texture_manager_get_texture(splash_tex_id);
    if (tex)
        gl_state_active_bind_texture_2d(0, tex->glid);
    shaders_ft.draw_arrays(GL_TRIANGLES, 0, count * 6);
    gl_state_bind_vertex_array(0);

    shader::unbind();
    gl_state_enable_cull_face();
}

static void draw_fog_particle(EffectFogRing* data, mat4* mat) {
    if (!data->num_vtx)
        return;

    auto elem = fog_ring_ssbo.find(data);
    if (elem == fog_ring_ssbo.end())
        return;

    GLShaderStorageBuffer& ssbo = elem->second;

    shaders_ft.set(SHADER_FT_FOGPTCL);
    gl_state_enable_blend();
    texture* tex = texture_manager_get_texture(data->tex_id);
    if (tex)
        gl_state_active_bind_texture_2d(0, tex->glid);
    gl_state_bind_vertex_array(rctx->common_vao);
    ssbo.Bind(0);
    shaders_ft.draw_arrays(GL_TRIANGLES, 0, data->num_vtx);
    gl_state_bind_vertex_array(0);
    gl_state_disable_blend();
    shader::unbind();
}

static void draw_ripple_particles(ripple_struct* data, mat4* mat) {
    if (data->count > 5000)
        return;

    vec3* vtx_data = (vec3*)ripple_emit_ssbo.MapMemory();
    if (!vtx_data)
        return;

    vec3* position = data->position;
    color4u8* color = data->color;

    for (size_t i = data->count; i; i--, vtx_data++, position++, color++)
        *vtx_data = { position->x, -position->z, (float_t)color->a * (float_t)(1.0 / 255.0) };

    ripple_emit_ssbo.UnmapMemory();

    int32_t size = (int32_t)(data->size + 0.5f);

    RenderTexture& rt = render_manager->get_render_texture(
        effect_ripple_data_get()->use_float_ripplemap ? 2 : 5);
    int32_t width = rt.GetWidth();
    int32_t height = rt.GetHeight();

    ripple_emit_scene_shader_data shader_data = {};
    shader_data.g_size_in_projection = {
        (float_t)size / (float_t)width,
        (float_t)size / (float_t)height,
        0.0f, 0.0f
    };
    shader_data.g_transform.x = (float_t)width / (float_t)(width - 2);
    shader_data.g_transform.y = (float_t)height / (float_t)(height - 2);
    shader_data.g_transform.z = 0.0079498291f / (float_t)(width - 2);
    shader_data.g_transform.w = -0.0079498291f / (float_t)(height - 2);
    shader_data.g_framebuffer_size = {
        1.0f / (float_t)width,
        1.0f / (float_t)height,
        0.0f, 0.0f };
    ripple_emit_scene_ubo.WriteMemory(shader_data);

    uniform->arr[U_RIPPLE] = data->ripple_uniform;
    uniform->arr[U_RIPPLE_EMIT] = data->ripple_emit_uniform;

    gl_state_begin_event("EffectRipple::Impl::draw_ripple_particles");
    gl_state_set_color_mask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    gl_state_bind_vertex_array(rctx->common_vao);

    shaders_ft.set(SHADER_FT_RIPEMIT);
    ripple_emit_scene_ubo.Bind(0);
    ripple_emit_ssbo.Bind(0);
    shaders_ft.draw_arrays(GL_TRIANGLES, 0, data->count * 6);

    shader::unbind();

    gl_state_bind_vertex_array(0);
    gl_state_set_color_mask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    gl_state_end_event();
}

static void draw_water_particle(water_particle* data, mat4* mat) {
    data->draw(mat);
}

static std::pair<vec3, float_t> sub_1406427A0(vec3 x, vec3 y) {
    float_t t1 = vec3::length_squared(x);
    if (t1 <= 1.0e-30f)
        return { y, 0.0f };

    float_t t2 = 1.0f / sqrtf(t1);
    float_t t3 = (t2 * 1.5f + t1 * t2 * t2 * t2 * -0.5f);
    return { t3 * x, t3 * t1 };
}

static int32_t leaf_particle_disp() {
    float_t size = leaf_ptcl_data[0].size;

    vec3 position[4];
    vec3 normal[4];
    position[0] = vec3(-0.5f, -0.5f, 0.0f) * size;
    position[1] = vec3( 0.5f, -0.5f, 0.2f) * size;
    position[2] = vec3( 0.5f,  0.5f, 0.0f) * size;
    position[3] = vec3(-0.5f,  0.5f, 0.2f) * size;
    normal[0] = sub_1406427A0(vec3( 0.0f,  0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)).first;
    normal[1] = sub_1406427A0(vec3(-0.4f,  0.4f, 1.0f), vec3(0.0f, 1.0f, 0.0f)).first;
    normal[2] = sub_1406427A0(vec3(-0.2f, -0.2f, 1.0f), vec3(0.0f, 1.0f, 0.0f)).first;
    normal[3] = sub_1406427A0(vec3( 0.4f, -0.4f, 1.0f), vec3(0.0f, 1.0f, 0.0f)).first;

    leaf_particle_vertex_data* vtx_data = (leaf_particle_vertex_data*)leaf_ptcl_vbo.MapMemory();
    if (!vtx_data)
        return 0;

    int32_t vtx_count = 0;
    leaf_particle_data* data = leaf_ptcl_data;
    bool split_tex = stage_param_data_leaf_current->split_tex;
    for (int32_t i = 0; i < leaf_particle_num_ptcls; i++, data++) {
        if (!data->type)
            continue;

        vec3 pos = *(vec3*)&data->position;

        mat3 mat;
        mat3_rotate_xyz((vec3*)&data->rotation, &mat);

        vec3 t0;
        vec3 t1;
        vec3 t2;
        vec3 t3;
        mat3_transform_vector(&mat, &position[0], &t0);
        mat3_transform_vector(&mat, &position[1], &t1);
        mat3_transform_vector(&mat, &position[2], &t2);
        mat3_transform_vector(&mat, &position[3], &t3);
        vtx_data[0].position = pos + t0;
        vtx_data[1].position = pos + t1;
        vtx_data[2].position = pos + t2;
        vtx_data[3].position = pos + t3;

        mat3_transform_vector(&mat, &normal[0], &t0);
        mat3_transform_vector(&mat, &normal[1], &t1);
        mat3_transform_vector(&mat, &normal[2], &t2);
        mat3_transform_vector(&mat, &normal[3], &t3);
        vtx_data[0].normal = t0;
        vtx_data[1].normal = t1;
        vtx_data[2].normal = t2;
        vtx_data[3].normal = t3;
        *(vec3*)&data[0].normal = t0;

        if (split_tex) {
            const float_t u = i & 0x01 ? 1.0f : 0.0f;
            const float_t v = i & 0x02 ? 1.0f : 0.0f;

            float_t u0 = u * 0.5f;
            float_t v0 = v * 0.5f;
            float_t u1 = (u + 1.0f) * 0.5f;
            float_t v1 = (v + 1.0f) * 0.5f;

            vtx_data[0].texcoord = vec2(u0, v0);
            vtx_data[1].texcoord = vec2(u1, v0);
            vtx_data[2].texcoord = vec2(u1, v1);
            vtx_data[3].texcoord = vec2(u0, v1);
        }
        else {
            vtx_data[0].texcoord = 0.0f;
            vtx_data[1].texcoord = vec2(1.0f, 0.0f);
            vtx_data[2].texcoord = 1.0f;
            vtx_data[3].texcoord = vec2(0.0f, 1.0f);
        }

        if (data->type == 1) {
            vtx_data[4] = vtx_data[0];
            vtx_data[5] = vtx_data[3];
            vtx_data[6] = vtx_data[2];
            vtx_data[7] = vtx_data[1];
            vtx_data[4].normal = -vtx_data[4].normal;
            vtx_data[5].normal = -vtx_data[5].normal;
            vtx_data[6].normal = -vtx_data[6].normal;
            vtx_data[7].normal = -vtx_data[7].normal;
            vtx_data += 8;
            vtx_count += 8;
        }
        else {
            vtx_data += 4;
            vtx_count += 4;
        }
    }

    leaf_ptcl_vbo.UnmapMemory();
    return vtx_count;
}

static int32_t particle_disp(particle_vertex_data* vtx_data, particle_rot_data* data, int32_t count) {
    int32_t vtx_count = 0;
    for (size_t i = 0; i < count; i++, data++) {
        if (!data->alive)
            continue;

        vec3 pos = *(vec3*)&data->position;

        mat3 mat;
        mat3_rotate_xyz((vec3*)&data->rotation, &mat);

        float_t size = data->size;

        vec3 t0 = vec3(0.0125f, 0.007216875f, 0.0f) * size;
        vec3 t1 = vec3(-0.0125f, 0.007216875f, 0.0f) * size;
        vec3 t2 = vec3(0.0f, -0.01443375f, 0.0f) * size;
        mat3_transform_vector(&mat, &t0, &t0);
        mat3_transform_vector(&mat, &t1, &t1);
        mat3_transform_vector(&mat, &t2, &t2);
        t0 += pos;
        t1 += pos;
        t2 += pos;

        vec3 normal = vec3(0.0f, 0.0f, 1.0f);
        mat3_transform_vector(&mat, &normal, &normal);
        *(vec3*)&data->normal = normal;

        vec4 color = data->color;
        vtx_data[0].position = t0;
        vtx_data[1].position = t1;
        vtx_data[2].position = t2;
        vtx_data[0].normal = normal;
        vtx_data[1].normal = normal;
        vtx_data[2].normal = normal;
        vtx_data[0].color = color;
        vtx_data[1].color = color;
        vtx_data[2].color = color;
        vtx_data[0].texcoord = vec2(0.0f);
        vtx_data[1].texcoord = vec2(1.0f, 0.5f);
        vtx_data[2].texcoord = vec2(0.0f, 1.0f);

        normal = -normal;
        vtx_data[3].position = t2;
        vtx_data[4].position = t1;
        vtx_data[5].position = t0;
        vtx_data[3].normal = normal;
        vtx_data[4].normal = normal;
        vtx_data[5].normal = normal;
        vtx_data[3].color = color;
        vtx_data[4].color = color;
        vtx_data[5].color = color;
        vtx_data[3].texcoord = vec2(0.0f, 1.0f);
        vtx_data[4].texcoord = vec2(1.0f, 0.5f);
        vtx_data[5].texcoord = vec2(0.0f);
        vtx_data += 6;
        vtx_count += 6;
    }
    return vtx_count;
}

static void rain_particle_free() {
    rain_ssbo.Destroy();

    rain_particle_scene_ubo.Destroy();
    rain_particle_batch_ubo.Destroy();
}

static void ripple_emit_init() {
    ripple_batch_ubo.Create(sizeof(ripple_batch_shader_data));
    ripple_scene_ubo.Create(sizeof(ripple_scene_shader_data));

    ripple_emit_ssbo.Create(sizeof(vec3) * ripple_emit_count);

    ripple_emit_scene_ubo.Create(sizeof(ripple_emit_scene_shader_data));
}

static void ripple_emit_free() {
    ripple_batch_ubo.Destroy();
    ripple_scene_ubo.Destroy();

    ripple_emit_ssbo.Destroy();

    ripple_emit_scene_ubo.Destroy();
}

static void snow_particle_data_emit_fallen(particle_data* data) {
    for (int32_t i = 0; i < 6; i++) {
        particle_data* d = snow_particle_emit_fallen();
        if (!d)
            break;

        d->position.x = (rand_state_array_get_float(4) - 0.5f) * 0.05f + data->position.x;
        d->position.y = data->position.y;
        d->position.z = (rand_state_array_get_float(4) - 0.5f) * 0.05f + data->position.z;
        d->size = (rand_state_array_get_float(4) * 0.5f + 0.2f) * data->size;
        d->velocity = 0.0f;
        d->alpha = data->alpha * 0.60f;
    }
}

static void snow_particle_data_kill_fallen(particle_data* data, bool kill) {
    if (kill)
        --snow_particle_fallen_count;

    data->position = { 0.0f, -10.0f, 0.0f, 0.0f };
    data->size = 0.0f;
    data->alpha = 1.0f;
}

static void snow_particle_data_reset(particle_data* data) {
    stage_param_data_snow* snow = stage_param_data_snow_current;
    vec3 position;
    position.x = rand_state_array_get_float(4) - 0.5f;
    position.y = 1.0f;
    position.z = rand_state_array_get_float(4) - 0.5f;
    *(vec3*)&data->position = position * snow->range + snow->offset;

    data->life_time = 1;

    vec3 velocity;
    velocity.x = rand_state_array_get_float(4);
    velocity.y = rand_state_array_get_float(4);
    velocity.z = rand_state_array_get_float(4);
    *(vec3*)&data->velocity = (velocity - 0.5f) * snow->vel_range + snow->velocity;
    data->direction = 0.0f;
}

static particle_data* snow_particle_emit_fallen() {
    if (snow_particle_fallen_count >= snow_ptcl_fallen_count)
        return 0;

    for (int32_t i = 0; i < snow_ptcl_fallen_count; i++) {
        particle_data* data = &snow_ptcl_fallen_data[snow_particle_fallen_index++];
        if (snow_particle_fallen_index >= snow_ptcl_fallen_count)
            snow_particle_fallen_index = 0;

        if (data->size == 0.0f) {
            snow_particle_fallen_count++;
            return data;
        }
    }
    return 0;
}

static vec3 snow_particle_get_random_velocity() {
    stage_param_data_snow* snow = stage_param_data_snow_current;
    vec3 velocity;
    velocity.x = rand_state_array_get_float(4);
    velocity.y = rand_state_array_get_float(4);
    velocity.z = rand_state_array_get_float(4);
    return (velocity - 0.5f) * snow->vel_range + snow->velocity;
}

static void snow_particle_free() {
    snow_ssbo.Destroy();
    snow_gpu_ssbo.Destroy();
    snow_fallen_ssbo.Destroy();

    snow_particle_scene_ubo.Destroy();
    snow_particle_batch_ubo.Destroy();
}
