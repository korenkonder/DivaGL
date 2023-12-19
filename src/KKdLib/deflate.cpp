/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "deflate.hpp"
#include <libdeflate.h>

namespace deflate {
    static int32_t compress_static(struct libdeflate_compressor* c, const void* src, size_t src_length,
        void** dst, size_t* dst_length, int32_t compression_level, mode mode);
    static int32_t decompress_static(struct libdeflate_decompressor* d, const void* src, size_t src_length,
        void** dst, size_t* dst_length, mode mode);

    int32_t compress(const void* src, size_t src_length, void** dst,
        size_t* dst_length, int32_t compression_level, mode mode) {
        if (!src_length)
            return -1;
        else if (!src)
            return -2;
        else if (!dst)
            return -3;
        else if (!dst_length)
            return -4;
        else if (mode < MODE_DEFLATE || mode > MODE_ZLIB)
            return -5;

        struct libdeflate_compressor* c = libdeflate_alloc_compressor(compression_level);
        if (!c)
            return -5;

        int32_t result = compress_static(c, src, src_length, dst, dst_length, compression_level, mode);
        libdeflate_free_compressor(c);
        return result >= 0 ? result : result - 0x10;
    }

    int32_t compress_gzip(const void* src, size_t src_length, void** dst,
        size_t* dst_length, int32_t compression_level, const char* file_name) {
        if (!src_length)
            return -1;
        else if (!src)
            return -2;
        else if (!dst)
            return -3;
        else if (!dst_length)
            return -4;

        struct libdeflate_compressor* c = libdeflate_alloc_compressor(compression_level);
        if (!c)
            return -5;

        int32_t result = compress_static(c, src, src_length, dst, dst_length, compression_level, MODE_GZIP);
        libdeflate_free_compressor(c);
        if (result < 0)
            return result - 0x10;

        size_t file_name_length = utf8_length(file_name);
        void* temp = force_malloc(*dst_length + file_name_length + 1);
        size_t t = (size_t)temp;
        size_t d = (size_t)*dst;
        memcpy((void*)t, (void*)d, 0x0A);
        memcpy((void*)(t + 0x0A), file_name, file_name_length + 1);
        memcpy((void*)(t + 0x0A + file_name_length + 1), (void*)(d + 0x0A), *dst_length - 0x0A);
        ((uint8_t*)t)[0x03] |= 0x08;
        return result;
    }

    int32_t decompress(const void* src, size_t src_length, void** dst,
        size_t* dst_length, mode mode) {
        if (!src_length)
            return -1;
        else if (!src)
            return -2;
        else if (!dst)
            return -3;
        else if (!dst_length)
            return -4;
        else if (mode < MODE_DEFLATE || mode > MODE_ZLIB)
            return -5;

        if (!*dst_length)
            *dst_length = 1;
        struct libdeflate_decompressor* d = libdeflate_alloc_decompressor();
        int32_t result = decompress_static(d, src, src_length, dst, dst_length, mode);
        libdeflate_free_decompressor(d);
        return result >= 0 ? result : result - 0x10;
    }

    static int32_t compress_static(struct libdeflate_compressor* c, const void* src, size_t src_length,
        void** dst, size_t* dst_length, int32_t compression_level, mode mode) {
        size_t dst_max_length;
        switch (mode) {
        case MODE_GZIP:
            dst_max_length = libdeflate_gzip_compress_bound(c, src_length);
            break;
        case MODE_ZLIB:
            dst_max_length = libdeflate_zlib_compress_bound(c, src_length);
            break;
        default:
            dst_max_length = libdeflate_deflate_compress_bound(c, src_length);
            break;
        }

        *dst = force_malloc(dst_max_length);
        if (!*dst)
            return -1;

        size_t dst_act_length;
        switch (mode) {
        case MODE_GZIP:
            dst_act_length = libdeflate_gzip_compress(c, src, src_length, *dst, dst_max_length);
            break;
        case MODE_ZLIB:
            dst_act_length = libdeflate_zlib_compress(c, src, src_length, *dst, dst_max_length);
            break;
        default:
            dst_act_length = libdeflate_deflate_compress(c, src, src_length, *dst, dst_max_length);
            break;
        }

        if (dst_act_length == dst_max_length) {
            *dst_length = dst_act_length;
            return 0;
        }

        void* temp = force_malloc(dst_act_length);
        if (!temp) {
            free_def(*dst);
            return -2;
        }

        memcpy(temp, *dst, dst_act_length);
        free_def(*dst);
        *dst = temp;
        *dst_length = dst_act_length;
        return 0;
    }

    static int32_t decompress_static(struct libdeflate_decompressor* d, const void* src, size_t src_length,
        void** dst, size_t* dst_length, mode mode) {
        *dst = force_malloc(*dst_length);
        if (!*dst)
            return -1;

        size_t dst_act_length = 0;
        enum libdeflate_result result;
        switch (mode) {
        case MODE_GZIP:
            result = libdeflate_gzip_decompress(d, src, src_length, *dst, *dst_length, &dst_act_length);
            break;
        case MODE_ZLIB:
            result = libdeflate_zlib_decompress(d, src, src_length, *dst, *dst_length, &dst_act_length);
            break;
        default:
            result = libdeflate_deflate_decompress(d, src, src_length, *dst, *dst_length, &dst_act_length);
            break;
        }

        switch (result) {
        case LIBDEFLATE_BAD_DATA:
            free_def(*dst);
            return -2;
            break;
        case LIBDEFLATE_INSUFFICIENT_SPACE:
            free_def(*dst);
            *dst_length *= 2;
            decompress_static(d, src, src_length, dst, dst_length, mode);
            break;
        default:
            if (dst_act_length >= *dst_length)
                break;

            void* temp = force_malloc(dst_act_length);
            if (!temp) {
                free_def(*dst);
                return -3;
            }

            memcpy(temp, *dst, dst_act_length);
            free_def(*dst);
            *dst = temp;
            *dst_length = dst_act_length;
            break;
        }
        return 0;
    }
}
