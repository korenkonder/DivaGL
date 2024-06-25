/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shadow.hpp"
#include "render_context.hpp"
#include "render_manager.hpp"
#include <Helpers.h>

Shadow*& shadow_ptr = *(Shadow**)0x000000014CC587B8;

void(FASTCALL* sub_1405E8A20)(Shadow* shad, int32_t index, vec3* pos)
    = (void(FASTCALL*)(Shadow* shad, int32_t index, vec3 * pos))0x00000001405E8A20;
void(FASTCALL* shadow_ptr_free)() = (void(FASTCALL*)())0x00000001405E8610;
void(FASTCALL* shadow_ptr_init)() = (void(FASTCALL*)())0x00000001405E8890;

void Shadow::ctrl() {
    for (int32_t i = 0; i < 2; i++)
        shadow_enable[i] = false;

    if (render_manager->shadow) {
        view_mat[0] = camera_data->view;
        view_mat[1] = camera_data->inv_view;

        light_data& light_chara = light_set_data[LIGHT_SET_MAIN].lights[LIGHT_CHARA];

        vec3 position;
        light_chara.get_position(position);
        float_t length = vec3::length(position);
        if (length < 0.000001f)
            direction = { 0.0f, 1.0f, 0.0f };
        else
            direction = -position * (1.0f / length);

        for (int32_t i = 0; i < 2; i++)
            if (disp_manager->get_obj_count((mdl::ObjType)(mdl::OBJ_TYPE_SHADOW_CHARA + i)))
                shadow_enable[i] = true;
    }

    int32_t count = 0;
    num_shadow = 0;
    for (int32_t i = 0; i < 2; i++)
        if (shadow_enable[i] && field_1D0[i].size() > 0) {
            num_shadow++;
            count += (int32_t)field_1D0[i].size();
        }
        else
            shadow_enable[i] = false;

    if (count < 3) {
        for (int32_t i = 0; i < 2; i++) {
            field_1A8[i] = 0.0f;
            field_1C8[i] = 0.0f;
            if (!shadow_enable[i] || field_1D0[i].size() < 1)
                continue;

            vec3 v7 = 0.0f;
            for (vec3& j : field_1D0[i])
                v7 += j;

            float_t v14 = (float_t)(int64_t)field_1D0[i].size();
            if (v14 < 0.0f)
                v14 += (float_t)UINT64_MAX;
            v7 *= 1.0f / v14;

            float_t v15 = 0.0f;
            for (vec3& j : field_1D0[i]) {
                vec3 v22 = v7 - j;
                vec3 v25 = direction * vec3::dot(v22, direction);
                float_t v24 = vec3::distance(v25, v22);
                v24 -= 0.25f;
                if (v24 < 0.0f)
                    v24 = 0.0f;
                if (v15 < v24)
                    v15 = v24;
            }
            field_1A8[i] = v7;
            field_1C8[i] = v15;
        }

        if (num_shadow > 0) {
            vec3 view_point = 0.0f;
            vec3 interest = 0.0f;
            for (int32_t i = 0; i < 2; i++) {
                if (!shadow_enable[i])
                    continue;

                vec3 v11 = field_1A8[i] - direction * field_208;
                float_t v9 = vec3::distance(this->view_point[i], v11);
                float_t v12 = vec3::distance(this->interest[i], field_1A8[i]);
                if (v9 > 0.1f || v12 > 0.1f) {
                    this->view_point[i] = v11;
                    this->interest[i] = field_1A8[i];
                }

                view_point += this->view_point[i];
                interest += this->interest[i];
            }

            view_point_shared = view_point * (1.0f / (float_t)num_shadow);
            interest_shared = interest * (1.0f / (float_t)num_shadow);
        }

        float_t v2 = max_def(field_1C8[0], field_1C8[1]);
        separate = false;
        shadow_range = v2 + 1.2f;
        field_200[0] = 0;
        field_200[1] = 1;
        if (num_shadow >= 2) {
            vec3 v12 = field_1A8[0] - interest_shared;
            vec3 v14 = field_1A8[1] - interest_shared;

            float_t v6 = vec3::length(direction * vec3::dot(v12, direction) - v12);

            float_t v16 = v6 - 0.25f;
            if (v16 < 0.0f)
                v16 = 0.0f;

            if (v16 > 1.2f) {
                shadow_range = v2 + 2.4f;
                separate = true;
            }
            else
                shadow_range = v2 + 1.2f + v16;

            if (vec3::dot(v12, direction) < vec3::dot(v14, direction)) {
                field_200[1] = 0;
                field_200[0] = 1;
            }
        }
    }
    else {
        vec3 v3;
        vec3 v86;
        if (direction.y * direction.y < 0.99f) {
            v86 = vec3::cross(direction, vec3(0.0f, 1.0f, 0.0f));
            v3 = vec3::normalize(vec3::cross(v86, direction));
            v86 = vec3::normalize(v86);
        }
        else {
            v3 = { 0.0f, 0.0f, 1.0f };
            v86 = { 1.0f, 0.0f, 0.0f };
        }

        for (int32_t i = 0; i < 2; i++) {
            field_1A8[i] = 0.0f;
            field_1C8[i] = 0.0;
            if (!shadow_enable[i] || field_1D0[i].size() < 1)
                continue;

            vec3 v22 = 0.0f;
            for (vec3& j : field_1D0[i])
                v22 += j;

            float_t v29 = (float_t)(int64_t)field_1D0[i].size();
            if (v29 < 0.0f)
                v29 += (float_t)UINT64_MAX;

            float_t v30 = 0.0f;
            vec3 v31 = v22 * (1.0f / v29);

            for (vec3& j : field_1D0[i]) {
                vec3 v34 = v31 - j;
                float_t v38 = fabsf(vec3::dot(v34, v3));
                float_t v39 = fabsf(vec3::dot(v34, v86));
                if (v39 >= v38)
                    v38 = v39;
                if (v30 < v38)
                    v30 = v38;
            }
            field_1A8[i] = v31;
            field_1C8[i] = v30;
        }

        if (num_shadow > 0) {
            for (int32_t i = 0; i < 2; i++) {
                if (!shadow_enable[i])
                    continue;

                vec3 v53 = field_1A8[i] - direction * field_208;
                float_t v51 = vec3::distance(view_point[i], v53);
                float_t v54 = vec3::distance(interest[i], field_1A8[i]);
                if (v51 > 0.1f || v54 > 0.1f) {
                    view_point[i] = v53;
                    interest[i] = field_1A8[i];
                }
            }

            vec3 view_point = 0.0f;
            vec3 interest = 0.0f;
            int32_t count = 0;
            for (int32_t i = 0; i < 2; i++) {
                int32_t c = (int32_t)field_1D0[i].size();
                view_point += this->view_point[i] * (float_t)c;
                interest += this->interest[i] * (float_t)c;
                count += c;
            }

            view_point_shared = view_point * (1.0f / (float_t)count);
            interest_shared = interest * (1.0f / (float_t)count);
        }

        float_t v2 = 0.0f;
        float_t v67 = max_def(field_1C8[0], field_1C8[1]);
        separate = false;
        shadow_range = v67 + 1.2f;
        field_200[0] = 0;
        field_200[1] = 1;
        if (num_shadow >= 2) {
            float_t v68 = 0.0f;
            float_t v69 = 0.0f;
            float_t v70 = 0.0f;
            for (int32_t i = 0; i < 2; i++) {
                if (!shadow_enable[i])
                    continue;

                for (vec3& j : field_1D0[i]) {
                    vec3 v74 = j - interest_shared;

                    float_t v77 = vec3::dot(v74, v86);
                    if (v2 > v77)
                        v2 = v77;
                    else if (v69 < v77)
                        v69 = v77;

                    float_t v78 = vec3::dot(v74, v3);
                    if (v68 > v78)
                        v68 = v78;
                    else if (v70 < v78)
                        v70 = v78;
                }
            }

            float_t v79 = -v2;
            if (v79 < v69)
                v79 = v69;
            if (v79 < -v68)
                v79 = -v68;
            if (v79 < v70)
                v79 = v70;

            if (v79 > v67 + 1.2f) {
                shadow_range = v67 + 2.4f;
                separate = true;
            }
            else
                shadow_range = v79 + 1.2f;

            if (vec3::dot(field_1A8[0] - interest_shared, direction)
                < vec3::dot(field_1A8[1] - interest_shared, direction)) {
                field_200[1] = 0;
                field_200[0] = 1;
            }
        }
    }

    for (prj::vector<vec3>& i : field_1D0)
        i.clear();
}

