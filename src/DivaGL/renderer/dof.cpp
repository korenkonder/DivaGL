/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "dof.hpp"
#include "../rob/rob.hpp"
#include "../camera.hpp"
#include "../gl_state.hpp"
#include "../render_context.hpp"
#include <streambuf>

struct dof_common_shader_data {
    vec4 g_depth_params; //x=(n-f)/(nf), y=1/n, z=coc_from_z_scale, w=coc_from_z_offset
    vec4 g_spread_scale; //x=scale_from_meter_to_pixel, y=scale_from_meter_to_sample,
        //  z=scale_from_pixel_to_sample, w=scale_from_sample_to_pixel
    vec4 g_depth_params2; //x=distance_to_focus_m, y=focus_range,
        // z=k/(fuzzing_range*fuzzing_range), w=max_coc_radius_in_pixel   //yzw=for_f2
};

static_assert(sizeof(dof_common_shader_data) == 0x30, "\"dof_common_shader_data\" struct should have a size of 0x30");

dof_debug* dof_debug_data = (dof_debug*)0x00000001411AB650;
dof_pv* dof_pv_data = (dof_pv*)0x0000001411AB630;

namespace renderer {
    DOF3::DOF3(int32_t width, int32_t height) : textures(), fbo(), samplers(), vao(), program() {
        this->width = width;
        this->height = height;

        init(width, height);
        load_shaders();
    }

    DOF3::~DOF3() {

    }

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

        this->width = width;
        this->height = height;

