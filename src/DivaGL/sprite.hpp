/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/rectangle.hpp"
#include "../KKdLib/vec.hpp"
#include "render_context.hpp"
#include "resolution_mode.hpp"
#include "texture.hpp"
#include "types.hpp"

struct spr_info {
    uint16_t index;
    uint16_t set_index;

    inline spr_info() {
        this->index = (uint16_t)-1;
        this->set_index = (uint16_t)-1;
    }

    inline spr_info(uint16_t index, uint16_t set_index) {
        this->index = index;
        this->set_index = set_index;
    }

    inline bool is_null() const {
        return index == (uint16_t)-1 && set_index == (uint16_t)-1;
    }

    inline bool not_null() const {
        return index != (uint16_t)-1 || set_index != (uint16_t)-1;
    }
};

constexpr bool operator==(const spr_info& left, const spr_info& right) {
    return *(uint32_t*)&left == *(uint32_t*)&right;
}

constexpr bool operator!=(const spr_info& left, const spr_info& right) {
    return !(left == right);
}

constexpr bool operator<(const spr_info& left, const spr_info& right) {
    return *(uint32_t*)&left < *(uint32_t*)&right;
}

constexpr bool operator>(const spr_info& left, const spr_info& right) {
    return right < left;
}

constexpr bool operator<=(const spr_info& left, const spr_info& right) {
    return !(right < left);
}

constexpr bool operator>=(const spr_info& left, const spr_info& right) {
    return !(left < right);
}

struct spr_db_spr {
    uint32_t id;
    prj::string name;
    spr_info info;
};

struct SpriteHeaderFile {
    uint32_t flag;
    uint32_t texofs;
    int32_t num_of_texture;
    int32_t num_of_sprite;
    uint32_t sprinfo_offset;
    uint32_t texname_offset;
    uint32_t sprname_offset;
    uint32_t sprdata_offset;
};

namespace spr {
    struct SprInfo {
        uint32_t texid;
        int32_t rotate;
        float_t su;
        float_t sv;
        float_t eu;
        float_t ev;
        float_t px;
        float_t py;
        float_t width;
        float_t height;
    };
};

struct SpriteData {
    uint32_t attr;
    resolution_mode resolution_mode;
};

struct SpriteHeader {
    uint32_t flag;
    uint32_t texofs;
    int32_t num_of_texture;
    int32_t num_of_sprite;
    spr::SprInfo* sprinfo;
    const char** texname;
    const char** sprname;
    SpriteData* sprdata;
};

struct SprSet {
    void* vftable;
    int32_t flag;
    int32_t index;
    SpriteHeaderFile* header_file;
    SpriteHeader* header;
    texture** textures;
    int32_t load_count;
    void* file_handler;
    bool ready;
    prj::shared_ptr<void*> alloc_handler; // prj::stack_allocator
    prj::string file;
};

namespace spr {
    enum SprAttr : uint32_t {
        SPR_ATTR_FLIP_H   = 0x00010000,
        SPR_ATTR_FLIP_V   = 0x00020000,
        SPR_ATTR_CTR_LT   = 0x00040000,
        SPR_ATTR_CTR_CT   = 0x00080000,
        SPR_ATTR_CTR_RT   = 0x00100000,
        SPR_ATTR_CTR_LC   = 0x00200000,
        SPR_ATTR_CTR_CC   = 0x00400000,
        SPR_ATTR_CTR_RC   = 0x00800000,
        SPR_ATTR_CTR_LB   = 0x01000000,
        SPR_ATTR_CTR_CB   = 0x02000000,
        SPR_ATTR_CTR_RB   = 0x04000000,
        SPR_ATTR_MATRIX   = 0x08000000,
        SPR_ATTR_10000000 = 0x10000000,
        SPR_ATTR_NOBLEND  = 0x20000000,

        SPR_ATTR_CTR      = 0x07FC0000,
    };

    enum SprKind {
        SPR_KIND_NORMAL = 0,
        SPR_KIND_LINE,
        SPR_KIND_LINES,
        SPR_KIND_RECT,
        SPR_KIND_MULTI,
        SPR_KIND_ARROW_A,
        SPR_KIND_ARROW_B,
        SPR_KIND_ARROW_AB,
        SPR_KIND_TRIANGLE,
        SPR_KIND_CIRCLE,
    };

