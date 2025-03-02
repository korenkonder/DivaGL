/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/image.hpp"
#include "../AFTModsShared/texture.hpp"
#include "wrap.hpp"

extern texture* texture_create_copy_texture_apply_color_tone(
    texture_id id, texture* org_tex, const color_tone* col_tone);

extern void texture_patch();
