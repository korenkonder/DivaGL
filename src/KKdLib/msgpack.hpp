/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <string>
#include <vector>
#include "default.hpp"
#include "prj/vector_pair.hpp"

enum msgpack_type : uint32_t {
    MSGPACK_NONE = 0,
    MSGPACK_NULL,
    MSGPACK_BOOL,
    MSGPACK_INT8,
    MSGPACK_UINT8,
    MSGPACK_INT16,
    MSGPACK_UINT16,
    MSGPACK_INT32,
    MSGPACK_UINT32,
    MSGPACK_INT64,
    MSGPACK_UINT64,
    MSGPACK_FLOAT32,
    MSGPACK_FLOAT64,
    MSGPACK_STRING,
    MSGPACK_ARRAY,
    MSGPACK_MAP,
};

struct msgpack;

typedef std::vector<msgpack> msgpack_array;
typedef prj::vector_pair<std::string, msgpack> msgpack_map;

union msgpack_data {
    bool b;
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    float_t f32;
    double_t f64;
    std::string* str;
    msgpack_array* arr;
    msgpack_map* map;
};

struct msgpack {
    msgpack_type type;
    msgpack_data data;

    msgpack* get_by_index(size_t index);
    void set_by_index(msgpack* m, size_t index);
    msgpack* get_by_name(const char* name);
    void set_by_name(const char* name, msgpack* m);
    msgpack* append(const char* name, msgpack* m);
    msgpack* append(const char* name, msgpack& m);
    msgpack* append(const char* name, msgpack&& m);
    msgpack* read(const char* name);
    msgpack* read(const char* name, msgpack_type type);
    msgpack* read_array(const char* name = 0);
    msgpack* read_map(const char* name = 0);
    bool read_bool(const char* name = 0);
    int8_t read_int8_t(const char* name = 0);
    uint8_t read_uint8_t(const char* name = 0);
    int16_t read_int16_t(const char* name = 0);
    uint16_t read_uint16_t(const char* name = 0);
    int32_t read_int32_t(const char* name = 0);
    uint32_t read_uint32_t(const char* name = 0);
    int64_t read_int64_t(const char* name = 0);
    uint64_t read_uint64_t(const char* name = 0);
    float_t read_float_t(const char* name = 0);
    double_t read_double_t(const char* name = 0);
    char* read_utf8_string(const char* name = 0);
    wchar_t* read_utf16_string(const char* name = 0);
    std::string read_string(const char* name = 0);
    std::wstring read_wstring(const char* name = 0);

    msgpack& operator=(const msgpack& m);

    inline msgpack() : data() {
        type = MSGPACK_NULL;
    }

    inline msgpack(const msgpack& m) : type(), data() {
        *this = m;
    }

    inline msgpack(msgpack_array& val) : data() {
        type = MSGPACK_ARRAY;
        data.arr = new msgpack_array;
        data.arr->assign(val.begin(), val.end());
        val.clear();
        val.shrink_to_fit();
    }

    inline msgpack(msgpack_array&& val) : data() {
        type = MSGPACK_ARRAY;
        data.arr = new msgpack_array;
        data.arr->assign(val.begin(), val.end());
        val.clear();
        val.shrink_to_fit();
    }

    inline msgpack(msgpack_map& val) : data() {
        type = MSGPACK_MAP;
        data.map = new msgpack_map;
        data.map->assign(val.begin(), val.end());
        val.clear();
        val.shrink_to_fit();
    }
    inline msgpack(msgpack_map&& val) : data() {
        type = MSGPACK_MAP;
        data.map = new msgpack_map;
        data.map->assign(val.begin(), val.end());
        val.clear();
        val.shrink_to_fit();
    }

    inline msgpack(bool val) : data() {
        type = MSGPACK_BOOL;
        data.b = val;
    }

    inline msgpack(int8_t val) : data() {
        type = MSGPACK_INT8;
        data.i64 = val;
    }

    inline msgpack(uint8_t val) : data() {
        type = MSGPACK_UINT8;
        data.u64 = val;
    }

    inline msgpack(int16_t val) : data() {
        type = MSGPACK_INT16;
        data.i64 = val;
    }

    inline msgpack(uint16_t val) : data() {
        type = MSGPACK_UINT16;
        data.u64 = val;
    }

    inline msgpack(int32_t val) : data() {
        type = MSGPACK_INT32;
        data.i64 = val;
    }

    inline msgpack(uint32_t val) : data() {
        type = MSGPACK_UINT32;
        data.u64 = val;
    }

    inline msgpack(int64_t val) : data() {
        type = MSGPACK_INT64;
        data.i64 = val;
    }

    inline msgpack(uint64_t val) : data() {
        type = MSGPACK_UINT64;
        data.u64 = val;
    }

    inline msgpack(float_t val) : data() {
        type = MSGPACK_FLOAT32;
        data.f32 = val;
    }

    inline msgpack(double_t val) : data() {
        type = MSGPACK_FLOAT64;
        data.f64 = val;
    }

    inline msgpack(const char* val) : data() {
        type = MSGPACK_STRING;
        data.str = new std::string;
        if (val)
            data.str->assign(val);
    }

    inline msgpack(const wchar_t* val) : data() {
        type = MSGPACK_STRING;
        data.str = new std::string;
        if (val) {
            char* temp = utf16_to_utf8(val);
            data.str->assign(temp);
            free_def(temp);
        }
    }

    inline msgpack(std::string& val) : data() {
        type = MSGPACK_STRING;
        data.str = new std::string;
        if (val.size())
            data.str->assign(val);
    }

    inline msgpack(std::string&& val) : data() {
        type = MSGPACK_STRING;
        data.str = new std::string;
        if (val.size())
            data.str->assign(val);
    }

    inline msgpack(std::wstring& val) : data() {
        type = MSGPACK_STRING;
        data.str = new std::string;
        if (val.size()) {
            char* temp = utf16_to_utf8(val.c_str());
            data.str->assign(temp);
            free_def(temp);
        }
    }

    inline msgpack(std::wstring&& val) : data() {
        type = MSGPACK_STRING;
        data.str = new std::string;
        if (val.size()) {
            char* temp = utf16_to_utf8(val.c_str());
            data.str->assign(temp);
            free_def(temp);
        }
    }

    inline ~msgpack() {
        switch (type) {
        case MSGPACK_STRING:
            delete data.str;
            break;
        case MSGPACK_ARRAY:
            delete data.arr;
            break;
        case MSGPACK_MAP:
            delete data.map;
            break;
        }
    }

    inline bool check_null() {
        if (type == MSGPACK_ARRAY)
            return !data.arr->size();
        else if (type == MSGPACK_MAP)
            return !data.map->size();
        return type == MSGPACK_NONE;
    }

    inline bool check_not_null() {
        if (type == MSGPACK_ARRAY)
            return !!data.arr->size();
        else if (type == MSGPACK_MAP)
            return !!data.map->size();
        return type != MSGPACK_NONE;
    }

    inline void clear() {
        switch (type) {
        case MSGPACK_STRING:
            delete data.str;
            break;
        case MSGPACK_ARRAY:
            delete data.arr;
            break;
        case MSGPACK_MAP:
            delete data.map;
            break;
        }
        type = MSGPACK_NONE;
        data = {};
    }
};
