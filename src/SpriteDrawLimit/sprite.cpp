/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "sprite.hpp"
#include <stdint.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Helpers.h>

#define FASTCALL __fastcall

namespace spr {
    struct SpriteVertex {
        float_t pos[3]; // vec3
        float_t uv[2]; // vec2
        uint32_t color; // color4u8
    };

    static_assert(sizeof(spr::SpriteVertex) == 0x18, "\"spr::SpriteVertex\" struct should have a size of 0x18");

    struct SpriteVertexData {
        size_t max_count;
        spr::SpriteVertex* array;
    };
}

static void* (*_operator_new)(size_t) = (void* (*)(size_t))0x000000014084530C;
static void(*_operator_delete)(void*) = (void(*)(void*))0x0000000140845378;

spr::SpriteVertexData& sprite_vertex_data = *(spr::SpriteVertexData*)0x000000014CC62480;

const size_t sprite_vertex_array_max_count = 0x2000;
spr::SpriteVertex* sprite_vertex_array = (spr::SpriteVertex*)0x000000014CC62480;

size_t& sprite_vertex_array_count = *(size_t*)0x000000014CC611B0;

HOOK(void, FASTCALL, spr__calc_sprite_texture_param_mid1, 0x0000000140638DD0);
HOOK(void, FASTCALL, spr__calc_sprite_texture_param_mid2, 0x0000000140638DF6);
HOOK(void, FASTCALL, spr__draw_sprite_scale_mid, 0x0000000140639DB7);
HOOK(void, FASTCALL, spr__draw_sprite_mid_spr_vertex_array_pos, 0x000000014063A961);
HOOK(void, FASTCALL, spr__draw_sprite_mid_spr_vertex_array_uv, 0x000000014063A97C);
HOOK(void, FASTCALL, spr__draw_sprite_mid_spr_vertex_array_color, 0x000000014063A99B);
HOOK(void, FASTCALL, spr__draw_sprite_mid_spr_kind_arrow_b, 0x000000014063AB7B);
HOOK(void, FASTCALL, spr__draw_sprite_mid_spr_kind_arrow_ab, 0x000000014063ABCC);
HOOK(void, FASTCALL, spr__draw_sprite_mid_spr_kind_triangle, 0x000000014063AC8D);

HOOK(void, FASTCALL, spr__SprArgs__SetVertexArray, 0x000000014063F040,
    __int64 This, spr::SpriteVertex* vertex_array, size_t num_vertex) {
    if (!sprite_vertex_data.array)
        sprite_vertex_data.array = (spr::SpriteVertex*)_operator_new(
            sizeof(spr::SpriteVertex) * sprite_vertex_data.max_count);

    if (sprite_vertex_array_count + num_vertex >= sprite_vertex_data.max_count) {
        while (sprite_vertex_array_count + num_vertex >= sprite_vertex_data.max_count)
            sprite_vertex_data.max_count *= 2;

        spr::SpriteVertex* _sprite_vertex_array = (spr::SpriteVertex*)_operator_new(
            sizeof(spr::SpriteVertex) * sprite_vertex_data.max_count);
        memmove(_sprite_vertex_array, sprite_vertex_data.array,
            sizeof(spr::SpriteVertex) * sprite_vertex_array_count);
        _operator_delete(sprite_vertex_data.array);
        sprite_vertex_data.array = _sprite_vertex_array;
    }

    *(size_t*)(This + 0xB8) = num_vertex;

    *(size_t*)(This + 0xB0) = sprite_vertex_array_count;
    memmove(sprite_vertex_data.array + *(size_t*)(This + 0xB0),
        vertex_array, sizeof(spr::SpriteVertex) * num_vertex);

    sprite_vertex_array_count += num_vertex;
}

HOOK(void, FASTCALL, sprite_manager_clear, 0x000000014063F860) {
    if (sprite_vertex_data.array)
        _operator_delete(sprite_vertex_data.array);
    originalsprite_manager_clear();
}

void sprite_draw_limit_patch() {
    sprite_vertex_data.array = 0;
    sprite_vertex_data.max_count = sprite_vertex_array_max_count;

    WRITE_NOP_7(0x0000000140638DD0);
    WRITE_NOP_7(0x0000000140638DF6);
    WRITE_NOP_7(0x0000000140639DB7);
    WRITE_NOP_7(0x000000014063A961);
    WRITE_NOP_7(0x000000014063A97C);
    WRITE_NOP_7(0x000000014063A99B);
    WRITE_NOP_7(0x000000014063AB7B);
    WRITE_NOP_7(0x000000014063ABCC);
    WRITE_NOP_7(0x000000014063AC8D);

    INSTALL_HOOK(spr__calc_sprite_texture_param_mid1);
    INSTALL_HOOK(spr__calc_sprite_texture_param_mid2);
    INSTALL_HOOK(spr__draw_sprite_scale_mid);
    INSTALL_HOOK(spr__draw_sprite_mid_spr_vertex_array_pos);
    INSTALL_HOOK(spr__draw_sprite_mid_spr_vertex_array_uv);
    INSTALL_HOOK(spr__draw_sprite_mid_spr_vertex_array_color);
    INSTALL_HOOK(spr__draw_sprite_mid_spr_kind_arrow_b);
    INSTALL_HOOK(spr__draw_sprite_mid_spr_kind_arrow_ab);
    INSTALL_HOOK(spr__draw_sprite_mid_spr_kind_triangle);
    INSTALL_HOOK(spr__SprArgs__SetVertexArray);
    INSTALL_HOOK(sprite_manager_clear);
}
