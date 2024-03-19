/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/vec.hpp"
#include "frame_rate_control.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include "types.hpp"

enum AetFlags {
    AET_END       = 0x000001,
    AET_REVERSE   = 0x000002,
    AET_PAUSE     = 0x000004,
    AET_HIDDEN    = 0x000008,
    AET_LOOP      = 0x010000,
    AET_PLAY_ONCE = 0x020000,
    AET_FLIP_H    = 0x040000,
    AET_FLIP_V    = 0x080000,
    AET_100000    = 0x100000,
};

struct aet_info {
    uint16_t index;
    uint16_t set_index;

    inline aet_info() {
        this->index = (uint16_t)-1;
        this->set_index = (uint16_t)-1;
    }

    inline aet_info(uint16_t index, uint16_t set_index) {
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

static_assert(sizeof(aet_info) == 0x04, "\"aet_info\" struct should have a size of 0x04");

inline bool operator >(const aet_info& left, const aet_info& right) {
    return *(uint32_t*)&left > *(uint32_t*)&right;
}

inline bool operator <(const aet_info& left, const aet_info& right) {
    return *(uint32_t*)&left < *(uint32_t*)&right;
}

inline bool operator >=(const aet_info& left, const aet_info& right) {
    return *(uint32_t*)&left >= *(uint32_t*)&right;
}

inline bool operator <=(const aet_info& left, const aet_info& right) {
    return *(uint32_t*)&left <= *(uint32_t*)&right;
}

inline bool operator ==(const aet_info& left, const aet_info& right) {
    return *(uint32_t*)&left == *(uint32_t*)&right;
}

inline bool operator !=(const aet_info& left, const aet_info& right) {
    return *(uint32_t*)&left != *(uint32_t*)&right;
}

struct AetArgs {
    union IDUnion {
        aet_info info;
        int32_t id;

        IDUnion() {
            id = -1;
            info = {};
        }
    };

    IDUnion id;
    const char* layer_name;
    const char* start_marker;
    const char* end_marker;
    float_t start_time;
    float_t end_time;
    AetFlags flags;
    int32_t index;
    int32_t layer;
    spr::SprPrio prio;
    resolution_mode mode;
    vec3 pos;
    vec3 rot;
    vec3 scale;
    vec3 anchor;
    float_t frame_speed;
    vec4 color;
    prj::map<prj::string, int32_t> layer_sprite;
    prj::string sound_path;
    prj::map<prj::string, prj::string> sound_replace;
    int32_t sound_queue_index;
    prj::map<uint32_t, uint32_t> sprite_replace;
    prj::map<uint32_t, texture*> sprite_texture;
    prj::map<uint32_t, uint32_t> sprite_discard;
    FrameRateControl* frame_rate_control;
    bool sound_voice;

    AetArgs();
    ~AetArgs();
};

static_assert(sizeof(AetArgs) == 0x108, "\"AetArgs\" struct should have a size of 0x108");

struct aet_layout_data {
    mat4 mat;
    vec3 position;
    vec3 anchor;
    float_t width;
    float_t height;
    float_t opacity;
    color4u8 color;
    resolution_mode mode;

    aet_layout_data();

    static void put_sprite(int32_t spr_id, spr::SprAttr attr,
        spr::SprPrio prio, const vec2* pos, const aet_layout_data* layout);
    static void set_args(const aet_layout_data* layout, spr::SprArgs* args);
};

static_assert(sizeof(aet_layout_data) == 0x6C, "\"aet_layout_data\" struct should have a size of 0x6C");

struct AetComp {
    prj::map<std::string, aet_layout_data> data;

    AetComp();
    ~AetComp();

    void Add(const char* name, const aet_layout_data& data);
    void Clear();
    const aet_layout_data* Find(const char* name);

    static void put_number_sprite(int32_t value, int32_t max_digits,
        AetComp* comp, const char** names, const int32_t* spr_ids,
        spr::SprPrio prio, const vec2* pos, bool all_digits);
};

static_assert(sizeof(AetComp) == 0x10, "\"AetComp\" struct should have a size of 0x10");

extern void (FASTCALL* aet_manager_unload_set)(int32_t set_id);
extern bool (FASTCALL* aet_manager_get_obj_end)(uint32_t id);
extern void (FASTCALL* aet_manager_init_aet_layout)(AetComp* comp, AetArgs* args);
extern float_t(FASTCALL* aet_manager_get_obj_frame)(uint32_t id);
extern void (FASTCALL* aet_manager_init_aet_layout_0)(AetComp* comp, int32_t aet_id, const char* layer_name,
    AetFlags flags, resolution_mode mode, const char* start_marker, float_t start_time);
extern uint32_t(FASTCALL* aet_manager_init_aet_object)(AetArgs& args);
extern void (FASTCALL* aet_manager_free_aet_object)(uint32_t id);
extern void (FASTCALL* aet_manager_read_file)(int32_t set_id, const prj::string mdata_dir);
extern void (FASTCALL* aet_manager_set_obj_visible)(uint32_t id, bool value);
extern void (FASTCALL* aet_manager_set_obj_frame)(uint32_t id, float_t value);
extern void (FASTCALL* aet_manager_set_obj_alpha)(uint32_t id, float_t value);
extern bool (FASTCALL* aet_manager_load_file)(int32_t set_id);

extern int32_t aet_database_get_aet_id_by_name(const prj::string& name);
extern int32_t aet_database_get_aet_set_id_by_name(const prj::string& name);

extern void aet_manager_set_obj_frame_rate_control(uint32_t id, FrameRateControl* value);
