/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "auth_3d.hpp"
#include "canonical_properties.hpp"
#include "file_handler.hpp"

struct struc_8 {
    float_t max_frame;
    void* data;
    size_t data_size;
    bool field_18;
    CanonicalProperties CanProp;
    int32_t field_60;
    int32_t field_64;
    int64_t field_68;
    bool field_70;
    void* binary_data;
    size_t binary_size;
    bool field_88;
};

static_assert(sizeof(struc_8) == 0x90, "\"struc_8\" struct should have a size of 0x90");

struct auth_3d_db_category {
    prj::string name;
    prj::vector<const char*> uids;
};

static_assert(sizeof(auth_3d_db_category) == 0x38, "\"auth_3d_db_category\" struct should have a size of 0x38");

struct auth_3d_uid_file {
    int32_t load_count;
    int32_t uid;
    prj::string file_name;
    int32_t state;
    struc_8 field_30;
    const char* name;
    void* farc; // auth_3d_farc
};

static_assert(sizeof(auth_3d_uid_file) == 0xD0, "\"auth_3d_uid_file\" struct should have a size of 0xD0");

#define AUTH_3D_DATA_COUNT 0x100

struct auth_3d_data_struct {
    prj::map<prj::string, void*> field_0; // struc_87
    prj::sorted_vector<const char*, int32_t> field_10;
    prj::vector<auth_3d_db_category> category;
    prj::vector<auth_3d_db_category> uid;
    p_file_handler auth_3d_db_file_handler;
    prj::vector<p_file_handler*> db_file_handlers;
    prj::sorted_vector<prj::string, prj::string> field_A0;
    int64_t field_E0;
    p_file_handler auth_3d_setting_file_handler;
    prj::vector<void*> farcs; // auth_3d_farc
    prj::vector<auth_3d_uid_file> uid_files;
    prj::vector<int32_t> loaded_ids;
    auth_3d data[AUTH_3D_DATA_COUNT];
    int64_t field_C7938;
};

static_assert(sizeof(auth_3d_data_struct) == 0xC7940, "\"auth_3d_data_struct\" struct should have a size of 0xC7940");

int32_t(FASTCALL* auth_3d_data_get_uid_by_name)(const char* name)
    = (int32_t(FASTCALL*)(const char* name)) 0x00000001401E7EA0;
bool (FASTCALL* auth_3d_data_check_category_loaded)(const char* category_name)
    = (bool (FASTCALL*)(const char* category_name))0x00000001401CBE20;
void (FASTCALL* auth_3d_data_load_category)(const char* category_name)
    = (void (FASTCALL*)(const char* category_name))0x00000001401CBE80;
void (FASTCALL* auth_3d_data_unload_category)(const char* category_name)
    = (void (FASTCALL*)(const char* category_name))0x00000001401CBAB0;

static auth_3d_data_struct* auth_3d_data = (auth_3d_data_struct*)0x0000000140EDA950;

static int32_t auth_3d_get_auth_3d_object_index(auth_3d* auth, object_info object_info, int32_t instance);
static const mat4* auth_3d_get_auth_3d_object_mat(auth_3d* auth, size_t index);
static const mat4* auth_3d_get_auth_3d_object_hrc_bone_mats(auth_3d* auth, size_t index);
static int32_t auth_3d_get_auth_3d_object_hrc_index(auth_3d* auth, object_info obj_info, int32_t instance);

auth_3d_id::auth_3d_id(int32_t uid) : id() {
    ((auth_3d_id * (FASTCALL*)(auth_3d_id * This, int32_t uid))0x00000001401CDC80)(this, uid);
}

auth_3d_id::auth_3d_id(const char* name) : id() {
    ((auth_3d_id * (FASTCALL*)(auth_3d_id * This, int32_t uid))0x00000001401CDC80)(
        this, auth_3d_data_get_uid_by_name(name));
}

bool auth_3d_id::check_not_empty() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->id != -1;
    }
    return false;
}

bool auth_3d_id::check_loading() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->state == 1;
    }
    return false;
}

bool auth_3d_id::check_loaded() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->state == 2;
    }
    return true;
}

auth_3d* auth_3d_id::get_auth_3d() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth;
    }
    return 0;
}

int32_t auth_3d_id::get_chara_id() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->chara_id;
    }
    return -1;
}

const mat4* auth_3d_id::get_auth_3d_object_mat(size_t index, bool hrc) {
    if (id < 0 || (id & 0x7FFF) >= AUTH_3D_DATA_COUNT)
        return 0;

    auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
    if (auth->id != id || !auth->enable)
        return 0;

    if (hrc)
        return auth_3d_get_auth_3d_object_hrc_bone_mats(auth, index);
    return auth_3d_get_auth_3d_object_mat(auth, index);
}

bool auth_3d_id::get_enable() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->enable;
    }
    return false;
}

bool auth_3d_id::get_ended() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->ended;
    }
    return true;
}

float_t auth_3d_id::get_frame() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->frame;
    }
    return 0.0f;
}

float_t auth_3d_id::get_frame_offset() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->frame_offset;
    }
    return 0.0f;
}

float_t auth_3d_id::get_last_frame() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->last_frame;
    }
    return 0.0f;
}

bool auth_3d_id::get_left_right_reverse() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->left_right_reverse;
    }
    return false;
}

float_t auth_3d_id::get_play_control_begin() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->play_control.begin;
    }
    return 0.0f;
}

float_t auth_3d_id::get_play_control_size() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->play_control.size;
    }
    return 0.0f;
}

