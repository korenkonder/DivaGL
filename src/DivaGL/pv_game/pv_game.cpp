/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "pv_game.hpp"
#include "../../KKdLib/io/memory_stream.hpp"
#include "../Glitter/glitter.hpp"
#include "../rob/rob.hpp"
#include "../auth_2d.hpp"
#include "../auth_3d.hpp"
#include "../print_work.hpp"
#include "dsc.hpp"
#include "pvpp.hpp"
#include "pvsr.hpp"
#include <Helpers.h>

enum dsc_x_func {
    DSC_X_END = 0,
    DSC_X_TIME,
    DSC_X_CHANGE_FIELD = 14,
    DSC_X_CREDIT_TITLE = 84,
    DSC_X_BAR_POINT = 85,
    DSC_X_BEAT_POINT,
    DSC_X_STAGE_EFFECT = 111,
    DSC_X_SONG_EFFECT,
    DSC_X_SONG_EFFECT_ATTACH,
    DSC_X_SET_STAGE_EFFECT_ENV = 116,
    DSC_X_CHARA_EFFECT = 124,
    DSC_X_MAX,
};

struct pv_disp2d {
    int32_t pv_id;
    int32_t title_start_2d_field;
    int32_t title_end_2d_field;
    int32_t title_start_2d_low_field;
    int32_t title_end_2d_low_field;
    int32_t title_start_3d_field;
    int32_t title_end_3d_field;
    bool target_shadow_type;
    int32_t pv_spr_set_id;
    int32_t pv_aet_set_id;
    int32_t pv_aet_id;
    prj::string title_2d_layer;
};

static_assert(sizeof(pv_disp2d) == 0x50, "\"pv_disp2d\" struct should have a size of 0x50");

struct pv_game_item_mask {
    bool arr[4];
};

static_assert(sizeof(pv_game_item_mask) == 0x04, "\"pv_game_item_mask\" struct should have a size of 0x04");

struct pv_game_init_data {
    int32_t pv_id;
    int32_t difficulty;
    int32_t edition;
    int32_t score_percentage_clear;
    int32_t life_gauge_safety_time;
    int32_t life_gauge_border;
    int32_t stage_index;
    int32_t modules[6];
    rob_chara_pv_data_item items[6];
    pv_game_item_mask items_mask[6];
};

static_assert(sizeof(pv_game_init_data) == 0xAC, "\"pv_game_init_data\" struct should have a size of 0xAC");

class TaskPvGame : public app::Task {
public:
    struct Args {
        pv_game_init_data init_data;
        prj::string se_name;
        prj::string slide_se_name;
        prj::string chainslide_first_name;
        prj::string chainslide_sub_name;
        prj::string chainslide_success_name;
        prj::string chainslide_failure_name;
        prj::string slidertouch_name;
        bool field_190;
        bool watch;
        bool no_fail;
        bool disp_lyrics;
        bool field_194;
        bool mute;
        bool ex_stage;
        bool success;
        bool test_pv;
        int32_t option;
    };

    static_assert(sizeof(TaskPvGame::Args) == 0x1A0, "\"TaskPvGame::Args\" struct should have a size of 0x1A0");

    struct Data {
        int32_t type;
        pv_game_init_data init_data;
        prj::string se_name;
        prj::string slide_se_name;
        prj::string chainslide_first_name;
        prj::string chainslide_sub_name;
        prj::string chainslide_success_name;
        prj::string chainslide_failure_name;
        prj::string slidertouch_name;
        bool field_190;
        bool music_play;
        bool no_fail;
        bool disp_lyrics;
        bool mute;
        bool ex_stage;
        bool success;
        int32_t option;
    };

    static_assert(sizeof(TaskPvGame::Data) == 0x1A0, "\"TaskPvGame::Data\" struct should have a size of 0x1A0");

    TaskPvGame::Data data;
};

static_assert(sizeof(TaskPvGame) == 0x208, "\"TaskPvGame\" struct should have a size of 0x208");

struct pv_game_parent {
    int8_t field_0;
    int8_t field_1;
    uint8_t pv_state;
    bool playing;
    int8_t field_4;
    int8_t field_5;
    int8_t field_6;
    int8_t field_7;
    int64_t curr_time;
    float_t delta_time;
    int32_t state;
    void(FASTCALL* update_func)(pv_game_parent*);
    int8_t field_20;
    int8_t field_21;
    int8_t field_22;
    int8_t field_23;
    int8_t field_24;
    int8_t field_25;
    int8_t field_26;
    int8_t field_27;
    bool init_time;
};

static_assert(sizeof(pv_game_parent) == 0x30, "\"pv_game_parent\" struct should have a size of 0x30");

struct x_pv_bar_beat_data {
    int32_t bar;
    float_t bar_time;
    int32_t beat_count;
    int32_t beat_counter;
    float_t beat_time[16];

    bool compare_bar_time_less(float_t time);
};

struct x_pv_bar_beat {
    float_t curr_time;
    float_t delta_time;
    float_t next_bar_time;
    float_t curr_bar_time;
    float_t next_beat_time;
    float_t curr_beat_time;
    float_t divisor;
    int32_t bar;
    int32_t counter;
    prj::vector<x_pv_bar_beat_data> data;

    x_pv_bar_beat();
    ~x_pv_bar_beat();

    float_t get_bar_current_frame();
    float_t get_delta_frame();
    int32_t get_bar_beat_from_time(int32_t* beat, float_t time);
    float_t get_next_bar_time(float_t time);
    float_t get_next_beat_time(float_t time);
    float_t get_time_from_bar_beat(int32_t bar, int32_t beat);
    void reset();
    void reset_time();
    void set_time(float_t curr_time, float_t delta_time);
};

class BPMFrameRateControl : public FrameRateControl {
public:
    x_pv_bar_beat* bar_beat;

    BPMFrameRateControl();
    virtual ~BPMFrameRateControl() override;

    virtual float_t get_delta_frame() override;

    void reset();
};

class PVStageFrameRateControl : public FrameRateControl {
public:
    float_t delta_frame;

    PVStageFrameRateControl();
    virtual ~PVStageFrameRateControl() override;

    virtual float_t get_delta_frame() override;
};

struct x_pv_game_song_effect_auth_3d {
    auth_3d_id id;
    const char* name;

    x_pv_game_song_effect_auth_3d();
    x_pv_game_song_effect_auth_3d(auth_3d_id id, const char* name);

    void reset();
};

struct x_pv_game_song_effect_glitter {
    const char* name;
    uint32_t hash;
    Glitter::SceneCounter scene_counter;
    bool field_8;
    bool field_9[ROB_CHARA_COUNT];

    x_pv_game_song_effect_glitter();

    void reset();
};

struct x_pv_game_song_effect {
    bool enable;
    int32_t chara_id;
    int64_t time;
    prj::vector<x_pv_game_song_effect_auth_3d> auth_3d;
    prj::vector<x_pv_game_song_effect_glitter> glitter;

    x_pv_game_song_effect();
    ~x_pv_game_song_effect();

    void reset();
};

struct x_pv_game_chara_effect_auth_3d {
    bool field_0;
    chara_index src_chara;
    chara_index dst_chara;
    prj::string file;
    prj::string category;
    int32_t object_set;
    auth_3d_id id;
    int64_t time;
    prj::string node_name;
    int32_t objhrc_index;
    mat4* node_mat;

    x_pv_game_chara_effect_auth_3d();
    ~x_pv_game_chara_effect_auth_3d();

    void reset();
};

struct x_pv_game_chara_effect {
    int32_t state;
    const pvpp* play_param;
    prj::vector<int64_t>* change_fields;
    prj::vector<x_pv_game_chara_effect_auth_3d> auth_3d[ROB_CHARA_COUNT];

    x_pv_game_chara_effect();
    ~x_pv_game_chara_effect();

    void ctrl();
    void load(int32_t pv_id, const pvpp* play_param, chara_index charas[6]);
    void load_data();
    void reset();
    void set_chara_effect(int32_t chara_id, int32_t index, int64_t time);
    void set_chara_effect_time(int32_t chara_id, int32_t index, int64_t time);
    void set_time(int64_t time);
    void stop();
    void stop_chara_effect(int32_t chara_id, int32_t index);
    void unload();
};

struct x_pv_game_effect {
    int32_t flags;
    int32_t state;
    const pvpp* play_param;
    prj::vector<int64_t>* change_fields;
    prj::vector<int32_t> pv_obj_set;
    prj::vector<prj::string> pv_auth_3d;
    prj::vector<uint32_t> pv_glitter;
    prj::vector<x_pv_game_song_effect> song_effect;

    x_pv_game_effect();
    ~x_pv_game_effect();

    void ctrl();
    void load(int32_t pv_id, const pvpp* play_param);
    void load_data(int32_t pv_id);
    void reset();
    void set_chara_id(int32_t index, int32_t chara_id, bool chara_scale);
    void set_song_effect(int32_t index, int64_t time);
    void set_song_effect_time(int32_t index, int64_t time, bool glitter);
    void set_song_effect_time_inner(int32_t index, int64_t time, bool glitter);
    void set_time(int64_t time, bool glitter);
    void stop();
    void stop_song_effect(int32_t index, bool free_glitter);
    void unload();
};

struct aet_obj_data_comp {
    int32_t aet_id;
    char layer_name[0x20];
    float_t frame;
    AetComp comp;

    aet_obj_data_comp();
    ~aet_obj_data_comp();

    const aet_layout_data* find_layout(const char* name);
    bool init_comp(int32_t aet_id, const std::string&& layer_name, float_t frame);
};

struct aet_obj_data {
    int32_t aet_id;
    std::string layer_name;
    aet_obj_data_comp* comp;
    uint32_t id;
    bool loop;
    bool hidden;
    bool field_2A;
    FrameRateControl* frame_rate_control;

    aet_obj_data();
    ~aet_obj_data();

    bool check_disp();
    const aet_layout_data* find_aet_layout(const char* name);
    uint32_t init(AetArgs& args);
    void reset();
};

struct x_pv_aet_disp_data {
    rectangle rect;
    bool disp;
    uint8_t opacity;

    x_pv_aet_disp_data();
};

struct x_pv_aet_disp_string {
    x_pv_aet_disp_data data;
    std::string str;

    x_pv_aet_disp_string();
    ~x_pv_aet_disp_string();

    void ctrl(const char* name, aet_obj_data* aet);
};

struct x_pv_str_array_pv {
    int32_t pv_id;
    const char* song_name;

    const char* get_song_name(int32_t index);
    const char* get_song_line_1(int32_t index);
    const char* get_song_line_2(int32_t index);
    const char* get_song_line_3(int32_t index);
};

struct x_pv_game_title {
    int32_t state;
    int32_t aet_set_id;
    int32_t spr_set_id;
    int32_t aet_id;
    aet_obj_data tit_layer;
    aet_obj_data txt_layer;
    x_pv_str_array_pv str_array;
    x_pv_aet_disp_string txt_data[4];
    int32_t txt_layer_index;

    x_pv_game_title();
    ~x_pv_game_title();

    void ctrl();
    void ctrl_txt_data();
    void disp();
    bool get_id(int8_t cloud_index, int32_t& aet_id);
    bool get_set_ids(int8_t cloud_index, int32_t& aet_set_id, int32_t& spr_set_id);
    void load(int32_t pv_id);
    void load_data();
    void reset();
    void show(int32_t index);
    void unload();
};

struct x_pv_game_pv_data {
    bool play;
    p_file_handler dsc_file_handler;
    dsc dsc;
    dsc_data* dsc_data_ptr;
    dsc_data* dsc_data_ptr_end;
    class TaskPvGameX* pv_game;
    int64_t dsc_time;
    bool pv_end;

    x_pv_game_pv_data();
    ~x_pv_game_pv_data();

    void ctrl(float_t delta_time, int64_t curr_time);
    bool dsc_ctrl(float_t delta_time, int64_t curr_time,
        float_t* dsc_time_offset);
    dsc_data* find(int32_t func_name, int32_t* time,
        int32_t* pv_branch_mode, dsc_data* start, dsc_data* end);
    void find_bar_beat(x_pv_bar_beat& bar_beat);
    void find_change_fields(prj::vector<int64_t>& change_fields);
    void find_stage_effects(prj::vector<prj::pair<int64_t, int32_t>>& stage_effects);
    void init(class TaskPvGameX* pv_game);
    void reset();
    void stop();
    void unload();
};

struct x_pv_game_stage;

struct x_pv_game_data {
    int32_t pv_id;
    p_file_handler play_param_file_handler;
    const pvpp* play_param;
    uint16_t field_1C;
    int32_t state;
    prj::vector<int64_t> change_fields;
    x_pv_bar_beat bar_beat;
    int32_t stage_effect_index;
    int32_t next_stage_effect_bar;
    prj::vector<prj::pair<int64_t, int32_t>> stage_effects;
    x_pv_game_effect effect;
    x_pv_game_chara_effect chara_effect;
    x_pv_game_title title;
    x_pv_game_pv_data pv_data;
    x_pv_game_stage* stage;

    x_pv_game_data();
    ~x_pv_game_data();

    void ctrl(int64_t curr_time, float_t delta_time);
    void ctrl_stage_effect_index();
    void disp();
    void load(int32_t pv_id, chara_index charas[6]);
    void reset();
    void stop();
    void unload();
    void unload_if_state_is_0();
};

struct x_pv_game_stage_env_data {
    auth_3d_id light_auth_3d_id;
    aet_obj_data data[8];

    x_pv_game_stage_env_data();
};

struct x_pv_game_stage_env_aet {
    int32_t state;
    float_t duration;
    aet_obj_data* prev[8];
    aet_obj_data* next[8];

    x_pv_game_stage_env_aet();
};

struct x_pv_game_stage_env {
    int32_t flags;
    int32_t pv_id;
    int32_t state;
    const pvsr* stage_resource;
    int32_t env_index;
    x_pv_game_stage_env_data data[64];
    PVStageFrameRateControl frame_rate_control;
    float_t trans_duration;
    float_t trans_remain;
    x_pv_game_stage_env_aet aet;

    x_pv_game_stage_env();
    ~x_pv_game_stage_env();

    void ctrl(float_t delta_time);
    const pvsr_auth_2d* get_aet(int32_t env_index, int32_t index);
    void load(int32_t pv_id, const pvsr* stage_resource);
    void reset();
    void reset_env();
    void set(int32_t env_index, float_t trans_time, float_t a4);
    void unload();

    void sub_810EE03E();
    bool sub_810EE198(float_t delta_time);
};

struct x_pv_game_stage_effect_auth_3d {
    bool repeat;
    bool field_1;
    auth_3d_id id;
    const char* name;

    x_pv_game_stage_effect_auth_3d();

    void reset();
};

struct x_pv_game_stage_effect_glitter {
    const char* name;
    uint32_t hash;
    Glitter::SceneCounter scene_counter;
    float_t fade_time;
    float_t fade_time_left;
    bool force_disp;

    x_pv_game_stage_effect_glitter();

    void reset();
};

struct x_pv_game_stage_change_effect {
    bool enable;
    prj::vector<x_pv_game_stage_effect_auth_3d> auth_3d;
    prj::vector<x_pv_game_stage_effect_glitter> glitter;
    int16_t bars_left;
    int16_t bar_count;

    x_pv_game_stage_change_effect();
    ~x_pv_game_stage_change_effect();

    void reset();
};

struct x_pv_game_stage_effect {
    const pvsr_stage_effect* stage_effect;
    prj::vector<x_pv_game_stage_effect_auth_3d> auth_3d;
    prj::vector<x_pv_game_stage_effect_glitter> glitter;
    int32_t main_auth_3d_index;
    bool set;

    x_pv_game_stage_effect();
    ~x_pv_game_stage_effect();

    void reset();
};

#define X_PV_GAME_STAGE_EFFECT_COUNT 12

struct x_pv_game_stage {
    int32_t flags;
    int32_t pv_id;
    int32_t field_8;
    int32_t state;
    //int32_t field_10;
    p_file_handler stage_resource_file_handler;
    const pvsr* stage_resource;
    x_pv_game_stage_env env;
    BPMFrameRateControl bpm_frame_rate_control;
    int32_t objhrc_set;
    prj::vector<prj::string> stage_auth_3d;
    prj::vector<uint32_t> stage_glitter;
    int32_t curr_stage_effect;
    int32_t next_stage_effect;
    int32_t stage_effect_transition_state;
    x_pv_game_stage_effect effect[X_PV_GAME_STAGE_EFFECT_COUNT];
    x_pv_game_stage_change_effect change_effect
        [X_PV_GAME_STAGE_EFFECT_COUNT][X_PV_GAME_STAGE_EFFECT_COUNT];

