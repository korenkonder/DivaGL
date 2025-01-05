/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"
#include "../mdl/disp_manager.hpp"
#include "../item_table.hpp"
#include "../object.hpp"

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

enum ex_expression_block_stack_type {
    EX_EXPRESSION_BLOCK_STACK_NUMBER          = 0x00,
    EX_EXPRESSION_BLOCK_STACK_VARIABLE        = 0x01,
    EX_EXPRESSION_BLOCK_STACK_VARIABLE_RADIAN = 0x02,
    EX_EXPRESSION_BLOCK_STACK_OP1             = 0x03,
    EX_EXPRESSION_BLOCK_STACK_OP2             = 0x04,
    EX_EXPRESSION_BLOCK_STACK_OP3             = 0x05,
};

enum eyes_base_adjust_type {
    EYES_BASE_ADJUST_NONE      = -1,
    EYES_BASE_ADJUST_DIRECTION = 0x00,
    EYES_BASE_ADJUST_CLEARANCE = 0x01,
    EYES_BASE_ADJUST_OFF       = 0x02,
    EYES_BASE_ADJUST_MAX       = 0x03,
};

enum mot_bone_index {
    MOT_BONE_N_HARA_CP               = 0x00,
    MOT_BONE_KG_HARA_Y               = 0x01,
    MOT_BONE_KL_HARA_XZ              = 0x02,
    MOT_BONE_KL_HARA_ETC             = 0x03,
    MOT_BONE_N_HARA                  = 0x04,
    MOT_BONE_CL_MUNE                 = 0x05,
    MOT_BONE_J_MUNE_WJ               = 0x06,
    MOT_BONE_E_MUNE_CP               = 0x07,
    MOT_BONE_N_MUNE_B                = 0x08,
    MOT_BONE_KL_MUNE_B_WJ            = 0x09,
    MOT_BONE_KL_KUBI                 = 0x0A,
    MOT_BONE_N_KAO                   = 0x0B,
    MOT_BONE_CL_KAO                  = 0x0C,
    MOT_BONE_J_KAO_WJ                = 0x0D,
    MOT_BONE_E_KAO_CP                = 0x0E,
    MOT_BONE_FACE_ROOT               = 0x0F,
    MOT_BONE_N_AGO                   = 0x10,
    MOT_BONE_KL_AGO_WJ               = 0x11,
    MOT_BONE_N_TOOTH_UNDER           = 0x12,
    MOT_BONE_TL_TOOTH_UNDER_WJ       = 0x13,
    MOT_BONE_N_EYE_L                 = 0x14,
    MOT_BONE_KL_EYE_L                = 0x15,
    MOT_BONE_N_EYE_L_WJ_EX           = 0x16,
    MOT_BONE_KL_HIGHLIGHT_L_WJ       = 0x17,
    MOT_BONE_N_EYE_R                 = 0x18,
    MOT_BONE_KL_EYE_R                = 0x19,
    MOT_BONE_N_EYE_R_WJ_EX           = 0x1A,
    MOT_BONE_KL_HIGHLIGHT_R_WJ       = 0x1B,
    MOT_BONE_N_EYELID_L_A            = 0x1C,
    MOT_BONE_TL_EYELID_L_A_WJ        = 0x1D,
    MOT_BONE_N_EYELID_L_B            = 0x1E,
    MOT_BONE_TL_EYELID_L_B_WJ        = 0x1F,
    MOT_BONE_N_EYELID_R_A            = 0x20,
    MOT_BONE_TL_EYELID_R_A_WJ        = 0x21,
    MOT_BONE_N_EYELID_R_B            = 0x22,
    MOT_BONE_TL_EYELID_R_B_WJ        = 0x23,
    MOT_BONE_N_KUTI_D                = 0x24,
    MOT_BONE_TL_KUTI_D_WJ            = 0x25,
    MOT_BONE_N_KUTI_D_L              = 0x26,
    MOT_BONE_TL_KUTI_D_L_WJ          = 0x27,
    MOT_BONE_N_KUTI_D_R              = 0x28,
    MOT_BONE_TL_KUTI_D_R_WJ          = 0x29,
    MOT_BONE_N_KUTI_DS_L             = 0x2A,
    MOT_BONE_TL_KUTI_DS_L_WJ         = 0x2B,
    MOT_BONE_N_KUTI_DS_R             = 0x2C,
    MOT_BONE_TL_KUTI_DS_R_WJ         = 0x2D,
    MOT_BONE_N_KUTI_L                = 0x2E,
    MOT_BONE_TL_KUTI_L_WJ            = 0x2F,
    MOT_BONE_N_KUTI_M_L              = 0x30,
    MOT_BONE_TL_KUTI_M_L_WJ          = 0x31,
    MOT_BONE_N_KUTI_M_R              = 0x32,
    MOT_BONE_TL_KUTI_M_R_WJ          = 0x33,
    MOT_BONE_N_KUTI_R                = 0x34,
    MOT_BONE_TL_KUTI_R_WJ            = 0x35,
    MOT_BONE_N_KUTI_U                = 0x36,
    MOT_BONE_TL_KUTI_U_WJ            = 0x37,
    MOT_BONE_N_KUTI_U_L              = 0x38,
    MOT_BONE_TL_KUTI_U_L_WJ          = 0x39,
    MOT_BONE_N_KUTI_U_R              = 0x3A,
    MOT_BONE_TL_KUTI_U_R_WJ          = 0x3B,
    MOT_BONE_N_MABU_L_D_A            = 0x3C,
    MOT_BONE_TL_MABU_L_D_A_WJ        = 0x3D,
    MOT_BONE_N_MABU_L_D_B            = 0x3E,
    MOT_BONE_TL_MABU_L_D_B_WJ        = 0x3F,
    MOT_BONE_N_MABU_L_D_C            = 0x40,
    MOT_BONE_TL_MABU_L_D_C_WJ        = 0x41,
    MOT_BONE_N_MABU_L_U_A            = 0x42,
    MOT_BONE_TL_MABU_L_U_A_WJ        = 0x43,
    MOT_BONE_N_MABU_L_U_B            = 0x44,
    MOT_BONE_TL_MABU_L_U_B_WJ        = 0x45,
    MOT_BONE_N_EYELASHES_L           = 0x46,
    MOT_BONE_TL_EYELASHES_L_WJ       = 0x47,
    MOT_BONE_N_MABU_L_U_C            = 0x48,
    MOT_BONE_TL_MABU_L_U_C_WJ        = 0x49,
    MOT_BONE_N_MABU_R_D_A            = 0x4A,
    MOT_BONE_TL_MABU_R_D_A_WJ        = 0x4B,
    MOT_BONE_N_MABU_R_D_B            = 0x4C,
    MOT_BONE_TL_MABU_R_D_B_WJ        = 0x4D,
    MOT_BONE_N_MABU_R_D_C            = 0x4E,
    MOT_BONE_TL_MABU_R_D_C_WJ        = 0x4F,
    MOT_BONE_N_MABU_R_U_A            = 0x50,
    MOT_BONE_TL_MABU_R_U_A_WJ        = 0x51,
    MOT_BONE_N_MABU_R_U_B            = 0x52,
    MOT_BONE_TL_MABU_R_U_B_WJ        = 0x53,
    MOT_BONE_N_EYELASHES_R           = 0x54,
    MOT_BONE_TL_EYELASHES_R_WJ       = 0x55,
    MOT_BONE_N_MABU_R_U_C            = 0x56,
    MOT_BONE_TL_MABU_R_U_C_WJ        = 0x57,
    MOT_BONE_N_MAYU_L                = 0x58,
    MOT_BONE_TL_MAYU_L_WJ            = 0x59,
    MOT_BONE_N_MAYU_L_B              = 0x5A,
    MOT_BONE_TL_MAYU_L_B_WJ          = 0x5B,
    MOT_BONE_N_MAYU_L_C              = 0x5C,
    MOT_BONE_TL_MAYU_L_C_WJ          = 0x5D,
    MOT_BONE_N_MAYU_R                = 0x5E,
    MOT_BONE_TL_MAYU_R_WJ            = 0x5F,
    MOT_BONE_N_MAYU_R_B              = 0x60,
    MOT_BONE_TL_MAYU_R_B_WJ          = 0x61,
    MOT_BONE_N_MAYU_R_C              = 0x62,
    MOT_BONE_TL_MAYU_R_C_WJ          = 0x63,
    MOT_BONE_N_TOOTH_UPPER           = 0x64,
    MOT_BONE_TL_TOOTH_UPPER_WJ       = 0x65,
    MOT_BONE_N_KUBI_WJ_EX            = 0x66,
    MOT_BONE_N_WAKI_L                = 0x67,
    MOT_BONE_KL_WAKI_L_WJ            = 0x68,
    MOT_BONE_TL_UP_KATA_L            = 0x69,
    MOT_BONE_C_KATA_L                = 0x6A,
    MOT_BONE_J_KATA_L_WJ_CU          = 0x6B,
    MOT_BONE_J_UDE_L_WJ              = 0x6C,
    MOT_BONE_E_UDE_L_CP              = 0x6D,
    MOT_BONE_KL_TE_L_WJ              = 0x6E,
    MOT_BONE_N_HITO_L_EX             = 0x6F,
    MOT_BONE_NL_HITO_L_WJ            = 0x70,
    MOT_BONE_NL_HITO_B_L_WJ          = 0x71,
    MOT_BONE_NL_HITO_C_L_WJ          = 0x72,
    MOT_BONE_N_KO_L_EX               = 0x73,
    MOT_BONE_NL_KO_L_WJ              = 0x74,
    MOT_BONE_NL_KO_B_L_WJ            = 0x75,
    MOT_BONE_NL_KO_C_L_WJ            = 0x76,
    MOT_BONE_N_KUSU_L_EX             = 0x77,
    MOT_BONE_NL_KUSU_L_WJ            = 0x78,
    MOT_BONE_NL_KUSU_B_L_WJ          = 0x79,
    MOT_BONE_NL_KUSU_C_L_WJ          = 0x7A,
    MOT_BONE_N_NAKA_L_EX             = 0x7B,
    MOT_BONE_NL_NAKA_L_WJ            = 0x7C,
    MOT_BONE_NL_NAKA_B_L_WJ          = 0x7D,
    MOT_BONE_NL_NAKA_C_L_WJ          = 0x7E,
    MOT_BONE_N_OYA_L_EX              = 0x7F,
    MOT_BONE_NL_OYA_L_WJ             = 0x80,
    MOT_BONE_NL_OYA_B_L_WJ           = 0x81,
    MOT_BONE_NL_OYA_C_L_WJ           = 0x82,
    MOT_BONE_N_STE_L_WJ_EX           = 0x83,
    MOT_BONE_N_SUDE_L_WJ_EX          = 0x84,
    MOT_BONE_N_SUDE_B_L_WJ_EX        = 0x85,
    MOT_BONE_N_HIJI_L_WJ_EX          = 0x86,
    MOT_BONE_N_UP_KATA_L_EX          = 0x87,
    MOT_BONE_N_SKATA_L_WJ_CD_EX      = 0x88,
    MOT_BONE_N_SKATA_B_L_WJ_CD_CU_EX = 0x89,
    MOT_BONE_N_SKATA_C_L_WJ_CD_CU_EX = 0x8A,
    MOT_BONE_N_WAKI_R                = 0x8B,
    MOT_BONE_KL_WAKI_R_WJ            = 0x8C,
    MOT_BONE_TL_UP_KATA_R            = 0x8D,
    MOT_BONE_C_KATA_R                = 0x8E,
    MOT_BONE_J_KATA_R_WJ_CU          = 0x8F,
    MOT_BONE_J_UDE_R_WJ              = 0x90,
    MOT_BONE_E_UDE_R_CP              = 0x91,
    MOT_BONE_KL_TE_R_WJ              = 0x92,
    MOT_BONE_N_HITO_R_EX             = 0x93,
    MOT_BONE_NL_HITO_R_WJ            = 0x94,
    MOT_BONE_NL_HITO_B_R_WJ          = 0x95,
    MOT_BONE_NL_HITO_C_R_WJ          = 0x96,
    MOT_BONE_N_KO_R_EX               = 0x97,
    MOT_BONE_NL_KO_R_WJ              = 0x98,
    MOT_BONE_NL_KO_B_R_WJ            = 0x99,
    MOT_BONE_NL_KO_C_R_WJ            = 0x9A,
    MOT_BONE_N_KUSU_R_EX             = 0x9B,
    MOT_BONE_NL_KUSU_R_WJ            = 0x9C,
    MOT_BONE_NL_KUSU_B_R_WJ          = 0x9D,
    MOT_BONE_NL_KUSU_C_R_WJ          = 0x9E,
    MOT_BONE_N_NAKA_R_EX             = 0x9F,
    MOT_BONE_NL_NAKA_R_WJ            = 0xA0,
    MOT_BONE_NL_NAKA_B_R_WJ          = 0xA1,
    MOT_BONE_NL_NAKA_C_R_WJ          = 0xA2,
    MOT_BONE_N_OYA_R_EX              = 0xA3,
    MOT_BONE_NL_OYA_R_WJ             = 0xA4,
    MOT_BONE_NL_OYA_B_R_WJ           = 0xA5,
    MOT_BONE_NL_OYA_C_R_WJ           = 0xA6,
    MOT_BONE_N_STE_R_WJ_EX           = 0xA7,
    MOT_BONE_N_SUDE_R_WJ_EX          = 0xA8,
    MOT_BONE_N_SUDE_B_R_WJ_EX        = 0xA9,
    MOT_BONE_N_HIJI_R_WJ_EX          = 0xAA,
    MOT_BONE_N_UP_KATA_R_EX          = 0xAB,
    MOT_BONE_N_SKATA_R_WJ_CD_EX      = 0xAC,
    MOT_BONE_N_SKATA_B_R_WJ_CD_CU_EX = 0xAD,
    MOT_BONE_N_SKATA_C_R_WJ_CD_CU_EX = 0xAE,
    MOT_BONE_KL_KOSI_Y               = 0xAF,
    MOT_BONE_KL_KOSI_XZ              = 0xB0,
    MOT_BONE_KL_KOSI_ETC_WJ          = 0xB1,
    MOT_BONE_CL_MOMO_L               = 0xB2,
    MOT_BONE_J_MOMO_L_WJ             = 0xB3,
    MOT_BONE_J_SUNE_L_WJ             = 0xB4,
    MOT_BONE_E_SUNE_L_CP             = 0xB5,
    MOT_BONE_KL_ASI_L_WJ_CO          = 0xB6,
    MOT_BONE_KL_TOE_L_WJ             = 0xB7,
    MOT_BONE_N_HIZA_L_WJ_EX          = 0xB8,
    MOT_BONE_CL_MOMO_R               = 0xB9,
    MOT_BONE_J_MOMO_R_WJ             = 0xBA,
    MOT_BONE_J_SUNE_R_WJ             = 0xBB,
    MOT_BONE_E_SUNE_R_CP             = 0xBC,
    MOT_BONE_KL_ASI_R_WJ_CO          = 0xBD,
    MOT_BONE_KL_TOE_R_WJ             = 0xBE,
    MOT_BONE_N_HIZA_R_WJ_EX          = 0xBF,
    MOT_BONE_N_MOMO_A_L_WJ_CD_EX     = 0xC0,
    MOT_BONE_N_MOMO_B_L_WJ_EX        = 0xC1,
    MOT_BONE_N_MOMO_C_L_WJ_EX        = 0xC2,
    MOT_BONE_N_MOMO_A_R_WJ_CD_EX     = 0xC3,
    MOT_BONE_N_MOMO_B_R_WJ_EX        = 0xC4,
    MOT_BONE_N_MOMO_C_R_WJ_EX        = 0xC5,
    MOT_BONE_N_HARA_CD_EX            = 0xC6,
    MOT_BONE_N_HARA_B_WJ_EX          = 0xC7,
    MOT_BONE_N_HARA_C_WJ_EX          = 0xC8,
    MOT_BONE_MAX                     = 0xC9,
};

