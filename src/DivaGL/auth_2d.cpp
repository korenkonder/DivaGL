/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "auth_2d.hpp"
#include "file_handler.hpp"
#include "task.hpp"

struct aet_comp;
struct aet_marker;
struct aet_scene;

typedef uint16_t aet_layer_flag;
typedef uint8_t aet_layer_quality;

union aet_item {
    void* none;
    uint32_t offset;
    struct aet_video* video;
    struct aet_audio* audio;
    aet_comp* comp;
};
enum aet_item_type : uint8_t {
    AET_ITEM_TYPE_NONE        = 0x00,
    AET_ITEM_TYPE_VIDEO       = 0x01,
    AET_ITEM_TYPE_AUDIO       = 0x02,
    AET_ITEM_TYPE_COMPOSITION = 0x03,
};

struct aet_layer {
    const char* name;
    float_t start_time;
    float_t end_time;
    float_t offset_time;
    float_t time_scale;
    aet_layer_flag flags;
    aet_layer_quality quality;
    aet_item_type item_type;
    aet_item item;
    aet_layer* parent;
    uint32_t markers_count;
    aet_marker* markers;
    struct aet_layer_video* video;
    struct aet_layer_audio* audio;
};

struct AetSet;

struct AetSet_vtbl {
    AetSet* (__fastcall* Dispose)(AetSet* _this, uint8_t flags);
    void(__fastcall* ReadFile)(AetSet* _this, const char* file_path);
    bool(__fastcall* LoadFile)(AetSet* _this);
    void(__fastcall* Unload)(AetSet* _this);
    bool(__fastcall* Load)(AetSet* _this);
};

struct AetSet {
    AetSet_vtbl* __vftable;
    int32_t index;
    aet_scene** scenes;
    uint32_t scenes_count;
    int32_t load_count;
    p_file_handler file_handler;
    bool ready;
    prj::shared_ptr<void*> alloc_handler; // prj::stack_allocator
};

struct AetObj;

struct AetObj_vtbl {
    AetObj*(FASTCALL * Dispose)(AetObj* _this, uint8_t flags);
    void(FASTCALL* Init)(AetObj* _this, AetArgs* init_data, aet_scene* scene, aet_comp* comp,
        aet_layer* layer, aet_marker* start_marker, aet_marker* end_marker, resolution_mode mode);
    void(FASTCALL* Ctrl)(AetObj* _this);
    void(FASTCALL* Disp)(AetObj* _this);
    bool(FASTCALL* StepFrame)(AetObj* _this);
    void(FASTCALL* SetPlay)(AetObj* _this, bool value);
    void(FASTCALL* SetVisible)(AetObj* _this, bool value);
    void(FASTCALL* SetPosition)(AetObj* _this, vec3* value);
    void(FASTCALL* SetScale)(AetObj* _this, vec3* value);
    void(FASTCALL* SetRotation)(AetObj* _this, vec3* value);
    void(FASTCALL* SetAlpha)(AetObj* _this, float_t value);
    void(FASTCALL* SetSpriteReplace)(AetObj* _this, prj::map<uint32_t, uint32_t>& value);
    void(FASTCALL* SetSpriteTexture)(AetObj* _this, prj::map<uint32_t, texture*>& value);
    void(FASTCALL* SetSpriteDiscard)(AetObj* _this, prj::map<uint32_t, uint32_t>& value);
    void(FASTCALL* SetColor)(AetObj* _this, vec4* value);
    void(FASTCALL* SetEndTime)(AetObj* _this, float_t value);
    void(FASTCALL* SetFrame)(AetObj* _this, float_t value);
    void(FASTCALL* SetPrio)(AetObj* _this, spr::SprPrio value);
    aet_info(FASTCALL* GetInfo)(AetObj* _this);
    bool(FASTCALL* GetPlay)(AetObj* _this);
    bool(FASTCALL* GetVisible)(AetObj* _this);
    float_t(FASTCALL* GetFrame)(AetObj* _this);
    bool(FASTCALL* GetEnd)(AetObj* _this);
    void(FASTCALL* CtrlComp)(AetObj* _this, aet_comp* comp, float_t frame);
    void(FASTCALL* DispComp)(AetObj* _this, mat4* mat, aet_comp* comp, float_t frame, float_t opacity);
    void(FASTCALL* CtrlLayer)(AetObj* _this, aet_layer* layer, float_t frame);
    void(FASTCALL* DispLayer)(AetObj* _this, mat4* mat, aet_layer* layer, float_t frame, float_t opacity);
    void(FASTCALL* DispVideo)(AetObj* _this, mat4* mat, aet_layer* layer, float_t frame, float_t opacity);
    void(FASTCALL* DispSprite)(AetObj* _this, mat4* mat, aet_layer* layer, float_t opacity);
    void(FASTCALL* DispSpriteSource)(AetObj* _this, mat4* mat,
        aet_layer* layer, uint32_t source_index, float_t opacity);
    void(FASTCALL* CalcMat)(AetObj* _this, mat4* mat, aet_layer* layer, float_t frame);
};

