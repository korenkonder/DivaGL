/*
    Original: https://github.com/kokke/tiny-AES-c
*/

/*

This is an implementation of the AES algorithm, specifically ECB, CTR and CBC mode.
Block size can be chosen in aes.h - available choices are AES128, AES192, AES256.

The implementation is verified against the test vectors in:
    National Institute of Standards and Technology Special Publication 800-38A 2001 ED

ECB-AES128
----------

    plain-text:
        6bc1bee22e409f96e93d7e117393172a
        ae2d8a571e03ac9c9eb76fac45af8e51
        30c81c46a35ce411e5fbc1191a0a52ef
        f69f2445df4f9b17ad2b417be66c3710

    key:
        2b7e151628aed2a6abf7158809cf4f3c

    resulting cipher
        3ad77bb40d7a3660a89ecaf32466ef97
        f5d3d58503b9699de785895a96fdbaaf
        43b1cd7f598ece23881b00e3ed030688
        7b0c785e27e8ad3f8223207104725dd4


NOTE:   String length must be evenly divisible by 16byte (str_len % 16 == 0)
                You should pad the end of the string with zeros if this is not the case.
                For AES192/256 the key size is proportionally larger.

*/

/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include "aes.hpp"

/*****************************************************************************/
/* Defines:                                                                  */
/*****************************************************************************/
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4

#define Nk128 4        // The number of 32 bit words in a key.
#define Nr128 10       // The number of rounds in AES Cipher.
#define Nk192 6
#define Nr192 12
#define Nk256 8
#define Nr256 14

// jcallan@github points out that declaring Multiply as a function
// reduces code size considerably with the Keil ARM compiler.
// See this link for more information: https://github.com/kokke/tiny-AES-C/pull/3
#ifndef MULTIPLY_AS_A_FUNCTION
    #define MULTIPLY_AS_A_FUNCTION 0
#endif

/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
typedef uint8_t state_t[4][4];

// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM -
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
static const uint8_t sbox[256] = {
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// The round constant word array, Rcon[i], contains the values given by
// x to the power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
static const uint8_t Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

/*
 * Jordan Goulder points out in PR #12 (https://github.com/kokke/tiny-AES-C/pull/12),
 * that you can remove most of the elements in the Rcon array, because they are unused.
 *
 * From Wikipedia's article on the Rijndael key schedule @ https://en.wikipedia.org/wiki/Rijndael_key_schedule#Rcon
 *
 * "Only the first some of these constants are actually used - up to rcon[10] for AES-128 (as 11 round keys are needed),
 *  up to rcon[8] for AES-192, up to rcon[7] for AES-256. rcon[0] is not used in AES algorithm."
 */

extern bool aes_ni;

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
static void key_expansion_aes128(uint8_t* RoundKey, const uint8_t* Key) {
    unsigned i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < Nk128; ++i) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = Nk128; i < Nb * (Nr128 + 1); ++i) {
        {
            k = (i - 1) * 4;
            tempa[0] = RoundKey[k + 0];
            tempa[1] = RoundKey[k + 1];
            tempa[2] = RoundKey[k + 2];
            tempa[3] = RoundKey[k + 3];
        }

        if (i % Nk128 == 0) {
            // This function shifts the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                const uint8_t u8tmp = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = u8tmp;
            }

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = sbox[tempa[0]];
                tempa[1] = sbox[tempa[1]];
                tempa[2] = sbox[tempa[2]];
                tempa[3] = sbox[tempa[3]];
            }

            tempa[0] = tempa[0] ^ Rcon[i / Nk128];
        }
        j = i * 4; k = (i - Nk128) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

static void key_expansion_aes192(uint8_t* RoundKey, const uint8_t* Key) {
    unsigned i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < Nk192; ++i) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = Nk192; i < Nb * (Nr192 + 1); ++i) {
        {
            k = (i - 1) * 4;
            tempa[0] = RoundKey[k + 0];
            tempa[1] = RoundKey[k + 1];
            tempa[2] = RoundKey[k + 2];
            tempa[3] = RoundKey[k + 3];
        }

        if (i % Nk192 == 0) {
            // This function shifts the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                const uint8_t u8tmp = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = u8tmp;
            }

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = sbox[tempa[0]];
                tempa[1] = sbox[tempa[1]];
                tempa[2] = sbox[tempa[2]];
                tempa[3] = sbox[tempa[3]];
            }

            tempa[0] = tempa[0] ^ Rcon[i / Nk192];
        }
        j = i * 4; k = (i - Nk192) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

static void key_expansion_aes256(uint8_t* RoundKey, const uint8_t* Key) {
    unsigned i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < Nk256; ++i) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = Nk256; i < Nb * (Nr256 + 1); ++i) {
        {
            k = (i - 1) * 4;
            tempa[0] = RoundKey[k + 0];
            tempa[1] = RoundKey[k + 1];
            tempa[2] = RoundKey[k + 2];
            tempa[3] = RoundKey[k + 3];
        }

        if (i % Nk256 == 0) {
            // This function shifts the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                const uint8_t u8tmp = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = u8tmp;
            }

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = sbox[tempa[0]];
                tempa[1] = sbox[tempa[1]];
                tempa[2] = sbox[tempa[2]];
                tempa[3] = sbox[tempa[3]];
            }

            tempa[0] = tempa[0] ^ Rcon[i / Nk256];
        }
        if (i % Nk256 == 4) {
            // Function Subword()
            {
                tempa[0] = sbox[tempa[0]];
                tempa[1] = sbox[tempa[1]];
                tempa[2] = sbox[tempa[2]];
                tempa[3] = sbox[tempa[3]];
            }
        }
        j = i * 4; k = (i - Nk256) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