    prj::map<int32_t, auth_3d_id> auth_3d_ids;

    x_pv_game_stage();
    ~x_pv_game_stage();

    bool check_stage_effect_has_change_effect(int32_t curr_stage_effect, int32_t next_stage_effect);
    void ctrl(float_t delta_time);
    void ctrl_inner();
    void load(int32_t pv_id, bool a4);
    void load_change_effect(int32_t curr_stage_effect, int32_t next_stage_effect);
    void reset();
    void reset_stage_effect();
    void reset_stage_env();
    bool set_change_effect_frame_part_1();
    void set_change_effect_frame_part_2(float_t frame);
    void set_env(int32_t env_index, float_t end_time, float_t start_time);
    void set_stage_effect(int32_t stage_effect);
    void set_stage_effect_auth_3d_frame(int32_t stage_effect, float_t frame);
    void set_stage_effect_glitter_frame(int32_t stage_effect, float_t frame);
    void stop_stage_change_effect();
    void stop_stage_effect();
    void stop_stage_effect_auth_3d(int32_t stage_effect);
    void stop_stage_effect_glitter(int32_t stage_effect);
    void unload();
};

class TaskPvGameX : public app::Task {
public:
    bool use;
    int32_t state;
    x_pv_game_data data;
    x_pv_game_stage stage_data;

    int32_t state_old;

    int32_t pv_id;
    chara_index charas[6];

    TaskPvGameX();
    virtual ~TaskPvGameX() override;

    virtual bool init() override;
    virtual bool ctrl() override;
    virtual bool dest() override;
    virtual void disp() override;

    void load(int32_t pv_id, int32_t modules[6]);
    void unload();

    void ctrl(int64_t curr_time, float_t delta_time);
    void set_song_effect_alpha_obj_flags(int32_t chara_id, int32_t type, float_t alpha);
    void stop_current_pv();
};

struct x_pv_game_str_array {
    prj::unordered_map<int32_t, prj::unordered_map<int32_t, const char*>> strings;
    p_file_handler file_handler;
    int32_t lang_sel;
    bool init;

    x_pv_game_str_array();
    ~x_pv_game_str_array();

    const char* get_string(int32_t id);
    void read();
};

TaskPvGame& task_pv_game = *(TaskPvGame*)0x000000014CC95270;
TaskPvGameX* task_pv_game_x;
x_pv_game_str_array* x_pv_game_str_array_ptr;
pv_disp2d& pv_disp2d_data = *(pv_disp2d*)0x0000000140C938E0;
size_t pv_game_ptr = 0x0000000140CDD8D0;
pv_game_parent& pv_game_parent_data = *(pv_game_parent*)0x0000000140D0B510;

void pv_game_load_state_4_tail_impl(size_t pv_game) {
    if (task_pv_game_x->use) {
        bool wait_load = false;
        wait_load |= task_pv_game_x->data.effect.state < 12;
        wait_load |= task_pv_game_x->stage_data.state < 11;
        if (wait_load)
            return;
    }

    *(uint32_t*)(pv_game + 0x08) = 5;
}

bool pv_game_load_state_6_head_impl() {
    if (task_pv_game_x->use) {
        int32_t objhrc_set = task_pv_game_x->stage_data.objhrc_set;
        if (objhrc_set != -1 && objset_info_storage_load_obj_set_check_not_read(objhrc_set))
            return true;
    }
    return false;
}

bool pv_game_load_state_9_mid_impl() {
    return task_pv_game_x->use ? task_pv_game_x->data.effect.state < 11 : false;
}

void pv_game_load_state_13_tail_impl(size_t pv_game) {
    if (task_pv_game_x->use) {
        bool wait_load = false;
        wait_load |= task_pv_game_x->data.effect.state < 12;
        wait_load |= task_pv_game_x->stage_data.state < 11;
        if (wait_load)
            return;
    }

    *(uint32_t*)(pv_game + 0x08) = 14;
}

HOOK(void, FASTCALL, pv_game_load_state_4_tail, 0x00000001400FF55B);
HOOK(void, FASTCALL, pv_game_load_state_6_head, 0x000000001400FF5ED);
HOOK(void, FASTCALL, pv_game_load_state_9_mid, 0x000000014010280D);
HOOK(void, FASTCALL, pv_game_load_state_13_tail, 0x000000014010386E);

HOOK(int64_t, FASTCALL, pv_game_pv_data__ctrl, 0x00000001401230E0,
    size_t _this, float_t delta_time, int64_t curr_time, bool a4) {
    int64_t ret = originalpv_game_pv_data__ctrl(_this, delta_time, curr_time, a4);
    if (task_pv_game_x->use) {
        int64_t curr_time = *(int64_t*)(_this + 0x2BF40);
        float_t curr_time_float = *(float_t*)(_this + 0x2BF48);

        x_pv_game_data& x_pv_data = task_pv_game_x->data;
        x_pv_data.ctrl_stage_effect_index();
        x_pv_data.bar_beat.set_time(curr_time_float, delta_time);
        x_pv_data.pv_data.ctrl(delta_time, curr_time);
    }
    return ret;
}

HOOK(bool, FASTCALL, task_pv_game_del_task, 0x0000000140674E90) {
    bool ret = originaltask_pv_game_del_task();
    if (!app::TaskWork::check_task_ready(task_pv_game_x))
        return ret;

    task_pv_game_x->del();
    return false;
}

HOOK(bool, FASTCALL, task_pv_game_add_task, 0x0000000140675030, TaskPvGame::Args* args) {
    bool ret = originaltask_pv_game_add_task(args);

    pv_game_init_data& init_data = task_pv_game.data.init_data;
    if (init_data.pv_id < 800 || init_data.pv_id >= 832)
        return ret;

    app::TaskWork::add_task(task_pv_game_x, 0, "PVGAME X", 0);
    task_pv_game_x->load(init_data.pv_id, init_data.modules);
    return ret;
}

HOOK(bool, FASTCALL, task_pv_game_init_demo_pv, 0x0000000140675470, int32_t pv_id, int32_t difficulty, bool music_play) {
    bool ret = originaltask_pv_game_init_demo_pv(pv_id, difficulty, music_play);

    pv_game_init_data& init_data = task_pv_game.data.init_data;
    if (init_data.pv_id < 800 || init_data.pv_id >= 832)
        return ret;

    app::TaskWork::add_task(task_pv_game_x, 0, "PVGAME X", 0);
    task_pv_game_x->load(init_data.pv_id, init_data.modules);
    return ret;
}

HOOK(void, FASTCALL, pv_game__set_data_itmpv_alpha_obj_flags, 0x0000000140115EC0,
    size_t pv_game, int32_t chara_id, int32_t type, float_t alpha) {
    originalpv_game__set_data_itmpv_alpha_obj_flags(pv_game, chara_id, type, alpha);
    if (task_pv_game_x->use)
        task_pv_game_x->set_song_effect_alpha_obj_flags(chara_id, type, alpha);
}

HOOK(void, FASTCALL, pv_game_pv_data__dsc_reset_position, 0x000000014011CB40, size_t _this) {
    originalpv_game_pv_data__dsc_reset_position(_this);

    if (task_pv_game_x->use) {
        x_pv_game_pv_data& pv_data = task_pv_game_x->data.pv_data;
        if (pv_data.dsc.data.size()) {
            pv_data.dsc_data_ptr = pv_data.dsc.data.data();
            pv_data.dsc_data_ptr_end = pv_data.dsc_data_ptr + pv_data.dsc.data.size();
        }
        else {
            pv_data.dsc_data_ptr = 0;
            pv_data.dsc_data_ptr_end = 0;
        }
    }
}

HOOK(bool, FASTCALL, pv_game_pv_data__load, 0x000000014011B7E0, size_t _this,
    prj::string&& file_path, size_t pv_game, bool music_play) {
    p_file_handler& dsc_file_handler = *(p_file_handler*)(_this + 0x2BF90);
    int32_t& dsc_state = *(int32_t*)(_this + 0x04);

    switch (dsc_state) {
    case 0:
        if (!dsc_file_handler.read_file_path(file_path.c_str(), prj::HeapCMallocTemp))
            break;

        if (task_pv_game_x->use) {
            bool load_x = false;
            char buf[0x200];
            sprintf_s(buf, sizeof(buf), "rom/pv_script/pv_%03d.dsc", task_pv_game_x->data.pv_id);
            if (task_pv_game_x->data.pv_data.dsc_file_handler.read_file_path(buf, prj::HeapCMallocSystem)) {
                sprintf_s(buf, sizeof(buf), "rom/pv/pv%03d.pvpp", task_pv_game_x->data.pv_id);
                if (task_pv_game_x->data.play_param_file_handler.read_file_path(buf, prj::HeapCMallocSystem)) {
                    sprintf_s(buf, sizeof(buf), "rom/pv_stage_rsrc/stgpv%03d_param.pvsr", task_pv_game_x->stage_data.pv_id);
                    if (task_pv_game_x->stage_data.stage_resource_file_handler.read_file_path(buf, prj::HeapCMallocTemp))
                        load_x = true;
                }
            }

            if (!load_x)
                task_pv_game_x->del();
        }

        dsc_state = 1;
        break;
    case 1: {
        if (dsc_file_handler.check_not_ready())
            break;

        if (task_pv_game_x->use)
            if (task_pv_game_x->data.pv_data.dsc_file_handler.check_not_ready()
                || task_pv_game_x->data.play_param_file_handler.check_not_ready()
                || task_pv_game_x->stage_data.stage_resource_file_handler.check_not_ready())
                break;

        memmove((int32_t*)(_this + 0x0C), dsc_file_handler.get_data(),
            min_def(dsc_file_handler.get_size(), 45000 * sizeof(int32_t)));
        dsc_file_handler.reset();

        static const char* (FASTCALL * pv_game_pv_data__dsc_process)(size_t _this, size_t pv_game)
            = (const char* (FASTCALL*)(size_t _this, size_t pv_game))0x000000014011C3A0;

        if (task_pv_game_x->use) {
            task_pv_game_x->data.pv_data.dsc.parse(
                task_pv_game_x->data.pv_data.dsc_file_handler.get_data(),
                task_pv_game_x->data.pv_data.dsc_file_handler.get_size());
            task_pv_game_x->data.pv_data.dsc_file_handler.reset();

            task_pv_game_x->data.play_param = pvpp::read(
                task_pv_game_x->data.play_param_file_handler.get_data(),
                task_pv_game_x->data.play_param_file_handler.get_size());
            task_pv_game_x->stage_data.stage_resource = pvsr::read(
                task_pv_game_x->stage_data.stage_resource_file_handler.get_data(),
                task_pv_game_x->stage_data.stage_resource_file_handler.get_size());
        }

        pv_game_pv_data__dsc_process(_this, pv_game);
        dsc_state = 2;
    } break;
    case 2: {
        static void (FASTCALL * pv_game_pv_data__init)(size_t _this, size_t pv_game, bool music_play)
            = (void (FASTCALL*)(size_t _this, size_t pv_game, bool music_play))0x000000014011B2B0;

        pv_game_pv_data__init(_this, pv_game, music_play);
        dsc_state = 0;
    } return true;
    }
    return false;
}

void pv_game_patch() {
    INSTALL_HOOK(pv_game_load_state_4_tail);
    INSTALL_HOOK(pv_game_load_state_6_head);
    INSTALL_HOOK(pv_game_load_state_9_mid);
    INSTALL_HOOK(pv_game_load_state_13_tail);
    INSTALL_HOOK(pv_game_pv_data__ctrl);
    INSTALL_HOOK(task_pv_game_del_task);
    INSTALL_HOOK(task_pv_game_add_task);
    INSTALL_HOOK(task_pv_game_init_demo_pv);
    INSTALL_HOOK(pv_game__set_data_itmpv_alpha_obj_flags);
    INSTALL_HOOK(pv_game_pv_data__dsc_reset_position);
    INSTALL_HOOK(pv_game_pv_data__load);

    WRITE_MEMORY_STRING(0x00000001401027D8, "\x41\x0F\x45\xDF\x75\x2F", 0x06);

    task_pv_game_x = new (_operator_new(sizeof(TaskPvGameX))) TaskPvGameX;
    x_pv_game_str_array_ptr = new (_operator_new(sizeof(x_pv_game_str_array))) x_pv_game_str_array;
}

bool x_pv_bar_beat_data::compare_bar_time_less(float_t time) {
    return (time + 0.0001f) < bar_time;
}

x_pv_bar_beat::x_pv_bar_beat() : curr_time(), delta_time(), next_bar_time(),
curr_bar_time(), next_beat_time(), curr_beat_time(), bar(), counter() {
    reset();
}

x_pv_bar_beat::~x_pv_bar_beat() {

}

float_t x_pv_bar_beat::get_bar_current_frame() {
    return (curr_time - curr_bar_time) / (next_bar_time - curr_bar_time) * 120.0f;
}

float_t x_pv_bar_beat::get_delta_frame() {
    return delta_time / (next_bar_time - curr_bar_time) * 120.0f;
}

int32_t x_pv_bar_beat::get_bar_beat_from_time(int32_t* beat, float_t time) {
    if (!data.size()) {
        int32_t bar = (int32_t)(float_t)(time / divisor);
        if (beat)
            *beat = (int32_t)(4.0f * (time - (float_t)(int32_t)(time / divisor) * divisor) / divisor) + 1;
        return bar + 1;
    }

    x_pv_bar_beat_data* d = data.data();
    const size_t size = data.size();

    size_t length = size;
    size_t temp;
    while (length > 0)
        if (d[temp = length / 2].compare_bar_time_less(time))
            length = temp;
        else {
            d += temp + 1;
            length -= temp + 1;
        }

    if (d == data.data() + size) {
        float_t divisor = this->divisor;
        float_t diff_time = time - data.back().bar_time;

        if (size > 1)
            divisor = data[size - 1].bar_time - data[size - 2].bar_time;

        if (beat)
            *beat = (int32_t)((float_t)data.back().beat_count * (diff_time
                - (float_t)(int32_t)(diff_time / divisor) * divisor) / divisor) + 1;
        return (int32_t)(diff_time / divisor) + data.back().bar;
    }

    int32_t bar = d->bar - 1;
    if (beat) {
        *beat = 1;
        if (d == data.data())
            *beat = (int32_t)((float_t)d->beat_count * time / d->bar_time) + 1;
        else {
            int32_t v18 = d[-1].beat_count - 1;
            if (v18 >= 0) {
                float_t* v19 = &d[-1].beat_time[v18];
                for (; *v19 > time; v19--)
                    if (--v18 < 0)
                        return bar;
                *beat = v18 + 1;
            }
        }
    }
    return bar;
}

float_t x_pv_bar_beat::get_next_bar_time(float_t time) {
    x_pv_bar_beat_data* d = data.data();
    const size_t size = data.size();

    size_t length = size;
    size_t temp;
    while (length > 0)
        if (d[temp = length / 2].compare_bar_time_less(time))
            length = temp;
        else {
            d += temp + 1;
            length -= temp + 1;
        }

    if (d == data.data() + data.size()) {
        float_t v11 = 0.0f;
        float_t v12 = divisor;
        if (data.size())
            v11 = data.back().bar_time;

        float_t bar_time = (float_t)((int32_t)((time - v11) / v12) + 1) * v12 + v11;
        if (bar_time < time + 0.00001f)
            bar_time += v12;
        return bar_time;
    }
    else
        return d->bar_time;
}

float_t x_pv_bar_beat::get_next_beat_time(float_t time) {
    x_pv_bar_beat_data* d = data.data();
    const size_t size = data.size();

    size_t length = size;
    size_t temp;
    while (length > 0)
        if (d[temp = length / 2].compare_bar_time_less(time))
            length = temp;
        else {
            d += temp + 1;
            length -= temp + 1;
        }

    if (d == data.data() + size) {
        float_t v12 = 0.0f;
        float_t v13 = divisor * 0.25f;
        if (size) {
            v12 = data[size - 1].bar_time;
            v13 = v12 - data[size - 2].beat_time[data[size - 2].beat_count - 1];
        }

        float_t beat_time = (float_t)((int32_t)((time - v12) / v13) + 1) * v13 + v12;
        if (beat_time < time + 0.00001f)
            beat_time += v13;
        return beat_time;
    }
    else if (d != data.data()) {
        int32_t v9 = d[-1].beat_count - 1;
        if (v9 >= 0) {
            float_t* beat_time = &d[-1].beat_time[v9];
            for (; time >= *beat_time; beat_time--)
                if (--v9 < 0)
                    return d->bar_time;
            return *beat_time;
        }
    }
    return d->bar_time;
}

