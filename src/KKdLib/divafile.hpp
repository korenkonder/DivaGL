/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"
#include "io/memory_stream.hpp"

namespace divafile {
    extern void decrypt(const char* path);
    extern void decrypt(const wchar_t* path);
    extern void decrypt(void* enc_data, void** dec_data, size_t* dec_size);
    extern void decrypt(stream& enc, memory_stream& dec);
    extern void encrypt(const char* path);
    extern void encrypt(const wchar_t* path);
    extern void encrypt(void* dec_data, size_t dec_size, void** enc_data, size_t* enc_size);
}
