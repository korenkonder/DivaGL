/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shader_table.hpp"
#include "../KKdLib/mat.hpp"
#include "render_context.hpp"
#include "render_texture.hpp"
#include "shader_glsl_ft.hpp"
#include "types.hpp"
#include "wrap.hpp"

Shader* shaders = (Shader*)0x000000014CC57C10;
ShaderTable* shader_table = (ShaderTable*)0x0000000140A41D20;
ShaderBindFunc* shader_name_bind_func_table = (ShaderBindFunc*)0x0000000140A40F50;
int32_t* current_vp = (int32_t*)0x000000014CC587B0;
int32_t* current_fp = (int32_t*)0x000000014CC587B4;

void FASTCALL shader_set(ShaderName name) {
    if (!name) {
        shader_unbind();
        return;
    }

    glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 24,
        (float)uniform->arr[U_TEXTURE_BLEND], 0.0, 0.0, 0.0);

    Shader* shad = &shaders[name];
    if (shad->bind_func)
        shad->bind_func(shad);
    else
        shader_bind(shad, shad->sub->sub_name);
}

void FASTCALL shader_bind_blinn(Shader* shad) {
    shader_bind(shad, uniform->arr[U_NORMAL] ? BLINN_FRAG : BLINN_VERT);
}

void FASTCALL shader_bind_cloth(Shader* shad) {
    shader_bind(shad, uniform->arr[U_NPR] ? CLOTH_NPR1
        : (uniform->arr[U_ANISO] ? CLOTH_ANISO : CLOTH_DEFAULT));
}

void FASTCALL shader_bind_hair(Shader* shad) {
    shader_bind(shad, uniform->arr[U_NPR] ? HAIR_NPR1
        : (uniform->arr[U_ANISO] ? HAIR_ANISO : HAIR_DEFAULT));
}

void FASTCALL shader_bind_membrane(Shader* shad) {
    uniform->arr[U_MEMBRANE] = 3;
    if (shader_bind(shad, MEMBRANE) < 0)
        return;

    uint32_t(FASTCALL * sub_140192E00)() = (uint32_t(FASTCALL*)())0x0000000140192E00;
    uint32_t v1 = sub_140192E00();
    mat4 mat = mat4_identity;
    mat4_mul_rotate_x(&mat, (float_t)((v1 & 0x1FF) * (M_PI / 256.0)), &mat);
    mat4_mul_rotate_z(&mat, (float_t)((v1 % 0x168) * (M_PI / 180.0)), &mat);

    vec4 vec = { 1.0f, 0.0f, 0.0f, 0.0f };
    mat4_transform_vector(&mat, &vec, &vec);

    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 10, vec.x, vec.y, vec.z, 0.0);
}

void FASTCALL shader_bind_eye_ball(Shader* shad) {
    uniform->arr[U18] = 0;
    if (shader_bind(&shaders[_GLASEYE], GLASS_EYE) >= 0) {
        float* glass_eye = (float*)0x0000000140CA2D70;
        void (FASTCALL * glass_eye_calc)(float* glass_eye) = (void (FASTCALL*)(float* glass_eye))0x00000001405E53D0;
        void (FASTCALL * glass_eye_set)(float* glass_eye) = (void (FASTCALL*)(float* glass_eye))0x00000001405E4750;
        glass_eye_calc(glass_eye);
        glass_eye_set(glass_eye);
    }
}

void FASTCALL shader_bind_tone_map(Shader* shad) {
    shader_bind(shad, uniform->arr[U_NPR] == 1 ? TONEMAP_NPR1 : TONEMAP);
}

int32_t FASTCALL shader_bind(Shader* shad, SubShaderName name) {
    int32_t num_sub = shad->num_sub;
    int32_t subshader_index = 0;
    if (num_sub <= 0)
        return -1;

    for (SubShader* i = shad->sub; i->sub_name != name; i++) {
        subshader_index++;
        if (subshader_index >= num_sub)
            return -1;
    }

    SubShader* subshader = &shad->sub[subshader_index];
    if (!subshader)
        return -1;

    int32_t num_uniform = shad->num_uniform;
    int32_t unival_vp_curr = 1;
    int32_t unival_fp_curr = 1;
    int32_t unival_vp = 0;
    int32_t unival_fp = 0;
    if (num_uniform > 0) {
        int32_t* vp_unival_max = subshader->vp_unival_max;
        int32_t* fp_unival_max = subshader->fp_unival_max;
        for (size_t i = 0; i < num_uniform; i++) {
            int32_t unival = uniform->arr[shad->use_uniform[i]];
            unival_vp += unival_vp_curr * min_def(unival, vp_unival_max[i]);
            unival_fp += unival_fp_curr * min_def(unival, fp_unival_max[i]);
            unival_vp_curr *= vp_unival_max[i] + 1;
            unival_fp_curr *= fp_unival_max[i] + 1;
        }
    }

    int32_t vp = subshader->vp[unival_vp];
    int32_t fp = subshader->fp[unival_fp];
    if (*current_vp != vp) {
        glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vp);
        *current_vp = vp;
    }

    if (*current_fp != fp) {
        glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fp);
        *current_fp = fp;
    }

    glEnableDLL(GL_VERTEX_PROGRAM_ARB);
    glEnableDLL(GL_FRAGMENT_PROGRAM_ARB);
    return 0;
}