enum mothead_data_type {
    MOTHEAD_DATA_TYPE_0                      = 0x00,
    MOTHEAD_DATA_TYPE_1                      = 0x01,
    MOTHEAD_DATA_TYPE_2                      = 0x02,
    MOTHEAD_DATA_TYPE_3                      = 0x03,
    MOTHEAD_DATA_TYPE_4                      = 0x04,
    MOTHEAD_DATA_TYPE_5                      = 0x05,
    MOTHEAD_DATA_TYPE_6                      = 0x06,
    MOTHEAD_DATA_TYPE_7                      = 0x07,
    MOTHEAD_DATA_TYPE_8                      = 0x08,
    MOTHEAD_DATA_TYPE_9                      = 0x09,
    MOTHEAD_DATA_TYPE_10                     = 0x0A,
    MOTHEAD_DATA_TYPE_11                     = 0x0B,
    MOTHEAD_DATA_TYPE_12                     = 0x0C,
    MOTHEAD_DATA_TYPE_13                     = 0x0D,
    MOTHEAD_DATA_TYPE_14                     = 0x0E,
    MOTHEAD_DATA_TYPE_15                     = 0x0F,
    MOTHEAD_DATA_TYPE_16                     = 0x10,
    MOTHEAD_DATA_TYPE_17                     = 0x11,
    MOTHEAD_DATA_TYPE_18                     = 0x12,
    MOTHEAD_DATA_TYPE_19                     = 0x13,
    MOTHEAD_DATA_TYPE_20                     = 0x14,
    MOTHEAD_DATA_TYPE_21                     = 0x15,
    MOTHEAD_DATA_TYPE_22                     = 0x16,
    MOTHEAD_DATA_TYPE_23                     = 0x17,
    MOTHEAD_DATA_TYPE_24                     = 0x18,
    MOTHEAD_DATA_TYPE_25                     = 0x19,
    MOTHEAD_DATA_TYPE_26                     = 0x1A,
    MOTHEAD_DATA_TYPE_27                     = 0x1B,
    MOTHEAD_DATA_TYPE_28                     = 0x1C,
    MOTHEAD_DATA_TYPE_29                     = 0x1D,
    MOTHEAD_DATA_TYPE_30                     = 0x1E,
    MOTHEAD_DATA_TYPE_31                     = 0x1F,
    MOTHEAD_DATA_TYPE_32                     = 0x20,
    MOTHEAD_DATA_TYPE_33                     = 0x21,
    MOTHEAD_DATA_TYPE_34                     = 0x22,
    MOTHEAD_DATA_TYPE_35                     = 0x23,
    MOTHEAD_DATA_TYPE_36                     = 0x24,
    MOTHEAD_DATA_TYPE_37                     = 0x25,
    MOTHEAD_DATA_TYPE_38                     = 0x26,
    MOTHEAD_DATA_TYPE_39                     = 0x27,
    MOTHEAD_DATA_TYPE_40                     = 0x28,
    MOTHEAD_DATA_TYPE_41                     = 0x29,
    MOTHEAD_DATA_TYPE_42                     = 0x2A,
    MOTHEAD_DATA_TYPE_43                     = 0x2B,
    MOTHEAD_DATA_TYPE_44                     = 0x2C,
    MOTHEAD_DATA_TYPE_45                     = 0x2D,
    MOTHEAD_DATA_TYPE_46                     = 0x2E,
    MOTHEAD_DATA_TYPE_47                     = 0x2F,
    MOTHEAD_DATA_TYPE_48                     = 0x30,
    MOTHEAD_DATA_TYPE_49                     = 0x31,
    MOTHEAD_DATA_SET_FACE_MOTION_ID          = 0x32,
    MOTHEAD_DATA_TYPE_51                     = 0x33,
    MOTHEAD_DATA_TYPE_52                     = 0x34,
    MOTHEAD_DATA_SET_FACE_MOTTBL_MOTION      = 0x35,
    MOTHEAD_DATA_SET_HAND_R_MOTTBL_MOTION    = 0x36,
    MOTHEAD_DATA_SET_HAND_L_MOTTBL_MOTION    = 0x37,
    MOTHEAD_DATA_SET_MOUTH_MOTTBL_MOTION     = 0x38,
    MOTHEAD_DATA_SET_EYES_MOTTBL_MOTION      = 0x39,
    MOTHEAD_DATA_SET_EYELID_MOTTBL_MOTION    = 0x3A,
    MOTHEAD_DATA_SET_ROB_CHARA_HEAD_OBJECT   = 0x3B,
    MOTHEAD_DATA_TYPE_60                     = 0x3C,
    MOTHEAD_DATA_SET_EYELID_MOTION_FROM_FACE = 0x3D,
    MOTHEAD_DATA_ROB_PARTS_ADJUST            = 0x3E,
    MOTHEAD_DATA_TYPE_63                     = 0x3F,
    MOTHEAD_DATA_WIND_RESET                  = 0x40,
    MOTHEAD_DATA_OSAGE_RESET                 = 0x41,
    MOTHEAD_DATA_OSAGE_STEP                  = 0x42,
    MOTHEAD_DATA_SLEEVE_ADJUST               = 0x43,
    MOTHEAD_DATA_TYPE_68                     = 0x44,
    MOTHEAD_DATA_TYPE_69                     = 0x45,
    MOTHEAD_DATA_TYPE_70                     = 0x46,
    MOTHEAD_DATA_OSAGE_MOVE_CANCEL           = 0x47,
    MOTHEAD_DATA_TYPE_72                     = 0x48,
    MOTHEAD_DATA_ROB_HAND_ADJUST             = 0x49,
    MOTHEAD_DATA_DISABLE_COLLISION           = 0x4A,
    MOTHEAD_DATA_ROB_ADJUST_GLOBAL           = 0x4B,
    MOTHEAD_DATA_ROB_ARM_ADJUST              = 0x4C,
    MOTHEAD_DATA_DISABLE_EYE_MOTION          = 0x4D,
    MOTHEAD_DATA_TYPE_78                     = 0x4E,
    MOTHEAD_DATA_ROB_CHARA_COLI_RING         = 0x4F,
    MOTHEAD_DATA_ADJUST_GET_GLOBAL_POSITION  = 0x50,
    MOTHEAD_DATA_MAX                         = 0x51,
};

