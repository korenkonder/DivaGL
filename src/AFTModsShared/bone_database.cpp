/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "bone_database.hpp"

bone_database_skeleton* (FASTCALL* bone_database_get_skeleton_by_type)(bone_database_skeleton_type type)
    = (bone_database_skeleton * (FASTCALL*)(bone_database_skeleton_type type))0x00000001401F10C0;
