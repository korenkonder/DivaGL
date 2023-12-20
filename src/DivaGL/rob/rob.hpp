/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"

enum chara_index {
    CHARA_NONE   = -1,
    CHARA_MIKU   = 0x00,
    CHARA_RIN    = 0x01,
    CHARA_LEN    = 0x02,
    CHARA_LUKA   = 0x03,
    CHARA_NERU   = 0x04,
    CHARA_HAKU   = 0x05,
    CHARA_KAITO  = 0x06,
    CHARA_MEIKO  = 0x07,
    CHARA_SAKINE = 0x08,
    CHARA_TETO   = 0x09,
    CHARA_MAX    = 0x0A,
};

enum item_id {
    ITEM_NONE        = -1,
    ITEM_BODY        = 0x00,
    ITEM_ATAMA       = 0x01,
    ITEM_KATA_R      = 0x02,
    ITEM_MUNE        = 0x03,
    ITEM_KATA_L      = 0x04,
    ITEM_UDE_R       = 0x05,
    ITEM_SENAKA      = 0x06,
    ITEM_UDE_L       = 0x07,
    ITEM_HARA        = 0x08,
    ITEM_KOSI        = 0x09,
    ITEM_TE_R        = 0x0A,
    ITEM_TE_L        = 0x0B,
    ITEM_MOMO        = 0x0C,
    ITEM_SUNE        = 0x0D,
    ITEM_ASI         = 0x0E,
    ITEM_KAMI        = 0x0F,
    ITEM_OUTER       = 0x10,
    ITEM_PANTS       = 0x11,
    ITEM_ZUJO        = 0x12,
    ITEM_MEGANE      = 0x13,
    ITEM_KUBI        = 0x14,
    ITEM_JOHA_USHIRO = 0x15,
    ITEM_KUCHI       = 0x16,
    ITEM_ITEM09      = 0x17,
    ITEM_ITEM10      = 0x18,
    ITEM_ITEM11      = 0x19,
    ITEM_ITEM12      = 0x1A,
    ITEM_ITEM13      = 0x1B,
    ITEM_ITEM14      = 0x1C,
    ITEM_ITEM15      = 0x1D,
    ITEM_ITEM16      = 0x1E,
    ITEM_MAX         = 0x1F,
};

enum item_sub_id {
    ITEM_SUB_NONE        = -1,
    ITEM_SUB_ZUJO        = 0x00,
    ITEM_SUB_KAMI        = 0x01,
    ITEM_SUB_HITAI       = 0x02,
    ITEM_SUB_ME          = 0x03,
    ITEM_SUB_MEGANE      = 0x04,
    ITEM_SUB_MIMI        = 0x05,
    ITEM_SUB_KUCHI       = 0x06,
    ITEM_SUB_MAKI        = 0x07,
    ITEM_SUB_KUBI        = 0x08,
    ITEM_SUB_INNER       = 0x09,
    ITEM_SUB_OUTER       = 0x0A,
    ITEM_SUB_KATA        = 0x0B,
    ITEM_SUB_U_UDE       = 0x0C,
    ITEM_SUB_L_UDE       = 0x0D,
    ITEM_SUB_TE          = 0x0E,
    ITEM_SUB_JOHA_MAE    = 0x0F,
    ITEM_SUB_JOHA_USHIRO = 0x10,
    ITEM_SUB_BELT        = 0x11,
    ITEM_SUB_KOSI        = 0x12,
    ITEM_SUB_PANTS       = 0x13,
    ITEM_SUB_ASI         = 0x14,
    ITEM_SUB_SUNE        = 0x15,
    ITEM_SUB_KUTSU       = 0x16,
    ITEM_SUB_HADA        = 0x17,
    ITEM_SUB_HEAD        = 0x18,
    ITEM_SUB_MAX         = 0x19,
};

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

extern const mat4* rob_chara_item_equip_mat;

extern void rob_patch();