enum MotionBlendType {
    MOTION_BLEND_NONE    = -1,
    MOTION_BLEND         = 0x00,
    MOTION_BLEND_FREEZE  = 0x01,
    MOTION_BLEND_CROSS   = 0x02,
    MOTION_BLEND_COMBINE = 0x03,
};

enum motion_bone_index {
    MOTION_BONE_NONE                    = -1,
    MOTION_BONE_N_HARA_CP               = 0x00,
    MOTION_BONE_KG_HARA_Y               = 0x01,
    MOTION_BONE_KL_HARA_XZ              = 0x02,
    MOTION_BONE_KL_HARA_ETC             = 0x03,
    MOTION_BONE_N_HARA                  = 0x04,
    MOTION_BONE_CL_MUNE                 = 0x05,
    MOTION_BONE_N_MUNE_B                = 0x06,
    MOTION_BONE_KL_MUNE_B_WJ            = 0x07,
    MOTION_BONE_KL_KUBI                 = 0x08,
    MOTION_BONE_N_KAO                   = 0x09,
    MOTION_BONE_CL_KAO                  = 0x0A,
    MOTION_BONE_FACE_ROOT               = 0x0B,
    MOTION_BONE_N_AGO                   = 0x0C,
    MOTION_BONE_KL_AGO_WJ               = 0x0D,
    MOTION_BONE_N_TOOTH_UNDER           = 0x0E,
    MOTION_BONE_TL_TOOTH_UNDER_WJ       = 0x0F,
    MOTION_BONE_N_EYE_L                 = 0x10,
    MOTION_BONE_KL_EYE_L                = 0x11,
    MOTION_BONE_N_EYE_L_WJ_EX           = 0x12,
    MOTION_BONE_KL_HIGHLIGHT_L_WJ       = 0x13,
    MOTION_BONE_N_EYE_R                 = 0x14,
    MOTION_BONE_KL_EYE_R                = 0x15,
    MOTION_BONE_N_EYE_R_WJ_EX           = 0x16,
    MOTION_BONE_KL_HIGHLIGHT_R_WJ       = 0x17,
    MOTION_BONE_N_EYELID_L_A            = 0x18,
    MOTION_BONE_TL_EYELID_L_A_WJ        = 0x19,
    MOTION_BONE_N_EYELID_L_B            = 0x1A,
    MOTION_BONE_TL_EYELID_L_B_WJ        = 0x1B,
    MOTION_BONE_N_EYELID_R_A            = 0x1C,
    MOTION_BONE_TL_EYELID_R_A_WJ        = 0x1D,
    MOTION_BONE_N_EYELID_R_B            = 0x1E,
    MOTION_BONE_TL_EYELID_R_B_WJ        = 0x1F,
    MOTION_BONE_N_KUTI_D                = 0x20,
    MOTION_BONE_TL_KUTI_D_WJ            = 0x21,
    MOTION_BONE_N_KUTI_D_L              = 0x22,
    MOTION_BONE_TL_KUTI_D_L_WJ          = 0x23,
    MOTION_BONE_N_KUTI_D_R              = 0x24,
    MOTION_BONE_TL_KUTI_D_R_WJ          = 0x25,
    MOTION_BONE_N_KUTI_DS_L             = 0x26,
    MOTION_BONE_TL_KUTI_DS_L_WJ         = 0x27,
    MOTION_BONE_N_KUTI_DS_R             = 0x28,
    MOTION_BONE_TL_KUTI_DS_R_WJ         = 0x29,
    MOTION_BONE_N_KUTI_L                = 0x2A,
    MOTION_BONE_TL_KUTI_L_WJ            = 0x2B,
    MOTION_BONE_N_KUTI_M_L              = 0x2C,
    MOTION_BONE_TL_KUTI_M_L_WJ          = 0x2D,
    MOTION_BONE_N_KUTI_M_R              = 0x2E,
    MOTION_BONE_TL_KUTI_M_R_WJ          = 0x2F,
    MOTION_BONE_N_KUTI_R                = 0x30,
    MOTION_BONE_TL_KUTI_R_WJ            = 0x31,
    MOTION_BONE_N_KUTI_U                = 0x32,
    MOTION_BONE_TL_KUTI_U_WJ            = 0x33,
    MOTION_BONE_N_KUTI_U_L              = 0x34,
    MOTION_BONE_TL_KUTI_U_L_WJ          = 0x35,
    MOTION_BONE_N_KUTI_U_R              = 0x36,
    MOTION_BONE_TL_KUTI_U_R_WJ          = 0x37,
    MOTION_BONE_N_MABU_L_D_A            = 0x38,
    MOTION_BONE_TL_MABU_L_D_A_WJ        = 0x39,
    MOTION_BONE_N_MABU_L_D_B            = 0x3A,
    MOTION_BONE_TL_MABU_L_D_B_WJ        = 0x3B,
    MOTION_BONE_N_MABU_L_D_C            = 0x3C,
    MOTION_BONE_TL_MABU_L_D_C_WJ        = 0x3D,
    MOTION_BONE_N_MABU_L_U_A            = 0x3E,
    MOTION_BONE_TL_MABU_L_U_A_WJ        = 0x3F,
    MOTION_BONE_N_MABU_L_U_B            = 0x40,
    MOTION_BONE_TL_MABU_L_U_B_WJ        = 0x41,
    MOTION_BONE_N_EYELASHES_L           = 0x42,
    MOTION_BONE_TL_EYELASHES_L_WJ       = 0x43,
    MOTION_BONE_N_MABU_L_U_C            = 0x44,
    MOTION_BONE_TL_MABU_L_U_C_WJ        = 0x45,
    MOTION_BONE_N_MABU_R_D_A            = 0x46,
    MOTION_BONE_TL_MABU_R_D_A_WJ        = 0x47,
    MOTION_BONE_N_MABU_R_D_B            = 0x48,
    MOTION_BONE_TL_MABU_R_D_B_WJ        = 0x49,
    MOTION_BONE_N_MABU_R_D_C            = 0x4A,
    MOTION_BONE_TL_MABU_R_D_C_WJ        = 0x4B,
    MOTION_BONE_N_MABU_R_U_A            = 0x4C,
    MOTION_BONE_TL_MABU_R_U_A_WJ        = 0x4D,
    MOTION_BONE_N_MABU_R_U_B            = 0x4E,
    MOTION_BONE_TL_MABU_R_U_B_WJ        = 0x4F,
    MOTION_BONE_N_EYELASHES_R           = 0x50,
    MOTION_BONE_TL_EYELASHES_R_WJ       = 0x51,
    MOTION_BONE_N_MABU_R_U_C            = 0x52,
    MOTION_BONE_TL_MABU_R_U_C_WJ        = 0x53,
    MOTION_BONE_N_MAYU_L                = 0x54,
    MOTION_BONE_TL_MAYU_L_WJ            = 0x55,
    MOTION_BONE_N_MAYU_L_B              = 0x56,
    MOTION_BONE_TL_MAYU_L_B_WJ          = 0x57,
    MOTION_BONE_N_MAYU_L_C              = 0x58,
    MOTION_BONE_TL_MAYU_L_C_WJ          = 0x59,
    MOTION_BONE_N_MAYU_R                = 0x5A,
    MOTION_BONE_TL_MAYU_R_WJ            = 0x5B,
    MOTION_BONE_N_MAYU_R_B              = 0x5C,
    MOTION_BONE_TL_MAYU_R_B_WJ          = 0x5D,
    MOTION_BONE_N_MAYU_R_C              = 0x5E,
    MOTION_BONE_TL_MAYU_R_C_WJ          = 0x5F,
    MOTION_BONE_N_TOOTH_UPPER           = 0x60,
    MOTION_BONE_TL_TOOTH_UPPER_WJ       = 0x61,
    MOTION_BONE_N_KUBI_WJ_EX            = 0x62,
    MOTION_BONE_N_WAKI_L                = 0x63,
    MOTION_BONE_KL_WAKI_L_WJ            = 0x64,
    MOTION_BONE_TL_UP_KATA_L            = 0x65,
    MOTION_BONE_C_KATA_L                = 0x66,
    MOTION_BONE_KL_TE_L_WJ              = 0x67,
    MOTION_BONE_N_HITO_L_EX             = 0x68,
    MOTION_BONE_NL_HITO_L_WJ            = 0x69,
    MOTION_BONE_NL_HITO_B_L_WJ          = 0x6A,
    MOTION_BONE_NL_HITO_C_L_WJ          = 0x6B,
    MOTION_BONE_N_KO_L_EX               = 0x6C,
    MOTION_BONE_NL_KO_L_WJ              = 0x6D,
    MOTION_BONE_NL_KO_B_L_WJ            = 0x6E,
    MOTION_BONE_NL_KO_C_L_WJ            = 0x6F,
    MOTION_BONE_N_KUSU_L_EX             = 0x70,
    MOTION_BONE_NL_KUSU_L_WJ            = 0x71,
    MOTION_BONE_NL_KUSU_B_L_WJ          = 0x72,
    MOTION_BONE_NL_KUSU_C_L_WJ          = 0x73,
    MOTION_BONE_N_NAKA_L_EX             = 0x74,
    MOTION_BONE_NL_NAKA_L_WJ            = 0x75,
    MOTION_BONE_NL_NAKA_B_L_WJ          = 0x76,
    MOTION_BONE_NL_NAKA_C_L_WJ          = 0x77,
    MOTION_BONE_N_OYA_L_EX              = 0x78,
    MOTION_BONE_NL_OYA_L_WJ             = 0x79,
    MOTION_BONE_NL_OYA_B_L_WJ           = 0x7A,
    MOTION_BONE_NL_OYA_C_L_WJ           = 0x7B,
    MOTION_BONE_N_STE_L_WJ_EX           = 0x7C,
    MOTION_BONE_N_SUDE_L_WJ_EX          = 0x7D,
    MOTION_BONE_N_SUDE_B_L_WJ_EX        = 0x7E,
    MOTION_BONE_N_HIJI_L_WJ_EX          = 0x7F,
    MOTION_BONE_N_UP_KATA_L_EX          = 0x80,
    MOTION_BONE_N_SKATA_L_WJ_CD_EX      = 0x81,
    MOTION_BONE_N_SKATA_B_L_WJ_CD_CU_EX = 0x82,
    MOTION_BONE_N_SKATA_C_L_WJ_CD_CU_EX = 0x83,
    MOTION_BONE_N_WAKI_R                = 0x84,
    MOTION_BONE_KL_WAKI_R_WJ            = 0x85,
    MOTION_BONE_TL_UP_KATA_R            = 0x86,
    MOTION_BONE_C_KATA_R                = 0x87,
    MOTION_BONE_KL_TE_R_WJ              = 0x88,
    MOTION_BONE_N_HITO_R_EX             = 0x89,
    MOTION_BONE_NL_HITO_R_WJ            = 0x8A,
    MOTION_BONE_NL_HITO_B_R_WJ          = 0x8B,
    MOTION_BONE_NL_HITO_C_R_WJ          = 0x8C,
    MOTION_BONE_N_KO_R_EX               = 0x8D,
    MOTION_BONE_NL_KO_R_WJ              = 0x8E,
    MOTION_BONE_NL_KO_B_R_WJ            = 0x8F,
    MOTION_BONE_NL_KO_C_R_WJ            = 0x90,
    MOTION_BONE_N_KUSU_R_EX             = 0x91,
    MOTION_BONE_NL_KUSU_R_WJ            = 0x92,
    MOTION_BONE_NL_KUSU_B_R_WJ          = 0x93,
    MOTION_BONE_NL_KUSU_C_R_WJ          = 0x94,
    MOTION_BONE_N_NAKA_R_EX             = 0x95,
    MOTION_BONE_NL_NAKA_R_WJ            = 0x96,
    MOTION_BONE_NL_NAKA_B_R_WJ          = 0x97,
    MOTION_BONE_NL_NAKA_C_R_WJ          = 0x98,
    MOTION_BONE_N_OYA_R_EX              = 0x99,
    MOTION_BONE_NL_OYA_R_WJ             = 0x9A,
    MOTION_BONE_NL_OYA_B_R_WJ           = 0x9B,
    MOTION_BONE_NL_OYA_C_R_WJ           = 0x9C,
    MOTION_BONE_N_STE_R_WJ_EX           = 0x9D,
    MOTION_BONE_N_SUDE_R_WJ_EX          = 0x9E,
    MOTION_BONE_N_SUDE_B_R_WJ_EX        = 0x9F,
    MOTION_BONE_N_HIJI_R_WJ_EX          = 0xA0,
    MOTION_BONE_N_UP_KATA_R_EX          = 0xA1,
    MOTION_BONE_N_SKATA_R_WJ_CD_EX      = 0xA2,
    MOTION_BONE_N_SKATA_B_R_WJ_CD_CU_EX = 0xA3,
    MOTION_BONE_N_SKATA_C_R_WJ_CD_CU_EX = 0xA4,
    MOTION_BONE_KL_KOSI_Y               = 0xA5,
    MOTION_BONE_KL_KOSI_XZ              = 0xA6,
    MOTION_BONE_KL_KOSI_ETC_WJ          = 0xA7,
    MOTION_BONE_CL_MOMO_L               = 0xA8,
    MOTION_BONE_KL_ASI_L_WJ_CO          = 0xA9,
    MOTION_BONE_KL_TOE_L_WJ             = 0xAA,
    MOTION_BONE_N_HIZA_L_WJ_EX          = 0xAB,
    MOTION_BONE_CL_MOMO_R               = 0xAC,
    MOTION_BONE_KL_ASI_R_WJ_CO          = 0xAD,
    MOTION_BONE_KL_TOE_R_WJ             = 0xAE,
    MOTION_BONE_N_HIZA_R_WJ_EX          = 0xAF,
    MOTION_BONE_N_MOMO_A_L_WJ_CD_EX     = 0xB0,
    MOTION_BONE_N_MOMO_B_L_WJ_EX        = 0xB1,
    MOTION_BONE_N_MOMO_C_L_WJ_EX        = 0xB2,
    MOTION_BONE_N_MOMO_A_R_WJ_CD_EX     = 0xB3,
    MOTION_BONE_N_MOMO_B_R_WJ_EX        = 0xB4,
    MOTION_BONE_N_MOMO_C_R_WJ_EX        = 0xB5,
    MOTION_BONE_N_HARA_CD_EX            = 0xB6,
    MOTION_BONE_N_HARA_B_WJ_EX          = 0xB7,
    MOTION_BONE_N_HARA_C_WJ_EX          = 0xB8,
    MOTION_BONE_MAX                     = 0xB9,
};