float_t x_pv_bar_beat::get_time_from_bar_beat(int32_t bar, int32_t beat) {
    if (bar < 0)
        return 0.0f;

    x_pv_bar_beat_data* d = data.data();
    const size_t size = data.size();

    int32_t beat_count;
    if (size) {
        int32_t bar_diff = bar - d->bar;
        if (bar_diff < 0)
            beat_count = d->beat_count;
        else if (size > bar_diff)
            beat_count = d[bar_diff].beat_count;
        else
            beat_count = d[size - 1].beat_count;

        if (!beat_count)
            beat_count = 4;
    }
    else
        beat_count = 4;

    beat--;
    if (!size)
        return (float_t)beat * divisor / (float_t)beat_count + (bar - 1) * divisor;

    int32_t bar_diff = bar - d->bar;
    if (bar_diff < 0)
        return 0.0f;

    if (size <= bar_diff) {
        float_t divisor = this->divisor;
        if (size > 1)
            divisor = d[size - 1].bar_time - d[size - 2].bar_time;
        return  (float_t)beat * divisor / (float_t)beat_count
            + (bar - d[size - 1].bar) * divisor + d[size - 1].bar_time;
    }
    else if (beat < 0 || beat >= d[bar_diff].beat_count)
        return d[bar_diff].bar_time;
    else
        return d[bar_diff].beat_time[beat];
}

void x_pv_bar_beat::reset() {
    curr_time = 0.0f;
    delta_time = 0.0f;
    next_bar_time = 0.0f;
    curr_bar_time = 0.0f;
    next_beat_time = 0.0f;
    curr_beat_time = 0.0f;
    divisor = 2.0f;
    bar = 0;
    data.clear();
    data.shrink_to_fit();
}

void x_pv_bar_beat::reset_time() {
    curr_time = 0.0f;
    delta_time = 0.0f;
    curr_bar_time = 0.0f;
    next_bar_time = get_next_bar_time(0.0f);
    curr_beat_time = 0.0f;
    next_beat_time = get_next_beat_time(0.0f);
    bar = 1;
    counter = 0;
}

void x_pv_bar_beat::set_time(float_t curr_time, float_t delta_time) {
    if (next_bar_time <= 0.0f)
        return;

    this->curr_time = curr_time;
    this->delta_time = delta_time;
    counter = 0;

    while (curr_time >= next_bar_time) {
        curr_bar_time = next_bar_time;
        bar++;
        counter++;
        next_bar_time = get_next_bar_time(next_bar_time);
    }

    while (curr_time >= next_beat_time) {
        curr_beat_time = next_beat_time;
        next_beat_time = get_next_beat_time(next_beat_time);
    }
}

BPMFrameRateControl::BPMFrameRateControl() : bar_beat() {

}

BPMFrameRateControl::~BPMFrameRateControl() {

}

float_t BPMFrameRateControl::get_delta_frame() {
    if (bar_beat)
        return bar_beat->get_delta_frame();
    return 0.0f;
}

void BPMFrameRateControl::reset() {
    bar_beat = 0;
    set_frame_speed(1.0f);
}

PVStageFrameRateControl::PVStageFrameRateControl() {
    delta_frame = 1.0f;
}

PVStageFrameRateControl::~PVStageFrameRateControl() {

}

float_t PVStageFrameRateControl::get_delta_frame() {
    return delta_frame;
}

x_pv_game_song_effect_auth_3d::x_pv_game_song_effect_auth_3d() : name() {

}

x_pv_game_song_effect_auth_3d::x_pv_game_song_effect_auth_3d(auth_3d_id id, const char* name) {
    this->id = id;
    this->name = name;
}

void x_pv_game_song_effect_auth_3d::reset() {
    id = {};
    name = 0;
}

x_pv_game_song_effect_glitter::x_pv_game_song_effect_glitter()
    : name(), hash(), scene_counter(), field_8(), field_9() {
    reset();
}

void x_pv_game_song_effect_glitter::reset() {
    name = 0;
    hash = hash_murmurhash_empty;
    scene_counter = 0;
    field_8 = false;
    for (bool& i : field_9)
        i = false;
}

x_pv_game_song_effect::x_pv_game_song_effect() : enable(), chara_id(), time() {
    reset();
}

x_pv_game_song_effect::~x_pv_game_song_effect() {

}

void x_pv_game_song_effect::reset() {
    enable = false;
    chara_id = -1;
    time = -1;
    auth_3d.clear();
    auth_3d.shrink_to_fit();
    glitter.clear();
    glitter.shrink_to_fit();
}

x_pv_game_chara_effect_auth_3d::x_pv_game_chara_effect_auth_3d()
    : field_0(), src_chara(), dst_chara(), objhrc_index(), node_mat() {
    id = {};
    time = -1;
    reset();
}

x_pv_game_chara_effect_auth_3d::~x_pv_game_chara_effect_auth_3d() {

}

void x_pv_game_chara_effect_auth_3d::reset() {
    field_0 = false;
    src_chara = CHARA_MAX;
    dst_chara = CHARA_MAX;
    file.clear();
    category.clear();
    object_set = -1;
    id = {};
    time = -1;
    node_name.clear();
    objhrc_index = -1;
    node_mat = 0;
}

x_pv_game_chara_effect::x_pv_game_chara_effect() : state(), change_fields() {
    reset();
}

x_pv_game_chara_effect::~x_pv_game_chara_effect() {

}

void x_pv_game_chara_effect::ctrl() {
    switch (state) {
    case 20: {
        bool wait_load = false;

        for (prj::vector<x_pv_game_chara_effect_auth_3d>& i : auth_3d)
            for (x_pv_game_chara_effect_auth_3d j : i) {
                if (!j.category.size())
                    continue;

                if (!auth_3d_data_check_category_loaded(j.category.c_str()))
                    wait_load |= true;

                if (objset_info_storage_load_obj_set_check_not_read(j.object_set))
                    wait_load |= true;
            }

        if (wait_load)
            break;

        for (prj::vector<x_pv_game_chara_effect_auth_3d>& i : auth_3d)
            for (x_pv_game_chara_effect_auth_3d& j : i) {
                if (!j.category.size())
                    continue;

                auth_3d_id id = auth_3d_id(j.file.c_str());
                if (!id.check_not_empty()) {
                    j.id = {};
                    j.file.clear();
                    j.category.clear();
                    continue;
                }

                id.read_file();
                id.set_enable(false);
                id.set_repeat(false);
                id.set_paused(true);
                id.set_visibility(false);

                if (j.field_0)
                    id.set_src_dst_chara(j.src_chara, j.dst_chara);

                j.id = id;
            }

        state = 21;
    } break;
    case 21: {
        bool wait_load = false;

        for (prj::vector<x_pv_game_chara_effect_auth_3d>& i : auth_3d)
            for (x_pv_game_chara_effect_auth_3d j : i) {
                if (!j.category.size())
                    continue;

                if (j.id.check_not_empty() && !j.id.check_loaded())
                    wait_load |= true;
            }

        if (!wait_load)
            state = 30;
    } break;
    }
}

void x_pv_game_chara_effect::load(int32_t pv_id, const pvpp* play_param, chara_index charas[6]) {
    if (state)
        return;

    this->play_param = play_param;

    int32_t num_chara = play_param->num_chara;
    for (int32_t i = 0; i < num_chara && i < ROB_CHARA_COUNT; i++) {
        const pvpp_chara_effect* chara_effect = play_param->chara_array[i].chara_effect;
        if (!chara_effect)
            continue;

        chara_index src_chara = (chara_index)chara_effect->base_chara;
        chara_index dst_chara = charas[i];
        chara_index dst_chara_mei = dst_chara == CHARA_SAKINE ? CHARA_MEIKO : dst_chara;

        /*if (dst_chara == CHARA_EXTRA)
            dst_chara = src_chara;*/

        const char* src_chara_str = chara_index_get_auth_3d_name(src_chara);
        const char* dst_chara_str = chara_index_get_auth_3d_name(dst_chara);
        const char* dst_chara_mei_str = chara_index_get_auth_3d_name(dst_chara_mei);

        int32_t num_auth_3d = chara_effect->num_auth_3d;
        for (int32_t j = 0; j < num_auth_3d; j++) {
            const pvpp_chara_effect_auth_3d* chr_eff_auth_3d = &chara_effect->auth_3d_array[j];

            prj::string file(chr_eff_auth_3d->auth_3d);
            prj::string object_set;
            if (chr_eff_auth_3d->object_set)
                object_set.assign(chr_eff_auth_3d->object_set);
            else
                object_set.assign(file);

            if (src_chara != dst_chara) {
                if (src_chara_str) {
                    size_t pos = object_set.find(src_chara_str);
                    if (pos != -1)
                        object_set.replace(pos, 3, dst_chara_str);
                    else if (dst_chara_mei_str) {
                        size_t pos = object_set.find(dst_chara_mei_str);
                        if (pos != -1)
                            object_set.replace(pos, 3, src_chara_str);
                    }
                }

                if (!chr_eff_auth_3d->u00)
                    if (src_chara_str) {
                        size_t pos = file.find(src_chara_str);
                        if (pos != -1)
                            file.replace(pos, 3, dst_chara_str);
                        else if (dst_chara_mei_str) {
                            size_t pos = file.find(dst_chara_mei_str);
                            if (pos != -1)
                                file.replace(pos, 3, src_chara_str);
                        }
                    }
            }

            prj::string category;
            if (!file.find("EFFCHRPV")) {
                size_t pos = file.find("_");
                if (pos != -1)
                    category.assign(file.substr(0, pos));
                else
                    category.assign(file);
                category.insert(0, "A3D_");
            }
            else
                category.assign(file);

            this->auth_3d[i].push_back({});
            x_pv_game_chara_effect_auth_3d& auth_3d = this->auth_3d[i].back();
            auth_3d.field_0 = chr_eff_auth_3d->u00;
            auth_3d.src_chara = src_chara;
            auth_3d.dst_chara = dst_chara;
            auth_3d.file.assign(file);
            auth_3d.category.assign(category);
            auth_3d.object_set = object_database_get_object_set_id(object_set.c_str());
        }
    }
    state = 10;
}

void x_pv_game_chara_effect::load_data() {
    if (state != 10)
        return;

    for (prj::vector<x_pv_game_chara_effect_auth_3d>& i : auth_3d)
        for (x_pv_game_chara_effect_auth_3d& j : i) {
            if (!j.category.size())
                continue;

            auth_3d_data_load_category(j.category.c_str());
            objset_info_storage_load_set(j.object_set);
        }

    state = 20;
}

void x_pv_game_chara_effect::reset() {
    state = 0;
    play_param = 0;
    change_fields = 0;

    for (prj::vector<x_pv_game_chara_effect_auth_3d>& i : auth_3d) {
        i.clear();
        i.shrink_to_fit();
    }
}

void x_pv_game_chara_effect::set_chara_effect(int32_t chara_id, int32_t index, int64_t time) {
    if (chara_id < 0 || chara_id >= ROB_CHARA_COUNT)
        return;

    prj::vector<x_pv_game_chara_effect_auth_3d>& auth_3d = this->auth_3d[chara_id];
    if (index < 0 || index >= auth_3d.size())
        return;

    auth_3d_id& id = auth_3d[index].id;

    if (!id.check_not_empty())
        return;

    if (id.get_enable())
        id.set_req_frame((float_t)((double_t)(time - auth_3d[index].time) * 0.000000001) * 60.0f);
    else {
        id.set_enable(true);
        id.set_paused(false);
        id.set_repeat(false);
        id.set_req_frame(0.0f);
        id.set_visibility(true);
        auth_3d[index].time = time;
    }

    float_t max_frame = -1.0f;
    if (change_fields) {
        int64_t* key = change_fields->data();
        size_t length = change_fields->size();
        size_t temp;
        while (length > 0)
            if (key[temp = length / 2] > time)
                length = temp;
            else {
                key += temp + 1;
                length -= temp + 1;
            }

        if (key != change_fields->data() + change_fields->size())
            max_frame = (float_t)((double_t)(*key - auth_3d[index].time) * 0.000000001) * 60.0f - 1.0f;
    }

    id.set_max_frame(max_frame);
}

void x_pv_game_chara_effect::set_chara_effect_time(int32_t chara_id, int32_t index, int64_t time) {
    if (!state || chara_id < 0 || chara_id >= ROB_CHARA_COUNT)
        return;

    for (x_pv_game_chara_effect_auth_3d& i : auth_3d[chara_id]) {
        if (!i.id.check_not_empty() || !i.id.get_enable())
            continue;

        i.id.set_req_frame((float_t)((double_t)(time - i.time) * 0.000000001) * 60.0f);
        i.id.set_paused(false);
    }
}

void x_pv_game_chara_effect::set_time(int64_t time) {
    if (!state)
        return;

    for (auto& i : auth_3d)
        for (x_pv_game_chara_effect_auth_3d& j : i) {
            if (!j.id.check_not_empty() || !j.id.get_enable())
                continue;

            j.id.set_req_frame((float_t)((double_t)(time - j.time) * 0.000000001) * 60.0f);
            j.id.set_paused(false);
        }
}

void x_pv_game_chara_effect::stop() {
    if (!state)
        return;

    for (prj::vector<x_pv_game_chara_effect_auth_3d>& i : auth_3d)
        for (x_pv_game_chara_effect_auth_3d& j : i)
            if (j.id.check_not_empty())
                j.id.set_enable(false);
}

void x_pv_game_chara_effect::stop_chara_effect(int32_t chara_id, int32_t index) {
    if (chara_id < 0 || chara_id >= ROB_CHARA_COUNT)
        return;

    for (x_pv_game_chara_effect_auth_3d& i : auth_3d[chara_id])
        if (i.id.check_not_empty())
            i.id.set_enable(false);
}

void x_pv_game_chara_effect::unload() {
    if (!state)
        return;

    if (state == 10 || state == 30) {
        for (prj::vector<x_pv_game_chara_effect_auth_3d>& i : auth_3d)
            for (x_pv_game_chara_effect_auth_3d& j : i) {
                if (!j.category.size())
                    continue;

                j.id.unload();
                auth_3d_data_unload_category(j.category.c_str());
                objset_info_storage_unload_set(j.object_set);
            }
        state = 10;
    }

    for (prj::vector<x_pv_game_chara_effect_auth_3d>& i : auth_3d) {
        i.clear();
        i.shrink_to_fit();
    }

    play_param = 0;
    state = 0;
}

x_pv_game_effect::x_pv_game_effect() : flags(), state(), play_param() {
    reset();
}

x_pv_game_effect::~x_pv_game_effect() {

}

