/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"

namespace deflate {
    enum mode {
        MODE_DEFLATE = 0,
        MODE_GZIP    = 1,
        MODE_ZLIB    = 2,
    };

    extern int32_t compress(const void* src, size_t src_length, void** dst,
        size_t* dst_length, int32_t compression_level, mode mode);
    extern int32_t compress_gzip(const void* src, size_t src_length, void** dst,
        size_t* dst_length, int32_t compression_level, const char* file_name = 0);
    extern int32_t decompress(const void* src, size_t src_length, void** dst,
        size_t* dst_length, mode mode);
}
