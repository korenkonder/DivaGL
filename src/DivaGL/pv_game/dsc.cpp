/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "dsc.hpp"
#include "../../KKdLib/f2/struct.hpp"

struct dsc_func {
    int32_t id;
    int32_t length;
    const char* name;
};

static const dsc_func dsc_x_func_data[] = {
    { 0, 0, u8"END" },
    { 1, 1, u8"TIME" },
    { 2, 4, u8"" },
    { 3, 2, u8"" },
    { 4, 2, u8"" },
    { 5, 2, u8"" },
    { 6, 12, u8"" },
    { 7, 4, u8"" },
    { 8, 2, u8"" },
    { 9, 6, u8"" },
    { 10, 2, u8"" },
    { 11, 1, u8"" },
    { 12, 6, u8"" },
    { 13, 2, u8"" },
    { 14, 2, u8"CHANGE_FIELD" },
    { 15, 1, u8"" },
    { 16, 3, u8"" },
    { 17, 2, u8"" },
    { 18, 3, u8"" },
    { 19, 5, u8"" },
    { 20, 5, u8"" },
    { 21, 4, u8"" },
    { 22, 4, u8"" },
    { 23, 5, u8"" },
    { 24, 2, u8"" },
    { 25, 0, u8"" },
    { 26, 2, u8"" },
    { 27, 4, u8"" },
    { 28, 2, u8"" },
    { 29, 2, u8"" },
    { 30, 1, u8"" },
    { 31, 21, u8"" },
    { 32, 0, u8"" },
    { 33, 3, u8"" },
    { 34, 2, u8"" },
    { 35, 5, u8"" },
    { 36, 1, u8"" },
    { 37, 1, u8"" },
    { 38, 7, u8"" },
    { 39, 1, u8"" },
    { 40, 1, u8"" },
    { 41, 2, u8"" },
    { 42, 1, u8"" },
    { 43, 2, u8"" },
    { 44, 1, u8"" },
    { 45, 2, u8"" },
    { 46, 3, u8"" },
    { 47, 3, u8"" },
    { 48, 1, u8"" },
    { 49, 2, u8"" },
    { 50, 2, u8"" },
    { 51, 3, u8"" },
    { 52, 6, u8"" },
    { 53, 6, u8"" },
    { 54, 1, u8"" },
    { 55, 1, u8"" },
    { 56, 2, u8"" },
    { 57, 3, u8"" },
    { 58, 1, u8"" },
    { 59, 2, u8"" },
    { 60, 2, u8"" },
    { 61, 4, u8"" },
    { 62, 4, u8"" },
    { 63, 1, u8"" },
    { 64, 2, u8"" },
    { 65, 1, u8"PV_BRANCH_MODE" },
    { 66, 2, u8"" },
    { 67, 1, u8"" },
    { 68, 1, u8"" },
    { 69, 3, u8"" },
    { 70, 3, u8"" },
    { 71, 3, u8"" },
    { 72, 2, u8"" },
    { 73, 1, u8"" },
    { 74, 2, u8"" },
    { 75, 2, u8"" },
    { 76, 2, u8"" },
    { 77, 2, u8"" },
    { 78, 2, u8"" },
    { 79, 2, u8"" },
    { 80, 3, u8"" },
    { 81, 2, u8"" },
    { 82, 2, u8"" },
    { 83, 2, u8"" },
    { 84, 1, u8"CREDIT_TITLE" },
    { 85, 1, u8"BAR_POINT" },
    { 86, 1, u8"BEAT_POINT" },
    { 87, 0, u8"" },
    { 88, 2, u8"" },
    { 89, 3, u8"" },
    { 90, 3, u8"" },
    { 91, 11, u8"" },
    { 92, 3, u8"" },
    { 93, 2, u8"" },
    { 94, 3, u8"" },
    { 95, 3, u8"" },
    { 96, 4, u8"" },
    { 97, 1, u8"" },
    { 98, 1, u8"" },
    { 99, 3, u8"" },
    { 100, 3, u8"" },
    { 101, 4, u8"" },
    { 102, 1, u8"" },
    { 103, 112, u8"" },
    { 104, 1, u8"" },
    { 105, 1, u8"" },
    { 106, 7, u8"" },
    { 107, 2, u8"" },
    { 108, 2, u8"" },
    { 109, 2, u8"" },
    { 110, 5, u8"" },
    { 111, 2, u8"STAGE_EFFECT" },
    { 112, 3, u8"SONG_EFFECT" },
    { 113, 3, u8"SONG_EFFECT_ATTACH" },
    { 114, 2, u8"" },
    { 115, 2, u8"" },
    { 116, 2, u8"SET_STAGE_EFFECT_ENV" },
    { 117, 2, u8"" },
    { 118, 2, u8"" },
    { 119, 2, u8"" },
    { 120, 2, u8"" },
    { 121, 3, u8"" },
    { 122, 5, u8"" },
    { 123, 3, u8"" },
    { 124, 3, u8"CHARA_EFFECT" },
    { 125, 2, u8"" },
    { 126, 3, u8"" },
    { 127, 2, u8"" },
    { 128, 2, u8"" },
    { 129, 2, u8"" },
    { 130, 2, u8"" },
    { 131, 2, u8"" },
    { 132, 2, u8"" },
    { 133, 2, u8"" },
    { 134, 2, u8"" },
    { 135, 2, u8"" },
    { 136, 2, u8"" },
    { 137, 2, u8"" },
    { 138, 2, u8"" },
    { 139, 2, u8"" },
    { 140, 2, u8"" },
    { 141, 2, u8"" },
    { 142, 2, u8"" },
    { 143, 2, u8"" },
    { 144, 2, u8"" },
    { 145, 2, u8"" },
    { 146, 2, u8"" },
    { 147, 2, u8"" },
    { 148, 2, u8"" },
    { 149, 2, u8"" },
    { 150, 2, u8"" },
    { 151, 5, u8"" },
    { 152, 9, u8"" },
    { 153, 9, u8"" },
    { 154, 5, u8"" },
    { 155, 3, u8"" },
    { 156, 7, u8"" },
    { 157, 7, u8"" },
    { 158, 6, u8"" },
    { 159, 9, u8"" },
    { 160, 5, u8"" },
    { 161, 5, u8"" },
    { 162, 2, u8"" },
};