void x_pv_game_effect::ctrl() {
    switch (state) {
    case 10: {
        bool wait_load = false;

        for (prj::string& i : pv_auth_3d)
            if (!auth_3d_data_check_category_loaded(i.c_str()))
                wait_load |= true;

        for (int32_t& i : pv_obj_set)
            if (objset_info_storage_load_obj_set_check_not_read(i))
                wait_load |= true;

        if (wait_load)
            break;

        const pvpp* play_param = this->play_param;
        int32_t num_effect = play_param->num_effect;

        for (int32_t i = 0; i < num_effect; i++) {
            x_pv_game_song_effect& song_effect = this->song_effect[i];
            const pvpp_effect& effect = play_param->effect_array[i];
            int32_t num_auth_3d = effect.num_auth_3d;
            for (int32_t j = 0; j < num_auth_3d; j++) {
                const char* name = effect.auth_3d_array[j].name;
                auth_3d_id id = auth_3d_id(name);
                if (!id.check_not_empty())
                    continue;

                id.read_file();
                id.set_enable(false);
                id.set_repeat(false);
                id.set_paused(false);
                id.set_visibility(false);
                song_effect.auth_3d.push_back({ id, name });
            }
        }
        state = 11;
    } break;
    case 11: {
        bool wait_load = false;

        for (x_pv_game_song_effect& i : song_effect)
            for (x_pv_game_song_effect_auth_3d& j : i.auth_3d)
                if (j.id.check_not_empty() && !j.id.check_loaded())
                    wait_load |= true;

        for (uint32_t& i : pv_glitter)
            if (!Glitter::glt_particle_manager_x->CheckNoFileReaders(i))
                wait_load |= true;

        if (wait_load)
            break;

        const pvpp* play_param = this->play_param;
        int32_t num_chara = play_param->num_chara;
        int32_t num_effect = play_param->num_effect;

        for (int32_t i = 0; i < num_effect; i++) {
            x_pv_game_song_effect& song_effect = this->song_effect[i];
            const pvpp_effect& effect = play_param->effect_array[i];
            int32_t num_glitter = effect.num_glitter;
            for (int32_t j = 0; j < num_glitter; j++) {
                const pvpp_glitter& glitter = effect.glitter_array[j];

                x_pv_game_song_effect_glitter v132;
                v132.name = glitter.name;
                v132.hash = hash_utf8_murmurhash(glitter.name);
                v132.scene_counter = 0;
                v132.field_8 = !!(glitter.unk2 & 0x01);
                for (bool& i : v132.field_9)
                    i = true;

                for (int32_t k = 0; k < num_chara && k < ROB_CHARA_COUNT; k++) {
                    bool v121 = false;
                    if (k < play_param->num_chara) {
                        const pvpp_chara& chara = play_param->chara_array[k];
                        int32_t num_glitter = chara.num_glitter;
                        for (int32_t l = 0; l < num_glitter; l++)
                            if (!strcmp(glitter.name, chara.glitter_array[l].name)) {
                                v121 = true;
                                break;
                            }
                    }
                    v132.field_9[k] = v121;
                }
                song_effect.glitter.push_back(v132);
            }
        }

        state = 12;
        break;
    }
    case 12: {
        state = 20;
        break;
    }
    case 20: {
        for (x_pv_game_song_effect& i : song_effect)
            if (i.enable)
                for (x_pv_game_song_effect_auth_3d& j : i.auth_3d)
                    j.id.set_chara_id(i.chara_id);
    } break;
    }
}

void x_pv_game_effect::load(int32_t pv_id, const pvpp* play_param) {
    this->play_param = play_param;

    int32_t num_effect = play_param->num_effect;
    if (!num_effect)
        return;

    song_effect.resize(num_effect);

    for (int32_t i = 0; i < num_effect; i++) {
        const pvpp_effect& eff = play_param->effect_array[i];
        if (!eff.num_auth_3d)
            continue;

        char buf[0x200];
        size_t len = sprintf_s(buf, sizeof(buf), "A3D_EFFPV%03d", pv_id);
        pv_auth_3d.push_back(prj::string(buf, len));

        len = sprintf_s(buf, sizeof(buf), "EFFPV%03d", pv_id);
        pv_obj_set.push_back(object_database_get_object_set_id(buf));
        break;
    }
}

void x_pv_game_effect::load_data(int32_t pv_id) {
    if (state)
        return;

    for (int32_t& i : pv_obj_set)
        objset_info_storage_load_set(i);

    for (prj::string& i : pv_auth_3d)
        auth_3d_data_load_category(i.c_str());

    uint32_t effect_count = play_param->num_effect;
    for (uint32_t i = 0; i < effect_count; i++) {
        const pvpp_effect& eff = play_param->effect_array[i];
        if (!eff.num_glitter)
            continue;

        char buf[0x200];
        size_t len = sprintf_s(buf, sizeof(buf), "eff_pv%03d_main", pv_id);
        uint32_t hash = Glitter::glt_particle_manager_x->LoadFile(buf, 0, -1.0f, true);
        if (hash != hash_murmurhash_empty)
            pv_glitter.push_back(hash);
        break;
    }

    state = 10;
}

void x_pv_game_effect::reset() {
    flags = 0;
    state = 0;
    play_param = 0;
    pv_obj_set.clear();
    pv_obj_set.shrink_to_fit();
    pv_auth_3d.clear();
    pv_auth_3d.shrink_to_fit();
    pv_glitter.clear();
    pv_glitter.shrink_to_fit();
    song_effect.clear();
    song_effect.shrink_to_fit();
}

void x_pv_game_effect::set_chara_id(int32_t index, int32_t chara_id, bool chara_item) {
    if (index < 0 || index >= song_effect.size())
        return;

    x_pv_game_song_effect& song_effect = this->song_effect[index];

    for (x_pv_game_song_effect_auth_3d& i : song_effect.auth_3d) {
        i.id.set_chara_id(chara_id);
        i.id.set_chara_item(chara_item);
    }

    song_effect.chara_id = chara_id;
}

void x_pv_game_effect::set_song_effect(int32_t index, int64_t time) {
    if (state && state != 20)
        return;

    if (index < 0 || index >= song_effect.size())
        return;

    x_pv_game_song_effect& song_effect = this->song_effect[index];

    if (song_effect.enable) {
        set_song_effect_time_inner(index, time, false);
        return;
    }

    const pvpp* play_param = this->play_param;
    int32_t num_chara = play_param->num_chara;

    int32_t chara_id = play_param->effect_array[index].chara_id;
    if (chara_id < 0 || chara_id >= ROB_CHARA_COUNT || chara_id >= num_chara)
        chara_id = -1;
    song_effect.chara_id = chara_id;

    for (x_pv_game_song_effect_auth_3d& i : song_effect.auth_3d) {
        i.id.set_enable(true);
        i.id.set_repeat(false);
        i.id.set_req_frame(0.0f);
        i.id.set_visibility(true);
    }

    for (x_pv_game_song_effect_glitter& i : song_effect.glitter) {
        i.scene_counter = Glitter::glt_particle_manager_x->LoadSceneEffect(i.hash);
        if (!i.scene_counter)
            continue;

        Glitter::glt_particle_manager_x->SetSceneFrameRate(i.scene_counter, 0);
    }

    song_effect.enable = true;
    song_effect.time = time;

    set_song_effect_time_inner(index, time, false);

    for (x_pv_game_song_effect_auth_3d& i : song_effect.auth_3d)
        i.id.set_chara_id(chara_id);
}

void x_pv_game_effect::set_song_effect_time(int32_t index, int64_t time, bool glitter) {
    if (index >= 0 && index < song_effect.size() && song_effect[index].enable)
        set_song_effect_time_inner(index, time, glitter);
}

void x_pv_game_effect::set_song_effect_time_inner(int32_t index, int64_t time, bool glitter) {
    if (index < 0 || index >= song_effect.size())
        return;

    x_pv_game_song_effect& song_effect = this->song_effect[index];

    float_t req_frame = (float_t)((double_t)(time - song_effect.time) * 0.000000001) * 60.0f;
    float_t max_frame = -1.0f;

    if (change_fields) {
        int64_t* key = change_fields->data();
        size_t length = change_fields->size();
        size_t temp;
        while (length > 0)
            if (key[temp = length / 2] > time)
                length = temp;
            else {
                key += temp + 1;
                length -= temp + 1;
            }

        if (key != change_fields->data() + change_fields->size())
            max_frame = (float_t)((double_t)(*key - song_effect.time) * 0.000000001) * 60.0f - 1.0f;
    }

    for (x_pv_game_song_effect_auth_3d& i : song_effect.auth_3d) {
        i.id.set_req_frame(req_frame);
        i.id.set_paused(false);
        i.id.set_max_frame(max_frame);
    }

    if (glitter)
        for (x_pv_game_song_effect_glitter& i : song_effect.glitter) {
            if (!i.scene_counter)
                continue;

            float_t frame = Glitter::glt_particle_manager_x->GetSceneFrameLifeTime(i.scene_counter, 0);
            Glitter::glt_particle_manager_x->SetSceneEffectReqFrame(i.scene_counter, req_frame - frame);
        }
}

void x_pv_game_effect::set_time(int64_t time, bool glitter) {
    size_t size = song_effect.size();
    for (size_t i = 0; i < size; i++)
        if (song_effect[i].enable)
            set_song_effect_time_inner((int32_t)i, time, glitter);
}

void x_pv_game_effect::stop() {
    size_t song_effect_count = song_effect.size();
    for (size_t i = 0; i < song_effect_count; i++)
        stop_song_effect((int32_t)i, false);

    for (uint32_t& i : pv_glitter)
        Glitter::glt_particle_manager_x->FreeScene(i);
}

void x_pv_game_effect::stop_song_effect(int32_t index, bool free_glitter) {
    if ((state && state != 20) || index < 0 || index >= song_effect.size() || !song_effect[index].enable)
        return;

    x_pv_game_song_effect& song_effect = this->song_effect[index];

    for (x_pv_game_song_effect_auth_3d& i : song_effect.auth_3d) {
        auth_3d_id& id = i.id;
        id.set_enable(false);
        id.set_req_frame(0.0f);
        id.set_visibility(false);
    }

    for (x_pv_game_song_effect_glitter& i : song_effect.glitter)
        if (i.scene_counter) {
            Glitter::glt_particle_manager_x->FreeSceneEffect(i.scene_counter, !free_glitter);
            i.scene_counter = 0;
        }

    song_effect.chara_id = -1;
    song_effect.enable = false;
}

void x_pv_game_effect::unload() {
    if (state && state != 20)
        return;

    size_t song_effect_count = song_effect.size();
    for (size_t i = 0; i < song_effect_count; i++)
        stop_song_effect((int32_t)i, false);

    for (uint32_t& i : pv_glitter)
        Glitter::glt_particle_manager_x->FreeScene(i);

    for (x_pv_game_song_effect& i : song_effect)
        for (x_pv_game_song_effect_auth_3d& j : i.auth_3d) {
            j.id.unload();
            j.id = {};
        }

    for (prj::string& i : pv_auth_3d)
        auth_3d_data_unload_category(i.c_str());

    for (int32_t& i : pv_obj_set)
        objset_info_storage_unload_set(i);

    for (uint32_t& i : pv_glitter)
        Glitter::glt_particle_manager_x->UnloadEffectGroup(i);

    song_effect.clear();
    song_effect.resize(song_effect_count);
    state = 0;
}

aet_obj_data_comp::aet_obj_data_comp() : aet_id(), layer_name() {
    frame = -1.0f;
}

aet_obj_data_comp::~aet_obj_data_comp() {

}

const aet_layout_data* aet_obj_data_comp::find_layout(const char* name) {
    return comp.Find(name);
}

bool aet_obj_data_comp::init_comp(int32_t aet_id, const std::string&& layer_name, float_t frame) {
    if (aet_id == -1)
        return false;

    if (comp.data.size() && this->aet_id == aet_id && !strncmp(this->layer_name,
        layer_name.c_str(), layer_name.size()) && fabsf(this->frame - frame) <= 0.000001f)
        return true;

    comp.Clear();
    this->aet_id = aet_id;

    size_t len = min_def(sizeof(this->layer_name) - 1, layer_name.size());
    strncpy_s(this->layer_name, sizeof(this->layer_name), layer_name.c_str(), len);
    this->layer_name[len] = 0;
    this->frame = frame;

    aet_manager_init_aet_layout_0(&comp, aet_id, layer_name.c_str(),
        (AetFlags)0, RESOLUTION_MODE_HD, 0, frame);
    return true;
}

aet_obj_data::aet_obj_data() : comp(), loop(), hidden(), field_2A(), frame_rate_control() {
    aet_id = -1;

    reset();
}

aet_obj_data::~aet_obj_data() {
    reset();
}

bool aet_obj_data::check_disp() {
    return aet_manager_get_obj_end(id);
}

const aet_layout_data* aet_obj_data::find_aet_layout(const char* name) {
    if (!name)
        return 0;

    if (!comp)
        comp = new (_operator_new(sizeof(aet_obj_data_comp))) aet_obj_data_comp;

    if (comp && comp->init_comp(aet_id, layer_name.c_str(), aet_manager_get_obj_frame(id)))
        return comp->find_layout(name);

    return 0;
}

static uint32_t(FASTCALL* get_frame_counter)() = (uint32_t(FASTCALL*)())0x0000000140192E00;

uint32_t aet_obj_data::init(AetArgs& args) {
    reset();

    aet_id = args.id.id;
    layer_name.assign(args.layer_name);
    id = aet_manager_init_aet_object(args);
    if (!id)
        return 0;

    aet_manager_set_obj_frame_rate_control(id, frame_rate_control);
    aet_manager_set_obj_visible(id, !hidden);
    field_2A = true;
    loop = !!(args.flags & AET_LOOP);
    return id;
}

void aet_obj_data::reset() {
    field_2A = false;

    if (id) {
        aet_manager_free_aet_object(id);
        id = 0;
    }

    aet_id = -1;
    layer_name.clear();
    layer_name.shrink_to_fit();

    if (comp) {
        comp->~aet_obj_data_comp();
        _operator_delete(comp);
        comp = 0;
    }
}

x_pv_aet_disp_data::x_pv_aet_disp_data() : disp(), opacity() {

}

x_pv_aet_disp_string::x_pv_aet_disp_string() {

}

x_pv_aet_disp_string::~x_pv_aet_disp_string() {

}

void x_pv_aet_disp_string::ctrl(const char* name, aet_obj_data* aet) {
    if (!aet) {
        data.disp = false;
        return;
    }

    const aet_layout_data* layout = aet->find_aet_layout(name);
    if (!layout) {
        data.disp = false;
        return;
    }

    data.disp = true;
    data.rect.pos = *(vec2*)&layout->position;
    data.rect.size.x = layout->height * layout->mat.row1.y * layout->mat.row0.x;
    data.rect.size.y = layout->height * layout->mat.row1.y;
    data.opacity = clamp_def((uint8_t)(int32_t)(layout->opacity * 255.0f), 0x00, 0xFF);
}

static const char* sub_8133DDF8(int32_t id) {
    return x_pv_game_str_array_ptr->get_string(id);
}

const char* x_pv_str_array_pv::get_song_name(int32_t index) {
    if (!index)
        return song_name;

    int32_t id = 20000010 + 1000 * pv_id + 4 * (index - 1);

    const char* str = sub_8133DDF8(id);
    if (str)
        return str;
    return "";
}

const char* x_pv_str_array_pv::get_song_line_1(int32_t index) {
    int32_t id = 20000002 + 1000 * pv_id;

    if (index)
        id = 20000011 + 1000 * pv_id + 4 * (index - 1);

    const char* str = sub_8133DDF8(id);
    if (str)
        return str;
    return "";
}

const char* x_pv_str_array_pv::get_song_line_2(int32_t index) {
    int32_t id = 20000003 + 1000 * pv_id;

    if (index)
        id = 20000012 + 1000 * pv_id + 4 * (index - 1);

    const char* str = sub_8133DDF8(id);
    if (str)
        return str;
    return "";
}

const char* x_pv_str_array_pv::get_song_line_3(int32_t index) {
    int32_t id = 20000004 + 1000 * pv_id;
    if (index)
        id = 20000013 + 1000 * pv_id + 4 * (index - 1);

    const char* str = sub_8133DDF8(id);
    if (str)
        return str;
    return "";
}

x_pv_game_title::x_pv_game_title() : state(), str_array(), txt_layer_index() {
    aet_set_id = -1;
    spr_set_id = -1;
    aet_id = -1;
}

x_pv_game_title::~x_pv_game_title() {
    unload();
}

void x_pv_game_title::ctrl() {
    switch (state) {
    case 2:
        if (!aet_manager_load_file(aet_set_id)
            && !sprite_manager_load_file(spr_set_id))
            state = 3;
        break;
    case 4:
        if (tit_layer.check_disp() && txt_layer.check_disp())
            state = 3;
        else if (txt_layer.id)
            ctrl_txt_data();
        break;
    }
}

void x_pv_game_title::ctrl_txt_data() {
    const char* comps[4] = {
        "p_txt01_c",
        "p_txt02_c",
        "p_txt03_c",
        "p_txt04_c",
    };

    const char** str = comps;
    for (x_pv_aet_disp_string& i : txt_data)
        i.ctrl(*str++, &txt_layer);
}

