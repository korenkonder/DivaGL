/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <vector>
#include "../default.hpp"
#include "../io/stream.hpp"

enum enrs_type {
    ENRS_WORD    = 0x0,
    ENRS_DWORD   = 0x1,
    ENRS_QWORD   = 0x2,
    ENRS_INVALID = 0x3,
};

struct enrs_sub_entry {
    uint32_t skip_bytes;
    uint32_t repeat_count;
    enrs_type type;
};

struct enrs_entry {
    uint32_t offset;
    uint32_t count;
    uint32_t size;
    uint32_t repeat_count;
    std::vector<enrs_sub_entry> sub;

    enrs_entry();
    enrs_entry(uint32_t offset, uint32_t count, uint32_t size, uint32_t repeat_count);
    ~enrs_entry();

    void append(uint32_t skip_bytes, uint32_t repeat_count, enrs_type type);
    void append(enrs_sub_entry&& data);

    enrs_entry& operator=(const enrs_entry& ee);
};

struct enrs {
    std::vector<enrs_entry> vec;

    enrs();
    ~enrs();

    void apply(void* data);
    uint32_t length();
    void read(stream& s);
    void write(stream& s);

    enrs& operator=(const enrs& e);
};
