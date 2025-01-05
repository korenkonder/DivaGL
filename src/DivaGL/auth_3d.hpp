/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "mdl/draw_object.hpp"
#include "frame_rate_control.hpp"
#include "object.hpp"

struct auth_3d_id {
    int32_t id;

    inline auth_3d_id() {
        id = -1;
    }

    inline bool is_null() {
        return id == -1;
    }

    inline bool not_null() {
        return id != -1;
    }

    auth_3d_id(int32_t uid);
    auth_3d_id(const char* name);
    bool check_not_empty();
    bool check_loading();
    bool check_loaded();
    void* get_auth_3d();
    const mat4* get_auth_3d_object_mat(size_t index, bool hrc);
    int32_t get_chara_id();
    bool get_enable();
    bool get_ended();
    float_t get_frame();
    float_t get_frame_offset();
    float_t get_last_frame();
    bool get_left_right_reverse();
    bool get_paused();
    float_t get_play_control_begin();
    float_t get_play_control_size();
    bool get_repeat();
    int32_t get_uid();
    void read_file();
    void set_alpha_obj_flags(float_t alpha, mdl::ObjFlags obj_flags);
    void set_camera_root_update(bool value);
    void set_chara_id(int32_t value);
    void set_chara_item(bool value);
    void set_enable(bool value);
    void set_frame_rate(FrameRateControl* value);
    void set_last_frame(float_t value);
    void set_left_right_reverse(bool value);
    void set_mat(const mat4& value);
    void set_max_frame(float_t value);
    void set_paused(bool value);
    void set_pos(int32_t value);
    void set_reflect(bool value);
    void set_repeat(bool value);
    void set_req_frame(float_t value);
    void set_shadow(bool value);
    void set_src_dst_chara(int32_t src_chara, int32_t dst_chara);
    void set_visibility(bool value);
    void unload();
};

static_assert(sizeof(auth_3d_id) == 0x04, "\"auth_3d_id\" struct should have a size of 0x04");

inline bool operator >(const auth_3d_id& left, const auth_3d_id& right) {
    return left.id > right.id;
}

inline bool operator <(const auth_3d_id& left, const auth_3d_id& right) {
    return left.id < right.id;
}

inline bool operator >=(const auth_3d_id& left, const auth_3d_id& right) {
    return left.id >= right.id;
}

inline bool operator <=(const auth_3d_id& left, const auth_3d_id& right) {
    return left.id <= right.id;
}

inline bool operator ==(const auth_3d_id& left, const auth_3d_id& right) {
    return left.id == right.id;
}

inline bool operator !=(const auth_3d_id& left, const auth_3d_id& right) {
    return left.id != right.id;
}

extern int32_t(FASTCALL* auth_3d_data_get_uid_by_name)(const char* name);
extern bool (FASTCALL* auth_3d_data_check_category_loaded)(const char* category_name);
extern void (FASTCALL* auth_3d_data_load_category)(const char* category_name);
extern void (FASTCALL* auth_3d_data_unload_category)(const char* category_name);

extern int32_t auth_3d_data_get_auth_3d_id(uint32_t uid, object_info obj_info,
    int32_t* object_index, bool* hrc, int32_t instance);

extern void auth_3d_patch();