enum rob_bone_index {
    ROB_BONE_NONE                    = -1,
    ROB_BONE_N_HARA_CP               = 0x00,
    ROB_BONE_KG_HARA_Y               = 0x01,
    ROB_BONE_KL_HARA_XZ              = 0x02,
    ROB_BONE_KL_HARA_ETC             = 0x03,
    ROB_BONE_N_HARA                  = 0x04,
    ROB_BONE_CL_MUNE                 = 0x05,
    ROB_BONE_N_MUNE_B                = 0x06,
    ROB_BONE_KL_MUNE_B_WJ            = 0x07,
    ROB_BONE_KL_KUBI                 = 0x08,
    ROB_BONE_N_KAO                   = 0x09,
    ROB_BONE_CL_KAO                  = 0x0A,
    ROB_BONE_FACE_ROOT               = 0x0B,
    ROB_BONE_N_AGO                   = 0x0C,
    ROB_BONE_KL_AGO_WJ               = 0x0D,
    ROB_BONE_N_TOOTH_UNDER           = 0x0E,
    ROB_BONE_TL_TOOTH_UNDER_WJ       = 0x0F,
    ROB_BONE_N_EYE_L                 = 0x10,
    ROB_BONE_KL_EYE_L                = 0x11,
    ROB_BONE_N_EYE_L_WJ_EX           = 0x12,
    ROB_BONE_KL_HIGHLIGHT_L_WJ       = 0x13,
    ROB_BONE_N_EYE_R                 = 0x14,
    ROB_BONE_KL_EYE_R                = 0x15,
    ROB_BONE_N_EYE_R_WJ_EX           = 0x16,
    ROB_BONE_KL_HIGHLIGHT_R_WJ       = 0x17,
    ROB_BONE_N_EYELID_L_A            = 0x18,
    ROB_BONE_TL_EYELID_L_A_WJ        = 0x19,
    ROB_BONE_N_EYELID_L_B            = 0x1A,
    ROB_BONE_TL_EYELID_L_B_WJ        = 0x1B,
    ROB_BONE_N_EYELID_R_A            = 0x1C,
    ROB_BONE_TL_EYELID_R_A_WJ        = 0x1D,
    ROB_BONE_N_EYELID_R_B            = 0x1E,
    ROB_BONE_TL_EYELID_R_B_WJ        = 0x1F,
    ROB_BONE_N_KUTI_D                = 0x20,
    ROB_BONE_TL_KUTI_D_WJ            = 0x21,
    ROB_BONE_N_KUTI_D_L              = 0x22,
    ROB_BONE_TL_KUTI_D_L_WJ          = 0x23,
    ROB_BONE_N_KUTI_D_R              = 0x24,
    ROB_BONE_TL_KUTI_D_R_WJ          = 0x25,
    ROB_BONE_N_KUTI_DS_L             = 0x26,
    ROB_BONE_TL_KUTI_DS_L_WJ         = 0x27,
    ROB_BONE_N_KUTI_DS_R             = 0x28,
    ROB_BONE_TL_KUTI_DS_R_WJ         = 0x29,
    ROB_BONE_N_KUTI_L                = 0x2A,
    ROB_BONE_TL_KUTI_L_WJ            = 0x2B,
    ROB_BONE_N_KUTI_M_L              = 0x2C,
    ROB_BONE_TL_KUTI_M_L_WJ          = 0x2D,
    ROB_BONE_N_KUTI_M_R              = 0x2E,
    ROB_BONE_TL_KUTI_M_R_WJ          = 0x2F,
    ROB_BONE_N_KUTI_R                = 0x30,
    ROB_BONE_TL_KUTI_R_WJ            = 0x31,
    ROB_BONE_N_KUTI_U                = 0x32,
    ROB_BONE_TL_KUTI_U_WJ            = 0x33,
    ROB_BONE_N_KUTI_U_L              = 0x34,
    ROB_BONE_TL_KUTI_U_L_WJ          = 0x35,
    ROB_BONE_N_KUTI_U_R              = 0x36,
    ROB_BONE_TL_KUTI_U_R_WJ          = 0x37,
    ROB_BONE_N_MABU_L_D_A            = 0x38,
    ROB_BONE_TL_MABU_L_D_A_WJ        = 0x39,
    ROB_BONE_N_MABU_L_D_B            = 0x3A,
    ROB_BONE_TL_MABU_L_D_B_WJ        = 0x3B,
    ROB_BONE_N_MABU_L_D_C            = 0x3C,
    ROB_BONE_TL_MABU_L_D_C_WJ        = 0x3D,
    ROB_BONE_N_MABU_L_U_A            = 0x3E,
    ROB_BONE_TL_MABU_L_U_A_WJ        = 0x3F,
    ROB_BONE_N_MABU_L_U_B            = 0x40,
    ROB_BONE_TL_MABU_L_U_B_WJ        = 0x41,
    ROB_BONE_N_EYELASHES_L           = 0x42,
    ROB_BONE_TL_EYELASHES_L_WJ       = 0x43,
    ROB_BONE_N_MABU_L_U_C            = 0x44,
    ROB_BONE_TL_MABU_L_U_C_WJ        = 0x45,
    ROB_BONE_N_MABU_R_D_A            = 0x46,
    ROB_BONE_TL_MABU_R_D_A_WJ        = 0x47,
    ROB_BONE_N_MABU_R_D_B            = 0x48,
    ROB_BONE_TL_MABU_R_D_B_WJ        = 0x49,
    ROB_BONE_N_MABU_R_D_C            = 0x4A,
    ROB_BONE_TL_MABU_R_D_C_WJ        = 0x4B,
    ROB_BONE_N_MABU_R_U_A            = 0x4C,
    ROB_BONE_TL_MABU_R_U_A_WJ        = 0x4D,
    ROB_BONE_N_MABU_R_U_B            = 0x4E,
    ROB_BONE_TL_MABU_R_U_B_WJ        = 0x4F,
    ROB_BONE_N_EYELASHES_R           = 0x50,
    ROB_BONE_TL_EYELASHES_R_WJ       = 0x51,
    ROB_BONE_N_MABU_R_U_C            = 0x52,
    ROB_BONE_TL_MABU_R_U_C_WJ        = 0x53,
    ROB_BONE_N_MAYU_L                = 0x54,
    ROB_BONE_TL_MAYU_L_WJ            = 0x55,
    ROB_BONE_N_MAYU_L_B              = 0x56,
    ROB_BONE_TL_MAYU_L_B_WJ          = 0x57,
    ROB_BONE_N_MAYU_L_C              = 0x58,
    ROB_BONE_TL_MAYU_L_C_WJ          = 0x59,
    ROB_BONE_N_MAYU_R                = 0x5A,
    ROB_BONE_TL_MAYU_R_WJ            = 0x5B,
    ROB_BONE_N_MAYU_R_B              = 0x5C,
    ROB_BONE_TL_MAYU_R_B_WJ          = 0x5D,
    ROB_BONE_N_MAYU_R_C              = 0x5E,
    ROB_BONE_TL_MAYU_R_C_WJ          = 0x5F,
    ROB_BONE_N_TOOTH_UPPER           = 0x60,
    ROB_BONE_TL_TOOTH_UPPER_WJ       = 0x61,
    ROB_BONE_N_KUBI_WJ_EX            = 0x62,
    ROB_BONE_N_WAKI_L                = 0x63,
    ROB_BONE_KL_WAKI_L_WJ            = 0x64,
    ROB_BONE_TL_UP_KATA_L            = 0x65,
    ROB_BONE_C_KATA_L                = 0x66,
    ROB_BONE_KATA_L_WJ_CU            = 0x67,
    ROB_BONE_UDE_L_WJ                = 0x68,
    ROB_BONE_KL_TE_L_WJ              = 0x69,
    ROB_BONE_N_HITO_L_EX             = 0x6A,
    ROB_BONE_NL_HITO_L_WJ            = 0x6B,
    ROB_BONE_NL_HITO_B_L_WJ          = 0x6C,
    ROB_BONE_NL_HITO_C_L_WJ          = 0x6D,
    ROB_BONE_N_KO_L_EX               = 0x6E,
    ROB_BONE_NL_KO_L_WJ              = 0x6F,
    ROB_BONE_NL_KO_B_L_WJ            = 0x70,
    ROB_BONE_NL_KO_C_L_WJ            = 0x71,
    ROB_BONE_N_KUSU_L_EX             = 0x72,
    ROB_BONE_NL_KUSU_L_WJ            = 0x73,
    ROB_BONE_NL_KUSU_B_L_WJ          = 0x74,
    ROB_BONE_NL_KUSU_C_L_WJ          = 0x75,
    ROB_BONE_N_NAKA_L_EX             = 0x76,
    ROB_BONE_NL_NAKA_L_WJ            = 0x77,
    ROB_BONE_NL_NAKA_B_L_WJ          = 0x78,
    ROB_BONE_NL_NAKA_C_L_WJ          = 0x79,
    ROB_BONE_N_OYA_L_EX              = 0x7A,
    ROB_BONE_NL_OYA_L_WJ             = 0x7B,
    ROB_BONE_NL_OYA_B_L_WJ           = 0x7C,
    ROB_BONE_NL_OYA_C_L_WJ           = 0x7D,
    ROB_BONE_N_STE_L_WJ_EX           = 0x7E,
    ROB_BONE_N_SUDE_L_WJ_EX          = 0x7F,
    ROB_BONE_N_SUDE_B_L_WJ_EX        = 0x80,
    ROB_BONE_N_HIJI_L_WJ_EX          = 0x81,
    ROB_BONE_N_UP_KATA_L_EX          = 0x82,
    ROB_BONE_N_SKATA_L_WJ_CD_EX      = 0x83,
    ROB_BONE_N_SKATA_B_L_WJ_CD_CU_EX = 0x84,
    ROB_BONE_N_SKATA_C_L_WJ_CD_CU_EX = 0x85,
    ROB_BONE_N_WAKI_R                = 0x86,
    ROB_BONE_KL_WAKI_R_WJ            = 0x87,
    ROB_BONE_TL_UP_KATA_R            = 0x88,
    ROB_BONE_C_KATA_R                = 0x89,
    ROB_BONE_KATA_R_WJ_CU            = 0x8A,
    ROB_BONE_UDE_R_WJ                = 0x8B,
    ROB_BONE_KL_TE_R_WJ              = 0x8C,
    ROB_BONE_N_HITO_R_EX             = 0x8D,
    ROB_BONE_NL_HITO_R_WJ            = 0x8E,
    ROB_BONE_NL_HITO_B_R_WJ          = 0x8F,
    ROB_BONE_NL_HITO_C_R_WJ          = 0x90,
    ROB_BONE_N_KO_R_EX               = 0x91,
    ROB_BONE_NL_KO_R_WJ              = 0x92,
    ROB_BONE_NL_KO_B_R_WJ            = 0x93,
    ROB_BONE_NL_KO_C_R_WJ            = 0x94,
    ROB_BONE_N_KUSU_R_EX             = 0x95,
    ROB_BONE_NL_KUSU_R_WJ            = 0x96,
    ROB_BONE_NL_KUSU_B_R_WJ          = 0x97,
    ROB_BONE_NL_KUSU_C_R_WJ          = 0x98,
    ROB_BONE_N_NAKA_R_EX             = 0x99,
    ROB_BONE_NL_NAKA_R_WJ            = 0x9A,
    ROB_BONE_NL_NAKA_B_R_WJ          = 0x9B,
    ROB_BONE_NL_NAKA_C_R_WJ          = 0x9C,
    ROB_BONE_N_OYA_R_EX              = 0x9D,
    ROB_BONE_NL_OYA_R_WJ             = 0x9E,
    ROB_BONE_NL_OYA_B_R_WJ           = 0x9F,
    ROB_BONE_NL_OYA_C_R_WJ           = 0xA0,
    ROB_BONE_N_STE_R_WJ_EX           = 0xA1,
    ROB_BONE_N_SUDE_R_WJ_EX          = 0xA2,
    ROB_BONE_N_SUDE_B_R_WJ_EX        = 0xA3,
    ROB_BONE_N_HIJI_R_WJ_EX          = 0xA4,
    ROB_BONE_N_UP_KATA_R_EX          = 0xA5,
    ROB_BONE_N_SKATA_R_WJ_CD_EX      = 0xA6,
    ROB_BONE_N_SKATA_B_R_WJ_CD_CU_EX = 0xA7,
    ROB_BONE_N_SKATA_C_R_WJ_CD_CU_EX = 0xA8,
    ROB_BONE_KL_KOSI_Y               = 0xA9,
    ROB_BONE_KL_KOSI_XZ              = 0xAA,
    ROB_BONE_KL_KOSI_ETC_WJ          = 0xAB,
    ROB_BONE_CL_MOMO_L               = 0xAC,
    ROB_BONE_J_MOMO_L_WJ             = 0xAD,
    ROB_BONE_J_SUNE_L_WJ             = 0xAE,
    ROB_BONE_KL_ASI_L_WJ_CO          = 0xAF,
    ROB_BONE_KL_TOE_L_WJ             = 0xB0,
    ROB_BONE_N_HIZA_L_WJ_EX          = 0xB1,
    ROB_BONE_CL_MOMO_R               = 0xB2,
    ROB_BONE_J_MOMO_R_WJ             = 0xB3,
    ROB_BONE_J_SUNE_R_WJ             = 0xB4,
    ROB_BONE_KL_ASI_R_WJ_CO          = 0xB5,
    ROB_BONE_KL_TOE_R_WJ             = 0xB6,
    ROB_BONE_N_HIZA_R_WJ_EX          = 0xB7,
    ROB_BONE_N_MOMO_A_L_WJ_CD_EX     = 0xB8,
    ROB_BONE_N_MOMO_B_L_WJ_EX        = 0xB9,
    ROB_BONE_N_MOMO_C_L_WJ_EX        = 0xBA,
    ROB_BONE_N_MOMO_A_R_WJ_CD_EX     = 0xBB,
    ROB_BONE_N_MOMO_B_R_WJ_EX        = 0xBC,
    ROB_BONE_N_MOMO_C_R_WJ_EX        = 0xBD,
    ROB_BONE_N_HARA_CD_EX            = 0xBE,
    ROB_BONE_N_HARA_B_WJ_EX          = 0xBF,
    ROB_BONE_N_HARA_C_WJ_EX          = 0xC0,
    ROB_BONE_MAX                     = 0xC1,
};