void x_pv_game_title::disp() {
    if (state != 4)
        return;

    font_info font(16);

    PrintWork print_work;
    print_work.SetFont(&font);
    print_work.prio = spr::SPR_PRIO_06;

    for (x_pv_aet_disp_string& i : txt_data) {
        if (!i.data.disp || !i.str.size())
            continue;

        font.set_glyph_size(i.data.rect.size.x, i.data.rect.size.y);
        print_work.color.a = i.data.opacity;
        print_work.line_origin_loc = i.data.rect.pos;
        print_work.text_current_loc = print_work.line_origin_loc;

        print_work.printf((app::text_flags)(app::TEXT_FLAG_ALIGN_FLAG_LOCATE_V_CENTER
            | app::TEXT_FLAG_ALIGN_FLAG_H_CENTER
            | app::TEXT_FLAG_FONT), i.str.c_str());
    }
}

bool x_pv_game_title::get_id(int8_t cloud_index, int32_t& aet_id) {
    switch (cloud_index) {
    case 1:
    default:
        aet_id = aet_database_get_aet_id_by_name("AET_PV_TIT01_MAIN");
        break;
    case 2:
        aet_id = aet_database_get_aet_id_by_name("AET_PV_TIT02_MAIN");
        break;
    case 3:
        aet_id = aet_database_get_aet_id_by_name("AET_PV_TIT03_MAIN");
        break;
    case 4:
        aet_id = aet_database_get_aet_id_by_name("AET_PV_TIT04_MAIN");
        break;
    case 5:
        aet_id = aet_database_get_aet_id_by_name("AET_PV_TIT05_MAIN");
        break;
    }
    return true;
}

bool x_pv_game_title::get_set_ids(int8_t cloud_index, int32_t& aet_set_id, int32_t& spr_set_id) {
    switch (cloud_index) {
    case 1:
    default:
        aet_set_id = aet_database_get_aet_set_id_by_name("AET_PV_TIT01");
        spr_set_id = sprite_database_get_spr_set_id_by_name("SPR_PV_TIT01");
        break;
    case 2:
        aet_set_id = aet_database_get_aet_set_id_by_name("AET_PV_TIT02");
        spr_set_id = sprite_database_get_spr_set_id_by_name("SPR_PV_TIT02");
        break;
    case 3:
        aet_set_id = aet_database_get_aet_set_id_by_name("AET_PV_TIT03");
        spr_set_id = sprite_database_get_spr_set_id_by_name("SPR_PV_TIT03");
        break;
    case 4:
        aet_set_id = aet_database_get_aet_set_id_by_name("AET_PV_TIT04");
        spr_set_id = sprite_database_get_spr_set_id_by_name("SPR_PV_TIT04");
        break;
    case 5:
        aet_set_id = aet_database_get_aet_set_id_by_name("AET_PV_TIT05");
        spr_set_id = sprite_database_get_spr_set_id_by_name("SPR_PV_TIT05");
        break;
    }
    return true;
}

void x_pv_game_title::load(int32_t pv_id) {
    if (state)
        return;

    str_array.pv_id = pv_id;
    str_array.song_name = sub_8133DDF8(20000000 + 1000 * pv_id);

    int32_t cloud_index = 1;
    switch (pv_id) {
    case 809:
    case 810:
    case 823:
    case 824:
    case 826:
    case 830:
        cloud_index = 1; // Classic
        break;
    case 807:
    case 808:
    case 811:
    case 812:
    case 813:
    case 825:
        cloud_index = 2; // Cute
        break;
    case 801:
    case 805:
    case 814:
    case 819:
    case 820:
    case 827:
        cloud_index = 3; // Cool
        break;
    case 802:
    case 815:
    case 816:
    case 821:
    case 822:
    case 828:
        cloud_index = 4; // Elegant
        break;
    case 803:
    case 804:
    case 806:
    case 817:
    case 818:
    case 829:
        cloud_index = 5; // Chaos
        break;
    case 831:
    case 832:
        cloud_index = 6; // DLC
        break;
    }

    get_set_ids(cloud_index, aet_set_id, spr_set_id);
    get_id(cloud_index, aet_id);

    state = 1;
}

void x_pv_game_title::load_data() {
    if (!state || state != 1)
        return;

    sprite_manager_read_file(spr_set_id, "");
    aet_manager_read_file(aet_set_id, "");
    state = 2;
}

void x_pv_game_title::reset() {
    tit_layer.reset();
    txt_layer.reset();
    txt_data[0].data.disp = false;
    txt_data[1].data.disp = false;
    txt_data[2].data.disp = false;
    txt_data[3].data.disp = false;
    if (state == 4)
        state = 3;
}

void x_pv_game_title::show(int32_t index) {
    if (!state)
        return;

    txt_data[0].str.assign(str_array.get_song_name(index));
    txt_data[1].str.assign(str_array.get_song_line_1(index));
    txt_data[2].str.assign(str_array.get_song_line_2(index));
    txt_data[3].str.assign(str_array.get_song_line_3(index));

    txt_layer_index = 0;

    int32_t txt_layer_index = 3;
    while (!txt_data[txt_layer_index].str.size())
        if (--txt_layer_index < 2)
            break;

    txt_layer_index = max_def(txt_layer_index + 1, 2);
    this->txt_layer_index = txt_layer_index;

    if (state != 3 || aet_id == -1)
        return;

    AetArgs args;
    args.id.id = aet_id;
    args.layer_name = "tit_01";
    args.prio = spr::SPR_PRIO_05;

    tit_layer.init(args);

    switch (txt_layer_index) {
    case 0:
    case 1:
    case 2:
    default:
        args.layer_name = "txt01";
        break;
    case 3:
        args.layer_name = "txt02";
        break;
    case 4:
        args.layer_name = "txt03";
        break;
    }

    txt_layer.init(args);

    ctrl_txt_data();

    state = 4;
}

void x_pv_game_title::unload() {
    if (state >= 3 && state && state != 2 && state != 1) {
        reset();
        sprite_manager_unload_set(spr_set_id);
        aet_manager_unload_set(aet_set_id);
        state = 1;
    }

    aet_set_id = -1;
    spr_set_id = -1;
    aet_id = -1;
    state = 0;
}

x_pv_game_pv_data::x_pv_game_pv_data() : pv_game(),
dsc_data_ptr(), dsc_data_ptr_end(), play(), dsc_time(), pv_end() {

}

x_pv_game_pv_data::~x_pv_game_pv_data() {

}

static void print_dsc_command(dsc& dsc, dsc_data* dsc_data_ptr, int64_t time) {
    static const char* (FASTCALL * dw_console_printf)(int32_t index, const char* fmt, ...)
        = (const char* (FASTCALL*)(int32_t index, const char* fmt, ...))0x000000014017B730;

    if (dsc_data_ptr->func < 0 || dsc_data_ptr->func >= DSC_X_MAX) {
        dw_console_printf(2, "UNKNOWN command(%d)\n", dsc_data_ptr->func);
        return;
    }

    if (dsc_data_ptr->func == DSC_X_TIME)
        return;

#pragma warning(suppress: 26451)
    dw_console_printf(2, "%.3f: X %s(", (float_t)time * 0.00001f, dsc_get_func_name(dsc_data_ptr->func));

    int32_t* data = dsc.get_func_data(dsc_data_ptr);

    int32_t length = dsc_get_func_length(dsc_data_ptr->func);
    for (int32_t i = 0; i < length; i++)
        if (i)
            dw_console_printf(2, ", %d", ((int32_t*)data)[i]);
        else
            dw_console_printf(2, "%d", ((int32_t*)data)[i]);

    dw_console_printf(2, ")\n");
}

void x_pv_game_pv_data::ctrl(float_t delta_time, int64_t curr_time) {
    dsc_data* prev_dsc_data_ptr = dsc_data_ptr;

    float_t dsc_time_offset = 0.0f;
    while (dsc_data_ptr != dsc_data_ptr_end) {
        bool music_play = false;
        bool next = dsc_ctrl(delta_time, curr_time, &dsc_time_offset);

        if (prev_dsc_data_ptr != dsc_data_ptr)
            print_dsc_command(dsc, prev_dsc_data_ptr, dsc_time);

        prev_dsc_data_ptr = dsc_data_ptr;

        if (!next)
            break;
    }
}

bool x_pv_game_pv_data::dsc_ctrl(float_t delta_time, int64_t curr_time,
    float_t* dsc_time_offset) {
    dsc_x_func func = (dsc_x_func)dsc_data_ptr->func;
    int32_t* data = dsc.get_func_data(dsc_data_ptr);

    switch (func) {
    case DSC_X_END: {
        play = false;
        pv_game->state_old = 21;
        return false;
    } break;
    case DSC_X_TIME: {
        dsc_time = (int64_t)data[0] * 10000;
        if (dsc_time > curr_time)
            return false;
    } break;
    case DSC_X_CHANGE_FIELD: {
        pv_game->data.effect.set_time(curr_time, false);
        pv_game->data.chara_effect.set_time(curr_time);
    } break;
    case DSC_X_CREDIT_TITLE: {
        int32_t index = data[0];

        if (index)
            pv_game->data.title.show(index - 1);
    } break;
    case DSC_X_STAGE_EFFECT: {
        int32_t stage_effect = data[0];

        if (stage_effect >= 8 && stage_effect <= 9)
            pv_game->stage_data.set_stage_effect(stage_effect);
    } break;
    case DSC_X_SONG_EFFECT: {
        bool enable = data[0] ? true : false;
        int32_t index = data[1];
        int32_t unk2 = data[2];

        if (unk2 && !(pv_game->data.field_1C & 0x10))
            break;

        x_pv_game_effect& effect = pv_game->data.effect;
        if (enable) {
            effect.set_song_effect(index, dsc_time);
            //if (a2->field_18)
            //    effect.set_song_effect_time(index/*, a2->field_20*/, true);
            //else
                effect.set_song_effect_time(index, curr_time, false);
        }
        else
            effect.stop_song_effect(index, true);
    } break;
    case DSC_X_SONG_EFFECT_ATTACH: {
        int32_t index = data[0];
        int32_t chara_id = data[1];
        int32_t chara_item = data[2];

        pv_game->data.effect.set_chara_id(index, chara_id, !!chara_item);
    } break;
    case DSC_X_SET_STAGE_EFFECT_ENV: {
        int32_t env_index = data[0];
        int32_t trans = data[1];
        pv_game->stage_data.set_env(env_index, (float_t)trans * (float_t)(1.0f / 60.0f), 0.0f);
    } break;
    case DSC_X_CHARA_EFFECT: {
        int32_t chara_id = data[0];
        bool enable = data[1] ? true : false;
        int32_t index = data[2];

        x_pv_game_chara_effect& chara_effect = pv_game->data.chara_effect;
        if (enable) {
            chara_effect.set_chara_effect(chara_id, index, dsc_time);
            chara_effect.set_chara_effect_time(chara_id, index, curr_time);
        }
        else
            chara_effect.stop_chara_effect(chara_id, index);
    } break;
    }

    dsc_data_ptr++;
    return true;
}

dsc_data* x_pv_game_pv_data::find(int32_t func_name, int32_t* time,
    int32_t* pv_branch_mode, dsc_data* start, dsc_data* end) {
    int32_t _time = -1;
    for (dsc_data* i = start; i != end; i++)
        if (i->func == func_name) {
            if (time)
                *time = _time;
            return i;
        }
        else if (i->func == DSC_X_END)
            break;
        else if (i->func == DSC_X_TIME) {
            int32_t* data = dsc.get_func_data(i);
            _time = data[0];
        }
    return 0;
}

void x_pv_game_pv_data::find_bar_beat(x_pv_bar_beat& bar_beat) {
    x_pv_bar_beat_data* bar_beat_data = 0;
    int32_t beat_counter = 0;

    int32_t time = -1;
    for (dsc_data& i : dsc.data) {
        if (i.func == DSC_X_END)
            break;

        int32_t* data = dsc.get_func_data(&i);
        switch (i.func) {
        case DSC_X_TIME: {
            time = data[0];
        } break;
        case DSC_X_BAR_POINT: {
            bar_beat.data.push_back({});
            bar_beat_data = &bar_beat.data.back();
            *bar_beat_data = {};
            bar_beat_data->bar = data[0];
            bar_beat_data->bar_time = (float_t)time * (float_t)(1.0 / 100000.0);
            bar_beat_data->beat_counter = beat_counter;
        } break;
        case DSC_X_BEAT_POINT: {
            if (!bar_beat_data)
                break;

            bar_beat_data->beat_counter = ++beat_counter;
            bar_beat_data->beat_time[bar_beat_data->beat_count++] = (float_t)time * (float_t)(1.0 / 100000.0);
        } break;
        }
    }
}

void x_pv_game_pv_data::find_change_fields(prj::vector<int64_t>& change_fields) {
    int32_t pv_branch_mode = 0;
    int32_t time = -1;
    int32_t prev_time = -1;

    dsc_data* i = dsc.data.data();
    dsc_data* i_end = dsc.data.data() + dsc.data.size();
    while (i = find(DSC_X_CHANGE_FIELD, &time, &pv_branch_mode, i, i_end)) {
        if (time < 0) {
            time = prev_time;
            if (prev_time < 0)
                break;
        }

        change_fields.push_back((int64_t)time * 10000);
        prev_time = time;
        i++;
    }
}

void x_pv_game_pv_data::find_stage_effects(prj::vector<prj::pair<int64_t, int32_t>>& stage_effects) {
    int32_t pv_branch_mode = 0;
    int32_t time = -1;
    int32_t prev_time = -1;

    dsc_data* i = dsc.data.data();
    dsc_data* i_end = dsc.data.data() + dsc.data.size();
    while (i = find(DSC_X_STAGE_EFFECT, &time, &pv_branch_mode, i, i_end)) {
        if (time < 0) {
            time = prev_time;
            if (prev_time < 0)
                break;
        }

        int32_t* data = dsc.get_func_data(i);
        int32_t stage_effect = data[0];

        if (stage_effect < 8 || stage_effect > 9)
            stage_effects.push_back({ (int64_t)time * 10000, stage_effect });
        prev_time = time;
        i++;
    }
}

void x_pv_game_pv_data::init(class TaskPvGameX* pv_game) {
    this->pv_game = pv_game;
    pv_end = false;
    play = true;
}

void x_pv_game_pv_data::reset() {
    dsc.data.clear();
    dsc.data.shrink_to_fit();
    dsc.data_buffer.clear();
    dsc.data_buffer.shrink_to_fit();
    dsc_data_ptr = 0;
    dsc_data_ptr_end = 0;
    dsc_time = -1;
}

void x_pv_game_pv_data::stop() {
    if (dsc.data.size()) {
        dsc_data_ptr = dsc.data.data();
        dsc_data_ptr_end = dsc_data_ptr + dsc.data.size();
    }
    else {
        dsc_data_ptr = 0;
        dsc_data_ptr_end = 0;
    }
    dsc_time = -1;
}

void x_pv_game_pv_data::unload() {
    dsc.data.clear();
    dsc.data.shrink_to_fit();
    dsc.data_buffer.clear();
    dsc.data_buffer.shrink_to_fit();
    dsc_data_ptr = 0;
    dsc_data_ptr_end = 0;
    dsc_time = -1;
}

x_pv_game_data::x_pv_game_data() : pv_id(), play_param_file_handler(), play_param(),
field_1C(), state(), stage_effect_index(), next_stage_effect_bar(), stage() {

}

x_pv_game_data::~x_pv_game_data() {

}

void x_pv_game_data::ctrl(int64_t curr_time, float_t delta_time) {
    switch (state) {
    case 10: {
        if (field_1C & 0x08) {
            state = 20;
            field_1C &= ~0x08;
        }
        else
            state = 0;
    } break;
    case 20: {
        effect.load_data(pv_id);
        chara_effect.load_data();
        title.load_data();
        state = 21;
    } break;
    case 21: {
        if (stage->state != 20)
            break;

        state = 22;
    } break;
    case 22: {
        bool wait_load = false;

        wait_load |= effect.state && effect.state != 20;
        wait_load |= chara_effect.state && chara_effect.state != 10 && chara_effect.state != 30;
        wait_load |= title.state && title.state != 3 && title.state != 4;

        if (wait_load)
            break;

        state = 23;
    } break;
    case 23: {
        pv_data.find_bar_beat(bar_beat);
        bar_beat.reset_time();
        stage->bpm_frame_rate_control.bar_beat = &bar_beat;
        state = 24;
    } break;
    case 24: {
        pv_data.find_stage_effects(stage_effects);
        state = 25;
    } break;
    case 25: {
        pv_data.find_change_fields(change_fields);
        effect.change_fields = &change_fields;
        chara_effect.change_fields = &change_fields;
        state = 30;
    } break;
    case 30: {
    } break;
    case 40: {
        //if (!sub_81254B9E())
            state = 0;
    } break;
    }

    effect.ctrl();
    chara_effect.ctrl();
    title.ctrl();
    field_1C &= ~0x02;
}

