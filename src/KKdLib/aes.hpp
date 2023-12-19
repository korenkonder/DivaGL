/*
    Original: https://github.com/kokke/tiny-AES-c
*/

#ifndef _AES_H_
#define _AES_H_

#include "default.hpp"
#include <immintrin.h>

//#define AES128 1
//#define AES192 1
//#define AES256 1

#define AES_BLOCKLEN 16 // Block length in bytes - AES is 128b block only

#define AES128_KEYLEN 16   // Key length in bytes
#define AES128_keyExpSize 176
#define AES192_KEYLEN 24
#define AES192_keyExpSize 208
#define AES256_KEYLEN 32
#define AES256_keyExpSize 240

struct aes128_ctx {
    union {
        uint8_t RoundKey[AES128_keyExpSize];
        __m128i RoundKeyNI[AES128_keyExpSize / sizeof(__m128i) + 9];
    };
    uint8_t Iv[AES_BLOCKLEN];
};

struct aes192_ctx {
    union {
        uint8_t RoundKey[AES192_keyExpSize];
        __m128i RoundKeyNI[AES192_keyExpSize / sizeof(__m128i) + 11];
    };
    uint8_t Iv[AES_BLOCKLEN];
};

struct aes256_ctx {
    union {
        uint8_t RoundKey[AES256_keyExpSize];
        __m128i RoundKeyNI[AES256_keyExpSize / sizeof(__m128i) + 13];
    };
    uint8_t Iv[AES_BLOCKLEN];
};

void aes128_init_ctx(aes128_ctx* ctx, const uint8_t* key);
void aes192_init_ctx(aes192_ctx* ctx, const uint8_t* key);
void aes256_init_ctx(aes256_ctx* ctx, const uint8_t* key);
void aes128_init_ctx_iv(aes128_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void aes192_init_ctx_iv(aes192_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void aes256_init_ctx_iv(aes256_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void aes128_ctx_set_iv(aes128_ctx* ctx, const uint8_t* iv);
void aes192_ctx_set_iv(aes192_ctx* ctx, const uint8_t* iv);
void aes256_ctx_set_iv(aes256_ctx* ctx, const uint8_t* iv);

// buffer size is exactly AES_BLOCKLEN bytes;
// you need only AES_init_ctx as IV is not used in ECB
void aes128_ecb_encrypt(aes128_ctx* ctx, uint8_t* buf);
void aes192_ecb_encrypt(aes192_ctx* ctx, uint8_t* buf);
void aes256_ecb_encrypt(aes256_ctx* ctx, uint8_t* buf);
void aes128_ecb_decrypt(aes128_ctx* ctx, uint8_t* buf);
void aes192_ecb_decrypt(aes192_ctx* ctx, uint8_t* buf);
void aes256_ecb_decrypt(aes256_ctx* ctx, uint8_t* buf);
void aes128_ecb_encrypt_buffer(aes128_ctx* ctx, uint8_t* buf, size_t length);
void aes192_ecb_encrypt_buffer(aes192_ctx* ctx, uint8_t* buf, size_t length);
void aes256_ecb_encrypt_buffer(aes256_ctx* ctx, uint8_t* buf, size_t length);
void aes128_ecb_decrypt_buffer(aes128_ctx* ctx, uint8_t* buf, size_t length);
void aes192_ecb_decrypt_buffer(aes192_ctx* ctx, uint8_t* buf, size_t length);
void aes256_ecb_decrypt_buffer(aes256_ctx* ctx, uint8_t* buf, size_t length);

void aes128_cbc_encrypt(aes128_ctx* ctx, uint8_t buf[AES_BLOCKLEN]);
void aes192_cbc_encrypt(aes192_ctx* ctx, uint8_t buf[AES_BLOCKLEN]);
void aes256_cbc_encrypt(aes256_ctx* ctx, uint8_t buf[AES_BLOCKLEN]);
void aes128_cbc_decrypt(aes128_ctx* ctx, uint8_t buf[AES_BLOCKLEN]);
void aes192_cbc_decrypt(aes192_ctx* ctx, uint8_t buf[AES_BLOCKLEN]);
void aes256_cbc_decrypt(aes256_ctx* ctx, uint8_t buf[AES_BLOCKLEN]);
void aes128_cbc_encrypt_buffer(aes128_ctx* ctx, uint8_t* buf, size_t length);
void aes192_cbc_encrypt_buffer(aes192_ctx* ctx, uint8_t* buf, size_t length);
void aes256_cbc_encrypt_buffer(aes256_ctx* ctx, uint8_t* buf, size_t length);
void aes128_cbc_decrypt_buffer(aes128_ctx* ctx, uint8_t* buf, size_t length);
void aes192_cbc_decrypt_buffer(aes192_ctx* ctx, uint8_t* buf, size_t length);
void aes256_cbc_decrypt_buffer(aes256_ctx* ctx, uint8_t* buf, size_t length);

void aes128_ctr_xcrypt_buffer(aes128_ctx* ctx, uint8_t* buf, uint32_t length);
void aes192_ctr_xcrypt_buffer(aes192_ctx* ctx, uint8_t* buf, uint32_t length);
void aes256_ctr_xcrypt_buffer(aes256_ctx* ctx, uint8_t* buf, uint32_t length);

#endif // _AES_H_
