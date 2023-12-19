/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "hash.hpp"

static const uint16_t hash_crc16_ccitt_table[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
};

// Empty string
const uint64_t hash_fnv1a64m_empty = 0xCBF29CE44FD0BFC1;
// Empty string
const uint32_t hash_murmurhash_empty = 0x0CAD3078;
// "NULL" string
const uint32_t hash_murmurhash_null = 0x5A009B23;
// Empty string
const uint32_t hash_crc16_ccitt_empty = 0xFFFF;

// FNV 1a 64-bit Modified
// 0x1403B04D0 in SBZV_7.10
uint64_t hash_fnv1a64m(const void* data, size_t size, bool make_upper) {
    const uint8_t* d = (const uint8_t*)data;

    uint64_t hash = 0xCBF29CE484222325;
    if (data)
        if (make_upper) // Hash text UPPERCASE
            for (size_t i = size; i; i--) {
                uint8_t c = *d++;
                if (c > 0x60 && c < 0x7B)
                    c -= 0x20;
                hash ^= c;
                hash *= 0x100000001B3;
            }
        else
            for (size_t i = size; i; i--) {
                hash ^= *d++;
                hash *= 0x100000001B3;
            }
    return (hash >> 32) ^ hash; // Actual Modification
}

// MurmurHash
// 0x814D7A9C in PCSB00554
// 0x8134C304 in PCSB01007
// 0x0069CEA4 in NPEB02013
uint32_t hash_murmurhash(const void* data, size_t size,
    uint32_t seed, bool upper, bool big_endian) {
    const uint8_t* d = (const uint8_t*)data;

    uint32_t a = 0;
    uint32_t b = 0;
    uint32_t hash = 0;
    size_t i = 0;

    const uint32_t m = 0x7FD652AD;
    const int32_t r = 16;

    hash = seed + 0xDEADBEEF;
    if (d)
        if (upper) {
            if (big_endian)
                for (i = 0; size > 3; size -= 4, i += 4, d += 4) {
                    b = load_reverse_endianness_uint32_t(d);
                    hash += b;
                    hash *= m;
                    hash ^= hash >> r;
                }
            else
                for (i = 0; size > 3; size -= 4, i += 4, d += 4) {
                    b = *(uint32_t*)d;
                    hash += b;
                    hash *= m;
                    hash ^= hash >> r;
                }

            if (size > 0) {
                if (size > 1) {
                    if (size > 2)
                        hash += (uint32_t)d[2] << 16;
                    hash += (uint32_t)d[1] << 8;
                }
                hash += d[0];
                hash *= m;
                hash ^= hash >> r;
            }
        }
        else {
            if (big_endian)
                for (i = 0; size > 3; size -= 4, i += 4, d += 4) {
                    b = load_reverse_endianness_uint32_t(d);

                    a = b & 0xFF;
                    if (a > 0x60 && a < 0x7B)
                        a -= 0x20;
                    b = (b & 0xFFFFFF00) | a;

                    a = (b >> 8) & 0xFF;
                    if (a > 0x60 && a < 0x7B)
                        a -= 0x20;
                    b = (b & 0xFFFF00FF) | (a << 8);

                    a = (b >> 16) & 0xFF;
                    if (a > 0x60 && a < 0x7B)
                        a -= 0x20;
                    b = (b & 0xFF00FFFF) | (a << 16);

                    a = (b >> 24) & 0xFF;
                    if (a > 0x60 && a < 0x7B)
                        a -= 0x20;
                    b = (b & 0x00FFFFFF) | (a << 24);

                    hash += b;
                    hash *= m;
                    hash ^= hash >> r;
                }
            else
                for (i = 0; size > 3; size -= 4, i += 4, d += 4) {
                    b = *(uint32_t*)d;

                    a = b & 0xFF;
                    if (a > 0x60 && a < 0x7B)
                        a -= 0x20;
                    b = (b & 0xFFFFFF00) | a;

                    a = (b >> 8) & 0xFF;
                    if (a > 0x60 && a < 0x7B)
                        a -= 0x20;
                    b = (b & 0xFFFF00FF) | (a << 8);

                    a = (b >> 16) & 0xFF;
                    if (a > 0x60 && a < 0x7B)
                        a -= 0x20;
                    b = (b & 0xFF00FFFF) | (a << 16);

                    a = (b >> 24) & 0xFF;
                    if (a > 0x60 && a < 0x7B)
                        a -= 0x20;
                    b = (b & 0x00FFFFFF) | (a << 24);

                    hash += b;
                    hash *= m;
                    hash ^= hash >> r;
                }

            switch (size) {
            case 3:
                b = d[2];
                if (b > 0x60 && b < 0x7B)
                    b -= 0x20;
                hash += b << 16;
            case 2:
                b = d[1];
                if (b > 0x60 && b < 0x7B)
                    b -= 0x20;
                hash += b << 8;
            case 1:
                b = d[0];
                if (b > 0x60 && b < 0x7B)
                    b -= 0x20;
                hash += b;

                hash *= m;
                hash ^= hash >> r;
                break;
            }
        }

    hash *= m;
    hash ^= hash >> 10;
    hash *= m;
    hash ^= hash >> 17;
    return hash;
}

// CRC16-CCITT
// 0x140011A90 in SBZV_7.10
uint16_t hash_crc16_ccitt(const void* data, size_t size, bool make_upper) {
    const uint8_t* d = (const uint8_t*)data;

    uint16_t hash = 0xFFFF;
    if (make_upper) // Modification for only uppercase latin text
        for (size_t i = size; i; i--) {
            uint8_t a = *d++;
            if (a > 0x60 && a < 0x7B)
                a -= 0x20;
            hash = (uint16_t)(hash_crc16_ccitt_table[(hash >> 8) ^ a] ^ (hash << 8));
        }
    else
        for (size_t i = size; i; i--)
            hash = (uint16_t)(hash_crc16_ccitt_table[(hash >> 8) ^ *d++] ^ (hash << 8));
    return hash;
}
