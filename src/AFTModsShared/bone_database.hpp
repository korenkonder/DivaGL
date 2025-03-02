/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"


enum bone_database_skeleton_type : int32_t {
    BONE_DATABASE_SKELETON_COMMON = 0,
    BONE_DATABASE_SKELETON_MIKU   = 1,
    BONE_DATABASE_SKELETON_KAITO  = 2,
    BONE_DATABASE_SKELETON_LEN    = 3,
    BONE_DATABASE_SKELETON_LUKA   = 4,
    BONE_DATABASE_SKELETON_MEIKO  = 5,
    BONE_DATABASE_SKELETON_RIN    = 6,
    BONE_DATABASE_SKELETON_HAKU   = 7,
    BONE_DATABASE_SKELETON_NERU   = 8,
    BONE_DATABASE_SKELETON_SAKINE = 9,
    BONE_DATABASE_SKELETON_TETO   = 10,
    BONE_DATABASE_SKELETON_NONE   = -1,
};

enum bone_database_bone_type : uint8_t {
    BONE_DATABASE_BONE_ROTATION          = 0x00,
    BONE_DATABASE_BONE_TYPE_1            = 0x01,
    BONE_DATABASE_BONE_POSITION          = 0x02,
    BONE_DATABASE_BONE_POSITION_ROTATION = 0x03,
    BONE_DATABASE_BONE_HEAD_IK_ROTATION  = 0x04,
    BONE_DATABASE_BONE_ARM_IK_ROTATION   = 0x05,
    BONE_DATABASE_BONE_LEGS_IK_ROTATION  = 0x06,
};

struct bone_database_bone {
    bone_database_bone_type type;
    bool has_parent;
    uint8_t parent;
    uint8_t pole_target;
    uint8_t mirror;
    uint8_t flags;
    const char* name;
};

static_assert(sizeof(bone_database_bone) == 0x10, "\"bone_database_skeleton\" struct should have a size of 0x10");

struct bone_database_skeleton {
    bone_database_bone* bones;
    uint32_t posittion_count;
    vec3* positions;
    float_t* heel_height;
    uint32_t object_bone_name_count;
    const char** object_bone_names;
    uint32_t motion_bone_name_count;
    const char** motion_bone_names;
    uint16_t* parent_indices;
    int32_t field_48;
    int32_t field_4C;
    int32_t field_50;
    int32_t field_54;
    int32_t field_58;
    int32_t field_5C;
};

static_assert(sizeof(bone_database_skeleton) == 0x60, "\"bone_database_skeleton\" struct should have a size of 0x60");

extern bone_database_skeleton* (FASTCALL* bone_database_get_skeleton_by_type)(bone_database_skeleton_type type);