void x_pv_game_data::ctrl_stage_effect_index() {
    if (stage_effect_index >= stage_effects.size())
        return;

    prj::pair<int64_t, int32_t>* curr_stage_effect = &stage_effects[stage_effect_index];
    if (next_stage_effect_bar > bar_beat.bar)
        return;

    stage->set_stage_effect(curr_stage_effect->second);
    stage_effect_index++;
    if (stage_effect_index >= stage_effects.size())
        return;

    prj::pair<int64_t, int32_t>* next_stage_effect = &stage_effects[stage_effect_index];

    float_t time = (float_t)((double_t)next_stage_effect->first * 0.000000001);
    int32_t bar = bar_beat.get_bar_beat_from_time(0, time);
    next_stage_effect_bar = --bar;

    int32_t v14;
    if (stage->check_stage_effect_has_change_effect(curr_stage_effect->second, next_stage_effect->second))
        v14 = bar / 2 - 2;
    else
        v14 = bar / 2 - 1;
    next_stage_effect_bar = 2 * max_def(v14, 0) + 1;
}

void x_pv_game_data::disp() {
    title.disp();
}

void x_pv_game_data::load(int32_t pv_id, chara_index charas[6]) {
    if (state)
        return;

    title.load(pv_id);
    effect.load(pv_id, play_param);
    chara_effect.load(pv_id, play_param, charas);
    state = 10;
}

void x_pv_game_data::reset() {
    pv_id = 0;
    play_param = 0;
    play_param_file_handler.reset();
    field_1C = 0;
    state = 0;
    change_fields.clear();
    change_fields.shrink_to_fit();
    bar_beat.reset();
    stage_effect_index = 0;
    next_stage_effect_bar = 0;
    stage_effects.clear();
    stage_effects.shrink_to_fit();
    effect.reset();
    chara_effect.reset();
    pv_data.reset();
    stage = 0;
}

void x_pv_game_data::stop() {
    stage->reset_stage_effect();
    stage->reset_stage_env();
    stage->curr_stage_effect = 0;
    stage->next_stage_effect = 0;
    stage->stage_effect_transition_state = 0;
    effect.stop();
    chara_effect.stop();
    title.reset();
    pv_data.stop();
    bar_beat.reset_time();
    stage_effect_index = 0;
    next_stage_effect_bar = 0;
    field_1C &= ~0xC0;
}

void x_pv_game_data::unload() {
    stop();

    title.unload();
    effect.unload();
    chara_effect.unload();
    pv_data.unload();
    state = 40;
}

void x_pv_game_data::unload_if_state_is_0() {
    if (!state)
        unload();
}

x_pv_game_stage_env_data::x_pv_game_stage_env_data() {

}

x_pv_game_stage_env_aet::x_pv_game_stage_env_aet() : state(), prev(), next() {
    duration = 1.0f;
}

x_pv_game_stage_env::x_pv_game_stage_env() : flags(), pv_id(),
state(), stage_resource(), trans_duration(), trans_remain() {
    env_index = -1;

    for (auto& i : data)
        for (auto& j : i.data)
            j.frame_rate_control = &frame_rate_control;
}

x_pv_game_stage_env::~x_pv_game_stage_env() {

}

void x_pv_game_stage_env::ctrl(float_t delta_time) {
    frame_rate_control.delta_frame = delta_time * 60.0f;

    switch (state) {
    case 10:
        state = 11;
    case 11:
        if (pv_disp2d_data.pv_id == pv_id
            && (pv_disp2d_data.pv_aet_set_id == -1
                || !aet_manager_load_file(pv_disp2d_data.pv_aet_set_id))
            && (pv_disp2d_data.pv_spr_set_id == -1
                || !sprite_manager_load_file(pv_disp2d_data.pv_spr_set_id)))
            state = 20;
        break;
    case 20:
        if (env_index != -1 && !sub_810EE198(delta_time))
            sub_810EE03E();
        break;
    }
}

const pvsr_auth_2d* x_pv_game_stage_env::get_aet(int32_t env_index, int32_t index) {
    if (env_index < 0 || env_index >= stage_resource->num_stage_effect_env)
        return 0;

    const pvsr_stage_effect_env* env = &stage_resource->stage_effect_env_array[env_index];
    if (index < env->num_aet)
        return &env->aet_array[index];
    return 0;
}

void x_pv_game_stage_env::load(int32_t pv_id, const pvsr* stage_resource) {
    if (this->stage_resource || !stage_resource || stage_resource->num_stage_effect_env > 64)
        return;

    this->pv_id = pv_id;
    this->stage_resource = stage_resource;

    state = 10;
}

void x_pv_game_stage_env::reset() {
    flags = 0;
    state = 0;
    stage_resource = 0;
    env_index = -1;
    frame_rate_control.delta_frame = 1.0f;
}

void x_pv_game_stage_env::reset_env() {
    for (auto& i : data)
        for (auto& j : i.data)
            j.reset();

    trans_duration = 0.0f;
    trans_remain = 0.0f;
    aet = {};
    env_index = -1;
}

static const float_t env_aet_opacity = 0.65f;

#pragma warning(push)
#pragma warning(disable: 6385)
void x_pv_game_stage_env::set(int32_t env_index, float_t end_time, float_t start_time) {
    if (!stage_resource || env_index < 0 || env_index >= 64
        || env_index >= stage_resource->num_stage_effect_env || this->env_index == env_index)
        return;

    float_t duration = max_def(end_time - start_time, 0.0f);
    if (trans_duration > 0.0f) {
        if (fabsf(end_time) > 0.000001f)
            return;

        trans_duration = 0.0f;
        trans_remain = 0.0f;
        aet = {};
    }

    if (this->env_index == -1 || fabsf(duration) <= 0.000001f) {
        float_t frame = start_time * 60.0f;
        if (this->env_index != -1)
            for (auto& i : data[this->env_index].data)
                i.reset();

        if (!(flags & 0x04))
            for (int32_t index = 0; index < 8; index++) {
                const pvsr_auth_2d* aet = get_aet(env_index, index);
                if (!aet)
                    break;

                aet_obj_data& aet_obj = data[env_index].data[index];

                AetArgs args;
                args.id.id = pv_disp2d_data.pv_aet_id;
                args.layer_name = aet->name;
                args.mode = RESOLUTION_MODE_HD;
                args.flags = AET_PLAY_ONCE;
                args.index = 0;
                args.prio = spr::SPR_PRIO_00;
                args.start_marker = 0;
                args.end_marker = 0;
                args.color = vec4(1.0f, 1.0f, 1.0f, env_aet_opacity);
                aet_obj.init(args);

                if (aet_obj.id)
                    aet_manager_set_obj_frame(aet_obj.id, frame);
            }
    }
    else {
        trans_duration = duration;
        trans_remain = duration;

        bool has_prev = false;
        for (int32_t index = 0; index < 8; index++)
            if (data[this->env_index].data[index].id) {
                aet.prev[index] = &data[this->env_index].data[index];
                has_prev = true;
            }
            else
                break;

        for (int32_t index = 0; index < 8; index++)
            if (get_aet(env_index, index))
                aet.next[index] = &data[env_index].data[index];
            else
                break;

        if (has_prev) {
            aet.state = 1;
            aet.duration = duration * 0.5f;
        }
        else {
            aet.state = 2;
            aet.duration = duration;
        }
    }

    this->env_index = env_index;
}

void x_pv_game_stage_env::unload() {
    if (!state)
        return;

    state = 0;
    stage_resource = 0;
}

void x_pv_game_stage_env::sub_810EE03E() {
    if (env_index < 0 || env_index >= 64)
        return;

    for (int32_t index = 0; index < 8; index++) {
        aet_obj_data& aet_obj = data[env_index].data[index];
        if (!aet_obj.id || !aet_obj.check_disp())
            continue;

        AetArgs args;
        if (stage_resource && env_index >= 0 && env_index < 64) {
            const pvsr_auth_2d* aet = get_aet(env_index, index);
            if (aet) {
                args.id.id = pv_disp2d_data.pv_aet_id;
                args.layer_name = aet->name;
                args.mode = RESOLUTION_MODE_HD;
                args.flags = AET_PLAY_ONCE;
                args.index = 0;
                args.prio = spr::SPR_PRIO_00;
                args.start_marker = 0;
                args.end_marker = 0;
                args.color = vec4(1.0f, 1.0f, 1.0f, env_aet_opacity);
            }
        }
        aet_obj.init(args);
    }
}

bool x_pv_game_stage_env::sub_810EE198(float_t delta_time) {
    if (trans_remain <= 0.0f)
        return false;

    trans_remain = max_def(trans_remain - delta_time, 0.0f);

    float_t t = 1.0f - trans_remain / trans_duration;
    switch (aet.state) {
    case 1: {
        float_t alpha = 1.0f - t * 2.0f;
        if (alpha > 0.0f) {
            for (int32_t index = 0; index < 8; index++) {
                aet_obj_data* aet_obj = aet.prev[index];
                if (!aet_obj)
                    break;

                aet_manager_set_obj_alpha(aet_obj->id, alpha * env_aet_opacity);
            }
            break;
        }

        for (int32_t index = 0; index < 8; index++) {
            aet_obj_data* aet_obj = aet.prev[index];
            if (!aet_obj)
                break;

            aet_obj->reset();
        }

        aet.state = 2;
    }
    case 2: {
        for (int32_t index = 0; index < 8; index++) {
            aet_obj_data* aet_obj = aet.next[index];
            if (!aet_obj)
                break;

            AetArgs args;
            if (stage_resource && env_index >= 0 && env_index < 64) {
                const pvsr_auth_2d* aet = get_aet(env_index, index);
                if (aet) {
                    args.id.id = pv_disp2d_data.pv_aet_id;
                    args.layer_name = aet->name;
                    args.mode = RESOLUTION_MODE_HD;
                    args.flags = AET_PLAY_ONCE;
                    args.index = 0;
                    args.prio = spr::SPR_PRIO_00;
                    args.start_marker = 0;
                    args.end_marker = 0;
                    args.color = vec4(1.0f, 1.0f, 1.0f, env_aet_opacity);
                }
            }
            aet_obj->init(args);

            aet_manager_set_obj_alpha(aet_obj->id, 0.0f);
        }

        aet.state = 3;
    } break;
    case 3: {
        float_t alpha = 1.0f - trans_remain / aet.duration;
        for (int32_t index = 0; index < 8; index++) {
            aet_obj_data* aet_obj = aet.next[index];
            if (!aet_obj)
                break;

            aet_manager_set_obj_alpha(aet_obj->id, alpha * env_aet_opacity);
        }
    } break;
    }

    if (trans_remain <= 0.0f) {
        trans_duration = 0.0f;
        trans_remain = 0.0f;
        aet = {};
    }
    return false;
}
#pragma warning(pop)

x_pv_game_stage_effect_auth_3d::x_pv_game_stage_effect_auth_3d() : repeat(), field_1(), name() {
    reset();
}

void x_pv_game_stage_effect_auth_3d::reset() {
    repeat = false;
    field_1 = true;
    id = {};
    name = 0;
}

x_pv_game_stage_effect_glitter::x_pv_game_stage_effect_glitter() :
    name(), hash(), scene_counter(), fade_time(), fade_time_left(), force_disp() {
    reset();
}

void x_pv_game_stage_effect_glitter::reset() {
    name = 0;
    hash = hash_murmurhash_empty;
    scene_counter = 0;
    fade_time = 0.0f;
    fade_time_left = 0.0f;
    force_disp = false;
}

x_pv_game_stage_change_effect::x_pv_game_stage_change_effect() : enable(), bars_left(), bar_count() {

}

x_pv_game_stage_change_effect::~x_pv_game_stage_change_effect() {

}

void x_pv_game_stage_change_effect::reset() {
    enable = false;
    auth_3d.clear();
    auth_3d.shrink_to_fit();
    glitter.clear();
    glitter.shrink_to_fit();
    bars_left = 0;
    bar_count = 0;
}

x_pv_game_stage_effect::x_pv_game_stage_effect() : stage_effect(), set() {
    main_auth_3d_index = -1;
}

x_pv_game_stage_effect::~x_pv_game_stage_effect() {

}

void x_pv_game_stage_effect::reset() {
    stage_effect = 0;
    auth_3d.clear();
    auth_3d.shrink_to_fit();
    glitter.clear();
    glitter.shrink_to_fit();
    main_auth_3d_index = -1;
    set = false;
}

x_pv_game_stage::x_pv_game_stage() : flags(), pv_id(), field_8(), state(), stage_resource_file_handler(),
stage_resource(), objhrc_set(), curr_stage_effect(), next_stage_effect(), stage_effect_transition_state() {
    reset();
}

x_pv_game_stage::~x_pv_game_stage() {

}

bool x_pv_game_stage::check_stage_effect_has_change_effect(int32_t curr_stage_effect, int32_t next_stage_effect) {
    if (curr_stage_effect == 7)
        return false;

    curr_stage_effect--;
    next_stage_effect--;

    if (curr_stage_effect < 0 || next_stage_effect < 0)
        return false;

    size_t stage_effect_count = stage_resource->num_stage_effect;
    if (curr_stage_effect >= stage_effect_count || next_stage_effect >= stage_effect_count)
        return false;

    const pvsr_stage_change_effect& stg_chg_eff = stage_resource->
        stage_change_effect_array[curr_stage_effect * PVSR_STAGE_EFFECT_COUNT + next_stage_effect];
    if (stg_chg_eff.enable)
        return true;
    return false;
}

