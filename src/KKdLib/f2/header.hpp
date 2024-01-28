/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../default.hpp"
#include "../io/stream.hpp"

struct f2_header {
    union {
        char signature_char[0x04];  // 0x00
        uint32_t signature;         // 0x00
    };
    uint32_t data_size;             // 0x04
    uint32_t length;                // 0x08
    union {                         // 0x0C
        struct {
            uint32_t flags0 : 27;
            uint32_t use_big_endian : 1;
            uint32_t use_section_size : 1;
            uint32_t flags1 : 3;
        };
        uint32_t flags;
    };
    uint32_t depth;                 // 0x10
    uint32_t section_size;          // 0x14
    uint32_t version;               // 0x18
    uint32_t unknown0;              // 0x1C
    uint32_t murmurhash;            // 0x20
    uint32_t unknown1[3];           // 0x24
    uint32_t inner_signature;       // 0x30
    uint32_t unknown2[3];           // 0x34
};

extern void f2_header_read(stream& s, f2_header* h);
extern void f2_header_write(stream& s, f2_header* h, bool extended);
extern void f2_header_write_end_of_container(stream& s, uint32_t depth);