inline static __m128i key_expansion_aes128_ni_assist(__m128i temp1, __m128i temp2) {
    __m128i temp3;
    temp2 = _mm_shuffle_epi32(temp2, 0xFF);
    temp3 = _mm_slli_si128(temp1, 0x04);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp3 = _mm_slli_si128(temp3, 0x04);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp3 = _mm_slli_si128(temp3, 0x04);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp1 = _mm_xor_si128(temp1, temp2);
    return temp1;
}

static void key_expansion_aes128_ni(__m128i* RoundKey, const uint8_t* Key) {
    __m128i temp1, temp2;
    temp1 = _mm_loadu_si128((__m128i*)&Key[0]);
    RoundKey[0] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x01);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[1] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x02);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[2] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x04);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[3] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x08);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[4] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x10);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[5] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x20);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[6] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x40);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[7] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x80);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[8] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x1B);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[9] = temp1;
    temp2 = _mm_aeskeygenassist_si128(temp1, 0x36);
    temp1 = key_expansion_aes128_ni_assist(temp1, temp2);
    RoundKey[10] = temp1;
    RoundKey[11] = _mm_aesimc_si128(RoundKey[9]);
    RoundKey[12] = _mm_aesimc_si128(RoundKey[8]);
    RoundKey[13] = _mm_aesimc_si128(RoundKey[7]);
    RoundKey[14] = _mm_aesimc_si128(RoundKey[6]);
    RoundKey[15] = _mm_aesimc_si128(RoundKey[5]);
    RoundKey[16] = _mm_aesimc_si128(RoundKey[4]);
    RoundKey[17] = _mm_aesimc_si128(RoundKey[3]);
    RoundKey[18] = _mm_aesimc_si128(RoundKey[2]);
    RoundKey[19] = _mm_aesimc_si128(RoundKey[1]);
}

inline static void key_expansion_aes192_ni_assist(__m128i* temp1, __m128i* temp2, __m128i* temp3) {
    __m128i temp4;
    *temp2 = _mm_shuffle_epi32(*temp2, 0x55);
    temp4 = _mm_slli_si128(*temp1, 0x04);
    *temp1 = _mm_xor_si128(*temp1, temp4);
    temp4 = _mm_slli_si128(temp4, 0x04);
    *temp1 = _mm_xor_si128(*temp1, temp4);
    temp4 = _mm_slli_si128(temp4, 0x04);
    *temp1 = _mm_xor_si128(*temp1, temp4);
    *temp1 = _mm_xor_si128(*temp1, *temp2);
    *temp2 = _mm_shuffle_epi32(*temp1, 0xFF);
    temp4 = _mm_slli_si128(*temp3, 0x04);
    *temp3 = _mm_xor_si128(*temp3, temp4);
    *temp3 = _mm_xor_si128(*temp3, *temp2);
}

static void key_expansion_aes192_ni(__m128i* RoundKey, const uint8_t* Key) {
    __m128i temp1, temp2, temp3;
    temp1 = _mm_loadu_si128((__m128i*)&Key[0]);
    temp3 = _mm_loadu_si128((__m128i*)&Key[16]);
    RoundKey[0] = temp1;
    RoundKey[1] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x01);
    key_expansion_aes192_ni_assist(&temp1, &temp2, &temp3);
    *(__m128d*)& RoundKey[1] = _mm_shuffle_pd(*(__m128d*)&RoundKey[1], *(__m128d*)&temp1, 0);
    *(__m128d*)& RoundKey[2] = _mm_shuffle_pd(*(__m128d*)&temp1, *(__m128d*)&temp3, 1);
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x02);
    key_expansion_aes192_ni_assist(&temp1, &temp2, &temp3);
    RoundKey[3] = temp1;
    RoundKey[4] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x04);
    key_expansion_aes192_ni_assist(&temp1, &temp2, &temp3);
    *(__m128d*)& RoundKey[4] = _mm_shuffle_pd(*(__m128d*)&RoundKey[4], *(__m128d*)&temp1, 0);
    *(__m128d*)& RoundKey[5] = _mm_shuffle_pd(*(__m128d*)&temp1, *(__m128d*)&temp3, 1);
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x08);
    key_expansion_aes192_ni_assist(&temp1, &temp2, &temp3);
    RoundKey[6] = temp1;
    RoundKey[7] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x10);
    key_expansion_aes192_ni_assist(&temp1, &temp2, &temp3);
    *(__m128d*)& RoundKey[7] = _mm_shuffle_pd(*(__m128d*)&RoundKey[7], *(__m128d*)&temp1, 0);
    *(__m128d*)& RoundKey[8] = _mm_shuffle_pd(*(__m128d*)&temp1, *(__m128d*)&temp3, 1);
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x20);
    key_expansion_aes192_ni_assist(&temp1, &temp2, &temp3);
    RoundKey[9] = temp1;
    RoundKey[10] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x40);
    key_expansion_aes192_ni_assist(&temp1, &temp2, &temp3);
    *(__m128d*)& RoundKey[10] = _mm_shuffle_pd(*(__m128d*)&RoundKey[10], *(__m128d*)&temp1, 0);
    *(__m128d*)& RoundKey[11] = _mm_shuffle_pd(*(__m128d*)&temp1, *(__m128d*)&temp3, 1);
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x80);
    key_expansion_aes192_ni_assist(&temp1, &temp2, &temp3);
    RoundKey[12] = temp1;
    RoundKey[13] = _mm_aesimc_si128(RoundKey[11]);
    RoundKey[14] = _mm_aesimc_si128(RoundKey[10]);
    RoundKey[15] = _mm_aesimc_si128(RoundKey[9]);
    RoundKey[16] = _mm_aesimc_si128(RoundKey[8]);
    RoundKey[17] = _mm_aesimc_si128(RoundKey[7]);
    RoundKey[18] = _mm_aesimc_si128(RoundKey[6]);
    RoundKey[19] = _mm_aesimc_si128(RoundKey[5]);
    RoundKey[20] = _mm_aesimc_si128(RoundKey[4]);
    RoundKey[21] = _mm_aesimc_si128(RoundKey[3]);
    RoundKey[22] = _mm_aesimc_si128(RoundKey[2]);
    RoundKey[23] = _mm_aesimc_si128(RoundKey[1]);
}