bool auth_3d_id::get_paused() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->paused;
    }
    return false;
}

bool auth_3d_id::get_repeat() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->repeat;
    }
    return false;
}

int32_t auth_3d_id::get_uid() {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            return auth->uid;
    }
    return -1;
}

void auth_3d_id::read_file() {
    ((void (FASTCALL*)(auth_3d_id * This))0x00000001401E0F80)(this);
}

void auth_3d_id::set_alpha_obj_flags(float_t alpha, mdl::ObjFlags obj_flags) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id) {
            auth->alpha = alpha;
            auth->obj_flags = obj_flags;
        }
    }
}

void auth_3d_id::set_camera_root_update(bool value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->camera_root_update = value;
    }
}

void auth_3d_id::set_chara_id(int32_t value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->chara_id = value;
    }
}

void auth_3d_id::set_chara_item(bool value) { // X
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->chara_item = value;
    }
}

void auth_3d_id::set_enable(bool value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->enable = value;
    }
}

void auth_3d_id::set_frame_rate(FrameRateControl* value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            if (value)
                auth->frame_rate = value;
            else
                auth->frame_rate = get_sys_frame_rate();
    }
}

void auth_3d_id::set_last_frame(float_t value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->last_frame = value;
    }
}

void auth_3d_id::set_left_right_reverse(bool value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->left_right_reverse = value;
    }
}

void auth_3d_id::set_mat(const mat4& value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->mat = value;
    }
}

void auth_3d_id::set_max_frame(float_t value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->max_frame = value;
    }
}

void auth_3d_id::set_paused(bool value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->paused = value;
    }
}

void auth_3d_id::set_pos(int32_t value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->pos = value;
    }
}

void auth_3d_id::set_reflect(bool value) { // X
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->reflect = value;
    }
}

void auth_3d_id::set_repeat(bool value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->repeat = value;
    }
}

void auth_3d_id::set_req_frame(float_t value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id) {
            auth->req_frame = value;
            auth->frame_changed = true;
        }
    }
}

void auth_3d_id::set_shadow(bool value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->shadow = value;
    }
}

void auth_3d_id::set_src_dst_chara(int32_t src_chara, int32_t dst_chara) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id) {
            auth->src_chara = (chara_index)src_chara;
            auth->dst_chara = (chara_index)dst_chara;
        }
    }
}

void auth_3d_id::set_visibility(bool value) {
    if (id >= 0 && ((id & 0x7FFF) < AUTH_3D_DATA_COUNT)) {
        auth_3d* auth = &auth_3d_data->data[id & 0x7FFF];
        if (auth->id == id)
            auth->visible = value;
    }
}

void auth_3d_id::unload() {
    ((void (FASTCALL*)(auth_3d_id * This))0x00000001401D0190)(this);
}

int32_t auth_3d_data_get_auth_3d_id(uint32_t uid, object_info obj_info,
    int32_t* object_index, bool* hrc, int32_t instance) {
    for (int32_t& i : auth_3d_data->loaded_ids) {
        if (i < 0 || (i & 0x7FFF) >= AUTH_3D_DATA_COUNT)
            continue;

        auth_3d* auth = &auth_3d_data->data[i & 0x7FFF];
        if (auth->id != i || !auth->enable)
            continue;

        bool ret = false;
        if (uid != -1) {
            if (auth->uid != uid)
                continue;
            ret = true;
        }

        int32_t obj_index = auth_3d_get_auth_3d_object_index(auth, obj_info, instance);
        if (obj_index >= 0) {
            if (object_index)
                *object_index = obj_index;
            if (hrc)
                *hrc = false;
            return i;
        }

        int32_t obj_hrc_index = auth_3d_get_auth_3d_object_hrc_index(auth, obj_info, instance);
        if (obj_hrc_index >= 0) {
            if (object_index)
                *object_index = obj_hrc_index;
            if (hrc)
                *hrc = true;
            return i;
        }

        if (ret)
            return -1;
    }
    return -1;
}

static int32_t auth_3d_get_auth_3d_object_index(auth_3d* auth, object_info object_info, int32_t instance) {
    int32_t obj_instance = 0;
    for (auth_3d_object& i : auth->object)
        if (object_info == i.object_info) {
            if (obj_instance == instance)
                return (int32_t)(&i - auth->object.data());
            obj_instance++;
        }
    return -1;
}

static const mat4* auth_3d_get_auth_3d_object_mat(auth_3d* auth, size_t index) {
    if (index >= auth->object.size())
        return 0;
    return &auth->object[index].model_transform.mat;
}

static const mat4* auth_3d_get_auth_3d_object_hrc_bone_mats(auth_3d* auth, size_t index) {
    if (index >= auth->object_hrc.size() || auth->object_hrc[index].node_bone_mats.size() < 1)
        return 0;
    return auth->object_hrc[index].node_bone_mats.data();
}

static int32_t auth_3d_get_auth_3d_object_hrc_index(auth_3d* auth, object_info obj_info, int32_t instance) {
    if (instance < 0) {
        for (auth_3d_object_hrc& i : auth->object_hrc)
            if (obj_info == i.object_info)
                return (int32_t)(&i - auth->object_hrc.data());
    }
    else {
        int32_t obj_instance = 0;
        for (auth_3d_object_hrc& i : auth->object_hrc)
            if (obj_info == i.object_info) {
                if (obj_instance == instance)
                    return (int32_t)(&i - auth->object_hrc.data());
                obj_instance++;
            }
    }
    return -1;
}
