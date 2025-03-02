/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/rectangle.hpp"
#include "../KKdLib/vec.hpp"
#include "../AFTModsShared/sprite.hpp"
#include "render_context.hpp"
#include "texture.hpp"

extern void sprite_manager_init();
extern void sprite_manager_draw(int32_t index, bool font, texture* tex);
extern void sprite_manager_set_res(double_t aspect, int32_t width, int32_t height);
extern void sprite_manager_set_view_projection(bool aet_3d);
extern void sprite_manager_free();

extern void sprite_patch();