    enum SprPrio {
        SPR_PRIO_00 = 0,
        SPR_PRIO_01,
        SPR_PRIO_02,
        SPR_PRIO_03,
        SPR_PRIO_04,
        SPR_PRIO_05,
        SPR_PRIO_06,
        SPR_PRIO_07,
        SPR_PRIO_08,
        SPR_PRIO_09,
        SPR_PRIO_10,
        SPR_PRIO_11,
        SPR_PRIO_12,
        SPR_PRIO_13,
        SPR_PRIO_14,
        SPR_PRIO_15,
        SPR_PRIO_16,
        SPR_PRIO_17,
        SPR_PRIO_18,
        SPR_PRIO_19,
        SPR_PRIO_20,
        SPR_PRIO_21,
        SPR_PRIO_22,
        SPR_PRIO_23,
        SPR_PRIO_24,
        SPR_PRIO_25,
        SPR_PRIO_26,
        SPR_PRIO_27,
        SPR_PRIO_28,
        SPR_PRIO_29,
        SPR_PRIO_30,
        SPR_PRIO_31,
        SPR_PRIO_MAX,

        SPR_PRIO_BACKGROUND = SPR_PRIO_03,
        SPR_PRIO_DEFAULT    = SPR_PRIO_07,
        SPR_PRIO_DEBUG      = SPR_PRIO_29,
    };

    struct SpriteVertex {
        vec3 pos;
        vec2 uv;
        color4u8 color;
    };

    struct SprArgs {
        enum Flags : uint32_t {
            SPRITE_SIZE = 0x01,
            TEXTURE_POS_SIZE = 0x02,
        };

        union IDUnion {
            int32_t index;
            uint32_t id;
            spr_info info;

            IDUnion() {
                index = -1;
                id = -1;
                info = {};
            }
        };

        SprKind kind;
        IDUnion id;
        color4u8_bgra color;
        SprAttr attr;
        int32_t blend;
        int32_t index;
        int32_t layer;
        SprPrio prio;
        resolution_mode resolution_mode_screen;
        resolution_mode resolution_mode_sprite;
        vec3 center;
        vec3 trans;
        vec3 scale;
        vec3 rot;
        vec2 skew_angle;
        mat4 mat;
        texture* texture;
        int32_t shader;
        union {
            size_t vertex_array;
            SpriteVertex* vertex_array_ptr;
        };
        size_t num_vertex;
        Flags flags;
        vec2 sprite_size;
        int32_t field_CC;
        vec2 texture_pos;
        vec2 texture_size;
        spr::SprArgs* next;

        SprArgs();

        SpriteVertex* GetVertexArray();
        void Reset();
        void SetSpriteSize(vec2 size);
        void SetTexturePosSize(float_t x, float_t y, float_t width, float_t height);
        void SetVertexArray(SpriteVertex* vertex_array, size_t num_vertex);

        static void SetNext(SprArgs* args, SprArgs* next);
    };

    static_assert(sizeof(spr::SprArgs) == 0xE8, "\"spr::SprArgs\" struct should have a size of 0xE8");

    extern void(FASTCALL* put_cross)(const mat4* mat, color4u8 color_x, color4u8 color_y, color4u8 color_z);
    extern void(FASTCALL* put_rgb_cross)(const mat4* mat);
    extern spr::SprArgs* (FASTCALL* put_sprite)(const spr::SprArgs* args);

    vec2 proj_sprite_3d_line(vec3 vec, bool offset);

    void put_sprite_3d_line(vec3 p1, vec3 p2, color4u8 color);
    void put_sprite_line(vec2 p1, vec2 p2, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer);
    void put_sprite_line_list(vec2* points, size_t count, resolution_mode mode,
        spr::SprPrio prio, color4u8 color, int32_t layer);
    void put_sprite_multi(rectangle rect, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer);
    void put_sprite_rect(rectangle rect, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer);
    void put_sprite_triangles(SpriteVertex* vert, size_t num, resolution_mode mode,
        SprPrio prio, int32_t spr_id, int32_t layer);
}

extern vec4& spr_color;

extern void (FASTCALL* sprite_manager_unload_set)(int32_t set_id);
extern size_t(FASTCALL* sprite_manager_get_reqlist_count)(int32_t index);
extern void (FASTCALL* sprite_manager_read_file)(int32_t set_id, const prj::string mdata_dir);
extern bool (FASTCALL* sprite_manager_load_file)(int32_t set_id);

extern const spr_db_spr* sprite_database_get_spr_by_id(int32_t id);
extern int32_t sprite_database_get_spr_set_id_by_name(const prj::string& name);

extern void sprite_manager_init();
extern void sprite_manager_draw(int32_t index, bool font, texture* tex);
extern void sprite_manager_set_res(double_t aspect, int32_t width, int32_t height);
extern void sprite_manager_set_view_projection(bool aet_3d);
extern void sprite_manager_free();

extern void sprite_patch();
