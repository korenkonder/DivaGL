/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <vector>
#include "default.hpp"
#include "f2/enrs.hpp"

enum txp_format {
    TXP_A8     = 0,
    TXP_RGB8   = 1,
    TXP_RGBA8  = 2,
    TXP_RGB5   = 3,
    TXP_RGB5A1 = 4,
    TXP_RGBA4  = 5,
    TXP_BC1    = 6,
    TXP_BC1a   = 7,
    TXP_BC2    = 8,
    TXP_BC3    = 9,
    TXP_BC4    = 10,
    TXP_BC5    = 11,
    TXP_L8     = 12,
    TXP_L8A8   = 13,
};

struct txp_mipmap {
    uint32_t width;
    uint32_t height;
    txp_format format;
    uint32_t size;
    std::vector<uint8_t> data;

    txp_mipmap();
    ~txp_mipmap();

    uint32_t get_size();
};

struct txp {
    bool has_cube_map;
    uint32_t array_size;
    uint32_t mipmaps_count;
    std::vector<txp_mipmap> mipmaps;

    txp();
    ~txp();

    static uint32_t get_size(txp_format format, uint32_t width, uint32_t height);
};

struct txp_set {
    std::vector<txp> textures;

    txp_set();
    ~txp_set();

    bool pack_file(void** data, size_t* size, bool big_endian);
    bool pack_file(std::vector<uint8_t>& data, bool big_endian);
    bool pack_file_modern(void** data, size_t* size, bool big_endian, uint32_t signature);
    bool produce_enrs(enrs* enrs);
    bool unpack_file(const void* data, bool big_endian);
    bool unpack_file_modern(const void* data, size_t size, uint32_t signature);
};
