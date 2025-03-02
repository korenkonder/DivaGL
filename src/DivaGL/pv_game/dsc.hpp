/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../AFTModsShared/types.hpp"

struct dsc_data {
    int32_t func;
    uint32_t data_offset;
};

struct dsc {
    prj::vector<dsc_data> data;
    prj::vector<int32_t> data_buffer;

    dsc();
    ~dsc();

    int32_t* get_func_data(dsc_data* data);
    bool parse(const void* data, size_t size);
};

extern const char* dsc_get_func_name(int32_t id);
extern int32_t dsc_get_func_length(int32_t id);