void x_pv_game_stage::ctrl(float_t delta_time) {
    switch (state) {
    case 10: {
        if (!stage_resource || stage_resource->num_stage_effect > 12)
            break;

        {
            char buf[0x200];
            size_t len = sprintf_s(buf, sizeof(buf), "EFFSTGPV%03d", pv_id);
            stage_auth_3d.push_back(prj::string(buf, len));
        }

        int32_t num_effect = stage_resource->num_effect;
        for (int32_t i = 0; i < num_effect; i++) {
            const pvsr_effect& effect = stage_resource->effect_array[i];
            uint32_t hash = Glitter::glt_particle_manager_x->LoadFile(effect.name, 0, effect.emission, true);
            if (hash != hash_murmurhash_empty)
                stage_glitter.push_back(hash);
        }

        env.load(pv_id, stage_resource);
        state = 11;
    } break;
    case 11: {
        bool wait_load = false;

        for (prj::string& i : stage_auth_3d)
            if (!auth_3d_data_check_category_loaded(i.c_str()))
                wait_load |= true;

        for (uint32_t& i : stage_glitter)
            if (!Glitter::glt_particle_manager_x->CheckNoFileReaders(i))
                wait_load |= true;

        if (wait_load)
            break;

        size_t stage_effect_count = stage_resource->num_stage_effect;

        for (int32_t i = 0; i < X_PV_GAME_STAGE_EFFECT_COUNT && i < stage_effect_count; i++) {
            const pvsr_stage_effect& stg_eff = stage_resource->stage_effect_array[i];
            x_pv_game_stage_effect& eff = effect[i];

            eff.stage_effect = &stg_eff;

            int32_t num_auth_3d = stg_eff.num_auth_3d;
            int32_t num_glitter = stg_eff.num_glitter;

            eff.auth_3d.resize(num_auth_3d);
            eff.glitter.resize(num_glitter);

            for (size_t j = 0; j < num_auth_3d; j++) {
                const pvsr_auth_3d& stg_eff_auth_3d = stg_eff.auth_3d_array[j];
                x_pv_game_stage_effect_auth_3d& eff_auth_3d = eff.auth_3d[j];

                eff_auth_3d.reset();

                int32_t stage_effect = i + 1;

                int32_t uid = auth_3d_data_get_uid_by_name(stg_eff_auth_3d.name);

                auto elem = auth_3d_ids.find(uid);
                if (elem == auth_3d_ids.end()) {
                    auth_3d_id id = auth_3d_id(uid);
                    if (!id.check_not_empty()) {
                        eff_auth_3d.id = {};
                        eff_auth_3d.name = 0;
                        continue;
                    }

                    id.read_file();
                    id.set_enable(false);
                    id.set_repeat(true);
                    id.set_paused(false);
                    id.set_visibility(false);
                    id.set_frame_rate(&bpm_frame_rate_control);
                    eff_auth_3d.id = id;
                    eff_auth_3d.name = stg_eff_auth_3d.name;

                    auth_3d_ids.insert({ uid, id });
                }
                else {
                    eff_auth_3d.id = elem->second;
                    eff_auth_3d.name = stg_eff_auth_3d.name;
                }

                if (stage_effect >= 8 && stage_effect <= 9)
                    eff_auth_3d.repeat = false;
                else if (stage_effect == 7)
                    eff_auth_3d.repeat = !!(stg_eff_auth_3d.flags & PVSR_AUTH_3D_REPEAT);
                else
                    eff_auth_3d.repeat = true;

                if (stg_eff_auth_3d.flags & PVSR_AUTH_3D_MAIN)
                    eff.main_auth_3d_index = (int32_t)j;
            }

            for (size_t i = 0; i < num_glitter; i++) {
                const pvsr_glitter& stg_eff_glt = stg_eff.glitter_array[i];
                x_pv_game_stage_effect_glitter& eff_glt = eff.glitter[i];

                eff_glt.reset();
                eff_glt.name = stg_eff_glt.name;
                eff_glt.hash = hash_utf8_murmurhash(stg_eff_glt.name);
                eff_glt.fade_time = (float_t)stg_eff_glt.fade_time;
                eff_glt.force_disp = !!(stg_eff_glt.flags & PVSR_GLITTER_FORCE_DISP);
            }
        }

        for (int32_t i = 1; i <= X_PV_GAME_STAGE_EFFECT_COUNT; i++)
            for (int32_t j = 1; j <= X_PV_GAME_STAGE_EFFECT_COUNT; j++)
                load_change_effect(i, j);

        state = 12;
    }
    case 12: {
        //if (env.state && env.state != 20)
        //    break;

        bool wait_load = false;
        for (int32_t i = 0; i < X_PV_GAME_STAGE_EFFECT_COUNT; i++) {
            x_pv_game_stage_effect& eff = effect[i];
            for (x_pv_game_stage_effect_auth_3d& j : eff.auth_3d)
                if (j.id.check_not_empty() && !j.id.check_loaded())
                    wait_load |= true;
        }

        for (int32_t i = 0; i < X_PV_GAME_STAGE_EFFECT_COUNT; i++)
            for (int32_t j = 0; j < X_PV_GAME_STAGE_EFFECT_COUNT; j++) {
                x_pv_game_stage_change_effect& chg_eff = change_effect[i][j];
                for (x_pv_game_stage_effect_auth_3d& k : chg_eff.auth_3d)
                    if (k.id.check_not_empty() && !k.id.check_loaded())
                        wait_load |= true;
            }

        if (wait_load)
            break;

        state = 20;
    }
    case 20:
        ctrl_inner();
        break;
    case 30:
        break;
    }

    env.ctrl(delta_time);
}

void x_pv_game_stage::ctrl_inner() {
    if (next_stage_effect && curr_stage_effect) {
        x_pv_bar_beat* bar_beat = bpm_frame_rate_control.bar_beat;
        if (!bar_beat)
            return;

        bool change_stage_effect = bar_beat && fabsf(bar_beat->delta_time) > 0.000001f
            && bar_beat->counter > 0 && !((bar_beat->bar - 1) % 2);

        int32_t curr_stg_eff = curr_stage_effect;
        if (curr_stg_eff >= 1 && curr_stg_eff <= 12)
            curr_stg_eff--;
        else
            curr_stg_eff = 0;

        int32_t next_stg_eff = next_stage_effect;
        if (next_stg_eff >= 1 && next_stg_eff <= 12)
            next_stg_eff--;
        else
            next_stg_eff = 0;

        x_pv_game_stage_effect& eff = effect[curr_stg_eff];
        x_pv_game_stage_change_effect& chg_eff = change_effect[curr_stg_eff][next_stg_eff];

        switch (stage_effect_transition_state) {
        case 0: {
            if (change_stage_effect) {
                if (set_change_effect_frame_part_1()) {
                    chg_eff.bars_left = chg_eff.bar_count;
                    stage_effect_transition_state = 1;
                    stop_stage_effect_auth_3d(curr_stage_effect);

                    set_change_effect_frame_part_2(bar_beat->get_bar_current_frame());
                }
                else {
                    stop_stage_effect_auth_3d(curr_stage_effect);
                    stop_stage_effect_glitter(curr_stage_effect);

                    set_stage_effect_auth_3d_frame(next_stage_effect, -1.0f);
                    set_stage_effect_glitter_frame(next_stage_effect, -1.0f);
                    curr_stage_effect = next_stage_effect;
                    next_stage_effect = 0;
                    stage_effect_transition_state = 0;
                }
            }
            else if (bar_beat->counter > 0) {
                prj::vector<x_pv_game_stage_effect_auth_3d>& auth_3d = eff.auth_3d;
                for (x_pv_game_stage_effect_auth_3d& i : auth_3d) {
                    auth_3d_id& id = i.id;
                    if (i.field_1)
                        id.set_repeat(false);
                }
            }
        } break;
        case 1: {
            if (change_stage_effect) {
                chg_eff.bars_left -= 2;
                change_stage_effect = chg_eff.bars_left <= 0;
            }

            if (change_stage_effect) {
                stop_stage_change_effect();
                stop_stage_effect_glitter(curr_stage_effect);

                set_stage_effect_auth_3d_frame(next_stage_effect, -1.0f);
                set_stage_effect_glitter_frame(next_stage_effect, -1.0f);
                curr_stage_effect = next_stage_effect;
                next_stage_effect = 0;
                stage_effect_transition_state = 0;
            }
            else {
                float_t delta_time_bar_beat = bar_beat->delta_time / (bar_beat->next_bar_time - bar_beat->curr_bar_time);

                prj::vector<x_pv_game_stage_effect_glitter>& glitter = eff.glitter;
                for (x_pv_game_stage_effect_glitter& i : glitter) {
                    if (fabsf(i.fade_time) <= 0.000001f)
                        Glitter::glt_particle_manager_x->SetSceneEffectExtColor(i.scene_counter,
                            -1.0f, -1.0f, -1.0f, 0.0f, 0, hash_murmurhash_empty);
                    else if (i.fade_time > 0.001f) {
                        i.fade_time_left -= delta_time_bar_beat * 4.0f;
                        if (i.fade_time_left < 0.0f)
                            i.fade_time_left = 0.0f;
                        Glitter::glt_particle_manager_x->SetSceneEffectExtColor(i.scene_counter,
                            -1.0f, -1.0f, -1.0f, i.fade_time_left / i.fade_time,  0, hash_murmurhash_empty);
                    }
                }
            }
        } break;
        }
    }
    else if (next_stage_effect)
        return;

    for (int32_t i = 8; i <= 9; i++) {
        if (!(flags & (1 << i)))
            continue;

        int32_t stage_effect;
        if (i > 0)
            stage_effect = i - 1;
        else
            stage_effect = 0;

        x_pv_game_stage_effect& eff = effect[stage_effect];
        prj::vector<x_pv_game_stage_effect_auth_3d>& auth_3d = eff.auth_3d;
        prj::vector<x_pv_game_stage_effect_glitter>& glitter = eff.glitter;

        bool stop = false;
        if (auth_3d.size())
            stop = auth_3d[eff.main_auth_3d_index].id.get_ended();

        if (!stop) {
            for (x_pv_game_stage_effect_glitter& j : glitter)
                if (Glitter::glt_particle_manager_x->CheckSceneEnded(j.scene_counter)) {
                    stop = true;
                    break;
                }
        }

        if (stop) {
            stop_stage_effect_auth_3d(i);
            stop_stage_effect_glitter(i);
            flags &= ~(1 << i);
        }
    }
}

void x_pv_game_stage::load(int32_t pv_id, bool a4) {
    if (!pv_id)
        return;

    flags &= ~0x02;
    if (a4)
        flags |= 0x02;

    field_8 = 1;
    this->pv_id = pv_id;
    state = 10;

    char buf[0x40];
    sprintf_s(buf, sizeof(buf), "STGPV%03dHRC", pv_id);
    objhrc_set = object_database_get_object_set_id(buf);
    objset_info_storage_load_set(objhrc_set);
}

void x_pv_game_stage::load_change_effect(int32_t curr_stage_effect, int32_t next_stage_effect) {
    curr_stage_effect--;
    next_stage_effect--;

    if (curr_stage_effect < 0 || next_stage_effect < 0)
        return;

    size_t stage_effect_count = stage_resource->num_stage_effect;
    if (curr_stage_effect >= stage_effect_count || next_stage_effect >= stage_effect_count)
        return;

    const pvsr_stage_change_effect& stg_chg_eff = stage_resource->
        stage_change_effect_array[curr_stage_effect * PVSR_STAGE_EFFECT_COUNT + next_stage_effect];
    x_pv_game_stage_change_effect& chg_eff = change_effect[curr_stage_effect][next_stage_effect];

    if (!stg_chg_eff.enable) {
        chg_eff.enable = false;
        return;
    }

    chg_eff.enable = true;
    chg_eff.bars_left = 0;
    chg_eff.bar_count = stg_chg_eff.bar_count > -1 ? stg_chg_eff.bar_count : 2;

    size_t num_auth_3d = stg_chg_eff.num_auth_3d;
    size_t num_glitter = stg_chg_eff.num_glitter;

    chg_eff.auth_3d.resize(num_auth_3d);
    chg_eff.glitter.resize(num_glitter);

    for (size_t i = 0; i < num_auth_3d; i++) {
        const pvsr_auth_3d& stg_chg_eff_auth_3d = stg_chg_eff.auth_3d_array[i];
        x_pv_game_stage_effect_auth_3d& chg_eff_auth_3d = chg_eff.auth_3d[i];

        chg_eff_auth_3d.reset();

        int32_t uid = auth_3d_data_get_uid_by_name(stg_chg_eff_auth_3d.name);

        auto elem = auth_3d_ids.find(uid);
        if (elem == auth_3d_ids.end()) {
            auth_3d_id id = auth_3d_id(uid);
            if (!id.check_not_empty()) {
                chg_eff_auth_3d.id = {};
                chg_eff_auth_3d.name = 0;
                continue;
            }

            id.read_file();
            id.set_enable(false);
            id.set_repeat(false);
            id.set_paused(false);
            id.set_visibility(false);
            id.set_frame_rate(&bpm_frame_rate_control);
            chg_eff_auth_3d.id = id;
            chg_eff_auth_3d.name = stg_chg_eff_auth_3d.name;

            auth_3d_ids.insert({ uid, id });
        }
        else {
            chg_eff_auth_3d.id = elem->second;
            chg_eff_auth_3d.name = stg_chg_eff_auth_3d.name;
        }

        chg_eff_auth_3d.field_1 = true;
        chg_eff_auth_3d.repeat = !!(stg_chg_eff_auth_3d.flags & PVSR_AUTH_3D_REPEAT);
    }

    for (size_t i = 0; i < num_glitter; i++) {
        const pvsr_glitter& stg_chg_eff_glt = stg_chg_eff.glitter_array[i];
        x_pv_game_stage_effect_glitter& chg_eff_glt = chg_eff.glitter[i];

        chg_eff_glt.reset();
        chg_eff_glt.name = stg_chg_eff_glt.name;
        chg_eff_glt.hash = hash_utf8_murmurhash(stg_chg_eff_glt.name);
        chg_eff_glt.force_disp = !!(stg_chg_eff_glt.flags & PVSR_GLITTER_FORCE_DISP);
    }
}

void x_pv_game_stage::reset() {
    flags = 0x01;
    pv_id = 0;
    field_8 = 0;
    state = 0;
    stage_resource = 0;
    stage_resource_file_handler.reset();
    bpm_frame_rate_control.reset();
    stage_auth_3d.clear();
    stage_auth_3d.shrink_to_fit();
    stage_glitter.clear();
    stage_glitter.shrink_to_fit();
    curr_stage_effect = 0;
    next_stage_effect = 0;
    stage_effect_transition_state = 0;

    for (int32_t i = 0; i < X_PV_GAME_STAGE_EFFECT_COUNT; i++)
        effect[i].reset();

    for (int32_t i = 0; i < X_PV_GAME_STAGE_EFFECT_COUNT; i++)
        for (int32_t j = 0; j < X_PV_GAME_STAGE_EFFECT_COUNT; j++)
            change_effect[i][j].reset();

    auth_3d_ids.clear();
}

void x_pv_game_stage::reset_stage_effect() {
    stop_stage_effect();

    for (int32_t i = 8; i <= 9; i++) {
        if (!(flags & (1 << i)))
            continue;

        stop_stage_effect_auth_3d(i);
        stop_stage_effect_glitter(i);
    }

    for (uint32_t& i : stage_glitter)
        Glitter::glt_particle_manager_x->FreeScene(i);
}

void x_pv_game_stage::reset_stage_env() {
    env.reset_env();
}

bool x_pv_game_stage::set_change_effect_frame_part_1() {
    if (!curr_stage_effect || !next_stage_effect)
        return false;

    int32_t curr_stg_eff = curr_stage_effect;
    if (curr_stg_eff >= 1 && curr_stg_eff <= 12)
        curr_stg_eff--;
    else
        curr_stg_eff = 0;

    int32_t next_sta_eff = next_stage_effect;
    if (next_sta_eff >= 1 && next_sta_eff <= 12)
        next_sta_eff--;
    else
        next_sta_eff = 0;

    x_pv_game_stage_change_effect& chg_eff = change_effect[curr_stg_eff][next_sta_eff];
    if (!chg_eff.enable)
        return false;
    return true;
}

void x_pv_game_stage::set_change_effect_frame_part_2(float_t frame) {
    if (!curr_stage_effect || !next_stage_effect)
        return;

    int32_t curr_stg_eff = curr_stage_effect;
    if (curr_stg_eff >= 1 && curr_stg_eff <= 12)
        curr_stg_eff--;
    else
        curr_stg_eff = 0;

    int32_t next_sta_eff = next_stage_effect;
    if (next_sta_eff >= 1 && next_sta_eff <= 12)
        next_sta_eff--;
    else
        next_sta_eff = 0;

    x_pv_game_stage_change_effect& chg_eff = change_effect[curr_stg_eff][next_sta_eff];
    if (!chg_eff.enable)
        return;

    for (x_pv_game_stage_effect_auth_3d& i : chg_eff.auth_3d) {
        auth_3d_id& id = i.id;
        id.set_camera_root_update(false);
        id.set_enable(true);
        id.set_repeat(i.repeat);
        id.set_req_frame(frame);
        id.set_max_frame(id.get_last_frame() - 1.0f);
        id.set_paused(false);
        id.set_visibility(!!(flags & 0x01));
        id.set_frame_rate(&bpm_frame_rate_control);
    }

    for (x_pv_game_stage_effect_glitter& i : chg_eff.glitter) {
        if (i.hash == hash_murmurhash_empty)
            continue;

        i.scene_counter = Glitter::glt_particle_manager_x->LoadSceneEffect(i.hash, true, 0x01);
        if (!i.scene_counter)
            continue;

        Glitter::glt_particle_manager_x->SetSceneFrameRate(i.scene_counter, &bpm_frame_rate_control);
        Glitter::glt_particle_manager_x->SetSceneEffectReqFrame(i.scene_counter, frame);
    }
}

void x_pv_game_stage::set_env(int32_t env_index, float_t end_time, float_t start_time) {
    env.set(env_index, end_time, start_time);
}

