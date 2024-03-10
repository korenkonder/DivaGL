/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "stage.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "object.hpp"
#include "render_context.hpp"
#include <Helpers.h>

enum stage_data_flags {
    STAGE_DATA_FLAG_1              = 0x01,
    STAGE_DATA_LIGHT_CHARA_AMBIENT = 0x02,
    STAGE_DATA_FLAG_4              = 0x04,
    STAGE_DATA_FLAG_8              = 0x08,
};

struct stage_data_reflect {
    reflect_refract_resolution_mode mode;
    int32_t blur_num;
    blur_filter_mode blur_filter;
};

static_assert(sizeof(stage_data_reflect) == 0x0C, "\"stage_data_reflect\" struct should have a size of 0x0C");

struct stage_data_refract {
    reflect_refract_resolution_mode mode;
};

static_assert(sizeof(stage_data_refract) == 0x04, "\"stage_data_refract\" struct should have a size of 0x04");

struct stage_data {
    const char* name;
    const char* auth_3d_name;
    int32_t object_set_id;
    object_info object_ground;
    object_info object_ring;
    object_info object_sky;
    object_info object_shadow;
    object_info object_reflect;
    object_info object_refract;
    uint32_t lens_flare_texture;
    uint32_t lens_shaft_texture;
    uint32_t lens_ghost_texture;
    float_t lens_shaft_inv_scale;
    uint32_t field_3C;
    uint32_t render_texture;
    uint32_t movie_texture;
    const char* collision_file_path;
    stage_data_reflect_type reflect_type;
    bool refract;
    stage_data_reflect* reflect_data;
    stage_data_refract* refract_data;
    stage_data_flags flags;
    float_t ring_rectangle_x;
    float_t ring_rectangle_y;
    float_t ring_rectangle_width;
    float_t ring_rectangle_height;
    float_t ring_height;
    float_t ring_out_height;
    int32_t field_84;
};

static_assert(sizeof(stage_data) == 0x88, "\"stage_data\" struct should have a size of 0x88");

struct stage {
    int32_t index;
    uint16_t counter;
    int32_t state;
    stage_data* stage_data;
    bool stage_display;
    bool lens_flare;
    bool ground;
    bool ring;
    bool sky;
    bool auth_3d_loaded;
    mat4 mat;
    float_t rot_y;
    uint32_t obj_set;
};

static_assert(sizeof(stage) == 0x68, "\"stage\" struct should have a size of 0x68");

extern size_t(FASTCALL* stage_data_handler_get_stage_data_count)() = (size_t(FASTCALL*)())0x000000014064AFC0;

static bool object_bounding_sphere_check_visibility_shadow(obj_bounding_sphere* sphere, mat4* mat);
static bool object_bounding_sphere_check_visibility_shadow_chara(obj_bounding_sphere* sphere);
static bool object_bounding_sphere_check_visibility_shadow_stage(obj_bounding_sphere* sphere);

