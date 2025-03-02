/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/vec.hpp"
#include "../../AFTModsShared/types.hpp"

enum pvpp_chara_id : int8_t {
    PVPP_CHARA_PNONE =   -1,
    PVPP_CHARA_1P    = 0x00,
    PVPP_CHARA_2P    = 0x01,
    PVPP_CHARA_3P    = 0x02,
    PVPP_CHARA_4P    = 0x03,
};

enum pvpp_chara_index : int8_t {
    PVPP_CHARA_NONE   =   -1,
    PVPP_CHARA_MIKU   = 0x00,
    PVPP_CHARA_RIN    = 0x01,
    PVPP_CHARA_LEN    = 0x02,
    PVPP_CHARA_LUKA   = 0x03,
    PVPP_CHARA_NERU   = 0x04,
    PVPP_CHARA_HAKU   = 0x05,
    PVPP_CHARA_KAITO  = 0x06,
    PVPP_CHARA_MEIKO  = 0x07,
    PVPP_CHARA_SAKINE = 0x08,
    PVPP_CHARA_TETO   = 0x09,
    PVPP_CHARA_EXTRA  = 0x0A,
};

struct pvpp_chara_effect_auth_3d {
    const char* auth_3d;
    const char* object_set;
    uint8_t u00;
};

struct pvpp_chara_effect {
    pvpp_chara_index base_chara;
    pvpp_chara_id chara_id;
    int8_t num_auth_3d;
    const pvpp_chara_effect_auth_3d* auth_3d_array;
};

struct pvpp_auth_3d {
    const char* name;
};

struct pvpp_glitter {
    const char* name;
    bool unk2;
};

struct pvpp_effect {
    pvpp_chara_id chara_id;
    int8_t num_auth_3d;
    int8_t num_glitter;
    const pvpp_auth_3d* auth_3d_array;
    const pvpp_glitter* glitter_array;
};

struct pvpp_chara {
    int8_t num_auth_3d;
    int8_t num_glitter;
    const pvpp_auth_3d* auth_3d_array;
    const pvpp_glitter* glitter_array;
    const pvpp_chara_effect* chara_effect;
};

struct pvpp {
    uint32_t signature;
    int8_t num_chara;
    int8_t num_effect;
    const pvpp_chara* chara_array;
    const pvpp_effect* effect_array;

    static const pvpp* read(const void* data, size_t size);
};