inline static void key_expansion_aes256_ni_assist_1(__m128i* temp1, __m128i* temp2) {
    __m128i temp4;
    *temp2 = _mm_shuffle_epi32(*temp2, 0xFF);
    temp4 = _mm_slli_si128(*temp1, 0x04);
    *temp1 = _mm_xor_si128(*temp1, temp4);
    temp4 = _mm_slli_si128(temp4, 0x04);
    *temp1 = _mm_xor_si128(*temp1, temp4);
    temp4 = _mm_slli_si128(temp4, 0x04);
    *temp1 = _mm_xor_si128(*temp1, temp4);
    *temp1 = _mm_xor_si128(*temp1, *temp2);
}

inline static void key_expansion_aes256_ni_assist_2(__m128i* temp1, __m128i* temp3) {
    __m128i temp2, temp4;
    temp4 = _mm_aeskeygenassist_si128(*temp1, 0x00);
    temp2 = _mm_shuffle_epi32(temp4, 0xAA);
    temp4 = _mm_slli_si128(*temp3, 0x04);
    *temp3 = _mm_xor_si128(*temp3, temp4);
    temp4 = _mm_slli_si128(temp4, 0x04);
    *temp3 = _mm_xor_si128(*temp3, temp4);
    temp4 = _mm_slli_si128(temp4, 0x04);
    *temp3 = _mm_xor_si128(*temp3, temp4);
    *temp3 = _mm_xor_si128(*temp3, temp2);
}

static void key_expansion_aes256_ni(__m128i* RoundKey, const uint8_t* Key) {
    __m128i temp1, temp2, temp3;
    temp1 = _mm_loadu_si128((__m128i*)&Key[0]);
    temp3 = _mm_loadu_si128((__m128i*)&Key[16]);
    RoundKey[0] = temp1;
    RoundKey[1] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x01);
    key_expansion_aes256_ni_assist_1(&temp1, &temp2);
    RoundKey[2] = temp1;
    key_expansion_aes256_ni_assist_2(&temp1, &temp3);
    RoundKey[3] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x02);
    key_expansion_aes256_ni_assist_1(&temp1, &temp2);
    RoundKey[4] = temp1;
    key_expansion_aes256_ni_assist_2(&temp1, &temp3);
    RoundKey[5] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x04);
    key_expansion_aes256_ni_assist_1(&temp1, &temp2);
    RoundKey[6] = temp1;
    key_expansion_aes256_ni_assist_2(&temp1, &temp3);
    RoundKey[7] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x08);
    key_expansion_aes256_ni_assist_1(&temp1, &temp2);
    RoundKey[8] = temp1;
    key_expansion_aes256_ni_assist_2(&temp1, &temp3);
    RoundKey[9] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x10);
    key_expansion_aes256_ni_assist_1(&temp1, &temp2);
    RoundKey[10] = temp1;
    key_expansion_aes256_ni_assist_2(&temp1, &temp3);
    RoundKey[11] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x20);
    key_expansion_aes256_ni_assist_1(&temp1, &temp2);
    RoundKey[12] = temp1;
    key_expansion_aes256_ni_assist_2(&temp1, &temp3);
    RoundKey[13] = temp3;
    temp2 = _mm_aeskeygenassist_si128(temp3, 0x40);
    key_expansion_aes256_ni_assist_1(&temp1, &temp2);
    RoundKey[14] = temp1;
    RoundKey[15] = _mm_aesimc_si128(RoundKey[13]);
    RoundKey[16] = _mm_aesimc_si128(RoundKey[12]);
    RoundKey[17] = _mm_aesimc_si128(RoundKey[11]);
    RoundKey[18] = _mm_aesimc_si128(RoundKey[10]);
    RoundKey[19] = _mm_aesimc_si128(RoundKey[9]);
    RoundKey[20] = _mm_aesimc_si128(RoundKey[8]);
    RoundKey[21] = _mm_aesimc_si128(RoundKey[7]);
    RoundKey[22] = _mm_aesimc_si128(RoundKey[6]);
    RoundKey[23] = _mm_aesimc_si128(RoundKey[5]);
    RoundKey[24] = _mm_aesimc_si128(RoundKey[4]);
    RoundKey[25] = _mm_aesimc_si128(RoundKey[3]);
    RoundKey[26] = _mm_aesimc_si128(RoundKey[2]);
    RoundKey[27] = _mm_aesimc_si128(RoundKey[1]);
}

