/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shadow.hpp"
#include "../AFTModsShared/light_param/light.hpp"
#include "render_context.hpp"
#include "render_manager.hpp"
#include <Helpers.h>

#define MIN_SHADOW_RANGE (1.2f)
#define SHADOW_RANGE_SEPARATE (MIN_SHADOW_RANGE * 2.0f)

Shadow*& shadow_ptr = *(Shadow**)0x000000014CC587B8;

void(FASTCALL* sub_1405E8A20)(Shadow* shad, int32_t index, vec3* pos)
    = (void(FASTCALL*)(Shadow* shad, int32_t index, vec3 * pos))0x00000001405E8A20;
void(FASTCALL* shadow_ptr_free)() = (void(FASTCALL*)())0x00000001405E8610;
void(FASTCALL* shadow_ptr_init)() = (void(FASTCALL*)())0x00000001405E8890;

static void sub_1405E60E0(Shadow* shad);
static void sub_1405E63D0(Shadow* shad);
static void sub_1405E6840(Shadow* shad);
static void sub_1405E6910(Shadow* shad);
static void sub_1405E6B70(Shadow* shad);

void Shadow::ctrl() {
    for (int32_t i = 0; i < 2; i++)
        shadow_enable[i] = false;

    if (render_manager.shadow) {
        view_mat[0] = camera_data.view;
        view_mat[1] = camera_data.inv_view;

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

    sub_1405E6840(this);
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

static void sub_1405E60E0(Shadow* shad) {
    for (int32_t i = 0; i < 2; i++) {
        shad->field_1A8[i] = 0.0f;
        shad->field_1C8[i] = 0.0f;

        if (!shad->shadow_enable[i] || !shad->field_1D0[i].size())
            continue;

        vec3 v7 = 0.0f;
        for (vec3& j : shad->field_1D0[i])
            v7 += j;

        float_t v14 = (float_t)(int64_t)shad->field_1D0[i].size();
        if (v14 < 0.0f)
            v14 += (float_t)UINT64_MAX;
        v7 *= 1.0f / v14;

        float_t v15 = 0.0f;
        for (vec3& j : shad->field_1D0[i]) {
            vec3 v22 = v7 - j;
            vec3 v23 = vec3::dot(v22, shad->direction);
            float_t v24 = vec3::distance(shad->direction * v23, v22);
            v15 = max_def(v15, max_def(v24 - 0.25f, 0.0f));
        }
        shad->field_1A8[i] = v7;
        shad->field_1C8[i] = v15;
    }

    for (prj::vector<vec3>& i : shad->field_1D0)
        i.clear();
}

static void sub_1405E63D0(Shadow* shad) {
    if (shad->num_shadow <= 0)
        return;

    vec3 view_point = 0.0f;
    vec3 interest = 0.0f;
    for (int32_t i = 0; i < 2; i++) {
        if (!shad->shadow_enable[i])
            continue;

        const vec3 v11 = shad->field_1A8[i] - shad->direction * shad->field_208;
        const float_t v9 = vec3::distance(shad->view_point[i], v11);
        const float_t v12 = vec3::distance(shad->interest[i], shad->field_1A8[i]);
        if (v9 > 0.1f || v12 > 0.1f) {
            shad->view_point[i] = v11;
            shad->interest[i] = shad->field_1A8[i];
        }

        view_point += shad->view_point[i];
        interest += shad->interest[i];
    }

    shad->view_point_shared = view_point * (1.0f / (float_t)shad->num_shadow);
    shad->interest_shared = interest * (1.0f / (float_t)shad->num_shadow);
}

static void sub_1405E6840(Shadow* shad) {
    int32_t count = 0;
    shad->num_shadow = 0;
    for (int32_t i = 0; i < 2; i++) {
        if (shad->shadow_enable[i] && shad->field_1D0[i].size()) {
            ++shad->num_shadow;
            count += (int32_t)shad->field_1D0->size();
        }
        else
            shad->shadow_enable[i] = false;
    }

    if (count < 3) {
        sub_1405E60E0(shad);
        sub_1405E63D0(shad);
        sub_1405E6910(shad);
    }
    else
        sub_1405E6B70(shad);
}

static void sub_1405E6910(Shadow* shad) {
    const float_t v2 = max_def(shad->field_1C8[0], shad->field_1C8[1]);
    shad->separate = false;
    shad->shadow_range = v2 + MIN_SHADOW_RANGE;
    shad->field_200[0] = 0;
    shad->field_200[1] = 1;

    if (shad->num_shadow < 2)
        return;

    const vec3 v12 = shad->field_1A8[0] - shad->interest_shared;
    const vec3 v14 = shad->field_1A8[1] - shad->interest_shared;
    float_t v16 = vec3::distance((shad->direction * vec3::dot(v12, shad->direction)), v12);
    v16 = max_def(v16 - 0.25f, 0.0f);

    if (v2 + MIN_SHADOW_RANGE + v16 > v2 + SHADOW_RANGE_SEPARATE) {
        shad->shadow_range = v2 + SHADOW_RANGE_SEPARATE;
        shad->separate = true;
    }
    else
        shad->shadow_range = v2 + MIN_SHADOW_RANGE + v16;

    if (vec3::dot(v12, shad->direction) < vec3::dot(v14, shad->direction)) {
        shad->field_200[0] = 1;
        shad->field_200[1] = 0;
    }
}

static void sub_1405E6B70(Shadow* shad) {
    vec3 v3;
    vec3 v86;
    if (shad->direction.y * shad->direction.y < 0.99f) {
        v86 = vec3::cross(shad->direction, vec3(0.0f, 1.0f, 0.0f));
        v3 = vec3::cross(v86, shad->direction);
        v86 = vec3::normalize(v86);
        v3 = vec3::normalize(v3);
    }
    else {
        v3 = { 0.0f, 0.0f, 1.0f };
        v86 = { 1.0f, 0.0f, 0.0f };
    }

    for (int32_t i = 0; i < 2; i++) {
        shad->field_1A8[i] = 0.0f;
        shad->field_1C8[i] = 0.0f;

        if (!shad->shadow_enable[i] || !shad->field_1D0[i].size())
            continue;

        vec3 v22 = 0.0f;
        for (vec3& j : shad->field_1D0[i])
            v22 += j;

        float_t v29 = (float_t)(int64_t)shad->field_1D0[i].size();
        if (v29 < 0.0f)
            v29 += (float_t)UINT64_MAX;
        v22 *= 1.0f / v29;

        float_t v30 = 0.0f;
        for (vec3& j : shad->field_1D0[i]) {
            const vec3 v35 = v22 - j;
            const float_t v39 = fabsf(vec3::dot(v35, v86));
            const float_t v38 = fabsf(vec3::dot(v35, v3));
            v30 = max_def(v30, max_def(v38, v39));
        }
        shad->field_1A8[i] = v22;
        shad->field_1C8[i] = v30;
    }

    if (shad->num_shadow > 0) {
        vec3 view_point = 0.0f;
        vec3 interest = 0.0f;
        for (int32_t i = 0; i < 2; i++) {
            if (!shad->shadow_enable[i])
                continue;

            const vec3 v41 = shad->field_1A8[0] - shad->direction * shad->field_208;
            const float_t v42 = vec3::distance(shad->view_point[0], v41);
            const float_t v45 = vec3::distance(shad->interest[0], shad->field_1A8[0]);
            if (v42 > 0.1f || v45 > 0.1f) {
                shad->view_point[0] = v41;
                shad->interest[0] = shad->field_1A8[0];
            }

            view_point += shad->view_point[i];
            interest += shad->interest[i];
        }

        shad->view_point_shared = view_point * (1.0f / (float_t)shad->num_shadow);
        shad->interest_shared = interest * (1.0f / (float_t)shad->num_shadow);
    }

    const float_t v67 = max_def(shad->field_1C8[0], shad->field_1C8[1]);
    shad->separate = 0;
    shad->shadow_range = v67 + MIN_SHADOW_RANGE;
    shad->field_200[0] = 0;
    shad->field_200[1] = 1;

    if (shad->num_shadow >= 2) {
        float_t v2 = 0.0f;
        float_t v68 = 0.0f;
        float_t v69 = 0.0f;
        float_t v70 = 0.0f;
        for (int32_t i = 0; i < 2; i++) {
            if (!shad->shadow_enable[i])
                continue;

            for (vec3& j : shad->field_1D0[i]) {
                const vec3 v74 = j - shad->interest_shared;

                const float_t v77 = vec3::dot(v86, v74);
                if (v2 > v77)
                    v2 = v77;
                else if (v69 < v77)
                    v69 = v77;

                const float_t v78 = vec3::dot(v3, v74);
                if (v68 > v78)
                    v68 = v78;
                else if (v70 < v78)
                    v70 = v78;
            }
        }

        const float_t v79 = max_def(max_def(max_def(-v2, v69), -v68), v70);

        if (v79 + MIN_SHADOW_RANGE > v67 + SHADOW_RANGE_SEPARATE) {
            shad->shadow_range = v67 + SHADOW_RANGE_SEPARATE;
            shad->separate = true;
        }
        else
            shad->shadow_range = v79 + MIN_SHADOW_RANGE;

        if (vec3::dot(shad->field_1A8[0] - shad->interest_shared, shad->direction)
            < vec3::dot(shad->field_1A8[1] - shad->interest_shared, shad->direction)) {
            shad->field_200[1] = 0;
            shad->field_200[0] = 1;
        }
    }

    for (prj::vector<vec3>& i : shad->field_1D0)
        i.clear();
}