enum rob_chara_type {
    ROB_CHARA_TYPE_NONE = -1,
    ROB_CHARA_TYPE_0    = 0x00,
    ROB_CHARA_TYPE_1    = 0x01,
    ROB_CHARA_TYPE_2    = 0x02,
    ROB_CHARA_TYPE_3    = 0x03,
};

enum rob_chara_data_hand_adjust_type : uint16_t {
    ROB_CHARA_DATA_HAND_ADJUST_NONE           = (uint16_t)-1,
    ROB_CHARA_DATA_HAND_ADJUST_NORMAL         = 0x00,
    ROB_CHARA_DATA_HAND_ADJUST_SHORT          = 0x01,
    ROB_CHARA_DATA_HAND_ADJUST_TALL           = 0x02,
    ROB_CHARA_DATA_HAND_ADJUST_MIN            = 0x03,
    ROB_CHARA_DATA_HAND_ADJUST_MAX            = 0x04,
    ROB_CHARA_DATA_HAND_ADJUST_OPPOSITE_CHARA = 0x05,
    ROB_CHARA_DATA_HAND_ADJUST_CUSTOM         = 0x06,
    ROB_CHARA_DATA_HAND_ADJUST_1P             = 0x07,
    ROB_CHARA_DATA_HAND_ADJUST_2P             = 0x08,
    ROB_CHARA_DATA_HAND_ADJUST_3P             = 0x09,
    ROB_CHARA_DATA_HAND_ADJUST_4P             = 0x0A,
    ROB_CHARA_DATA_HAND_ADJUST_ITEM           = 0x0F, // X
};

enum rob_osage_parts {
    ROB_OSAGE_PARTS_NONE        = -1,
    ROB_OSAGE_PARTS_LEFT        = 0x00,
    ROB_OSAGE_PARTS_RIGHT       = 0x01,
    ROB_OSAGE_PARTS_CENTER      = 0x02,
    ROB_OSAGE_PARTS_LONG_C      = 0x03,
    ROB_OSAGE_PARTS_SHORT_L     = 0x04,
    ROB_OSAGE_PARTS_SHORT_R     = 0x05,
    ROB_OSAGE_PARTS_APPEND_L    = 0x06,
    ROB_OSAGE_PARTS_APPEND_R    = 0x07,
    ROB_OSAGE_PARTS_MUFFLER     = 0x08,
    ROB_OSAGE_PARTS_WHITE_ONE_L = 0x09,
    ROB_OSAGE_PARTS_PONY        = 0x0A,
    ROB_OSAGE_PARTS_ANGEL_L     = 0x0B,
    ROB_OSAGE_PARTS_ANGEL_R     = 0x0C,
    ROB_OSAGE_PARTS_MAX         = 0x0D,
};

enum ExNodeType {
    EX_NONE       = 0x00,
    EX_OSAGE      = 0x01,
    EX_EXPRESSION = 0x02,
    EX_CONSTRAINT = 0x03,
    EX_CLOTH      = 0x04,
};

namespace SkinParam {
    enum CollisionType {
        CollisionTypeEnd     = 0x0,
        CollisionTypeBall    = 0x1,
        CollisionTypeCapsule = 0x2,
        CollisionTypePlane   = 0x3,
        CollisionTypeEllipse = 0x4,
        CollisionTypeAABB    = 0x5,
        CollisionTypeMax     = 0x6,
    };

    enum RootCollisionType {
        RootCollisionTypeEnd             = 0x00,
        RootCollisionTypeCapsule         = 0x01,
        RootCollisionTypeCapsuleWithRoot = 0x02,
        RootCollisionTypeMax             = 0x03,
    };
}

struct rob_chara;
struct rob_chara_bone_data;

struct bone_node_expression_data {
    vec3 position;
    vec3 rotation;
    vec3 scale;
    vec3 parent_scale;
};

static_assert(sizeof(bone_node_expression_data) == 0x30, "\"bone_node_expression_data\" struct should have a size of 0x30");

