/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <vector>
#include "../default.hpp"
#include "../io/stream.hpp"

struct pof {
    std::vector<int64_t> vec;
    bool shift_x;

    pof();
    ~pof();

    void add(stream& s, int64_t offset);
    void read(stream& s);
    void write(stream& s);
    uint32_t length();
};

extern void io_write_offset_pof_add(stream& s, int64_t val,
    int32_t offset, bool is_x, pof* pof);
extern void io_write_offset_f2_pof_add(stream& s, int64_t val,
    int32_t offset, pof* pof);
extern void io_write_offset_x_pof_add(stream& s, int64_t val, pof* pof);