struct AetObj {
    AetObj_vtbl* __vftable;
    aet_info info;
    aet_scene* scene;
    aet_comp* comp;
    aet_layer* layer;
    float_t start_time;
    float_t end_time;
    AetFlags flags;
    int32_t index;
    int32_t layer_index;
    spr::SprPrio prio;
    resolution_mode src_mode;
    vec3 pos;
    vec3 rot;
    vec3 scale;
    vec3 anchor;
    float_t frame_speed;
    vec4 color;
    float_t frame;
    resolution_mode dst_mode;
    vec2 scale_size;
    bool matte;
    spr::SprArgs spr_args;
    float_t opacity;
    prj::map<prj::string, int32_t> layer_sprite;
    prj::string sound_path;
    prj::map<prj::string, prj::string> sound_replace;
    int32_t sound_queue_index;
    prj::map<uint32_t, uint32_t> sprite_replace;
    prj::map<uint32_t, texture*> sprite_texture;
    prj::map<uint32_t, uint32_t> sprite_discard;
    FrameRateControl* frame_rate_control;
    bool sound_voice;
    bool use_float;

    void SetFrameRateControl(FrameRateControl* value);
};

class AetMgr : public app::Task {
public:
    prj::map<int32_t, AetSet> sets;
    prj::map<uint32_t, AetObj> objects;
    prj::list<prj::map<uint32_t, AetObj>::iterator> free_objects;
    uint32_t load_counter;

    AetObj* get_obj(uint32_t id);
    void set_obj_frame_rate_control(uint32_t id, FrameRateControl* value);
};

void (FASTCALL* aet_manager_unload_set)(int32_t set_id)
    = (void (FASTCALL*)(int32_t set_id))0x000000014019CDD0;
bool (FASTCALL* aet_manager_get_obj_end)(uint32_t id)
    = (bool (FASTCALL*)(uint32_t id))0x000000014019CE60;
void (FASTCALL* aet_manager_init_aet_layout)(AetComp* comp, AetArgs* args)
    = (void (FASTCALL*)(AetComp* comp, AetArgs * args))0x000000014019CEB0;
float_t(FASTCALL* aet_manager_get_obj_frame)(uint32_t id) = (float_t(FASTCALL*)(uint32_t id))0x000000014019CE70;
void (FASTCALL* aet_manager_init_aet_layout_0)(AetComp* comp, int32_t aet_id, const char* layer_name,
    AetFlags flags, resolution_mode mode, const char* start_marker, float_t start_time)
    = (void (FASTCALL * )(AetComp * comp, int32_t aet_id, const char* layer_name,
        AetFlags flags, resolution_mode mode, const char* start_marker, float_t start_time))0x000000014019CF40;
uint32_t(FASTCALL* aet_manager_init_aet_object)(AetArgs& args)
    = (uint32_t(FASTCALL*)(AetArgs & args))0x000000014019D3F0;
void (FASTCALL* aet_manager_free_aet_object)(uint32_t id)
    = (void (FASTCALL*)(uint32_t id))0x000000014019D570;
void (FASTCALL* aet_manager_read_file)(int32_t set_id, const prj::string mdata_dir)
    = (void (FASTCALL*)(int32_t set_id, const prj::string mdata_dir))0x000000014019D650;
void (FASTCALL* aet_manager_set_obj_visible)(uint32_t id, bool value)
    = (void (FASTCALL*)(uint32_t id, bool value))0x000000014019D750;
void (FASTCALL* aet_manager_set_obj_frame)(uint32_t id, float_t value)
    = (void (FASTCALL*)(uint32_t id, float_t value))0x000000014019D790;
void (FASTCALL* aet_manager_set_obj_alpha)(uint32_t id, float_t value)
    = (void (FASTCALL*)(uint32_t id, float_t value))0x000000014019D7B0;
