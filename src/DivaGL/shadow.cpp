/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shadow.hpp"

Shadow** shadow_ptr = (Shadow**)0x000000014CC587B8;

Shadow* shadow_ptr_get() {
    return *shadow_ptr;
}
