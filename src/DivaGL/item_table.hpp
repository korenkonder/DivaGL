/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/image.hpp"
#include "object.hpp"
#include "types.hpp"

enum item_id {
    ITEM_NONE        = -1,
    ITEM_BODY        = 0x00,
    ITEM_ATAMA       = 0x01,
    ITEM_KATA_R      = 0x02,
    ITEM_MUNE        = 0x03,
    ITEM_KATA_L      = 0x04,
    ITEM_UDE_R       = 0x05,
    ITEM_SENAKA      = 0x06,
    ITEM_UDE_L       = 0x07,
    ITEM_HARA        = 0x08,
    ITEM_KOSI        = 0x09,
    ITEM_TE_R        = 0x0A,
    ITEM_TE_L        = 0x0B,
    ITEM_MOMO        = 0x0C,
    ITEM_SUNE        = 0x0D,
    ITEM_ASI         = 0x0E,
    ITEM_KAMI        = 0x0F,
    ITEM_OUTER       = 0x10,
    ITEM_PANTS       = 0x11,
    ITEM_ZUJO        = 0x12,
    ITEM_MEGANE      = 0x13,
    ITEM_KUBI        = 0x14,
    ITEM_JOHA_USHIRO = 0x15,
    ITEM_KUCHI       = 0x16,
    ITEM_ITEM09      = 0x17,
    ITEM_ITEM10      = 0x18,
    ITEM_ITEM11      = 0x19,
    ITEM_ITEM12      = 0x1A,
    ITEM_ITEM13      = 0x1B,
    ITEM_ITEM14      = 0x1C,
    ITEM_ITEM15      = 0x1D,
    ITEM_ITEM16      = 0x1E,
    ITEM_MAX         = 0x1F,
};

enum item_sub_id {
    ITEM_SUB_NONE        = -1,
    ITEM_SUB_ZUJO        = 0x00,
    ITEM_SUB_KAMI        = 0x01,
    ITEM_SUB_HITAI       = 0x02,
    ITEM_SUB_ME          = 0x03,
    ITEM_SUB_MEGANE      = 0x04,
    ITEM_SUB_MIMI        = 0x05,
    ITEM_SUB_KUCHI       = 0x06,
    ITEM_SUB_MAKI        = 0x07,
    ITEM_SUB_KUBI        = 0x08,
    ITEM_SUB_INNER       = 0x09,
    ITEM_SUB_OUTER       = 0x0A,
    ITEM_SUB_KATA        = 0x0B,
    ITEM_SUB_U_UDE       = 0x0C,
    ITEM_SUB_L_UDE       = 0x0D,
    ITEM_SUB_TE          = 0x0E,
    ITEM_SUB_JOHA_MAE    = 0x0F,
    ITEM_SUB_JOHA_USHIRO = 0x10,
    ITEM_SUB_BELT        = 0x11,
    ITEM_SUB_KOSI        = 0x12,
    ITEM_SUB_PANTS       = 0x13,
    ITEM_SUB_ASI         = 0x14,
    ITEM_SUB_SUNE        = 0x15,
    ITEM_SUB_KUTSU       = 0x16,
    ITEM_SUB_HADA        = 0x17,
    ITEM_SUB_HEAD        = 0x18,
    ITEM_SUB_MAX         = 0x19,
};


union item_cos_data {
    struct {
        int32_t zujo;
        int32_t kami;
        int32_t hitai;
        int32_t me;
        int32_t megane;
        int32_t mimi;
        int32_t kuchi;
        int32_t maki;
        int32_t kubi;
        int32_t inner;
        int32_t outer;
        int32_t joha_mae;
        int32_t joha_ushiro;
        int32_t hada;
        int32_t kata;
        int32_t u_ude;
        int32_t l_ude;
        int32_t te;
        int32_t belt;
        int32_t cosi;
        int32_t pants;
        int32_t asi;
        int32_t sune;
        int32_t kutsu;
        int32_t head;
    } data;
    int32_t arr[ITEM_SUB_MAX];
};

static_assert(sizeof(item_cos_data) == 0x64, "\"item_cos_data\" struct should have a size of 0x64");

struct item_table_item_data_obj {
    object_info obj_info;
    item_id rpk;
};

static_assert(sizeof(item_table_item_data_obj) == 0x08, "\"item_table_item_data_obj\" struct should have a size of 0x08");

struct item_table_item_data_ofs {
    item_sub_id sub_id;
    int32_t no;
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

static_assert(sizeof(item_table_item_data_ofs) == 0x2C, "\"item_table_item_data_obj\" struct should have a size of 0x2C");

struct item_table_item_data_tex {
    int32_t org;
    int32_t chg;
};

static_assert(sizeof(item_table_item_data_tex) == 0x08, "\"item_table_item_data_tex\" struct should have a size of 0x08");

struct item_table_item_data_col {
    int32_t tex_id;
    int32_t flag;
    color_tone col_tone;
};

static_assert(sizeof(item_table_item_data_col) == 0x54, "\"item_table_item_data_col\" struct should have a size of 0x54");

struct item_table_item_data {
    prj::vector<item_table_item_data_obj> obj;
    prj::vector<item_table_item_data_ofs> ofs;
    prj::vector<item_table_item_data_tex> tex;
    prj::vector<item_table_item_data_col> col;
};

static_assert(sizeof(item_table_item_data) == 0x60, "\"item_table_item_data\" struct should have a size of 0x60");

struct item_table_item {
    int32_t flag;
    prj::string name;
    prj::list<int32_t> objset;
    int32_t type;
    int32_t attr;
    int32_t des_id;
    item_sub_id sub_id;
    item_table_item_data data;
    int32_t exclusion_point;
    int32_t field_AC;
    int32_t org_itm;
    bool npr_flag;
    float_t face_depth;
};

static_assert(sizeof(item_table_item) == 0xC0, "\"item_table_item\" struct should have a size of 0xC0");