bool (FASTCALL* aet_manager_load_file)(int32_t set_id)
    = (bool (FASTCALL*)(int32_t set_id))0x000000014019D970;

AetMgr& aet_manager = *(AetMgr*)0x0000000140EC5390;

AetArgs::AetArgs() : layer_name(), start_marker(), end_marker(),
flags(), index(), layer(), frame_rate_control(), sound_voice() {
    start_time = -1.0f;
    end_time = -1.0f;
    prio = spr::SPR_PRIO_DEFAULT;
    mode = RESOLUTION_MODE_HD;
    scale = 1.0f;
    frame_speed = 1.0f;
    color = 1.0f;
    sound_queue_index = 1;
}

AetArgs::~AetArgs() {

}

aet_layout_data::aet_layout_data() : width(), height() {
    mat = mat4_identity;
    opacity = 1.0f;
    color = 0xFFFFFFFF;
    mode = RESOLUTION_MODE_HD;
}

void aet_layout_data::put_sprite(int32_t spr_id, spr::SprAttr attr,
    spr::SprPrio prio, const vec2* pos, const aet_layout_data* layout) {
    if (!layout)
        return;

    spr::SprArgs args;
    aet_layout_data::set_args(layout, &args);
    args.id.id = spr_id;
    args.attr = attr;
    args.prio = prio;
    if (pos)
        *(vec2*)&args.trans.x += *pos;
    spr::put_sprite(&args);
}

void aet_layout_data::set_args(const aet_layout_data* layout, spr::SprArgs* args) {
    if (!layout || !args)
        return;

    args->trans = layout->position;
    args->center = layout->anchor;
    args->scale.x = layout->mat.row0.x;
    args->scale.y = layout->mat.row1.y;
    args->scale.z = layout->mat.row2.z;
}

AetComp::AetComp() {

}

AetComp::~AetComp() {

}

void AetComp::Add(const char* name, const aet_layout_data& data) {
    this->data.insert({ name, data });
}

void AetComp::Clear() {
    data.clear();
}

const aet_layout_data* AetComp::Find(const char* name) {
    auto elem = data.find(name);
    if (elem != data.end())
        return &elem->second;
    return 0;
}

void AetComp::put_number_sprite(int32_t value, int32_t max_digits,
    AetComp* comp, const char** names, const int32_t* spr_ids,
    spr::SprPrio prio, const vec2* pos, bool all_digits) {
    if (!comp || !names || !spr_ids)
        return;

    for (int32_t i = 0; i < max_digits; i++) {
        int32_t digit = value % 10;
        value /= 10;

        const aet_layout_data* layout = comp->Find(names[i]);
        if (layout)
            aet_layout_data::put_sprite(spr_ids[digit],
                spr::SPR_ATTR_CTR_CC, prio, pos, layout);

        if (!all_digits && !value)
            break;
    }
}

int32_t aet_database_get_aet_id_by_name(const prj::string& name) {
    int32_t(FASTCALL * aet_database_struct__get_aet_id_by_name)
        (size_t _this, const prj::string & name)
        = (int32_t(FASTCALL*)(size_t _this, const prj::string & name))0x0000000140112180;
    return aet_database_struct__get_aet_id_by_name(0x0000000140EC5430, name);
}

int32_t aet_database_get_aet_set_id_by_name(const prj::string& name) {
    int32_t(FASTCALL * aet_database_struct__get_aet_set_id_by_name)
        (size_t _this, const prj::string & name)
        = (int32_t(FASTCALL*)(size_t _this, const prj::string & name))0x0000000140112190;
    return aet_database_struct__get_aet_set_id_by_name(0x0000000140EC5430, name);
}

void aet_manager_set_obj_frame_rate_control(uint32_t id, FrameRateControl* value) {
    aet_manager.set_obj_frame_rate_control(id, value);
}

void AetObj::SetFrameRateControl(FrameRateControl* value) {
    frame_rate_control = value;
}

AetObj* AetMgr::get_obj(uint32_t id) {
    auto elem = objects.find(id);
    if (elem != objects.end())
        return &elem->second;
    return 0;
}

void AetMgr::set_obj_frame_rate_control(uint32_t id, FrameRateControl* value) {
    AetObj* obj = get_obj(id);
    if (obj)
        obj->SetFrameRateControl(value);
}