dsc::dsc() {

}

dsc::~dsc() {

}

int32_t* dsc::get_func_data(dsc_data* data) {
    return data_buffer.data() + data->data_offset;
}

bool dsc::parse(const void* data, size_t size) {
    if (!data || !(size / 4))
        return false;

    int32_t* data_dsc = (int32_t*)data;

    f2_struct st;
    st.read(data, size);
    data_dsc = (int32_t*)st.data.data();
    if (st.header.attrib.get_big_endian())
        for (size_t i = size / 4; i; i--, data_dsc++)
            *data_dsc = load_reverse_endianness_uint32_t(data_dsc);

    data_dsc = (int32_t*)st.data.data();
    if (*data_dsc++ != 0x13120420)
        return false;

    data_dsc++;

    size /= 4;

    size_t func_count = 0;
    size_t data_size = 0;
    int32_t* func_data = data_dsc;
    for (size_t i = 0; i < size; i++) {
        if (!*func_data) {
            size = i;
            break;
        }

        int32_t func_length = dsc_get_func_length(*func_data++);
        if (func_length < 0) {
            size = i;
            break;
        }

        data_size += func_length;
        func_data += func_length;
        i += func_length;
        func_count++;
    }

    this->data.reserve(func_count);
    data_buffer.reserve(data_size);

    func_data = data_dsc;
    for (size_t i = 0; i < func_count; i++) {
        dsc_data data;
        data.func = *func_data++;
        data.data_offset = (uint32_t)data_buffer.size();
        this->data.push_back(data);

        int32_t func_length = dsc_get_func_length(data.func);
        data_buffer.resize((size_t)data.data_offset + func_length);
        memmove(data_buffer.data() + data.data_offset, func_data, sizeof(uint32_t) * func_length);
        func_data += func_length;
    }
    return true;
}

const char* dsc_get_func_name(int32_t id) {
    return id > -1 && id < 163 ? dsc_x_func_data[id].name : 0;
}

int32_t dsc_get_func_length(int32_t id) {
    return id > -1 && id < 163 ? dsc_x_func_data[id].length : 0;
}