void x_pv_game_stage::set_stage_effect(int32_t stage_effect) {
    if (stage_effect < 1 || stage_effect > 12)
        return;

    if (stage_effect >= 8 && stage_effect <= 9) {
        if (flags & (1 << stage_effect)) {
            stop_stage_effect_auth_3d(stage_effect);
            stop_stage_effect_glitter(stage_effect);
        }

        set_stage_effect_auth_3d_frame(stage_effect, 0.0f);
        set_stage_effect_glitter_frame(stage_effect, 0.0f);
        flags |= 1 << stage_effect;
        return;
    }

    if (curr_stage_effect == stage_effect && !next_stage_effect || next_stage_effect)
        return;
    else if (curr_stage_effect) {
        if (!(flags & 0x10)) {
            next_stage_effect = stage_effect;
            stage_effect_transition_state = 0;

            if (((bpm_frame_rate_control.bar_beat->bar - 1) % 2)
                && curr_stage_effect >= 1 && curr_stage_effect <= 12) {
                prj::vector<x_pv_game_stage_effect_auth_3d>& auth_3d = effect[curr_stage_effect - 1ULL].auth_3d;
                for (x_pv_game_stage_effect_auth_3d& i : auth_3d) {
                    auth_3d_id& id = i.id;
                    if (i.field_1)
                        id.set_repeat(false);
                }
            }
            return;
        }
        else if (curr_stage_effect >= 1 && curr_stage_effect <= 12) {
            stop_stage_effect_auth_3d(curr_stage_effect);
            stop_stage_effect_glitter(curr_stage_effect);
        }
    }

    curr_stage_effect = stage_effect;
    set_stage_effect_auth_3d_frame(stage_effect, -1.0f);
    set_stage_effect_glitter_frame(stage_effect, -1.0f);
}

void x_pv_game_stage::set_stage_effect_auth_3d_frame(int32_t stage_effect, float_t frame) {
    if (stage_effect < 1 || stage_effect > 12)
        return;

    if (frame < 0.0f)
        frame = bpm_frame_rate_control.bar_beat->get_bar_current_frame();

    prj::vector<x_pv_game_stage_effect_auth_3d>& auth_3d = effect[stage_effect - 1ULL].auth_3d;
    for (x_pv_game_stage_effect_auth_3d& i : auth_3d) {
        auth_3d_id& id = i.id;
        id.set_camera_root_update(false);
        id.set_enable(true);
        id.set_repeat(true);
        float_t last_frame = id.get_last_frame();
        if (i.repeat && last_frame <= frame) {
            int32_t frame_offset = (int32_t)id.get_frame_offset();
            int32_t _frame = frame_offset + (int32_t)(frame - (float_t)frame_offset) % (int32_t)last_frame;

            frame = (float_t)_frame + (frame - prj::floorf(frame));
        }
        id.set_req_frame(frame);
        id.set_max_frame(-1.0f);
        id.set_paused(false);
        id.set_visibility(true);
        id.set_frame_rate(&bpm_frame_rate_control);
    }
}

void x_pv_game_stage::set_stage_effect_glitter_frame(int32_t stage_effect, float_t frame) {
    if (stage_effect < 1 || stage_effect > 12)
        return;

    if (frame < 0.0f)
        frame = bpm_frame_rate_control.bar_beat->get_bar_current_frame();

    prj::vector<x_pv_game_stage_effect_glitter>& glitter = effect[stage_effect - 1ULL].glitter;
    for (x_pv_game_stage_effect_glitter& i : glitter) {
        i.scene_counter = Glitter::glt_particle_manager_x->LoadSceneEffect(i.hash, true, 0x01);
        if (!i.scene_counter)
            continue;

        Glitter::glt_particle_manager_x->SetSceneFrameRate(i.scene_counter, &bpm_frame_rate_control);

        int32_t life_time = 0;
        Glitter::glt_particle_manager_x->GetSceneFrameLifeTime(i.scene_counter, &life_time);
        if (life_time <= 0)
            continue;

        if (stage_effect < 8 || stage_effect > 9) {
            float_t _life_time = (float_t)life_time;
            while (frame >= _life_time)
                frame -= _life_time;
            Glitter::glt_particle_manager_x->SetSceneEffectReqFrame(i.scene_counter, frame);
        }
        else if (frame < (float_t)life_time)
            Glitter::glt_particle_manager_x->SetSceneEffectReqFrame(i.scene_counter, frame);
        else
            Glitter::glt_particle_manager_x->FreeSceneEffect(i.scene_counter, false);

        i.fade_time_left = i.fade_time;
    }
}

void x_pv_game_stage::stop_stage_change_effect() {
    if (!next_stage_effect)
        return;

    int32_t curr_stg_eff = curr_stage_effect;
    if (curr_stg_eff >= 1 && curr_stg_eff <= 12)
        curr_stg_eff--;
    else
        curr_stg_eff = 0;

    int32_t next_stg_eff = next_stage_effect;
    if (next_stg_eff >= 1 && next_stg_eff <= 12)
        next_stg_eff--;
    else
        next_stg_eff = 0;

    x_pv_game_stage_change_effect& chg_eff = change_effect[curr_stg_eff][next_stg_eff];

    for (x_pv_game_stage_effect_auth_3d& i : chg_eff.auth_3d) {
        auth_3d_id& id = i.id;
        id.set_visibility(false);
        id.set_enable(false);
    }

    for (x_pv_game_stage_effect_glitter& i : chg_eff.glitter)
        if (i.scene_counter) {
            Glitter::glt_particle_manager_x->FreeSceneEffect(i.scene_counter, false);
            i.scene_counter = 0;
        }
}

void x_pv_game_stage::stop_stage_effect() {
    if (curr_stage_effect < 1 || curr_stage_effect > 12)
        return;

    if (next_stage_effect) {
        switch (stage_effect_transition_state) {
        case 0:
            stop_stage_effect_auth_3d(curr_stage_effect);
            break;
        case 1:
            stop_stage_change_effect();
            break;
        }
        stop_stage_effect_glitter(curr_stage_effect);
    }
    else {
        stop_stage_effect_auth_3d(curr_stage_effect);
        stop_stage_effect_glitter(curr_stage_effect);
    }

    curr_stage_effect = 0;
    next_stage_effect = 0;
}

void x_pv_game_stage::stop_stage_effect_auth_3d(int32_t stage_effect) {
    if (stage_effect < 1 || stage_effect > 12)
        return;

    prj::vector<x_pv_game_stage_effect_auth_3d>& auth_3d = effect[stage_effect - 1ULL].auth_3d;
    for (x_pv_game_stage_effect_auth_3d& i : auth_3d) {
        auth_3d_id& id = i.id;
        id.set_visibility(false);
        id.set_enable(false);
    }
}

void x_pv_game_stage::stop_stage_effect_glitter(int32_t stage_effect) {
    if (stage_effect < 1 || stage_effect > 12)
        return;

    prj::vector<x_pv_game_stage_effect_glitter>& glitter = effect[stage_effect - 1ULL].glitter;
    for (x_pv_game_stage_effect_glitter& i : glitter)
        if (i.scene_counter) {
            Glitter::glt_particle_manager_x->FreeSceneEffect(i.scene_counter, false);
            i.scene_counter = 0;
        }
}

void x_pv_game_stage::unload() {
    if (!stage_resource || !pv_id)
        return;

    for (prj::pair<uint32_t, auth_3d_id> i : auth_3d_ids)
        i.second.unload();

    auth_3d_ids.clear();

    for (int32_t i = 0; i < X_PV_GAME_STAGE_EFFECT_COUNT; i++) {
        x_pv_game_stage_effect& eff = effect[i];
        for (x_pv_game_stage_effect_glitter& j : eff.glitter)
            if (j.scene_counter) {
                Glitter::glt_particle_manager_x->FreeSceneEffect(j.scene_counter, false);
                j.scene_counter = 0;
            }
    }

    if (stage_effect_transition_state == 1)
        stop_stage_change_effect();

    for (int32_t i = 0; i < X_PV_GAME_STAGE_EFFECT_COUNT; i++)
        for (int32_t j = 0; j < X_PV_GAME_STAGE_EFFECT_COUNT; j++) {
            x_pv_game_stage_change_effect& chg_eff = change_effect[i][j];
            for (x_pv_game_stage_effect_glitter& k : chg_eff.glitter)
                if (k.scene_counter) {
                    Glitter::glt_particle_manager_x->FreeSceneEffect(k.scene_counter, false);
                    k.scene_counter = 0;
                }

            chg_eff.enable = false;
            chg_eff.auth_3d.clear();
            chg_eff.auth_3d.shrink_to_fit();
            chg_eff.glitter.clear();
            chg_eff.glitter.shrink_to_fit();
            chg_eff.bars_left = 0;
            chg_eff.bar_count = 0;
        }

    for (uint32_t& i : stage_glitter)
        Glitter::glt_particle_manager_x->UnloadEffectGroup(i);

    stage_glitter.clear();

    env.unload();

    objset_info_storage_unload_set(objhrc_set);
    objhrc_set = -1;

    pv_id = 0;

    state = 30;
}

TaskPvGameX::TaskPvGameX() : use(), state(), state_old(), pv_id(), charas() {

}

TaskPvGameX::~TaskPvGameX() {

}

bool TaskPvGameX::init() {
    use = true;
    x_pv_game_str_array_ptr->read();
    return true;
}

bool TaskPvGameX::ctrl() {
    if (state_old == 20)
        ctrl(pv_game_parent_data.curr_time, pv_game_parent_data.delta_time);
    else
        ctrl(0, 0.0f);

    switch (state_old) {
    case 0:
        break;
    case 1: {
        data.pv_data.init(this);

        state_old = 2;
    } break;
    case 2: {
        if (*(int32_t*)(pv_game_ptr + 0x08) != 5)
            break;

        data.load(pv_id, charas);

        state_old = 9;
    } break;
    case 9: {
        if (!data.state || data.state == 10) {
            data.state = 10;
            data.field_1C |= 0x08;
            break;
        }

        state = 10;
        state_old = 10;
    } break;
    case 10: {
        if (data.state != 30)
            break;

        state_old = 19;
    } break;
    case 19: {
        if (pv_game_parent_data.state < 13)
            break;

        Glitter::glt_particle_manager_x->SetPause(false);
        data.stop();
        data.state = 30;
        state_old = 20;
    } break;
    case 20: {
        if (!data.pv_data.play || data.pv_data.pv_end || pv_game_parent_data.pv_state == 2)
            state_old = 21;
        else if (pv_game_parent_data.state < 13) {
            data.stop();
            data.state = 30;
            state_old = 19;
        }
    } break;
    case 21: {
        data.unload();
        stage_data.unload();
        state = 50;

        state_old = 22;
    } break;
    case 22: {
        if (state)
            break;

        Glitter::glt_particle_manager_x->FreeScenes();
    } break;
    }
    return false;
}

bool TaskPvGameX::dest() {
    unload();
    use = false;
    return true;
}

void TaskPvGameX::disp() {
    data.disp();
}

void TaskPvGameX::load(int32_t pv_id, int32_t modules[6]) {
    this->pv_id = pv_id;

    {
#pragma warning(push)
#pragma warning(disable: 26495)
        struct rob_sleeve_data {
            float_t radius;
            float_t cyofs;
            float_t czofs;
            float_t ymin;
            float_t ymax;
            float_t zmin;
            float_t zmax;
            float_t mune_xofs;
            float_t mune_yofs;
            float_t mune_zofs;
            float_t mune_rad;
        };

        static_assert(sizeof(rob_sleeve_data) == 0x2C, "\"rob_sleeve_data\" struct should have a size of 0x2C");

        struct module_data {
            int32_t id;
            int32_t sort_index;
            chara_index chara_index;
            int32_t cos;
            rob_sleeve_data sleeve_l;
            rob_sleeve_data sleeve_r;
            int32_t spr_sel_md_id_spr_set_id;
            int32_t spr_sel_md_id_cmn_spr_set_id;
            int32_t spr_sel_md_id_md_img_id_spr_id;
            int32_t spr_sel_md_id_cmn_md_img_spr_id;
            bool field_78;
            bool field_79;
            prj::string name;
            int32_t field_A0;
            time_t field_A8;
            time_t field_B0;
        };

        static_assert(sizeof(module_data) == 0xB8, "\"module_data\" struct should have a size of 0xB8");
#pragma warning(pop)

        static size_t(FASTCALL* module_data_handler_data_get)() = (size_t(FASTCALL*)()) 0x00000001403F8C30;
        static bool (FASTCALL* module_data_handler__get_module_by_id)(size_t _this, int32_t id, module_data* data)
            = (bool (FASTCALL*)(size_t _this, int32_t id, module_data * data)) 0x00000001403F8B40;

        for (int32_t i = 0; i < 6; i++) {
            module_data mdl;
            if (module_data_handler__get_module_by_id(module_data_handler_data_get(), modules[i], &mdl))
                charas[i] = mdl.chara_index;
            else
                charas[i] = CHARA_MIKU;
        }
    }

    state = 10;
    state_old = 1;
    data.pv_id = pv_id;
    data.stage = &stage_data;

    stage_data.load(pv_id, false);
}

void TaskPvGameX::unload() {
    state_old = 0;

    data.unload();
    data.reset();
    stage_data.unload();
    stage_data.reset();

    pv_id = 0;
}

void TaskPvGameX::ctrl(int64_t curr_time, float_t delta_time) {
    data.ctrl(curr_time, delta_time);

    switch (state) {
    case 10:
        if (stage_data.state == 20)
            state = 20;
        break;
    case 30: {
        if (data.state == 10 || data.state == 20)
            break;

        stop_current_pv();
        data.unload_if_state_is_0();

        state = 20;
    } break;
    case 40: {
        if (data.state == 10 || data.state == 20)
            break;

        stop_current_pv();

        if (!data.state) {
            data.field_1C &= ~0xD1;
            data.state = 0;
            data.stage_effect_index = 0;
            data.next_stage_effect_bar = 0;
            data.pv_data.reset();
        }

        if (!data.state)
            data.state = 20;
        else if (data.state == 10)
            data.field_1C |= 0x08;
        state = 20;
    }
    case 50: {
        if (!data.state && stage_data.state != 20)
            state = 0;
    } break;
    }

    stage_data.ctrl(delta_time);
}

void TaskPvGameX::set_song_effect_alpha_obj_flags(int32_t chara_id, int32_t type, float_t alpha) {
    x_pv_game_data& pv_data = task_pv_game_x->data;

    if (chara_id < 0 || chara_id >= pv_data.play_param->num_chara)
        return;

    mdl::ObjFlags flags;
    switch (type) {
    case 0:
    default:
        flags = mdl::OBJ_ALPHA_ORDER_1;
        break;
    case 1:
        flags = mdl::OBJ_ALPHA_ORDER_2;
        break;
    case 2:
        flags = mdl::OBJ_ALPHA_ORDER_3;
        break;
    }

    for (x_pv_game_song_effect& i : pv_data.effect.song_effect) {
        if (i.chara_id != chara_id)
            continue;

        for (x_pv_game_song_effect_auth_3d& j : i.auth_3d)
            j.id.set_alpha_obj_flags(alpha, flags);

        for (x_pv_game_song_effect_glitter& j : i.glitter)
            Glitter::glt_particle_manager_x->SetSceneEffectExtColor(j.scene_counter,
                -1.0f, -1.0f, -1.0f, alpha, false, hash_murmurhash_empty);
    }
}

void TaskPvGameX::stop_current_pv() {
    data.stop();
}

x_pv_game_str_array::x_pv_game_str_array() : init() {
    lang_sel = 0;
}

x_pv_game_str_array::~x_pv_game_str_array() {

}

const char* x_pv_game_str_array::get_string(int32_t id) {
    auto lang_elem = strings.find(lang_sel);
    if (lang_elem != strings.end()) {
        auto elem = lang_elem->second.find(id);
        if (elem != lang_elem->second.end())
            return elem->second;
    }
    return 0;
}

void x_pv_game_str_array::read() {
    if (init)
        return;

    file_handler.read_file_path("rom/str_array_x.bin", prj::HeapCMallocSystem);
    file_handler.read_now();

    if (!file_handler.get_data() || !file_handler.get_size()) {
        init = true;
        return;
    }

    size_t data = (size_t)file_handler.get_data();
    if (*(uint32_t*)data != reverse_endianness_uint32_t('strx')) {
        init = true;
        return;
    }

    uint32_t lang_count = *(uint32_t*)(data + 0x04);
    size_t lang_data = data + *(uint32_t*)(data + 0x08);
    for (uint32_t i = lang_count; i; i--, lang_data += 0x0C) {
        int32_t lang_id = *(int32_t*)lang_data;
        uint32_t strings_count = *(uint32_t*)(lang_data + 0x04);
        uint32_t strings_offset = *(uint32_t*)(lang_data + 0x08);

        prj::unordered_map<int32_t, const char*>& strings = this->strings[lang_id];
        size_t strings_data = data + strings_offset;
        for (uint32_t j = strings_count; j; j--, strings_data += 0x08)
            strings[*(int32_t*)strings_data] = (const char*)(data + *(uint32_t*)(strings_data + 0x04));
    }

    init = true;
}