HOOK(void, FASTCALL, stage__disp, 0x0000000140649560, stage* s) {
    if (s->state != 6 || !s->stage_display)
        return;

    mdl::DispManager& disp_manager = *::disp_manager;

    mat4 mat;
    mat4_rotate_y(s->rot_y, &mat);
    mat4_transpose(&mat, &mat);

    if (s->stage_data->object_ground.not_null() && s->ground)
        disp_manager.entry_obj_by_object_info(&mat, s->stage_data->object_ground);

    if (s->stage_data->object_ring.not_null() && s->ring)
        disp_manager.entry_obj_by_object_info(&mat, s->stage_data->object_ring);

    if (s->stage_data->object_reflect.not_null()) {
        disp_manager.set_obj_flags((mdl::ObjFlags)(mdl::OBJ_NO_TRANSLUCENCY | mdl::OBJ_REFLECT));
        disp_manager.entry_obj_by_object_info(&mat, s->stage_data->object_reflect);
        disp_manager.set_obj_flags();
    }

    if (s->stage_data->object_refract.not_null()) {
        disp_manager.set_obj_flags((mdl::ObjFlags)(mdl::OBJ_NO_TRANSLUCENCY | mdl::OBJ_REFRACT));
        disp_manager.entry_obj_by_object_info(&mat, s->stage_data->object_refract);
        disp_manager.set_obj_flags();
    }

    if (s->stage_data->object_sky.not_null() && s->sky) {
        mat4 t = s->mat;
        mat4_mul(&mat, &t, &t);
        disp_manager.entry_obj_by_object_info(&t, s->stage_data->object_sky);
    }

    if (s->stage_data->lens_flare_texture != -1 && s->lens_flare) {
        int32_t object_set_id = s->stage_data->object_set_id;
        rndr::Render* rend = render;
        rend->lens_flare_texture = object_database_get_set_texture(
            object_set_id, s->stage_data->lens_flare_texture);
        if (rend->lens_flare_texture) {
            rend->lens_shaft_texture = object_database_get_set_texture(
                object_set_id, s->stage_data->lens_shaft_texture);
            rend->lens_ghost_texture = object_database_get_set_texture(
                object_set_id, s->stage_data->lens_ghost_texture);
            rend->lens_ghost_count = 16;
            rend->lens_shaft_inv_scale = s->stage_data->lens_shaft_inv_scale;
        }
    }
}

static void stage__disp_shadow_object(object_info object, mat4* mat) {
    mdl::DispManager& disp_manager = *::disp_manager;

    for (int32_t i = SHADOW_CHARA; i < SHADOW_MAX; i++) {
        disp_manager.set_shadow_type((shadow_type_enum)i);
        disp_manager.set_culling_finc(i == SHADOW_CHARA
            ? object_bounding_sphere_check_visibility_shadow_chara
            : object_bounding_sphere_check_visibility_shadow_stage);
        disp_manager.set_obj_flags((mdl::ObjFlags)(mdl::OBJ_NO_TRANSLUCENCY | mdl::OBJ_SHADOW_OBJECT));
        disp_manager.entry_obj_by_object_info(mat, object);
    }

    disp_manager.set_obj_flags();
    disp_manager.set_culling_finc();
    disp_manager.set_shadow_type();
}


HOOK(void, FASTCALL, stage__disp_shadow, 0x00000001406497C0, stage* s) {
    if (s->state != 6 || !s->stage_display)
        return;

    mat4 mat;
    mat4_rotate_y(s->rot_y, &mat);
    mat4_transpose(&mat, &mat);
    if (s->stage_data->object_shadow.not_null())
        stage__disp_shadow_object(s->stage_data->object_shadow, &mat);
}

void stage_patch() {
    INSTALL_HOOK(stage__disp);
    INSTALL_HOOK(stage__disp_shadow);
}

static bool object_bounding_sphere_check_visibility_shadow(obj_bounding_sphere* sphere, mat4* mat) {
    mat4 view;
    mat4_transpose(&camera_data->view, &view);

    vec3 center;
    mat4_transform_point(mat, &sphere->center, &center);
    mat4_transform_point(&view, &center, &center);
    float_t radius = sphere->radius;

    Shadow* shad = shadow_ptr_get();
    float_t shadow_range = shad->get_shadow_range();
    if ((center.x + radius) < -shadow_range
        || (center.x - radius) > shadow_range
        || (center.y + radius) < -shadow_range
        || (center.y - radius) > shadow_range
        || (center.z - radius) > -shad->z_near
        || (center.z + radius) < -shad->z_far)
        return false;
    return true;
}

static bool object_bounding_sphere_check_visibility_shadow_chara(obj_bounding_sphere* sphere) {
    mat4 mat;
    Shadow* shad = shadow_ptr_get();
    mat4_look_at(&shad->view_point[0], &shad->interest[0], &mat);
    return object_bounding_sphere_check_visibility_shadow(sphere, &mat);
}

static bool object_bounding_sphere_check_visibility_shadow_stage(obj_bounding_sphere* sphere) {
    mat4 mat;
    Shadow* shad = shadow_ptr_get();
    mat4_look_at(&shad->view_point[1], &shad->interest[1], &mat);
    return object_bounding_sphere_check_visibility_shadow(sphere, &mat);
}
