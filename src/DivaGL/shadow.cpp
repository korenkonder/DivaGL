/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shadow.hpp"

Shadow** shadow_ptr = (Shadow**)0x000000014CC587B8;

void(FASTCALL* sub_1405E8A20)(Shadow* shad, int32_t index, vec3* pos)
    = (void(FASTCALL*)(Shadow* shad, int32_t index, vec3 * pos))0x000000014063FA90;

float_t Shadow::get_shadow_range() {
    return shadow_range * shadow_range_factor;
}

Shadow* shadow_ptr_get() {
    return *shadow_ptr;
}