void Shadow::free() {
    for (RenderTexture& i : render_textures)
        i.Free();

    reset();
}

float_t Shadow::get_shadow_range() {
    return shadow_range * shadow_range_factor;
}

int32_t Shadow::init() {
    struct shadow_texture_init_params {
        int32_t width;
        int32_t height;
        int32_t max_level;
        GLenum color_format;
        GLenum depth_format;
    } init_params[] = {
        { 0x800, 0x800, 0, GL_RGBA8, GL_DEPTH_COMPONENT32F },
        { 0x200, 0x200, 3, GL_RGBA8, GL_ZERO },
        { 0x200, 0x200, 3, GL_RGBA8, GL_ZERO },
        { 0x800, 0x800, 0, GL_R32F , GL_ZERO },
        { 0x800, 0x800, 0, GL_R32F , GL_ZERO },
        { 0x200, 0x200, 0, GL_R32F , GL_ZERO },
        { 0x200, 0x200, 0, GL_R32F , GL_ZERO },
    };

    shadow_texture_init_params* v3 = init_params;
    for (int32_t i = 0; i < 7; i++, v3++)
        if (render_textures[i].Init(v3->width, v3->height,
            v3->max_level, v3->color_format, v3->depth_format) < 0)
            return -1;

    for (int32_t i = 0; i < 3; i++) {
        gl_state_bind_texture_2d(render_textures[i].GetColorTex());
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteriDLL(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        static const vec4 border_color = 1.0f;
        glTexParameterfvDLL(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&border_color);
    }
    gl_state_bind_texture_2d(0);
    gl_state_get_error();
    return 0;
}

void Shadow::reset() {
    shadow_range = 1.2f;
    shadow_range_factor = 1.0f;

    for (int32_t i = 0; i < 2; i++) {
        view_point[i] = 1.0f;
        field_1C0[i] = 0.0f;
        field_1C8[i] = 0.0f;
        field_200[i] = i;
    }

    for (RenderTexture*& i : curr_render_textures)
        i = 0;

    view_mat[0] = mat4_identity;
    view_mat[1] = mat4_identity;
    blur_filter = BLUR_FILTER_9;
    near_blur = 1;
    field_2BC = 2;
    far_blur = 1;
    distance = 4.0f;
    field_2C4 = 0.4f;
    z_near = 0.1f;
    z_far = 20.0f;
    field_2D0 = 1.4f;
    field_2D4 = 10000.0f;
    field_2D8 = 80.0f;
    field_2DC = 2.0f;
    field_2E0 = 0.05f;
    shadow_ambient = 0.4f;
    num_shadow = 0;
    direction = vec3(0.0f, -1.0f, -1.0f) * (1.0f / sqrtf(2.0f));
    show_texture = false;
    self_shadow = true;
    separate = false;
    field_208 = (z_far - z_near) * 0.5f;
}

Shadow* shadow_ptr_get() {
    return shadow_ptr;
}

HOOK(void, FASTCALL, shadow_ctrl, 0x0000000140502390) {
    shadow_ptr_get()->ctrl();
}

HOOK(void, FASTCALL, Shadow__free, 0x00000001405E8350, Shadow* This) {
    This->free();
}

void shadow_patch() {
    INSTALL_HOOK(shadow_ctrl);
    INSTALL_HOOK(Shadow__free);
}
