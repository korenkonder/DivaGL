/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../default.hpp"
#include "enrs.hpp"
#include "header.hpp"
#include "pof.hpp"

struct f2_struct;

struct f2_struct {
    f2_header header;
    std::vector<uint8_t> data;
    std::vector<f2_struct> sub_structs;

    enrs enrs;
    pof pof;

    f2_struct();
    ~f2_struct();

    void read(const char* path);
    void read(const wchar_t* path);
    void read(const void* data, size_t size);
    void read(stream& s);
    void write(const char* path, bool use_depth = true, bool shift_x = false);
    void write(const wchar_t* path, bool use_depth = true, bool shift_x = false);
    void write(void** data, size_t* size, bool use_depth = true, bool shift_x = false);
    void write(stream& s, bool use_depth = true, bool shift_x = false);
};