void FASTCALL shader_unbind() {
    if (current_vp) {
        if (*current_vp)
            glDisableDLL(GL_VERTEX_PROGRAM_ARB);
        *current_vp = 0;
    }

    if (current_fp) {
        if (*current_fp)
            glDisableDLL(GL_FRAGMENT_PROGRAM_ARB);
        *current_fp = 0;
    }
}

void FASTCALL shader_load_all_shaders() {
    {
        farc f;
        f.read("plugins\\divagl_ft_shaders.farc", true, false);
        shaders_ft.load(&f, true, "ft", shader_ft_table, shader_ft_table_size,
            shader_ft_bind_func_table, shader_ft_bind_func_table_size,
            shader_ft_get_index_by_name);
    }
    return;

    void (FASTCALL * shader_load_program_file)(GLuint program, const char* file_name, bool vertex_program)
        = (void (FASTCALL*)(GLuint program, const char* file_name, bool vertex_program))0x00000001405E52D0;

    ShaderTable* shaders_table = &shader_table[_BLINN];
    Shader* shader = &shaders[_BLINN];
    int32_t curr_shader = SHADER_END - 1;
    do {
        shader->name = shaders_table->name;
        shader->name_index = shaders_table->name_index;
        shader->num_sub = shaders_table->num_sub;
        int32_t num_sub = shaders_table->num_sub;

        const SubShaderTable* sub_table = shaders_table->sub;
        SubShader* sub = (SubShader*)_operator_new(sizeof(SubShader) * num_sub);
        memset(sub, 0, sizeof(SubShader) * num_sub);
        shader->sub = sub;
        shader->num_uniform = shaders_table->num_uniform;
        shader->use_uniform = shaders_table->use_uniform;

        for (int32_t i = 0; i < num_sub; i++, sub_table++, sub++) {
            sub->sub_name = sub_table->sub_name;
            sub->vp_unival_max = (int32_t*)sub_table->vp_unival_max;
            sub->fp_unival_max = (int32_t*)sub_table->fp_unival_max;
            sub->num_vp = sub_table->num_vp;
            sub->num_fp = sub_table->num_fp;
            sub->vp = (GLuint*)_operator_new(sizeof(GLuint) * sub_table->num_vp);
            sub->fp = (GLuint*)_operator_new(sizeof(GLuint) * sub_table->num_fp);

            glGenProgramsARB((GLsizei)sub_table->num_vp, (GLuint*)sub->vp);
            glGenProgramsARB((GLsizei)sub_table->num_fp, (GLuint*)sub->fp);

            for (int32_t j = 0; j < sub_table->num_vp; j++)
                shader_load_program_file(sub->vp[j], sub_table->vp[j], true);

            for (int32_t j = 0; j < sub_table->num_fp; j++)
                shader_load_program_file(sub->fp[j], sub_table->fp[j], false);
        }
        shader->bind_func = 0;

        ShaderBindFunc* bind_func_table = shader_name_bind_func_table;
        ShaderName bind_func_name = _BLINN;
        while (shader->name_index != bind_func_name) {
            bind_func_name = (++bind_func_table)->name_index;
            if (bind_func_table->name_index == SHADER_END)
                break;
        }
        shader->bind_func = bind_func_table->bind_func;
        shader++;
        shaders_table++;
        curr_shader--;
    } while (curr_shader);
}

void FASTCALL shader_free() {
    shaders_ft.unload();
    return;
    Shader* shader = &shaders[SHADER_FFP];
    for (int32_t i = 0; i < SHADER_END; i++, shader++) {
        if (!shader->sub)
            continue;

        for (int32_t j = 0; j < shader->num_sub; j++) {
            SubShader* sub = shader->sub;
            if (sub[j].vp) {
                glDeleteProgramsARB(sub[j].num_vp, sub[j].vp);
                _operator_delete(sub[j].vp);
                sub[j].vp = 0;
            }

            if (sub[j].fp) {
                glDeleteProgramsARB(sub[j].num_fp, sub[j].fp);
                _operator_delete(sub[j].fp);
                sub[j].fp = 0;
            }
        }

        if (shader->sub) {
            _operator_delete(shader->sub);
            shader->sub = 0;
        }
    }
}