        init_textures(width, height);
    }

    void DOF3::free() {
        if (vao) {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }

        common_ubo.Destroy();
        texcoords_ubo.Destroy();

        if (samplers[0]) {
            glDeleteSamplers(2, samplers);
            samplers[0] = 0;
        }

        if (textures[0]) {
            glDeleteTextures(6, textures);
            textures[0] = 0;
        }
    }

    void DOF3::init(int32_t width, int32_t height) {
        free();

        init_textures(width, height);

        glGenSamplers(2, samplers);
        glSamplerParameteri(samplers[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(samplers[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(samplers[0], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(samplers[0], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(samplers[1], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(samplers[1], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(samplers[1], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(samplers[1], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        common_ubo.Create(sizeof(dof_common_shader_data));

        vec2 data[50] = {};
        renderer::DOF3::calculate_texcoords(data, 3.0);
        texcoords_ubo.Create(sizeof(data), data);

        glGenVertexArrays(1, &vao);
    }

    void renderer::DOF3::load_shaders() {
        const size_t buf_len = 0x8000;
        char* buf = (char*)_operator_new(buf_len);
        if (!buf)
            return;

        GLProgram::ShaderData shader_data[2];
        shader_data[0].type = GL_VERTEX_SHADER;
        shader_data[0].string =
            u8"#version 430\n"
            u8"out VertexData{\n"
            u8"    vec2 texcoord;\n"
            u8"}result;\n"
            u8"void main(){\n"
            u8"    gl_Position.x = -1.f + (gl_VertexID / 2) * 4;\n"
            u8"    gl_Position.y = 1.f - (gl_VertexID % 2) * 4;\n"
            u8"    gl_Position.z = 0.f;\n"
            u8"    gl_Position.w = 1.f;\n"
            u8"    result.texcoord = gl_Position.xy * 0.5f + 0.5f;\n"
            u8"}\n";
        shader_data[0].length = 273;

        shader_data[1].type = GL_FRAGMENT_SHADER;
        shader_data[1].string =
            u8"#version 430\n"
            u8"layout(binding = 0) uniform sampler2D g_tile;//r=min_depth_m, g=max_coc_pixel\n"
            u8"layout(location = 0) out vec2 result;//r=min_depth_m, g=max_coc_pixel\n"
            u8"in VertexData{\n"
            u8"    vec2 texcoord;\n"
            u8"}frg;\n"
            u8"void main(){\n"
            u8"    //3x3サンプル。\n"
            u8"    vec2 texture_size = textureSize(g_tile, 0);\n"
            u8"    vec2 texel_size = vec2(1.f / texture_size.x, 1.f / texture_size.y);\n"
            u8"    vec2 uv = frg.texcoord;\n"
            u8"    const vec2 v00 = textureLod(g_tile, uv + vec2(-texel_size.x, -texel_size.y), 0).rg;\n"
            u8"    const vec2 v01 = textureLod(g_tile, uv + vec2(0.f, -texel_size.y), 0).rg;\n"
            u8"    const vec2 v02 = textureLod(g_tile, uv + vec2(texel_size.x, -texel_size.y), 0).rg;\n"
            u8"    const vec2 v10 = textureLod(g_tile, uv + vec2(-texel_size.x, 0.f), 0).rg;\n"
            u8"    const vec2 v11 = textureLod(g_tile, uv + vec2(0.f, 0.f), 0).rg;\n"
            u8"    const vec2 v12 = textureLod(g_tile, uv + vec2(texel_size.x, 0.f), 0).rg;\n"
            u8"    const vec2 v20 = textureLod(g_tile, uv + vec2(-texel_size.x, texel_size.y), 0).rg;\n"
            u8"    const vec2 v21 = textureLod(g_tile, uv + vec2(0.f, texel_size.y), 0).rg;\n"
            u8"    const vec2 v22 = textureLod(g_tile, uv + vec2(texel_size.x, texel_size.y), 0).rg;\n"
            u8"    //depthはmin。\n"
            u8"    result.r = min(v00.r, v01.r);\n"
            u8"    result.r = min(result.r, v02.r);\n"
            u8"    result.r = min(result.r, v10.r);\n"
            u8"    result.r = min(result.r, v11.r);\n"
            u8"    result.r = min(result.r, v12.r);\n"
            u8"    result.r = min(result.r, v20.r);\n"
            u8"    result.r = min(result.r, v21.r);\n"
            u8"    result.r = min(result.r, v22.r);\n"
            u8"    //cocはmax。\n"
            u8"    result.g = max(v00.g, v01.g);\n"
            u8"    result.g = max(result.g, v02.g);\n"
            u8"    result.g = max(result.g, v10.g);\n"
            u8"    result.g = max(result.g, v11.g);\n"
            u8"    result.g = max(result.g, v12.g);\n"
            u8"    result.g = max(result.g, v20.g);\n"
            u8"    result.g = max(result.g, v21.g);\n"
            u8"    result.g = max(result.g, v22.g);\n"
            u8"}\n";
        shader_data[1].length = 1741;

        program[0].compile_program(shader_data, 2);

        GLProgram* program = &this->program[1];
        for (int32_t i = 0; i < 2; i++) {
            int32_t common_length = 0;
            int32_t shader_length = 0;

            memcpy_s(buf, buf_len, "#version 430\n", 13);
            common_length += 13;

            memcpy_s(buf + common_length, buf_len - common_length, "#define USE_F2_COC (", 20);
            common_length += 20;

            buf[common_length++] = (char)('0' + i);

            memcpy_s(buf + common_length, buf_len - common_length, ")\n", 2);
            common_length += 2;

            memcpy_s(buf + common_length, buf_len - common_length,
                u8"layout(binding = 0) uniform Common{\n"
                u8"    uniform vec4 g_depth_params;//x=(n-f)/(nf), y=1/n, z=coc_from_z_scale, w=coc_from_z_offset\n"
                u8"    uniform vec4 g_spread_scale;//x=scale_from_meter_to_pixel, y=scale_from_meter_to_sample, z=scale_from_pi"
                u8"xel_to_sample, w=scale_from_sample_to_pixel\n"
                u8"    uniform vec4 g_depth_params2;//x=distance_to_focus_m, y=focus_range, z=k/(fuzzing_range*fuzzing_range), "
                u8"w=max_coc_radius_in_pixel   //yzw=for_f2\n"
                u8"};\n"
                u8"#define SCALE_FROM_METER_TO_PIXEL g_spread_scale.x\n"
                u8"#define SCALE_FROM_METER_TO_SAMPLE g_spread_scale.y\n"
                u8"#define SCALE_FROM_PIXEL_TO_SAMPLE g_spread_scale.z\n"
                u8"#define SCALE_FROM_SAMPLE_TO_PIXEL g_spread_scale.w\n"
                u8"#define SAMPLE_DIVISION (7)\n"
                u8"#define SAMPLE_COUNT (SAMPLE_DIVISION * SAMPLE_DIVISION)\n"
                u8"#define PI 3.1415926535f\n"
                u8"#define DOF_SINGLE_PIXEL_RADIUS (0.5f * 1.414213562f)\n"
                u8"#define DOF_DEPTH_SCALE_FOREGROUND 0.f//0で前ボケをあきらめる//0.05f//小さいと前景よりに。\n"
                u8"#define MAX_COC_RADIUS_PIXEL (8)\n"
                u8"float calculate_depth_m_from_value_in_zbuffer(const float value_in_zbuffer){\n"
                u8"    return 1.f / (value_in_zbuffer * g_depth_params.x + g_depth_params.y);//線形化。\n"
                u8"}\n"
                u8"vec4 calculate_depth_m_from_value_in_zbuffer(const vec4 value_in_zbuffer){\n"
                u8"    return vec4(1.f) / (value_in_zbuffer * g_depth_params.xxxx + g_depth_params.yyyy);\n"
                u8"}\n"
                u8"float clamp_coc_pixel(const float radius_pixel){\n"
                u8"    return min(radius_pixel, MAX_COC_RADIUS_PIXEL);\n"
                u8"    //return clamp(radius_pixel, -MAX_COC_RADIUS_PIXEL, MAX_COC_RADIUS_PIXEL);\n"
                u8"}\n"
                u8"#if USE_F2_COC\n"
                u8"float calculate_coc_pixel_from_depth(const float linear_depth){\n"
                u8"    float depth_dist = max(abs(g_depth_params2.x - linear_depth) - g_depth_params2.y, 0.f);\n"
                u8"    float dof_ratio = 1.f - exp(depth_dist * depth_dist * g_depth_params2.z);\n"
                u8"    return dof_ratio * g_depth_params2.w;\n"
                u8"}\n"
                u8"float calculate_coc_pixel_from_value_in_zbuffer(const float value_in_zbuffer){\n"
                u8"    float d = calculate_depth_m_from_value_in_zbuffer(value_in_zbuffer);\n"
                u8"    return calculate_coc_pixel_from_depth(d);\n"
                u8"}\n"
                u8"#else\n"
                u8"const float TEST_SCALE = 10.f;\n"
                u8"float calculate_coc_pixel_from_value_in_zbuffer(const float value_in_zbuffer){\n"
                u8"    return clamp_coc_pixel(abs(value_in_zbuffer * g_depth_params.z + g_depth_params.w) * 1000.f * TEST_SCALE"
                u8");//適当。1[m]=100[pixel]と換算。@todo    適正化。\n"
                u8"}\n"
                u8"float calculate_coc_pixel_from_depth(const float linear_depth){\n"
                u8"    float z = (1.f / linear_depth - g_depth_params.y) / g_depth_params.x;//0割りあり得るなぁ。@todo  事前に計算をまとめる。\n"
                u8"    return calculate_coc_pixel_from_value_in_zbuffer(z);\n"
                u8"}\n"
                u8"#endif\n"
                u8"float calculate_sample_alpha(const float sample_coc_in_pixel){\n"
                u8"    return\n"
                u8"        1.f / (PI * max(DOF_SINGLE_PIXEL_RADIUS * DOF_SINGLE_PIXEL_RADIUS, sample_coc_in_pixel * sample_coc_"
                u8"in_pixel));//CoCの面積の逆数。面積で割ることでエネルギーが保存される。\n"
                u8"}\n"
                u8"float calculate_background_depth_weight(const float depth, const float tile_min_depth){//1で背景。0で前景。\n"
                u8"    float d = DOF_DEPTH_SCALE_FOREGROUND * (depth - tile_min_depth);\n"
                u8"    return smoothstep(0.f, 1.f, d);\n"
                u8"}\n", 3025);
            common_length += 3024;

            memcpy_s(buf + common_length, buf_len - common_length,
                u8"layout(binding = 0) uniform sampler2D g_depth;\n"
                u8"layout(location = 0) out vec2 result;//r=min_depth_m, g=max_coc_pixel\n"
                u8"in VertexData{\n"
                u8"    vec2 texcoord;\n"
                u8"}frg;\n"
                u8"void main(){\n"
                u8"    //タイル化。r=最小深度, g=最大CoC。\n"
                u8"#define N (20)\n"
                u8"    ivec2 texture_size = textureSize(g_depth, 0);\n"
                u8"    const vec2 step = vec2(1.f / float(texture_size.x), 1.f / float(texture_size.y));\n"
                u8"    vec2 lt_uv = frg.texcoord + (-N / 2.f + 0.5f) * step;\n"
                u8"    vec2 uv = lt_uv;\n"
                u8"    float min_z = 1.f;\n"
                u8"    float max_coc = 0.f;\n"
                u8"    for(int i = 0; i < N; ++ i){\n"
                u8"        uv.x = lt_uv.x;\n"
                u8"        for(int j = 0; j < N; ++ j){//@todo tehxtureGatherOffsetsでループ回数を減らしてみよ。\n"
                u8"            const float z = textureLod(g_depth, uv, 0).r;\n"
                u8"            if(z < min_z){\n"
                u8"                min_z = z;\n"
                u8"            }\n"
                u8"            const float coc = calculate_coc_pixel_from_value_in_zbuffer(z);\n"
                u8"            if(max_coc < coc){\n"
                u8"                max_coc = coc;\n"
                u8"            }\n"
                u8"            uv.x += step.x;\n"
                u8"        }\n"
                u8"        uv.y += step.y;\n"
                u8"    }\n"
                u8"    result.r = calculate_depth_m_from_value_in_zbuffer(min_z);//線形化。\n"
                u8"    result.g = max_coc;\n"
                u8"    //result.g = 8.f<=max_coc?1.f:0.f;\n"
                u8"}\n", 1154);
            shader_length = 1153;

            shader_data[1].type = GL_FRAGMENT_SHADER;
            shader_data[1].string = buf;
            shader_data[1].length = common_length + shader_length;

            program[0].compile_program(shader_data, 2);

            memcpy_s(buf + common_length, buf_len - common_length,
                u8"layout(binding = 0) uniform sampler2D g_depth_point_sampler;\n"
                u8"layout(binding = 1) uniform sampler2D g_color_linear_sampler;\n"
                u8"layout(binding = 2) uniform sampler2D g_tile_sampler;\n"
                u8"layout(location = 0) out vec3 result_prefilter;\n"
                u8"layout(location = 1) out vec3 result_presort;\n"
                u8"in VertexData{\n"
                u8"    vec2 texcoord;\n"
                u8"}frg;\n"
                u8"float fetch_max_depth(in const vec2 texcoord){\n"
                u8"    vec4 z = textureGather(g_depth_point_sampler, texcoord, 0);//bilinearのフットプリントで、4点をサンプル。\n"
                u8"    return max(max(z.x, z.y), max(z.z, z.w));//最も遠い深度を選択。\n"
                u8"}\n"
                u8"float calculate_min_bilateral_weight(\n"
                u8"    in const vec2 texcoord,\n"
                u8"    in const float depth_center,\n"
                u8"    in const float side_center){//ここでdepth_centerが前景なら、前景サンプルのみを使うように。depth_centerが背景なら背景サンプルのみを使うように。\n"
                u8"    const float depth_scale = 1.f;\n"
                u8"#if 1\n"
                u8"    vec4 z = calculate_depth_m_from_value_in_zbuffer(textureGather(g_depth_point_sampler, texcoord, 0));//非線形のまま、差が最大のものを選択した方が良いかも。\n"
                u8"    z -= vec4(depth_center);\n"
                u8"    z = abs(z);\n"
                u8"    float d = max(max(z.x, z.y), max(z.z, z.w));//最も離れた深度を選択。\n"
                u8"    return 1.f / (d * depth_scale + 1.f);\n"
                u8"#elif 0//おもいぞ!\n"
                u8"    vec4 z = textureGather(g_depth_point_sampler, texcoord, 0);\n"
                u8"    float max_z = calculate_depth_m_from_value_in_zbuffer(max(max(z.x, z.y), max(z.z, z.w)));//最も遠い深度を選択。\n"
                u8"    if(0.f < side_center * (max_z - g_depth_params2.x)){//中央タップと同じ側(前景,背景)にある。\n"
                u8"        z = calculate_depth_m_from_value_in_zbuffer(z);\n"
                u8"        z -= vec4(depth_center);\n"
                u8"        z = abs(z);\n"
                u8"        float d = max(max(z.x, z.y), max(z.z, z.w));//最も離れた深度を選択。\n"
                u8"        return 1.f / (d * depth_scale + 1.f);\n"
                u8"    }else{\n"
                u8"        return 0.f;\n"
                u8"    }\n"
                u8"#else//おもいぞ!\n"
                u8"    vec4 z = calculate_depth_m_from_value_in_zbuffer(textureGather(g_depth_point_sampler, texcoord, 0));\n"
                u8"    vec4 d = z - vec4(depth_center);\n"
                u8"    d = abs(d);//最大の成分を選ぶ。\n"
                u8"    float rz, rd;\n"
                u8"    if(d.x < d.y){//y, z, w\n"
                u8"        if(d.y < d.z){//z, w\n"
                u8"            if(d.z < d.w){//w\n"
                u8"                rz = z.w;\n"
                u8"                rd = d.w;\n"
                u8"            }else{//z\n"
                u8"                rz = z.z;\n"
                u8"                rd = d.z;\n"
                u8"            }\n"
                u8"        }else if(d.y < d.w){//w\n"
                u8"            rz = z.w;\n"
                u8"            rd = d.w;\n"
                u8"        }else{//y\n"
                u8"            rz = z.y;\n"
                u8"            rd = d.y;\n"
                u8"        }\n"
                u8"    }else if(d.x < d.z){//z, w\n"
                u8"        if(d.z < d.w){//w\n"
                u8"            rz = z.w;\n"
                u8"            rd = d.w;\n"
                u8"        }else{//z\n"
                u8"            rz = z.z;\n"
                u8"            rd = d.z;\n"
                u8"        }\n"
                u8"    }else if(d.x < d.w){//w\n"
                u8"        rz = z.w;\n"
                u8"        rd = d.w;\n"
                u8"    }else{//x\n"
                u8"        rz = z.x;\n"
                u8"        rd = d.x;\n"
                u8"    }\n"
                u8"    return (0.f <= (rz - g_depth_params2.x) * side_center) ? (1.f / (rd * depth_scale + 1.f)) : 0.f;//中央タップと側(前景, 背景)が異なる場合は重み0。\n"
                u8"#endif\n"
                u8"}\n"
                u8"float luminace_from_rgb(in const vec3 rgb){\n"
                u8"    return dot(vec3(0.3f, 0.59f, 0.11f), rgb);\n"
                u8"}\n"
                u8"vec3 calculate_karis_average(in const vec3 color, const float sharpness){\n"
                u8"    return color * 1.f / (1.f + (1.f - sharpness) * luminace_from_rgb(color));\n"
                u8"}\n"
                u8"//9点サンプルして深度を重みにしたバイラテラルフィルタで合成。\n"
                u8"vec3 prefilter(in const float coc_center_pixel, in const float depth_center){\n"
                u8"    const float side_center = depth_center - g_depth_params2.x;//前景->side_center<0, 背景->0<side_center。\n"
                u8"    //concentric_mapping\n"
                u8"    const float tw = 1280.f;\n"
                u8"    const float th = 720.f;\n"
                u8"    const vec2 offsets[8] = {//半径1[pixel]。中央タップは別計算。\n"
                u8"        vec2(-0.7071066f / tw, -0.7071068f / th),\n"
                u8"        vec2(        0.f / tw,        -1.f / th),\n"
                u8"        vec2(  0.707107f / tw, -0.7071065f / th),\n"
                u8"        vec2(       -1.f / tw,         0.f / th),\n"
                u8"        vec2(        1.f / tw,         0.f / th),\n"
                u8"        vec2(-0.7071067f / tw,  0.7071067f / th),\n"
                u8"        vec2(        0.f / tw,         1.f / th),\n"
                u8"        vec2( 0.7071067f / tw,  0.7071067f / th),\n"
                u8"    };\n"
                u8"    const float radius_in_pixel = coc_center_pixel / 3.f;//3で割るの理由は、cocは49タップでの広さでこのタップ間の広さは1/3だからだ。ここではタップ間を9タップで補間する。\n"
                u8"    //中央タップ。\n"
                u8"    vec3 c_t = textureLod(g_color_linear_sampler, frg.texcoord, 0.f).rgb;\n"
                u8"    float w_t = 1.f;//中央タップは重み1。深度差0のため。\n"
                u8"    //近傍タップ。\n"
                u8"    for(int i = 0; i < 8; ++ i){\n"
                u8"        const vec2 uv = frg.texcoord + offsets[i] * radius_in_pixel;\n"
                u8"        float w = calculate_min_bilateral_weight(uv, depth_center, side_center);//depth, gather4, 最小のバイラテラル重みを算出。\n"
                u8"        //c_t += calculate_karis_average(textureLod(g_color_linear_sampler, uv, 0.f).rgb, 0.5f) * w;//それを重みとしてカラーを累積。\n"
                u8"        c_t += textureLod(g_color_linear_sampler, uv, 0.f).rgb * w;//それを重みとしてカラーを累積。\n"
                u8"        w_t += w;//重みを累積。\n"
                u8"    }\n"
                u8"    c_t *= 1.f / w_t;\n"
                u8"    c_t = pow(c_t, vec3(2.2f));\n"
                u8"    return c_t;\n"
                u8"}\n"
                u8"vec3 presort(const float coc_center_pixel, const float depth_center){\n"
                u8"    const vec2 tile = textureLod(g_tile_sampler, frg.texcoord, 0).rg;\n"
                u8"    const float tile_min_depth = tile.r;\n"
                u8"    const float sample_alpha = calculate_sample_alpha(coc_center_pixel);\n"
                u8"    const float background_depth_weight = calculate_background_depth_weight(depth_center, tile_min_depth);\n"
                u8"    const float foreground_depth_weight = 1.f - background_depth_weight;\n"
                u8"    return vec3(\n"
                u8"        coc_center_pixel * SCALE_FROM_PIXEL_TO_SAMPLE,\n"
                u8"        sample_alpha * background_depth_weight,\n"
                u8"        sample_alpha * foreground_depth_weight);\n"
                u8"}\n"
                u8"void main(){\n"
                u8"    //中央タップのzを求める。\n"
                u8"    const float z_center = fetch_max_depth(frg.texcoord);//バイリニアのフットプリント上で、最も遠い深度を選択。//value_in_zbuffer\n"
                u8"    const float depth_center = calculate_depth_m_from_value_in_zbuffer(z_center);\n"
                u8"    const float coc_center_pixel = calculate_coc_pixel_from_value_in_zbuffer(z_center);\n"
                u8"    //prefilter.\n"
                u8"    result_prefilter = prefilter(coc_center_pixel, depth_center);\n"
                u8"    //presort.\n"
                u8"    result_presort = presort(coc_center_pixel, depth_center);\n"
                u8"}\n", 6219);
            shader_length = 6218;

            shader_data[1].type = GL_FRAGMENT_SHADER;
            shader_data[1].string = buf;
            shader_data[1].length = common_length + shader_length;

            program[1].compile_program(shader_data, 2);

            memcpy_s(buf + common_length, buf_len - common_length,
                u8"layout(binding = 0) uniform sampler2D g_presort;//r=coc[sample], g=background_weight(=depth_cmp*sample_alpha"
                u8"), b=foreground_weight(=depth_cmp*sample_alpha)\n"
                u8"layout(binding = 1) uniform sampler2D g_color;\n"
                u8"layout(binding = 2) uniform sampler2D g_tile;//r=min_depth_m, g=max_coc_pixel\n"
                u8"layout(binding = 1) uniform Texcoords{\n"
                u8"    vec2 g_offsets[SAMPLE_COUNT];\n"
                u8"};\n"
                u8"layout(location = 0) out vec3 result_color;\n"
                u8"layout(location = 1) out float result_alpha;\n"
                u8"in VertexData{\n"
                u8"    vec2 texcoord;\n"
                u8"}frg;\n"
                u8"#define DOF_SPREAD_TOW_POWER (4.f)\n"
                u8"void main(){\n"
                u8"    vec4 background = vec4(0.f);\n"
                u8"    vec4 foreground = vec4(0.f);\n"
                u8"    float v = 0.f;\n"
                u8"    const vec2 tile = textureLod(g_tile, frg.texcoord, 0).rg;\n"
                u8"    const float tile_max_coc_pixel = tile.g;\n"
                u8"    const float tile_min_depth = tile.r;\n"
                u8"    float coc_pixel_sum = 0.f;\n"
                u8"    for(int i = 0; i < SAMPLE_COUNT; ++ i){\n"
                u8"        vec2 offset_coc_pixel = g_offsets[i].xy;//[pixel]\n"
                u8"        float offset_max_radius_pixel = (SAMPLE_DIVISION - 1) / 2 * SCALE_FROM_SAMPLE_TO_PIXEL;\n"
                u8"        offset_coc_pixel /= offset_max_radius_pixel;//正規化。\n"
                u8"        offset_coc_pixel *= clamp_coc_pixel(tile_max_coc_pixel);\n"
                u8"        vec2 uv_offset;\n"
                u8"        uv_offset.x = offset_coc_pixel.x / 1280.f;\n"
                u8"        uv_offset.y = offset_coc_pixel.y / 720.f;\n"
                u8"        vec2 uv = frg.texcoord + uv_offset;\n"
                u8"        const vec3 color = textureLod(g_color, uv, 0).rgb;\n"
                u8"        const vec3 presort = textureLod(g_presort, uv, 0).rgb;\n"
                u8"        const float sample_coc_sample = clamp_coc_pixel(presort.r * SCALE_FROM_SAMPLE_TO_PIXEL) * SCALE_FROM"
                u8"_PIXEL_TO_SAMPLE;\n"
                u8"        const float offset_coc_sample = length(offset_coc_pixel * SCALE_FROM_PIXEL_TO_SAMPLE);//[sample]\n"
                u8"        const float spread_cmp = clamp(sample_coc_sample - offset_coc_sample + 1.f, 0.f, 1.f);\n"
                u8"        const float spread_toe = (offset_coc_sample <= 1.f) ? pow(spread_cmp, DOF_SPREAD_TOW_POWER) : spread"
                u8"_cmp;//toeの適用は先頭サンプル(=中央タップ)だけで良い。\n"
                u8"        const float sample_back_weight = spread_toe * presort.g;\n"
                u8"        const float sample_fore_weight = spread_toe * presort.b;\n"
                u8"        background.rgb += color * sample_back_weight;\n"
                u8"        background.a += sample_back_weight;\n"
                u8"        foreground.rgb += color * sample_fore_weight;\n"
                u8"        foreground.a += sample_fore_weight;\n"
                u8"        coc_pixel_sum += presort.r;\n"
                u8"    }\n"
                u8"    //スライドではこう↓だが、↓↓のようにtile_min_depthから求めたsample_alpha()で割るべきだろう。\n"
                u8"    const float alpha = clamp(2.f * (1.f / SAMPLE_COUNT) * (1.f / calculate_sample_alpha(tile_max_coc_pixel)"
                u8") * foreground.a, 0.f, 1.f);\n"
                u8"    //const float alpha = clamp(2.f * (1.f / SAMPLE_COUNT) * (1.f / calculate_sample_alpha(calculate_coc_pix"
                u8"el_from_depth(tile_min_depth))) * foreground.a, 0.f, 1.f);\n"
                u8"    const float epsilon = 0.0001f;\n"
                u8"    result_color = mix(\n"
                u8"        background.rgb / max(background.a, epsilon),\n"
                u8"        foreground.rgb / max(foreground.a, epsilon),\n"
                u8"        alpha);\n"
                u8"    result_alpha = alpha;\n"
                u8"    //result_alpha = coc_pixel_sum / SAMPLE_COUNT / tile_max_coc_pixel;\n"
                u8"}\n", 3015);
            shader_length = 3014;

            shader_data[1].type = GL_FRAGMENT_SHADER;
            shader_data[1].string = buf;
            shader_data[1].length = common_length + shader_length;

            program[2].compile_program(shader_data, 2);

            memcpy_s(buf + common_length, buf_len - common_length,
                u8"layout(binding = 0) uniform sampler2D g_filtered_rgb_sampler;\n"
                u8"layout(binding = 1) uniform sampler2D g_filtered_a_sampler;\n"
                u8"layout(binding = 2) uniform sampler2D g_tile_sampler;\n"
                u8"layout(binding = 3) uniform sampler2D g_full_res_color_sampler;\n"
                u8"layout(binding = 4) uniform sampler2D g_full_res_depth_sampler;\n"
                u8"layout(location = 0) out vec4 result;\n"
                u8"in VertexData{\n"
                u8"    vec2 texcoord;\n"
                u8"}frg;\n"
                u8"void sort3(\n"
                u8"    out vec4 out_min,\n"
                u8"    out vec4 out_mid,\n"
                u8"    out vec4 out_max,\n"
                u8"    const in vec4 p0,\n"
                u8"    const in vec4 p1,\n"
                u8"    const in vec4 p2){\n"
                u8"    vec4 a = min(p1, p2);\n"
                u8"    vec4 b = max(p1, p2);\n"
                u8"    vec4 c = max(p0, a);\n"
                u8"    out_min = min(p0, a);\n"
                u8"    out_mid = min(b, c);\n"
                u8"    out_max = max(b, c);\n"
                u8"}\n"
                u8"vec4 median9(\n"
                u8"    const in vec4 p0,\n"
                u8"    const in vec4 p1,\n"
                u8"    const in vec4 p2,\n"
                u8"    const in vec4 p3,\n"
                u8"    const in vec4 p4,\n"
                u8"    const in vec4 p5,\n"
                u8"    const in vec4 p6,\n"
                u8"    const in vec4 p7,\n"
                u8"    const in vec4 p8){\n"
                u8"    vec4 min012, mid012, max012;\n"
                u8"    sort3(min012, mid012, max012, p0, p1, p2);\n"
                u8"    vec4 min345, mid345, max345;\n"
                u8"    sort3(min345, mid345, max345, p3, p4, p5);\n"
                u8"    vec4 min678, mid678, max678;\n"
                u8"    sort3(min678, mid678, max678, p6, p7, p8);\n"
                u8"    vec4 a = max(max(min012, min345), min678);\n"
                u8"    vec4 c = min(min(max678, max345), max012);\n"
                u8"    vec4 t0, b, t1;\n"
                u8"    sort3(t0, b, t1, mid012, mid345, mid678);\n"
                u8"    vec4 m;\n"
                u8"    sort3(t0, m, t1, a, b, c);\n"
                u8"    return m;\n"
                u8"}\n"
                u8"vec3 tone_map(in const vec3 color, const float exposure){\n"
                u8"    return pow(color, vec3(1.f / 2.2f));\n"
                u8"}\n"
                u8"vec4 fetch_half(in const vec2 texcoord){\n"
                u8"    return vec4(\n"
                u8"        textureLod(g_filtered_rgb_sampler, texcoord, 0.f).rgb,\n"
                u8"        textureLod(g_filtered_a_sampler, texcoord, 0.f).r);\n"
                u8"}\n"
                u8"vec4 upsample(){\n"
                u8"    //3x3タップのメディアンフィルタを適用。\n"
                u8"    vec2 offset = vec2(1.f / 1280.f, 1.f / 720.f);\n"
                u8"    vec4 p0 = fetch_half(frg.texcoord + vec2(-offset.x, -offset.y));\n"
                u8"    vec4 p1 = fetch_half(frg.texcoord + vec2(      0.f, -offset.y));\n"
                u8"    vec4 p2 = fetch_half(frg.texcoord + vec2( offset.x, -offset.y));\n"
                u8"    vec4 p3 = fetch_half(frg.texcoord + vec2(-offset.x, 0.f));\n"
                u8"    vec4 p4 = fetch_half(frg.texcoord + vec2(      0.f, 0.f));\n"
                u8"    vec4 p5 = fetch_half(frg.texcoord + vec2( offset.x, 0.f));\n"
                u8"    vec4 p6 = fetch_half(frg.texcoord + vec2(-offset.x, offset.y));\n"
                u8"    vec4 p7 = fetch_half(frg.texcoord + vec2(      0.f, offset.y));\n"
                u8"    vec4 p8 = fetch_half(frg.texcoord + vec2( offset.x, offset.y));\n"
                u8"    return median9(p0, p1, p2, p3, p4, p5, p6, p7, p8);\n"
                u8"}\n"
                u8"void main(){\n"
                u8"    vec4 full_res = textureLod(g_full_res_color_sampler, frg.texcoord, 0.f);\n"
                u8"    full_res.rgb = pow(full_res.rgb, vec3(2.2f));\n"
                u8"    vec4 half_res = upsample();\n"
                u8"    half_res.a = textureLod(g_filtered_a_sampler, frg.texcoord, 0.f).r;//test\n"
                u8"    float full_z = textureLod(g_full_res_depth_sampler, frg.texcoord, 0.f).r;\n"
                u8"    float full_depth = calculate_depth_m_from_value_in_zbuffer(full_z);\n"
                u8"    float full_coc_pixel = calculate_coc_pixel_from_value_in_zbuffer(full_z);\n"
                u8"    vec2 tile = textureLod(g_tile_sampler, frg.texcoord, 0.f).rg;\n"
                u8"    float tile_min_depth = tile.r;\n"
                u8"    float tile_max_coc = tile.g;\n"
                u8"    float background_factor = calculate_sample_alpha(full_coc_pixel);\n"
                u8"    float foreground_factor = calculate_sample_alpha(tile_max_coc);\n"
                u8"    //float foreground_factor = calculate_sample_alpha(calculate_coc_pixel_from_depth(tile_min_depth));\n"
                u8"    float combined_factor = mix(background_factor, foreground_factor, half_res.a);\n"
                u8"    combined_factor += background_factor;\n"
                u8"#if 1\n"
                u8"    //enahance\n"
                u8"    combined_factor=1.f-combined_factor;\n"
                u8"    combined_factor*=combined_factor;\n"
                u8"    combined_factor=1.f-combined_factor;\n"
                u8"    combined_factor *= 4.f;//適当。\n"
                u8"    combined_factor -= 0.2f;//実解像度に半解像度が薄く合成される個所を捨てる。みっともないので。\n"
                u8"    combined_factor = clamp(combined_factor, 0.f, 1.f);\n"
                u8"#endif\n"
                u8"    result.rgb = mix(half_res.rgb, full_res.rgb, combined_factor);\n"
                u8"    result.rgb = max(tone_map(result.rgb, 1.f), vec3(0.f));\n"
                u8"    result.a = 1.f;\n"
                u8"}\n", 3927);
            shader_length = 3926;

            shader_data[1].type = GL_FRAGMENT_SHADER;
            shader_data[1].string = buf;
            shader_data[1].length = common_length + shader_length;

            program[3].compile_program(shader_data, 2);
            program += 4;
        }

        _operator_delete(buf);
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
        fbo[0].bind_buffer();
        glViewportDLL(0, 0, fbo[0].width, fbo[0].height);
        program[f2 ? 5 : 1].use_program();
        common_ubo.Bind(0);
        gl_state_active_bind_texture_2d(0, depth_texture);
        gl_state_bind_sampler(0, samplers[1]);
        glDrawArraysDLL(GL_TRIANGLE_STRIP, 0, 4);

        fbo[1].bind_buffer();
        glViewportDLL(0, 0, fbo[1].width, fbo[1].height);
        program[0].use_program();
        gl_state_active_bind_texture_2d(0, textures[0]);
        gl_state_bind_sampler(0, samplers[1]);
        glDrawArraysDLL(GL_TRIANGLE_STRIP, 0, 4);
        gl_state_end_event();

    }

    void DOF3::downsample(GLuint color_texture, GLuint depth_texture, bool f2) {
        gl_state_begin_event("renderer::DOF3::downsample");
        fbo[2].bind_buffer();
        glViewportDLL(0, 0, fbo[2].width, fbo[2].height);
        program[f2 ? 6 : 2].use_program();
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
        fbo[3].bind_buffer();
        glViewportDLL(0, 0, fbo[3].width, fbo[3].height);
        program[f2 ? 7 : 3].use_program();
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
        program[f2 ? 8 : 4].use_program();
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

    void DOF3::init_textures(int32_t width, int32_t height) {
        int32_t w20 = max_def(width / 20, 1);
        int32_t h20 = max_def(height / 20, 1);
        int32_t w2 = max_def(width / 2, 1);
        int32_t h2 = max_def(height / 2, 1);

        glGenTextures(6, textures);
        gl_state_bind_texture_2d(textures[0]);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_RG16F, w20, h20, 0, GL_RG, GL_HALF_FLOAT, 0);
        fbo[0].init(w20, h20, &textures[0], 1, 0);

        gl_state_bind_texture_2d(textures[1]);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_RG16F, w20, h20, 0, GL_RG, GL_HALF_FLOAT, 0);
        fbo[1].init(w20, h20, &textures[1], 1, 0);

        gl_state_bind_texture_2d(textures[2]);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, w2, h2, 0, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV, 0);
        gl_state_bind_texture_2d(textures[3]);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, w2, h2, 0, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV, 0);
        fbo[2].init(w2, h2, &textures[2], 2, 0);

        gl_state_bind_texture_2d(textures[4]);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, w2, h2, 0, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV, 0);
        gl_state_bind_texture_2d(textures[5]);
        glTexImage2DDLL(GL_TEXTURE_2D, 0, GL_R8, w2, h2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
        fbo[3].init(w2, h2, &textures[4], 2, 0);
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

    void DOF3::calculate_texcoords(vec2* data, float_t size) {
        size_t i;
        size_t j;
        float_t v6;
        float_t v7;
        float_t v8;
        float_t v9;
        float_t v11;

        const float_t t = (float_t)(1.0 / 3.0);
        size *= 3.0f;
        for (i = 0; i < 7; i++) {
            v6 = (float_t)i * t - 1.0f;
            for (j = 0; j < 7; j++) {
                v7 = (float_t)j * t - 1.0f;
                if (-v6 >= v7) {
                    if (v7 < v6) {
                        v8 = -v7;
                        v9 = (v6 / v7) + 4.0f;
                    }
                    else if (v6 == 0.0f) {
                        v8 = 0.0f;
                        v9 = 0.0f;
                    }
                    else {
                        v8 = -v6;
                        v9 = 6.0f - (v7 / v6);
                    }
                }
                else if (v6 < v7) {
                    v8 = (float_t)j * t - 1.0f;
                    v9 = v6 / v7;
                }
                else {
                    v8 = (float_t)i * t - 1.0f;
                    v9 = 2.0f - (v7 / v6);
                }
                v8 *= size;
                v11 = v9 * (float_t)(M_PI * 0.25);

                *data++ = vec2(cosf(v11), sinf(v11)) * v8;
            }
        }
    }
}