struct bone_node {
    const char* name;
    mat4* mat;
    bone_node* parent;
    bone_node_expression_data exp_data;
    mat4* ex_data_mat;
};

static_assert(sizeof(bone_node) == 0x50, "\"bone_node\" struct should have a size of 0x50");

struct rob_sleeve_data {
    float_t radius;
    float_t cyofs;
    float_t czofs;
    float_t ymin;
    float_t ymax;
    float_t zmin;
    float_t zmax;
    float_t mune_xofs;
    float_t mune_yofs;
    float_t mune_zofs;
    float_t mune_rad;
};

static_assert(sizeof(rob_sleeve_data) == 0x2C, "\"rob_sleeve_data\" struct should have a size of 0x2C");

struct eyes_adjust {
    bool xrot_adjust;
    eyes_base_adjust_type base_adjust;
    float_t neg;
    float_t pos;
};

static_assert(sizeof(eyes_adjust) == 0x10, "\"eyes_adjust\" struct should have a size of 0x10");

union rob_chara_pv_data_item {
    struct {
        int32_t head;
        int32_t face;
        int32_t chest;
        int32_t back;
    };
    int32_t arr[4];
};

static_assert(sizeof(rob_chara_pv_data_item) == 0x10, "\"rob_chara_pv_data_item\" struct should have a size of 0x10");

struct rob_chara_pv_data {
    rob_chara_type type;
    bool field_4;
    bool field_5;
    bool field_6;
    vec3 field_8;
    int16_t rot_y_int16;
    int16_t field_16;
    rob_sleeve_data sleeve_l;
    rob_sleeve_data sleeve_r;
    int32_t field_70;
    uint32_t motion_face_ids[10];
    int32_t chara_size_index;
    bool height_adjust;
    rob_chara_pv_data_item item;
    eyes_adjust eyes_adjust;
};

static_assert(sizeof(rob_chara_pv_data) == 0xC4, "\"rob_chara_pv_data\" struct should have a size of 0xC4");

struct rob_chara_item_equip_object;

class ExNodeBlock;

struct ExNodeBlock_vtbl {
    ExNodeBlock* (FASTCALL* Dispose)(ExNodeBlock* This, uint8_t);
    void(FASTCALL* Field_8)(ExNodeBlock* This);
    void(FASTCALL* Field_10)(ExNodeBlock* This);
    void(FASTCALL* Field_18)(ExNodeBlock* This, int32_t stage, bool disable_external_force);
    void(FASTCALL* Field_20)(ExNodeBlock* This);
    void(FASTCALL* SetOsagePlayData)(ExNodeBlock* This);
    void(FASTCALL* Disp)(ExNodeBlock* This);
    void(FASTCALL* Reset)(ExNodeBlock* This);
    void(FASTCALL* Field_40)(ExNodeBlock* This);
    void(FASTCALL* Field_48)(ExNodeBlock* This);
    void(FASTCALL* Field_50)(ExNodeBlock* This);
    void(FASTCALL* Field_58)(ExNodeBlock* This);
};

static_assert(sizeof(ExNodeBlock_vtbl) == 0x60, "\"ExNodeBlock_vtbl\" struct should have a size of 0x60");

class ExNodeBlock {
public:
    ExNodeBlock_vtbl* __vftable;
    bone_node* bone_node_ptr;
    ExNodeType type;
    const char* name;
    bone_node* parent_bone_node;
    prj::string parent_name;
    ExNodeBlock* parent_node;
    rob_chara_item_equip_object* item_equip_object;
    bool field_58;
    bool field_59;
    bool has_children_node;
};

static_assert(sizeof(ExNodeBlock) == 0x60, "\"ExNodeBlock\" struct should have a size of 0x60");

struct RobOsageNode;

struct RobOsageNodeDataNormalRef {
    bool set;
    RobOsageNode* n;
    RobOsageNode* u;
    RobOsageNode* d;
    RobOsageNode* l;
    RobOsageNode* r;
    mat4 mat;
};

static_assert(sizeof(RobOsageNodeDataNormalRef) == 0x70, "\"RobOsageNodeDataNormalRef\" struct should have a size of 0x70");

struct skin_param_hinge {
    float_t ymin;
    float_t ymax;
    float_t zmin;
    float_t zmax;
};

static_assert(sizeof(skin_param_hinge) == 0x10, "\"skin_param_hinge\" struct should have a size of 0x10");

struct skin_param_osage_node {
    float_t coli_r;
    float_t weight;
    float_t inertial_cancel;
    skin_param_hinge hinge;
};

static_assert(sizeof(skin_param_osage_node) == 0x1C, "\"skin_param_osage_node\" struct should have a size of 0x1C");

struct RobOsageNodeResetData {
    vec3 pos;
    vec3 delta_pos;
    vec3 rotation;
    float_t length;
};

static_assert(sizeof(RobOsageNodeResetData) == 0x28, "\"RobOsageNodeResetData\" struct should have a size of 0x28");

struct skin_param_osage_root;

struct RobOsageNodeData {
    float_t force;
    prj::vector<RobOsageNode*> boc;
    RobOsageNodeDataNormalRef normal_ref;
    skin_param_osage_node skp_osg_node;
};

static_assert(sizeof(RobOsageNodeData) == 0xB0, "\"RobOsageNodeData\" struct should have a size of 0xB0");

struct opd_blend_data {
    int32_t motion_id;
    float_t frame;
    float_t frame_count;
    bool use_blend;
    MotionBlendType type;
    float_t blend;
};

static_assert(sizeof(opd_blend_data) == 0x18, "\"opd_blend_data\" struct should have a size of 0x18");

struct opd_vec3_data {
    const float_t* x;
    const float_t* y;
    const float_t* z;
};

static_assert(sizeof(opd_vec3_data) == 0x18, "\"opd_vec3_data\" struct should have a size of 0x18");

struct opd_node_data {
    float_t length;
    vec3 rotation;
};

static_assert(sizeof(opd_node_data) == 0x10, "\"opd_node_data\" struct should have a size of 0x10");

struct opd_node_data_pair {
    opd_node_data curr;
    opd_node_data prev;
};

static_assert(sizeof(opd_node_data_pair) == 0x20, "\"opd_node_data_pair\" struct should have a size of 0x20");

struct RobOsageNode {
    float_t length;
    vec3 pos;
    vec3 fixed_pos;
    vec3 delta_pos;
    vec3 vel;
    float_t child_length;
    bone_node* bone_node_ptr;
    mat4* bone_node_mat;
    mat4 mat;
    RobOsageNode* sibling_node;
    float_t max_distance;
    vec3 rel_pos;
    RobOsageNodeResetData reset_data;
    float_t hit;
    float_t friction;
    vec3 external_force;
    float_t force;
    RobOsageNodeData* data_ptr;
    RobOsageNodeData data;
    prj::vector<opd_vec3_data> opd_data;
    opd_node_data_pair opd_node_data;
};

static_assert(sizeof(RobOsageNode) == 0x1D0, "\"RobOsageNode\" struct should have a size of 0x1D0");

namespace SkinParam {
    struct CollisionParam {
        CollisionType type;
        int32_t node_idx[2];
        float_t radius;
        vec3 pos[2];

        CollisionParam();
    };

    static_assert(sizeof(SkinParam::CollisionParam) == 0x28, "\"SkinParam::CollisionParam\" struct should have a size of 0x28");
};

struct skin_param_osage_root_normal_ref {
    prj::string n;
    prj::string u;
    prj::string d;
    prj::string l;
    prj::string r;
};

static_assert(sizeof(skin_param_osage_root_normal_ref) == 0xA0, "\"skin_param_osage_root_normal_ref\" struct should have a size of 0xA0");

struct skin_param_osage_root_boc {
    int32_t ed_node;
    prj::string ed_root;
    int32_t st_node;
};

static_assert(sizeof(skin_param_osage_root_boc) == 0x30, "\"skin_param_osage_root_boc\" struct should have a size of 0x30");

struct skin_param_osage_root {
    int32_t field_0;
    float_t force;
    float_t force_gain;
    float_t air_res;
    float_t rot_y;
    float_t rot_z;
    float_t init_rot_y;
    float_t init_rot_z;
    float_t hinge_y;
    float_t hinge_z;
    const char* name;
    prj::vector<SkinParam::CollisionParam> coli;
    float_t coli_r;
    float_t friction;
    float_t wind_afc;
    int32_t yz_order;
    prj::vector<skin_param_osage_root_boc> boc;
    SkinParam::RootCollisionType coli_type;
    float_t stiffness;
    float_t move_cancel;
    prj::string colli_tgt_osg;
    prj::vector<skin_param_osage_root_normal_ref> normal_ref;
};

static_assert(sizeof(skin_param_osage_root) == 0xB8, "\"skin_param_osage_root\" struct should have a size of 0xB8");

struct skin_param {
    prj::vector<SkinParam::CollisionParam> coli;
    float_t friction;
    float_t wind_afc;
    float_t air_res;
    vec3 rot;
    vec3 init_rot;
    SkinParam::RootCollisionType coli_type;
    float_t stiffness;
    float_t move_cancel;
    float_t coli_r;
    skin_param_hinge hinge;
    float_t force;
    float_t force_gain;
    prj::vector<RobOsageNode>* colli_tgt_osg;
};

static_assert(sizeof(skin_param) == 0x70, "\"skin_param\" struct should have a size of 0x70");

struct OsageCollision {
    struct Work {
        SkinParam::CollisionType type;
        float_t radius;
        vec3 pos[2];
        vec3 vec_center;
        float_t vec_center_length;
        float_t vec_center_length_squared;
        float_t friction;
    };

    static_assert(sizeof(OsageCollision::Work) == 0x38, "\"OsageCollision::Work\" struct should have a size of 0x38");

    prj::vector<Work> work_list;
};

static_assert(sizeof(OsageCollision) == 0x18, "\"OsageCollision\" struct should have a size of 0x18");

struct osage_ring_data {
    float_t ring_rectangle_x;
    float_t ring_rectangle_y;
    float_t ring_rectangle_width;
    float_t ring_rectangle_height;
    float_t ring_height;
    float_t ring_out_height;
    bool init;
    OsageCollision coli;
    prj::vector<SkinParam::CollisionParam> skp_root_coli;
};

static_assert(sizeof(osage_ring_data) == 0x50, "\"OsageCollision\" struct should have a size of 0x50");

struct skin_param_file_data;

struct CLOTHNode {
    uint32_t flags;
    vec3 pos;
    vec3 fixed_pos;
    vec3 prev_pos;
    vec3 delta_pos;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    float_t tangent_sign;
    vec2 texcoord;
    vec3 direction;
    float_t dist_up;
    float_t dist_down;
    float_t dist_right;
    float_t dist_left;
    vec3 field_80;
    RobOsageNodeResetData reset_data;
    prj::vector<opd_vec3_data> opd_data;
    opd_node_data_pair opd_node_data;
};

static_assert(sizeof(CLOTHNode) == 0xF0, "\"CLOTHNode\" struct should have a size of 0xF0");

struct CLOTHLine {
    size_t idx[2];
    float_t length;
};

