/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "sprite.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include <Helpers.h>

namespace spr {
    struct SpriteVertexData {
        size_t max_count;
        spr::SpriteVertex* array;
    };
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

spr::SpriteVertexData& sprite_vertex_data = *(spr::SpriteVertexData*)0x000000014CC62480;

const size_t sprite_vertex_array_max_count = 0x2000;
spr::SpriteVertex* sprite_vertex_array = (spr::SpriteVertex*)0x000000014CC62480;

size_t& sprite_vertex_array_count = *(size_t*)0x000000014CC611B0;

bool sprite_break_sprite_vertex_limit = false;

namespace spr {
    void(FASTCALL* put_cross)(const mat4& mat, color4u8 color_x, color4u8 color_y, color4u8 color_z)
        = (void(FASTCALL*)(const mat4 & mat, color4u8 color_x, color4u8 color_y, color4u8 color_z))0x00000001404BC920;
    void(FASTCALL* put_rgb_cross)(const mat4& mat) = (void(FASTCALL*)(const mat4 & mat))0x00000001404BC8F0;
    spr::SprArgs* (FASTCALL* put_sprite)(const spr::SprArgs& args)
        = (spr::SprArgs * (FASTCALL*)(const spr::SprArgs & args))0x0000000140640740;

    SprArgs::SprArgs() : kind(), attr(), blend(), index(), layer(),
        prio(), resolution_mode_screen(), resolution_mode_sprite(), texture(),
        shader(), vertex_array(), num_vertex(), flags(), field_CC(), next() {
        Reset();
    }

    SpriteVertex* SprArgs::GetVertexArray() {
        if (sprite_break_sprite_vertex_limit)
            return sprite_vertex_data.array + vertex_array;
        else
            return vertex_array_ptr;
    }

    void SprArgs::Reset() {
        kind = SPR_KIND_NORMAL;
        id = {};
        color = 0xFFFFFFFF;
        attr = (SprAttr)0;
        blend = 0;
        index = -1;
        layer = 0;
        prio = SPR_PRIO_DEFAULT;
        resolution_mode_screen = RESOLUTION_MODE_HD;
        resolution_mode_sprite = RESOLUTION_MODE_HD;
        center = 0.0f;
        trans = 0.0f;
        scale = 1.0f;
        rot = 0.0f;
        skew_angle = 0.0f;
        mat = mat4_identity;
        texture = 0;
        shader = SHADER_FFP;
        if (sprite_break_sprite_vertex_limit)
            vertex_array = -1;
        else
            vertex_array = 0;
        num_vertex = 0;
        flags = (SprArgs::Flags)0;
        sprite_size = 0.0f;
        field_CC = 0;
        texture_pos = 0.0f;
        texture_size = 1.0f;
        next = 0;
    }

    void SprArgs::SetSpriteSize(vec2 size) {
        if (flags & SPRITE_SIZE)
            return;

        enum_or(flags, SPRITE_SIZE);
        sprite_size = size;
        field_CC = 0;
    }

    void SprArgs::SetTexturePosSize(float_t x, float_t y, float_t width, float_t height) {
        if (flags & TEXTURE_POS_SIZE)
            return;

        enum_or(flags, TEXTURE_POS_SIZE);
        texture_pos.x = x;
        texture_pos.y = y;
        texture_size.x = width;
        texture_size.y = height;
    }

    void SprArgs::SetVertexArray(SpriteVertex* vertex_array, size_t num_vertex) {
        void (FASTCALL * spr__SprArgs__SetVertexArray)(spr::SprArgs * This,
            spr::SpriteVertex * vertex_array, size_t num_vertex)
            = (void (FASTCALL*)(spr::SprArgs * This,
                spr::SpriteVertex * vertex_array, size_t num_vertex))0x000000014063F040;
        spr__SprArgs__SetVertexArray(this, vertex_array, num_vertex);
    }

    void SprArgs::SetNext(SprArgs* args, SprArgs* next) {
        while (args->next)
            args = args->next;
        args->next = next;
        next->kind = SPR_KIND_LINE;
    }

    vec2 proj_sprite_3d_line(vec3 vec, bool offset) {
        mat4 view;
        mat4_transpose(&camera_data.view, &view);

        mat4_transform_point(&view, &vec, &vec);
        if (fabsf(vec.z) < 1.0e-10f)
            return 0.0f;

        vec2 sc_vec = camera_data.depth * *(vec2*)&vec.x * (1.0f / vec.z);

        resolution_struct* res_wind_int = res_window_internal_get();
        sc_vec.x = (float_t)res_wind_int->width * 0.5f - sc_vec.x;
        sc_vec.y = (float_t)res_wind_int->height * 0.5f + sc_vec.y;
        if (offset) {
            resolution_struct* res_wind = res_window_get();
            sc_vec.x = (float_t)res_wind_int->x_offset + sc_vec.x;
            sc_vec.y = (float_t)(res_wind->height
                - res_wind_int->y_offset - res_wind_int->height) + sc_vec.y;
        }
        return sc_vec;
    }