void aes128_init_ctx(aes128_ctx* ctx, const uint8_t* key) {
    if (aes_ni)
        key_expansion_aes128_ni(ctx->RoundKeyNI, key);
    else
        key_expansion_aes128(ctx->RoundKey, key);
}

void aes192_init_ctx(aes192_ctx* ctx, const uint8_t* key) {
    if (aes_ni)
        key_expansion_aes192_ni(ctx->RoundKeyNI, key);
    else
        key_expansion_aes192(ctx->RoundKey, key);
}

void aes256_init_ctx(aes256_ctx* ctx, const uint8_t* key) {
    if (aes_ni)
        key_expansion_aes256_ni(ctx->RoundKeyNI, key);
    else
        key_expansion_aes256(ctx->RoundKey, key);
}

void aes128_init_ctx_iv(aes128_ctx* ctx, const uint8_t* key, const uint8_t* iv) {
    if (aes_ni)
        key_expansion_aes128_ni(ctx->RoundKeyNI, key);
    else
        key_expansion_aes128(ctx->RoundKey, key);
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}

void aes192_init_ctx_iv(aes192_ctx* ctx, const uint8_t* key, const uint8_t* iv) {
    if (aes_ni)
        key_expansion_aes192_ni(ctx->RoundKeyNI, key);
    else
        key_expansion_aes192(ctx->RoundKey, key);
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}

void aes256_init_ctx_iv(aes256_ctx* ctx, const uint8_t* key, const uint8_t* iv) {
    if (aes_ni)
        key_expansion_aes256_ni(ctx->RoundKeyNI, key);
    else
        key_expansion_aes256(ctx->RoundKey, key);
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}

void aes128_ctx_set_iv(aes128_ctx* ctx, const uint8_t* iv) {
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}

void aes192_ctx_set_iv(aes192_ctx* ctx, const uint8_t* iv) {
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}

