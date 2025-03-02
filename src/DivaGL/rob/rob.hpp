/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"
#include "../../AFTModsShared/rob/rob.hpp"

extern const mat4* rob_chara_item_equip_mat;

extern const mat4* rob_chara_get_item_adjust_data_mat(rob_chara* rob_chr);

extern void rob_patch();
