/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"

#define ROB_CHARA_COUNT 6

extern mat4* (FASTCALL* rob_chara_bone_data_get_mats_mat)(size_t rob_bone_data, size_t index);
extern void(FASTCALL* sub_1405163C0)(size_t rob_chr, int32_t index, mat4* mat);
extern float_t(FASTCALL* rob_chara_get_max_face_depth)(size_t rob_chr);
extern mat4* (FASTCALL* sub_140516740)(size_t rob_chr);
extern mat4* (FASTCALL* rob_chara_get_adjust_data_mat)(size_t rob_chr);
extern bool(FASTCALL* rob_chara_array_check_visibility)(size_t rob_chr, int32_t chara_id);
extern size_t(FASTCALL* get_rob_chara_smth)();
extern size_t(FASTCALL* rob_chara_array_get)(size_t rob_chr_smth, int32_t chara_id);
extern size_t(FASTCALL* rob_chara_array_get_bone_data)(size_t rob_chr_smth, int32_t chara_id);
extern bool (FASTCALL* rob_chara_pv_data_array_check_chara_id)(size_t rob_chr_smth, int32_t chara_id);

extern void rob_patch();