void aes256_ctx_set_iv(aes256_ctx* ctx, const uint8_t* iv) {
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
inline static void add_round_key(uint8_t round, state_t* state, const uint8_t* RoundKey) {
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            (*state)[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
inline static void sub_bytes(state_t* state) {
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            (*state)[j][i] = sbox[(*state)[j][i]];
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
inline static void shift_rows(state_t* state) {
    uint8_t temp;

    // Rotate first row 1 columns to left
    temp           = (*state)[0][1];
    (*state)[0][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[3][1];
    (*state)[3][1] = temp;

    // Rotate second row 2 columns to left
    temp           = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp           = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to left
    temp           = (*state)[0][3];
    (*state)[0][3] = (*state)[3][3];
    (*state)[3][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[1][3];
    (*state)[1][3] = temp;
}

inline static uint8_t xtime(uint8_t x) {
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
inline static void mix_columns(state_t* state) {
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0; i < 4; ++i) {
        t   = (*state)[i][0];
        Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
        Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
    }
}

inline static uint8_t Multiply(uint8_t x, uint8_t y) {
    return ((y & 1) * x) ^
        (((y >> 1) & 0x01) * xtime(x)) ^
        (((y >> 2) & 0x01) * xtime(xtime(x))) ^
        (((y >> 3) & 0x01) * xtime(xtime(xtime(x))));
}

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
inline static void inv_mix_columns(state_t* state) {
    int32_t i;
    uint8_t a, b, c, d;
    for (i = 0; i < 4; ++i) {
        a = (*state)[i][0];
        b = (*state)[i][1];
        c = (*state)[i][2];
        d = (*state)[i][3];

        (*state)[i][0] = Multiply(a, 0x0E) ^ Multiply(b, 0x0B) ^ Multiply(c, 0x0D) ^ Multiply(d, 0x09);
        (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0E) ^ Multiply(c, 0x0B) ^ Multiply(d, 0x0D);
        (*state)[i][2] = Multiply(a, 0x0D) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0E) ^ Multiply(d, 0x0B);
        (*state)[i][3] = Multiply(a, 0x0B) ^ Multiply(b, 0x0D) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0E);
    }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
inline static void inv_sub_bytes(state_t* state) {
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            (*state)[j][i] = rsbox[(*state)[j][i]];
}

inline static void inv_shift_rows(state_t* state) {
    uint8_t temp;

    // Rotate first row 1 columns to right
    temp = (*state)[3][1];
    (*state)[3][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[0][1];
    (*state)[0][1] = temp;

    // Rotate second row 2 columns to right
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to right
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[1][3];
    (*state)[1][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[3][3];
    (*state)[3][3] = temp;
}

// Cipher is the main function that encrypts the PlainText.
inline static void cipher_aes128(state_t* state, const uint8_t* RoundKey) {
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    add_round_key(0, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1; ; ++round) {
        sub_bytes(state);
        shift_rows(state);
        if (round == Nr128)
            break;

        mix_columns(state);
        add_round_key(round, state, RoundKey);
    }
    // Add round key to last round
    add_round_key(Nr128, state, RoundKey);
}

inline static void cipher_aes192(state_t* state, const uint8_t* RoundKey) {
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    add_round_key(0, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1; ; ++round) {
        sub_bytes(state);
        shift_rows(state);
        if (round == Nr192)
            break;

        mix_columns(state);
        add_round_key(round, state, RoundKey);
    }
    // Add round key to last round
    add_round_key(Nr192, state, RoundKey);
}

inline static void cipher_aes256(state_t* state, const uint8_t* RoundKey) {
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    add_round_key(0, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1; ; ++round) {
        sub_bytes(state);
        shift_rows(state);
        if (round == Nr256)
            break;

        mix_columns(state);
        add_round_key(round, state, RoundKey);
    }
    // Add round key to last round
    add_round_key(Nr256, state, RoundKey);
}

inline static void cipher_aes128_ni(void* state, __m128i* round_key) {
    __m128i m = _mm_loadu_si128((__m128i*)state);
    m = _mm_xor_si128(m, round_key[0]);
    m = _mm_aesenc_si128(m, round_key[1]);
    m = _mm_aesenc_si128(m, round_key[2]);
    m = _mm_aesenc_si128(m, round_key[3]);
    m = _mm_aesenc_si128(m, round_key[4]);
    m = _mm_aesenc_si128(m, round_key[5]);
    m = _mm_aesenc_si128(m, round_key[6]);
    m = _mm_aesenc_si128(m, round_key[7]);
    m = _mm_aesenc_si128(m, round_key[8]);
    m = _mm_aesenc_si128(m, round_key[9]);
    m = _mm_aesenclast_si128(m, round_key[10]);
    _mm_storeu_si128((__m128i*)state, m);
}

inline static void cipher_aes192_ni(void* state, __m128i* round_key) {
    __m128i m = _mm_loadu_si128((__m128i*)state);
    m = _mm_xor_si128(m, round_key[0]);
    m = _mm_aesenc_si128(m, round_key[1]);
    m = _mm_aesenc_si128(m, round_key[2]);
    m = _mm_aesenc_si128(m, round_key[3]);
    m = _mm_aesenc_si128(m, round_key[4]);
    m = _mm_aesenc_si128(m, round_key[5]);
    m = _mm_aesenc_si128(m, round_key[6]);
    m = _mm_aesenc_si128(m, round_key[7]);
    m = _mm_aesenc_si128(m, round_key[8]);
    m = _mm_aesenc_si128(m, round_key[9]);
    m = _mm_aesenc_si128(m, round_key[10]);
    m = _mm_aesenc_si128(m, round_key[11]);
    m = _mm_aesenclast_si128(m, round_key[12]);
    _mm_storeu_si128((__m128i*)state, m);
}

inline static void cipher_aes256_ni(void* state, __m128i* round_key) {
    __m128i m = _mm_loadu_si128((__m128i*)state);
    m = _mm_xor_si128(m, round_key[0]);
    m = _mm_aesenc_si128(m, round_key[1]);
    m = _mm_aesenc_si128(m, round_key[2]);
    m = _mm_aesenc_si128(m, round_key[3]);
    m = _mm_aesenc_si128(m, round_key[4]);
    m = _mm_aesenc_si128(m, round_key[5]);
    m = _mm_aesenc_si128(m, round_key[6]);
    m = _mm_aesenc_si128(m, round_key[7]);
    m = _mm_aesenc_si128(m, round_key[8]);
    m = _mm_aesenc_si128(m, round_key[9]);
    m = _mm_aesenc_si128(m, round_key[10]);
    m = _mm_aesenc_si128(m, round_key[11]);
    m = _mm_aesenc_si128(m, round_key[12]);
    m = _mm_aesenc_si128(m, round_key[13]);
    m = _mm_aesenclast_si128(m, round_key[14]);
    _mm_storeu_si128((__m128i*)state, m);
}

inline static void inv_cipher_aes128(state_t* state, const uint8_t* RoundKey) {
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    add_round_key(Nr128, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without InvMixColumn()
    for (round = (Nr128 - 1); ; --round) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(round, state, RoundKey);
        if (round == 0)
            break;

        inv_mix_columns(state);
    }

}

inline static void inv_cipher_aes192(state_t* state, const uint8_t* RoundKey) {
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    add_round_key(Nr192, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without InvMixColumn()
    for (round = (Nr192 - 1); ; --round) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(round, state, RoundKey);
        if (round == 0)
            break;

        inv_mix_columns(state);
    }

}

inline static void inv_cipher_aes256(state_t* state, const uint8_t* RoundKey) {
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    add_round_key(Nr256, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without InvMixColumn()
    for (round = (Nr256 - 1); ; --round) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(round, state, RoundKey);
        if (round == 0)
            break;

        inv_mix_columns(state);
    }

}

inline static void inv_cipher_aes128_ni(void* state, __m128i* round_key) {
    __m128i m = _mm_loadu_si128((__m128i*)state);
    m = _mm_xor_si128(m, round_key[10]);
    m = _mm_aesdec_si128(m, round_key[11]);
    m = _mm_aesdec_si128(m, round_key[12]);
    m = _mm_aesdec_si128(m, round_key[13]);
    m = _mm_aesdec_si128(m, round_key[14]);
    m = _mm_aesdec_si128(m, round_key[15]);
    m = _mm_aesdec_si128(m, round_key[16]);
    m = _mm_aesdec_si128(m, round_key[17]);
    m = _mm_aesdec_si128(m, round_key[18]);
    m = _mm_aesdec_si128(m, round_key[19]);
    m = _mm_aesdeclast_si128(m, round_key[0]);
    _mm_storeu_si128((__m128i*)state, m);
}

inline static void inv_cipher_aes192_ni(void* state, __m128i* round_key) {
    __m128i m = _mm_loadu_si128((__m128i*)state);
    m = _mm_xor_si128(m, round_key[12]);
    m = _mm_aesdec_si128(m, round_key[13]);
    m = _mm_aesdec_si128(m, round_key[14]);
    m = _mm_aesdec_si128(m, round_key[15]);
    m = _mm_aesdec_si128(m, round_key[16]);
    m = _mm_aesdec_si128(m, round_key[17]);
    m = _mm_aesdec_si128(m, round_key[18]);
    m = _mm_aesdec_si128(m, round_key[19]);
    m = _mm_aesdec_si128(m, round_key[20]);
    m = _mm_aesdec_si128(m, round_key[21]);
    m = _mm_aesdec_si128(m, round_key[22]);
    m = _mm_aesdec_si128(m, round_key[23]);
    m = _mm_aesdeclast_si128(m, round_key[0]);
    _mm_storeu_si128((__m128i*)state, m);
}

inline static void inv_cipher_aes256_ni(void* state, __m128i* round_key) {
    __m128i m = _mm_loadu_si128((__m128i*)state);
    m = _mm_xor_si128(m, round_key[14]);
    m = _mm_aesdec_si128(m, round_key[15]);
    m = _mm_aesdec_si128(m, round_key[16]);
    m = _mm_aesdec_si128(m, round_key[17]);
    m = _mm_aesdec_si128(m, round_key[18]);
    m = _mm_aesdec_si128(m, round_key[19]);
    m = _mm_aesdec_si128(m, round_key[20]);
    m = _mm_aesdec_si128(m, round_key[21]);
    m = _mm_aesdec_si128(m, round_key[22]);
    m = _mm_aesdec_si128(m, round_key[23]);
    m = _mm_aesdec_si128(m, round_key[24]);
    m = _mm_aesdec_si128(m, round_key[25]);
    m = _mm_aesdec_si128(m, round_key[26]);
    m = _mm_aesdec_si128(m, round_key[27]);
    m = _mm_aesdeclast_si128(m, round_key[0]);
    _mm_storeu_si128((__m128i*)state, m);
}

/*****************************************************************************/
/* Public functions:                                                         */
/*****************************************************************************/
void aes128_ecb_encrypt(aes128_ctx* ctx, uint8_t* buf) {
    // The next function call encrypts the PlainText with the Key using AES algorithm.
    if (aes_ni)
        cipher_aes128_ni(buf, ctx->RoundKeyNI);
    else
        cipher_aes128((state_t*)buf, ctx->RoundKey);
}

void aes192_ecb_encrypt(aes192_ctx* ctx, uint8_t* buf) {
    // The next function call encrypts the PlainText with the Key using AES algorithm.
    if (aes_ni)
        cipher_aes192_ni(buf, ctx->RoundKeyNI);
    else
        cipher_aes192((state_t*)buf, ctx->RoundKey);
}

void aes256_ecb_encrypt(aes256_ctx* ctx, uint8_t* buf) {
    // The next function call encrypts the PlainText with the Key using AES algorithm.
    if (aes_ni)
        cipher_aes256_ni(buf, ctx->RoundKeyNI);
    else
        cipher_aes256((state_t*)buf, ctx->RoundKey);
}

void aes128_ecb_decrypt(aes128_ctx* ctx, uint8_t* buf) {
    // The next function call decrypts the PlainText with the Key using AES algorithm.
    if (aes_ni)
        inv_cipher_aes128_ni(buf, ctx->RoundKeyNI);
    else
        inv_cipher_aes128((state_t*)buf, ctx->RoundKey);
}

void aes192_ecb_decrypt(aes192_ctx* ctx, uint8_t* buf) {
    // The next function call decrypts the PlainText with the Key using AES algorithm.
    if (aes_ni)
        inv_cipher_aes192_ni(buf, ctx->RoundKeyNI);
    else
        inv_cipher_aes192((state_t*)buf, ctx->RoundKey);
}

void aes256_ecb_decrypt(aes256_ctx* ctx, uint8_t* buf) {
    // The next function call decrypts the PlainText with the Key using AES algorithm.
    if (aes_ni)
        inv_cipher_aes256_ni(buf, ctx->RoundKeyNI);
    else
        inv_cipher_aes256((state_t*)buf, ctx->RoundKey);
}

void aes128_ecb_encrypt_buffer(aes128_ctx* ctx, uint8_t* buf, size_t length) {
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            cipher_aes128_ni(buf, ctx->RoundKeyNI);
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            cipher_aes128((state_t*)buf, ctx->RoundKey);
}

void aes192_ecb_encrypt_buffer(aes192_ctx* ctx, uint8_t* buf, size_t length) {
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            cipher_aes192_ni(buf, ctx->RoundKeyNI);
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            cipher_aes192((state_t*)buf, ctx->RoundKey);
}

void aes256_ecb_encrypt_buffer(aes256_ctx* ctx, uint8_t* buf, size_t length) {
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            cipher_aes256_ni(buf, ctx->RoundKeyNI);
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            cipher_aes256((state_t*)buf, ctx->RoundKey);
}

void aes128_ecb_decrypt_buffer(aes128_ctx* ctx, uint8_t* buf, size_t length) {
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            inv_cipher_aes128_ni(buf, ctx->RoundKeyNI);
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            inv_cipher_aes128((state_t*)buf, ctx->RoundKey);
}

void aes192_ecb_decrypt_buffer(aes192_ctx* ctx, uint8_t* buf, size_t length) {
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            inv_cipher_aes192_ni(buf, ctx->RoundKeyNI);
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            inv_cipher_aes192((state_t*)buf, ctx->RoundKey);
}

void aes256_ecb_decrypt_buffer(aes256_ctx* ctx, uint8_t* buf, size_t length) {
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            inv_cipher_aes256_ni(buf, ctx->RoundKeyNI);
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN)
            inv_cipher_aes256((state_t*)buf, ctx->RoundKey);
}

inline static void XorWithIv(uint8_t* buf, const uint8_t* Iv) {
    _mm_storeu_si128((__m128i*)buf,
        _mm_xor_si128(
            _mm_loadu_si128((const __m128i*)buf),
            _mm_loadu_si128((const __m128i*)Iv)
        )
    );
}

void aes128_cbc_encrypt(aes128_ctx* ctx, uint8_t buf[AES_BLOCKLEN]) {
    uint8_t* Iv = ctx->Iv;
    if (aes_ni) {
        XorWithIv(buf, Iv);
        cipher_aes128_ni(buf, ctx->RoundKeyNI);
        Iv = buf;
    }
    else {
        XorWithIv(buf, Iv);
        cipher_aes128((state_t*)buf, ctx->RoundKey);
        Iv = buf;
    }
    /* store Iv in ctx for next call */
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void aes192_cbc_encrypt(aes192_ctx* ctx, uint8_t buf[AES_BLOCKLEN]) {
    uint8_t* Iv = ctx->Iv;
    if (aes_ni) {
        XorWithIv(buf, Iv);
        cipher_aes192_ni(buf, ctx->RoundKeyNI);
        Iv = buf;
    }
    else {
        XorWithIv(buf, Iv);
        cipher_aes192((state_t*)buf, ctx->RoundKey);
        Iv = buf;
    }
    /* store Iv in ctx for next call */
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void aes256_cbc_encrypt(aes256_ctx* ctx, uint8_t buf[AES_BLOCKLEN]) {
    uint8_t* Iv = ctx->Iv;
    if (aes_ni) {
        XorWithIv(buf, Iv);
        cipher_aes256_ni(buf, ctx->RoundKeyNI);
        Iv = buf;
    }
    else {
        XorWithIv(buf, Iv);
        cipher_aes256((state_t*)buf, ctx->RoundKey);
        Iv = buf;
    }
    /* store Iv in ctx for next call */
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void aes128_cbc_decrypt(aes128_ctx* ctx, uint8_t buf[AES_BLOCKLEN]) {
    uint8_t storeNextIv[AES_BLOCKLEN];
    if (aes_ni) {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        inv_cipher_aes128_ni(buf, ctx->RoundKeyNI);
        XorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
    }
    else {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        inv_cipher_aes128((state_t*)buf, ctx->RoundKey);
        XorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
    }
}

void aes192_cbc_decrypt(aes192_ctx* ctx, uint8_t buf[AES_BLOCKLEN]) {
    uint8_t storeNextIv[AES_BLOCKLEN];
    if (aes_ni) {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        inv_cipher_aes192_ni(buf, ctx->RoundKeyNI);
        XorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
    }
    else {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        inv_cipher_aes192((state_t*)buf, ctx->RoundKey);
        XorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
    }
}

void aes256_cbc_decrypt(aes256_ctx* ctx, uint8_t buf[AES_BLOCKLEN]) {
    uint8_t storeNextIv[AES_BLOCKLEN];
    if (aes_ni) {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        inv_cipher_aes256_ni(buf, ctx->RoundKeyNI);
        XorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
    }
    else {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        inv_cipher_aes256((state_t*)buf, ctx->RoundKey);
        XorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
    }
}

void aes128_cbc_encrypt_buffer(aes128_ctx *ctx, uint8_t* buf, size_t length) {
    uint8_t* Iv = ctx->Iv;
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            XorWithIv(buf, Iv);
            cipher_aes128_ni(buf, ctx->RoundKeyNI);
            Iv = buf;
        }
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            XorWithIv(buf, Iv);
            cipher_aes128((state_t*)buf, ctx->RoundKey);
            Iv = buf;
        }
    /* store Iv in ctx for next call */
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void aes192_cbc_encrypt_buffer(aes192_ctx *ctx, uint8_t* buf, size_t length) {
    uint8_t* Iv = ctx->Iv;
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            XorWithIv(buf, Iv);
            cipher_aes192_ni(buf, ctx->RoundKeyNI);
            Iv = buf;
        }
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            XorWithIv(buf, Iv);
            cipher_aes192((state_t*)buf, ctx->RoundKey);
            Iv = buf;
        }
    /* store Iv in ctx for next call */
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void aes256_cbc_encrypt_buffer(aes256_ctx *ctx, uint8_t* buf, size_t length) {
    uint8_t* Iv = ctx->Iv;
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            XorWithIv(buf, Iv);
            cipher_aes256_ni(buf, ctx->RoundKeyNI);
            Iv = buf;
        }
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            XorWithIv(buf, Iv);
            cipher_aes256((state_t*)buf, ctx->RoundKey);
            Iv = buf;
        }
    /* store Iv in ctx for next call */
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void aes128_cbc_decrypt_buffer(aes128_ctx* ctx, uint8_t* buf, size_t length) {
    uint8_t storeNextIv[AES_BLOCKLEN];
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            memcpy(storeNextIv, buf, AES_BLOCKLEN);
            inv_cipher_aes128_ni(buf, ctx->RoundKeyNI);
            XorWithIv(buf, ctx->Iv);
            memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        }
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            memcpy(storeNextIv, buf, AES_BLOCKLEN);
            inv_cipher_aes128((state_t*)buf, ctx->RoundKey);
            XorWithIv(buf, ctx->Iv);
            memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        }
}

void aes192_cbc_decrypt_buffer(aes192_ctx* ctx, uint8_t* buf, size_t length) {
    uint8_t storeNextIv[AES_BLOCKLEN];
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            memcpy(storeNextIv, buf, AES_BLOCKLEN);
            inv_cipher_aes192_ni(buf, ctx->RoundKeyNI);
            XorWithIv(buf, ctx->Iv);
            memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        }
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            memcpy(storeNextIv, buf, AES_BLOCKLEN);
            inv_cipher_aes192((state_t*)buf, ctx->RoundKey);
            XorWithIv(buf, ctx->Iv);
            memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        }
}

void aes256_cbc_decrypt_buffer(aes256_ctx* ctx, uint8_t* buf, size_t length) {
    uint8_t storeNextIv[AES_BLOCKLEN];
    if (aes_ni)
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            memcpy(storeNextIv, buf, AES_BLOCKLEN);
            inv_cipher_aes256_ni(buf, ctx->RoundKeyNI);
            XorWithIv(buf, ctx->Iv);
            memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        }
    else
        for (size_t i = 0; i < length; i += AES_BLOCKLEN, buf += AES_BLOCKLEN) {
            memcpy(storeNextIv, buf, AES_BLOCKLEN);
            inv_cipher_aes256((state_t*)buf, ctx->RoundKey);
            XorWithIv(buf, ctx->Iv);
            memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        }
}

/* Symmetrical operation: same function for encrypting as for decrypting. Note any IV/nonce should never be reused with the same key */
void aes128_ctr_xcrypt_buffer(aes128_ctx* ctx, uint8_t* buf, uint32_t length) {
    uint8_t buffer[AES_BLOCKLEN];

    int32_t bi;
    size_t i;
    for (i = 0, bi = AES_BLOCKLEN; i < length; i++, bi++) {
        if (bi == AES_BLOCKLEN) { /* we need to regen xor compliment in buffer */
            memcpy(buffer, ctx->Iv, AES_BLOCKLEN);
            if (aes_ni)
                cipher_aes128_ni(buffer, ctx->RoundKeyNI);
            else
                cipher_aes128((state_t*)buffer, ctx->RoundKey);

            /* Increment Iv and handle overflow */
            for (bi = (AES_BLOCKLEN - 1); bi >= 0; bi--) {
                /* inc will overflow */
                if (ctx->Iv[bi] == 255) {
                    ctx->Iv[bi] = 0;
                    continue;
                }
                ctx->Iv[bi]++;
                break;
            }
            bi = 0;
        }

        buf[i] ^= buffer[bi];
    }
}

void aes192_ctr_xcrypt_buffer(aes192_ctx* ctx, uint8_t* buf, uint32_t length) {
    uint8_t buffer[AES_BLOCKLEN];

    int32_t bi;
    size_t i;
    for (i = 0, bi = AES_BLOCKLEN; i < length; i++, bi++) {
        if (bi == AES_BLOCKLEN) { /* we need to regen xor compliment in buffer */
            memcpy(buffer, ctx->Iv, AES_BLOCKLEN);
            if (aes_ni)
                cipher_aes192_ni(buffer, ctx->RoundKeyNI);
            else
                cipher_aes192((state_t*)buffer, ctx->RoundKey);

            /* Increment Iv and handle overflow */
            for (bi = (AES_BLOCKLEN - 1); bi >= 0; bi--) {
                /* inc will overflow */
                if (ctx->Iv[bi] == 255) {
                    ctx->Iv[bi] = 0;
                    continue;
                }
                ctx->Iv[bi]++;
                break;
            }
            bi = 0;
        }

        buf[i] ^= buffer[bi];
    }
}

void aes256_ctr_xcrypt_buffer(aes256_ctx* ctx, uint8_t* buf, uint32_t length) {
    uint8_t buffer[AES_BLOCKLEN];

    int32_t bi;
    size_t i;
    for (i = 0, bi = AES_BLOCKLEN; i < length; i++, bi++) {
        if (bi == AES_BLOCKLEN) { /* we need to regen xor compliment in buffer */
            memcpy(buffer, ctx->Iv, AES_BLOCKLEN);
            if (aes_ni)
                cipher_aes256_ni(buffer, ctx->RoundKeyNI);
            else
                cipher_aes256((state_t*)buffer, ctx->RoundKey);

            /* Increment Iv and handle overflow */
            for (bi = (AES_BLOCKLEN - 1); bi >= 0; bi--) {
                /* inc will overflow */
                if (ctx->Iv[bi] == 255) {
                    ctx->Iv[bi] = 0;
                    continue;
                }
                ctx->Iv[bi]++;
                break;
            }
            bi = 0;
        }

        buf[i] ^= buffer[bi];
    }
}