static_assert(sizeof(CLOTHLine) == 0x18, "\"CLOTHLine\" struct should have a size of 0x18");

class CLOTH;

struct CLOTH_vtbl {
    CLOTH* (FASTCALL* Dispose)(CLOTH* This, uint8_t);
    void(FASTCALL* Field_8)(CLOTH* This);
    void(FASTCALL* Field_10)(CLOTH* This);
    void(FASTCALL* Field_18)(CLOTH* This, int32_t stage, bool disable_external_force);
    void(FASTCALL* Field_20)(CLOTH* This);
    void(FASTCALL* SetOsagePlayData)(CLOTH* This);
    void(FASTCALL* Disp)(CLOTH* This);
    void(FASTCALL* Reset)(CLOTH* This);
    void(FASTCALL* Field_40)(CLOTH* This);
    void(FASTCALL* Field_48)(CLOTH* This);
    void(FASTCALL* Field_50)(CLOTH* This);
    void(FASTCALL* Field_58)(CLOTH* This);
};

static_assert(sizeof(CLOTH_vtbl) == 0x60, "\"CLOTH_vtbl\" struct should have a size of 0x60");

class CLOTH {
public:
    CLOTH_vtbl* __vftable;
    int32_t field_8;
    size_t root_count;
    size_t nodes_count;
    prj::vector<CLOTHNode> nodes;
    vec3 wind_direction;
    float_t field_44;
    bool set_external_force;
    vec3 external_force;
    prj::vector<CLOTHLine> lines;
    skin_param* skin_param_ptr;
    skin_param skin_param;
    OsageCollision::Work coli[64];
    OsageCollision::Work coli_ring[64];
    osage_ring_data ring;
    mat4* mats;
};

static_assert(sizeof(CLOTH) == 0x1D40, "\"CLOTH\" struct should have a size of 0x1D40");

struct RobClothRoot {
    vec3 pos;
    vec3 normal;
    vec4 tangent;
    bone_node* node[4];
    mat4* node_mat[4];
    mat4* mat[4];
    float_t weight[4];
    mat4 field_98;
    mat4 field_D8;
    mat4 field_118;
};

static_assert(sizeof(RobClothRoot) == 0x158, "\"RobClothRoot\" struct should have a size of 0x158");

struct RobClothSubMeshArray {
    obj_sub_mesh arr[4];
};

static_assert(sizeof(RobClothSubMeshArray) == 0x1C0, "\"RobClothSubMeshArray\" struct should have a size of 0x1C0");

class RobCloth : public CLOTH {
public:
    prj::vector<RobClothRoot> root;
    rob_chara_item_equip_object* itm_eq_obj;
    struct obj_skin_block_cloth_root* cls_root;
    struct obj_skin_block_cloth* cls_data;
    float_t move_cancel;
    bool osage_reset;
    obj_mesh mesh[2];
    RobClothSubMeshArray submesh[2];
    obj_axis_aligned_bounding_box axis_aligned_bounding_box;
#if SHARED_OBJECT_BUFFER
    obj_mesh_vertex_buffer_aft vertex_buffer[2];
#else
    obj_mesh_vertex_buffer vertex_buffer[2];
#endif
    obj_mesh_index_buffer index_buffer[2];
    prj::map<prj::pair<int32_t, int32_t>, prj::list<RobOsageNodeResetData>> motion_reset_data;
    prj::list<RobOsageNodeResetData>* reset_data_list;
};

static_assert(sizeof(RobCloth) == 0x23C0, "\"RobCloth\" struct should have a size of 0x23C0");

class ExClothBlock : public ExNodeBlock {
public:
    RobCloth rob;
    struct obj_skin_block_cloth* cls_data;
    mat4* mats;
    size_t index;
};

static_assert(sizeof(ExClothBlock) == 0x2438, "\"ExClothBlock\" struct should have a size of 0x2438");

struct skin_param_file_data {
    skin_param skin_param;
    prj::vector<RobOsageNodeData> nodes_data;
    bool field_88;
};

static_assert(sizeof(skin_param_file_data) == 0x90, "\"skin_param_file_data\" struct should have a size of 0x90");

struct osage_setting_osg_cat {
    rob_osage_parts parts;
    size_t exf;
};

static_assert(sizeof(osage_setting_osg_cat) == 0x10, "\"osage_setting_osg_cat\" struct should have a size of 0x10");

struct RobOsage {
    skin_param* skin_param_ptr;
    bone_node_expression_data exp_data;
    prj::vector<RobOsageNode> nodes;
    RobOsageNode end_node;
    skin_param skin_param;
    osage_setting_osg_cat osage_setting;
    bool field_2A0;
    bool field_2A1;
    float_t field_2A4;
    OsageCollision::Work coli[64];
    OsageCollision::Work coli_ring[64];
    vec3 wind_direction;
    float_t field_1EB4;
    int32_t yz_order;
    int32_t field_1EBC;
    mat4* root_matrix_ptr;
    mat4 root_matrix;
    float_t move_cancel;
    bool field_1F0C;
    bool osage_reset;
    bool prev_osage_reset;
    bool disable_collision;
    osage_ring_data ring;
    prj::map<prj::pair<int32_t, int32_t>, prj::list<RobOsageNodeResetData>> motion_reset_data;
    prj::list<RobOsageNodeResetData>* reset_data_list;
    bool set_external_force;
    vec3 external_force;

    void SetAirRes(float_t air_res);
    void SetColiR(float_t coli_r);
    void SetForce(float_t force, float_t force_gain);
    void SetHinge(float_t hinge_y, float_t hinge_z);
    void SetInitRot(float_t init_rot_y, float_t init_rot_z);
    void SetMotionResetData(uint32_t motion_id, float_t frame);
    void SetNodesExternalForce(vec3* external_force, float_t strength);
    void SetNodesForce(float_t force);
    void SetRot(float_t rot_y, float_t rot_z);
};

static_assert(sizeof(RobOsage) == 0x1F88, "\"RobOsage\" struct should have a size of 0x1F88");

class ExOsageBlock : public ExNodeBlock {
public:
    size_t index;
    RobOsage rob;
    mat4* mats;
    int32_t field_1FF8;
    float_t step;
};

static_assert(sizeof(ExOsageBlock) == 0x2000, "\"ExOsageBlock\" struct should have a size of 0x2000");

struct rob_chara_item_equip;

struct rob_chara_item_equip_object {
    size_t index;
    mat4* mats;
    object_info obj_info;
    int32_t field_14;
    prj::vector<texture_pattern_struct> texture_pattern;
    texture_data_struct texture_data;
    bool null_blocks_data_set;
    bone_node_expression_data exp_data;
    float_t alpha;
    mdl::ObjFlags obj_flags;
    bool can_disp;
    int32_t field_A4;
    mat4* mat;
    int32_t osage_iterations;
    bone_node* bone_nodes;
    prj::vector<ExNodeBlock*> node_blocks;
    prj::vector<bone_node> ex_data_bone_nodes;
    prj::vector<mat4> ex_data_bone_mats;
    prj::vector<mat4> ex_data_mats;
    prj::vector_pair<const char*, uint32_t> ex_bones;
    int64_t field_138;
    prj::vector<struct ExNullBlock*> null_blocks;
    prj::vector<ExOsageBlock*> osage_blocks;
    prj::vector<struct ExConstraintBlock*> constraint_blocks;
    prj::vector<struct ExExpressionBlock*> expression_blocks;
    prj::vector<ExClothBlock*> cloth_blocks;
    bool field_1B8;
    size_t osage_nodes_count;
    bool use_opd;
    obj_skin_ex_data* skin_ex_data;
    obj_skin* skin;
    rob_chara_item_equip* item_equip;

    void disp(const mat4& mat);
    int32_t get_bone_index(const char* name);
    bone_node* get_bone_node(int32_t bone_index);
    bone_node* get_bone_node(const char* name);
    void skp_load(void* can_prop); // CanonicalProperties
};

static_assert(sizeof(rob_chara_item_equip_object) == 0x1E8, "\"rob_chara_item_equip_object\" struct should have a size of 0x1E8");

struct rob_chara_item_equip {
    bone_node* bone_nodes;
    mat4* matrices;
    rob_chara_item_equip_object* item_equip_object;
    int32_t field_18[31];
    bool item_equip_range;
    item_id first_item_equip_object;
    item_id max_item_equip_object;
    int32_t field_A0;
    shadow_type_enum shadow_type;
    vec3 position;
    prj::vector<texture_pattern_struct> texture_pattern;
    object_info field_D0;
    item_id field_D4;
    bool disable_update;
    int32_t field_DC;
    vec4 texture_color_coefficients;
    float_t wet;
    float_t wind_strength;
    bool chara_color;
    bool npr_flag;
    mat4 mat;
    mat4 field_13C[30];
    int32_t field_8BC;
    int32_t field_8C0;
    int32_t field_8C4;
    int32_t field_8C8;
    int32_t field_8CC;
    int32_t field_8D0;
    int32_t field_8D4;
    int32_t field_8D8;
    int32_t field_8DC;
    int32_t field_8E0;
    int32_t field_8E4;
    int32_t field_8E8;
    int32_t field_8EC;
    int32_t field_8F0;
    int32_t field_8F4;
    int32_t field_8F8;
    int32_t field_8FC;
    int64_t field_900;
    int64_t field_908;
    int64_t field_910;
    int64_t field_918;
    int64_t field_920;
    int64_t field_928;
    int64_t field_930;
    float_t osage_step;
    bool use_opd;
    prj::vector<opd_blend_data> opd_blend_data;
    bool parts_short;
    bool parts_append;
    bool parts_white_one_l;

    rob_chara_item_equip_object* get_item_equip_object(item_id id);
};

static_assert(sizeof(rob_chara_item_equip) == 0x960, "\"rob_chara_item_equip\" struct should have a size of 0x960");

struct item_cos_texture_change_tex {
    texture* org;
    texture* chg;
    bool changed;
};

static_assert(sizeof(item_cos_texture_change_tex) == 0x18, "\"rob_chara_item_cos_data\" struct should have a size of 0x18");

struct rob_chara_item_cos_data {
    ::chara_index chara_index;
    ::chara_index chara_index_2nd;
    item_cos_data cos;
    item_cos_data cos_2nd;
    prj::map<int32_t, std::vector<item_cos_texture_change_tex>> texture_change;
    prj::map<int32_t, std::vector<uint32_t>> item_change;
    prj::map<object_info, item_id> field_F0;
    prj::map<int32_t, item_id> field_100;
    std::vector<texture_pattern_struct> texture_pattern[31];
    std::map<int32_t, object_info> head_replace;
};

static_assert(sizeof(rob_chara_item_cos_data) == 0x408, "\"rob_chara_item_cos_data\" struct should have a size of 0x408");

struct struc_264 {
    uint8_t data[0x1D8];
};

static_assert(sizeof(struc_264) == 0x1D8, "\"struc_264\" struct should have a size of 0x1D8");

struct RobSubAction {
    uint8_t data[0xB0];
};

static_assert(sizeof(RobSubAction) == 0xB0, "\"RobSubAction\" struct should have a size of 0xB0");