    void put_sprite_3d_line(vec3 p1, vec3 p2, color4u8 color) {
        vec2 sc_p1 = proj_sprite_3d_line(p1, true);
        vec2 sc_p2 = proj_sprite_3d_line(p2, true);
        spr::put_sprite_line(sc_p1, sc_p2, RESOLUTION_MODE_MAX, SPR_PRIO_DEBUG, color, 0);
    }

    void put_sprite_line(vec2 p1, vec2 p2, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer) {
        spr::SprArgs args;
        args.trans.x = p1.x;
        args.trans.y = p1.y;
        args.trans.z = 0.0f;
        args.layer = layer;
        args.kind = SPR_KIND_LINES;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = color;
        args.SetSpriteSize({ p2.x - p1.x, p2.y - p1.y });
        enum_or(args.attr, SPR_ATTR_MATRIX);
        spr::put_sprite(args);
    }

    void put_sprite_line_list(vec2* points, size_t count, resolution_mode mode,
        spr::SprPrio prio, color4u8 color, int32_t layer) {
        spr::SprArgs args;
        args.kind = SPR_KIND_ARROW_AB;
        args.layer = layer;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = color;

        std::vector<spr::SpriteVertex> vertex_array;
        vertex_array.reserve(count);

        for (size_t i = 0; i < count; i++, points++) {
            spr::SpriteVertex vert = {};
            *(vec2*)&vert.pos = *points;
            vertex_array.push_back(vert);
        }

        args.SetVertexArray(vertex_array.data(), vertex_array.size());
        spr::put_sprite(args);
    }

    void put_sprite_multi(rectangle rect, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer) {
        spr::SprArgs args;
        args.trans.x = rect.pos.x;
        args.trans.y = rect.pos.y;
        args.trans.z = 0.0f;
        args.layer = layer;
        args.kind = SPR_KIND_MULTI;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = color;
        args.SetSpriteSize(rect.size);
        enum_or(args.attr, SPR_ATTR_MATRIX);
        spr::put_sprite(args);
    }

    void put_sprite_rect(rectangle rect, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer) {
        spr::SprArgs args;
        args.trans.x = rect.pos.x;
        args.trans.y = rect.pos.y;
        args.trans.z = 0.0f;
        args.layer = layer;
        args.kind = SPR_KIND_RECT;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = color;
        args.SetSpriteSize(rect.size);
        enum_or(args.attr, SPR_ATTR_MATRIX);
        spr::put_sprite(args);
    }

    void put_sprite_triangles(SpriteVertex* vert, size_t num, resolution_mode mode,
        SprPrio prio, int32_t spr_id, int32_t layer) {
        spr::SprArgs args;
        args.layer = layer;
        args.kind = SPR_KIND_TRIANGLE;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = vert[0].color;
        if (spr_id != -1)
            args.id.info = sprite_database_get_spr_by_id(spr_id)->info;
        enum_or(args.attr, SPR_ATTR_MATRIX);
        args.SetVertexArray(vert, num);
        spr::put_sprite(args);
    }
}

const spr_db_spr* sprite_database_get_spr_by_id(int32_t id) {
    const spr_db_spr* (FASTCALL* sprite_database_struct__get_spr_by_id)
        (size_t _this, int32_t id)
        = (const spr_db_spr * (FASTCALL*)(size_t _this, int32_t id))0x000000014063C800;
    return sprite_database_struct__get_spr_by_id(0x000000014CC62290, id);
}

int32_t sprite_database_get_spr_set_id_by_name(const prj::string& name) {
    int32_t(FASTCALL * sprite_database_struct__get_spr_set_id_by_name)
        (size_t _this, const prj::string & name)
        = (int32_t(FASTCALL*)(size_t _this, const prj::string & name))0x00000001401121A0;
    return sprite_database_struct__get_spr_set_id_by_name(0x000000014CC62290, name);
}

void sprite_shared_init() {
    const uint8_t orig_spr__SprArgs__SetVertexArray[] = {
        0x40, 0x53, 0x48, 0x83, 0xEC, 0x20, 0x48, 0x8B,
        0x05, 0x63, 0x21, 0x62, 0x0C, 0x4C, 0x8B, 0xC9,
    };

    if (memcmp((void*)0x000000014063F040, (void*)orig_spr__SprArgs__SetVertexArray,
        sizeof(orig_spr__SprArgs__SetVertexArray)))
        sprite_break_sprite_vertex_limit = true;
    else
        sprite_break_sprite_vertex_limit = false;
}
