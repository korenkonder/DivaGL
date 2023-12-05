/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"

extern void inject_data(void* address, const void* data, size_t count);
extern void inject_uint8_t(void* address, uint8_t data);
extern void inject_uint16_t(void* address, uint16_t data);
extern void inject_uint32_t(void* address, uint32_t data);
extern void inject_uint64_t(void* address, uint64_t data);
extern void inject_patches();