struct struc_389 {
    float_t frame;
    float_t prev_frame;
    float_t last_set_frame;
};

static_assert(sizeof(struc_389) == 0x0C, "\"struc_389\" struct should have a size of 0x0C");

struct struc_406 {
    float_t frame;
    float_t field_4;
    float_t step;
};

static_assert(sizeof(struc_406) == 0x0C, "\"struc_406\" struct should have a size of 0x0C");

struct rob_chara_data_hand_adjust {
    bool enable;
    int16_t scale_select;
    rob_chara_data_hand_adjust_type type;
    float_t current_scale;
    float_t scale;
    float_t duration;
    float_t current_time;
    float_t rotation_blend;
    float_t scale_blend;
    bool enable_scale;
    bool disable_x;
    bool disable_y;
    bool disable_z;
    vec3 offset;
    vec3 field_30;
    float_t arm_length;
    int32_t field_40;
};

static_assert(sizeof(rob_chara_data_hand_adjust) == 0x44, "\"rob_chara_data_hand_adjust\" struct should have a size of 0x44");

struct rob_chara_motion {
    uint32_t motion_id;
    uint32_t prev_motion_id;
    struc_389 frame_data;
    struc_406 step_data;
    uint8_t data[0x1198];
    rob_chara_data_hand_adjust hand_adjust[2];
    rob_chara_data_hand_adjust hand_adjust_prev[2];
    uint8_t data1[0x30];
};

static_assert(sizeof(rob_chara_motion) == 0x12F8, "\"rob_chara_motion\" struct should have a size of 0x12F8");

struct struc_526 {
    int32_t field_0;
    int32_t field_4;
};

struct struc_228 {
    int32_t field_0;
    int32_t field_4;
    uint32_t field_8;
    int32_t field_C;
};

struct struc_227 {
    int32_t field_0;
    float_t field_4;
    float_t field_8;
};

struct struc_652 {
    int32_t motion_id;
    float_t frame_count;
    float_t frame;
    int16_t field_C;
    struc_228 field_10;
    struc_228 field_20;
    struc_228 field_30;
    struc_228 field_40;
    int16_t field_50;
    int16_t field_52;
    int16_t field_54;
    int32_t field_58;
    int32_t field_5C;
    int32_t field_60;
    int32_t field_64;
    int32_t field_68;
    int32_t loop_count;
    float_t loop_begin;
    float_t loop_end;
    float_t field_78;
    float_t field_7C;
    float_t field_80;
    int8_t field_84;
    int32_t field_88;
    int32_t field_8C;
    int32_t field_90;
    int16_t field_94;
    int16_t field_96;
    int16_t field_98;
    int32_t field_9C;
    int16_t field_A0;
    int32_t field_A4;
    int64_t field_A8;
    struc_227 field_B0[26];
    int32_t field_1E8;
    float_t field_1EC;
    float_t field_1F0;
    float_t field_1F4;
    float_t field_1F8;
    float_t field_1FC;
    float_t field_200;
    int32_t field_204;
    int32_t field_208;
    int32_t field_20C;
    int64_t field_210;
    float_t field_218;
    float_t field_21C;
    int16_t field_220;
    prj::list<void*> field_228;
    int16_t field_238;
    float_t field_23C;
    int32_t field_240;
    int16_t field_244;
    const void* field_248;
    int64_t field_250;
    float_t field_258;
    int32_t field_25C;
    struc_526 field_260;
    struc_526 field_268;
    int32_t field_270;
    int16_t field_274;
    int16_t field_276;
    int32_t field_278;
    int32_t field_27C;
    int32_t field_280;
    int16_t field_284;
    int64_t field_288;
    int32_t field_290;
    int32_t field_294;
    int32_t field_298;
    float_t field_29C;
    int8_t field_2A0;
    float_t field_2A4;
    float_t field_2A8;
    float_t field_2AC;
    int64_t field_2B0;
    int16_t field_2B8;
    int32_t field_2BC;
    float_t field_2C0;
    float_t field_2C4;
    int32_t field_2C8;
    int32_t field_2CC;
    int64_t field_2D0;
    int64_t field_2D8;
    int64_t field_2E0;
    int16_t field_2E8;
    int32_t field_2EC;
    int32_t field_2F0;
    struc_526 field_2F4;
    int32_t field_2FC;
    int8_t field_300;
    int32_t field_304;
    int32_t field_308;
    float_t field_30C;
    int32_t field_310;
    int16_t field_314;
    vec3 field_318;
    float_t field_324;
    float_t field_328;
    int32_t iterations;
};

struct struc_377 {
    struct mothead_data* current;
    struct mothead_data* data;
};

struct struc_226 {
    int8_t field_0[27];
};

struct struc_225 {
    float_t field_0[27];
};

struct struc_224 {
    int32_t field_0[27];
};

struct struc_306 {
    int16_t field_0;
    float_t frame;
    float_t field_8;
    int16_t field_C;
    int16_t field_E;
    vec3 field_10;
    vec3 field_1C;
    vec3 field_28;
    int32_t field_34;
    int32_t field_38;
    int32_t field_3C;
    int32_t field_40;
    int32_t field_44;
    int32_t field_48;
};

struct struc_651 {
    struc_377 field_0;
    int32_t field_10;
    int32_t field_14;
    int32_t field_18;
    float_t field_1C;
    vec3 field_20;
    struc_226 field_2C[3];
    struc_225 field_80[3];
    struc_224 field_1C4[3];
    int64_t field_308;
    float_t field_310;
    float_t field_314;
    int8_t field_318;
    int32_t field_31C;
    float_t field_320;
    float_t field_324;
    float_t field_328;
    float_t field_32C;
    float_t field_330;
    float_t field_334;
    int8_t field_338;
    struc_306 field_33C[4];
};

struct struc_223 {
    struc_652 field_0;
    struc_651 field_330;
    int64_t* field_7A0;
    int32_t motion_set_id;
};

static_assert(sizeof(struc_223) == 0x7B0, "\"struc_223\" struct should have a size of 0x7B0");

struct rob_chara_data_miku_rot {
    uint8_t data[0xAC];
};

static_assert(sizeof(rob_chara_data_miku_rot) == 0xAC, "\"rob_chara_data_miku_rot\" struct should have a size of 0xAC");

struct rob_chara_adjust_data {
    float_t scale;
    bool height_adjust;
    float_t pos_adjust_y;
    vec3 pos_adjust;
    vec3 offset;
    bool offset_x;
    bool offset_y;
    bool offset_z;
    bool get_global_position;
    vec3 pos;
    mat4 mat;
    float_t left_hand_scale;
    float_t right_hand_scale;
    float_t left_hand_scale_default;
    float_t right_hand_scale_default;
};

static_assert(sizeof(rob_chara_adjust_data) == 0x84, "\"rob_chara_adjust_data\" struct should have a size of 0x84");

struct struc_209 {
    uint8_t data[0x1F28];
};

static_assert(sizeof(struc_209) == 0x1F28, "\"struc_209\" struct should have a size of 0x1F28");

struct rob_chara_data {
    uint8_t field_0;
    uint8_t field_1;
    uint8_t field_2;
    uint8_t field_3;
    int32_t field_4;
    struc_264 field_8;
    RobSubAction rob_sub_action;
    rob_chara_motion motion;
    struc_223 field_1588;
    rob_chara_data_miku_rot miku_rot;
    rob_chara_adjust_data adjust_data;
    struc_209 field_1E68;
    float_t field_3D90;
    int32_t field_3D94;
    int16_t field_3D98;
    int16_t field_3D9A;
    int32_t field_3D9C;
    int32_t field_3DA0;
    int8_t field_3DA4;
    int64_t field_3DA8;
    int64_t field_3DB0;
    int32_t field_3DB8;
    int32_t field_3DBC;
    int32_t field_3DC0;
    int32_t field_3DC4;
    int32_t field_3DC8;
    int32_t field_3DCC;
    int32_t field_3DD0;
    float_t field_3DD4;
    int32_t field_3DD8;
    float_t field_3DDC;
    int8_t field_3DE0;
};

static_assert(sizeof(rob_chara_data) == 0x3DE8, "\"rob_chara_data\" struct should have a size of 0x3DE8");

struct rob_touch {
    uint8_t data[0x28];
};

static_assert(sizeof(rob_touch) == 0x28, "\"rob_touch\" struct should have a size of 0x28");

struct rob_chara {
    int8_t chara_id;
    int8_t field_1;
    int8_t field_2;
    int8_t field_3;
    int32_t type;
    int16_t field_8;
    int16_t field_A;
    bool field_C;
    bool field_D;
    chara_index chara_index;
    int32_t cos_id;
    int32_t field_18;
    float_t frame_speed;
    void* field_20;
    struct rob_chara_bone_data* bone_data;
    rob_chara_item_equip* item_equip;
    rob_chara_item_cos_data item_cos_data;
    rob_chara_data data;
    rob_chara_data data_prev;
    struct chara_init_data* chara_init_data;
    struct rob_detail* rob_detail;
    rob_chara_pv_data pv_data;
    int32_t field_80E4;
    rob_touch rob_touch;

    inline bool is_visible() {
        return !!(data.field_0 & 0x01);
    }
};

static_assert(sizeof(rob_chara) == 0x8110, "\"rob_chara\" struct should have a size of 0x8110");

#define ROB_CHARA_COUNT 6

extern const mat4* (FASTCALL* rob_chara_bone_data_get_mats_mat)(size_t rob_bone_data, size_t index);
extern bool (FASTCALL* pv_osage_manager_array_get_disp)(int32_t* chara_id);
extern const char* (FASTCALL* chara_index_get_auth_3d_name)(chara_index chara_index);
extern void(FASTCALL* sub_1405163C0)(rob_chara* rob_chr, int32_t index, mat4* mat);
extern float_t(FASTCALL* rob_chara_get_max_face_depth)(rob_chara* rob_chr);
extern const mat4* (FASTCALL* rob_chara_get_bone_data_mat)(rob_chara* rob_chr, mot_bone_index index);
extern const mat4* (FASTCALL* sub_140516740)(rob_chara* rob_chr);
extern bool(FASTCALL* rob_chara_array_check_visibility)(size_t rob_chr_smth, int32_t chara_id);
extern rob_chara* (FASTCALL* rob_chara_array_get)(size_t rob_chr_smth, int32_t chara_id);
extern size_t(FASTCALL* get_rob_chara_smth)();
extern rob_chara_item_equip* (FASTCALL* rob_chara_array_get_item_equip)(size_t rob_chr_smth, int32_t chara_id);
extern size_t(FASTCALL* rob_chara_array_get_bone_data)(size_t rob_chr_smth, int32_t chara_id);
extern bool (FASTCALL* rob_chara_pv_data_array_check_chara_id)(size_t rob_chr_smth, int32_t chara_id);

extern const mat4* rob_chara_item_equip_mat;

extern const mat4* rob_chara_get_adjust_data_mat(rob_chara* rob_chr);
extern const mat4* rob_chara_get_item_adjust_data_mat(rob_chara* rob_chr);

extern void rob_patch();
