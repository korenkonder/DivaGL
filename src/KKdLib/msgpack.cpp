/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "msgpack.hpp"

msgpack* msgpack::get_by_index(size_t index) {
    if (type != MSGPACK_ARRAY)
        return 0;

    msgpack_array* ptr = data.arr;
    if (index < ptr->size())
        return &ptr->data()[index];
    return 0;
}

void msgpack::set_by_index(msgpack* m, size_t index) {
    if (type != MSGPACK_ARRAY)
        return;

    msgpack_array* ptr = data.arr;
    if (index < ptr->size()) {
        msgpack& msg = ptr->data()[index];
        msg = *m;
    }
}

msgpack* msgpack::get_by_name(const char* name) {
    if (type != MSGPACK_MAP)
        return 0;

    msgpack_map* ptr = data.map;
    for (auto& i : *ptr)
        if (!i.first.compare(name))
            return &i.second;

    return 0;
}

void msgpack::set_by_name(const char* name, msgpack* m) {
    if (type != MSGPACK_MAP)
        return;

    msgpack_map* ptr = data.map;
    for (auto& i : *ptr)
        if (!i.first.compare(name)) {
            i.second = *m;
            return;
        }

    append(name, m);
}

msgpack* msgpack::append(const char* name, msgpack* m) {
    if (type != MSGPACK_MAP)
        return 0;

    msgpack* tm = get_by_name(name);
    if (tm) {
        *tm = *m;
        m->clear();
        return tm;
    }
    else {
        data.map->push_back(name, *m);
        m->clear();
        return &data.map->back().second;
    }
}

msgpack* msgpack::append(const char* name, msgpack& m) {
    if (type != MSGPACK_MAP)
        return 0;

    msgpack* tm = get_by_name(name);
    if (tm) {
        *tm = m;
        m.clear();
        return tm;
    }
    else {
        data.map->push_back(name, m);
        m.clear();
        return &data.map->back().second;
    }
}

msgpack* msgpack::append(const char* name, msgpack&& m) {
    if (type != MSGPACK_MAP)
        return 0;

    msgpack* tm = get_by_name(name);
    if (tm) {
        *tm = m;
        m.clear();
        return tm;
    }
    else {
        data.map->push_back(name, m);
        m.clear();
        return &data.map->back().second;
    }
}

msgpack* msgpack::read(const char* name) {
    if (!this)
        return 0;

    return name ? get_by_name(name) : this;
}

msgpack* msgpack::read(const char* name, msgpack_type type) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (m) {
        if (m->type == type)
            return m;
        else if (m->type >= MSGPACK_INT8 && m->type <= MSGPACK_FLOAT64 && m->type <= type)
            return m;
        return m;
    }
    return 0;
}

msgpack* msgpack::read_array(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (m && m->type == MSGPACK_ARRAY)
        return m;
    return 0;
}

msgpack* msgpack::read_map(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (m && m->type == MSGPACK_MAP)
        return m;
    return 0;
}

bool msgpack::read_bool(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (m && m->type == MSGPACK_BOOL)
        return m->data.b;
    return 0;
}

int8_t msgpack::read_int8_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return m->data.i8;
    case MSGPACK_UINT8:
        return (int8_t)m->data.u8;
    }
    return 0;
}

uint8_t msgpack::read_uint8_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return (uint8_t)m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    }
    return 0;
}

int16_t msgpack::read_int16_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    case MSGPACK_INT16:
        return m->data.i16;
    case MSGPACK_UINT16:
        return (int16_t)m->data.u16;
    }
    return 0;
}

uint16_t msgpack::read_uint16_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return (uint16_t)(int16_t)m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    case MSGPACK_INT16:
        return (uint16_t)m->data.i16;
    case MSGPACK_UINT16:
        return m->data.u16;
    }
    return 0;
}

int32_t msgpack::read_int32_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    case MSGPACK_INT16:
        return m->data.i16;
    case MSGPACK_UINT16:
        return m->data.u16;
    case MSGPACK_INT32:
        return m->data.i32;
    case MSGPACK_UINT32:
        return (int32_t)m->data.u32;
    }
    return 0;
}

uint32_t msgpack::read_uint32_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return (uint32_t)(int32_t)m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    case MSGPACK_INT16:
        return (uint32_t)(int32_t)m->data.i16;
    case MSGPACK_UINT16:
        return m->data.u16;
    case MSGPACK_INT32:
        return (uint32_t)(int32_t)m->data.i32;
    case MSGPACK_UINT32:
        return m->data.u32;
    }
    return 0;
}

int64_t msgpack::read_int64_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    case MSGPACK_INT16:
        return m->data.i16;
    case MSGPACK_UINT16:
        return m->data.u16;
    case MSGPACK_INT32:
        return m->data.i32;
    case MSGPACK_UINT32:
        return m->data.u32;
    case MSGPACK_INT64:
        return m->data.i64;
    case MSGPACK_UINT64:
        return (int64_t)m->data.u64;
    }
    return 0;
}

uint64_t msgpack::read_uint64_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return (uint64_t)(int64_t)m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    case MSGPACK_INT16:
        return (uint64_t)(int64_t)m->data.i16;
    case MSGPACK_UINT16:
        return m->data.u16;
    case MSGPACK_INT32:
        return (uint64_t)(int64_t)m->data.i32;
    case MSGPACK_UINT32:
        return m->data.u32;
    case MSGPACK_INT64:
        return (uint64_t)m->data.i64;
    case MSGPACK_UINT64:
        return m->data.u64;
    }
    return 0;
}

float_t msgpack::read_float_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    case MSGPACK_INT16:
        return m->data.i16;
    case MSGPACK_UINT16:
        return m->data.u16;
    case MSGPACK_INT32:
        return (float_t)m->data.i32;
    case MSGPACK_UINT32:
        return (float_t)m->data.u32;
    case MSGPACK_INT64:
        return (float_t)m->data.i64;
    case MSGPACK_UINT64:
        return (float_t)m->data.u64;
    case MSGPACK_FLOAT32:
        return m->data.f32;
    case MSGPACK_FLOAT64:
        return (float_t)m->data.f64;
    }
    return 0;
}

double_t msgpack::read_double_t(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (!m)
        return 0;

    switch (m->type) {
    case MSGPACK_INT8:
        return m->data.i8;
    case MSGPACK_UINT8:
        return m->data.u8;
    case MSGPACK_INT16:
        return m->data.i16;
    case MSGPACK_UINT16:
        return m->data.u16;
    case MSGPACK_INT32:
        return m->data.i32;
    case MSGPACK_UINT32:
        return m->data.u32;
    case MSGPACK_INT64:
        return (double_t)m->data.i64;
    case MSGPACK_UINT64:
        return (double_t)m->data.u64;
    case MSGPACK_FLOAT32:
        return m->data.f32;
    case MSGPACK_FLOAT64:
        return m->data.f64;
    }
    return 0;
}

char* msgpack::read_utf8_string(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (m && m->type == MSGPACK_STRING) {
        size_t length = m->data.str->size();
        char* val = force_malloc<char>(length + 1);
        memcpy(val, m->data.str->c_str(), length);
        val[length] = 0;
        return val;
    }
    return 0;
}

wchar_t* msgpack::read_utf16_string(const char* name) {
    if (!this)
        return 0;

    msgpack* m = name ? get_by_name(name) : this;
    if (m && m->type == MSGPACK_STRING)
        return utf8_to_utf16(m->data.str->c_str());
    return 0;
}

std::string msgpack::read_string(const char* name) {
    if (!this)
        return {};

    msgpack* m = name ? get_by_name(name) : this;
    if (m && m->type == MSGPACK_STRING)
        return *m->data.str;
    return {};
}

std::wstring msgpack::read_wstring(const char* name) {
    if (!this)
        return {};

    msgpack* m = name ? get_by_name(name) : this;
    if (m && m->type == MSGPACK_STRING)
        return utf8_to_utf16(*m->data.str);
    return {};
}

msgpack& msgpack::operator=(const msgpack& m) {
    switch (m.type) {
    case MSGPACK_BOOL:
    case MSGPACK_INT8:
    case MSGPACK_UINT8:
    case MSGPACK_INT16:
    case MSGPACK_UINT16:
    case MSGPACK_INT32:
    case MSGPACK_UINT32:
    case MSGPACK_INT64:
    case MSGPACK_UINT64:
    case MSGPACK_FLOAT32:
    case MSGPACK_FLOAT64:
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

        memcpy(&data, &m.data, sizeof(data));
        break;
    case MSGPACK_STRING:
        switch (type) {
        case MSGPACK_STRING:
            break;
        case MSGPACK_ARRAY:
            delete data.arr;
            data.str = new std::string;
            break;
        case MSGPACK_MAP:
            delete data.map;
            data.str = new std::string;
            break;
        default:
            data.str = new std::string;
            break;
        }
        data.str->assign(*m.data.str);
        break;
    case MSGPACK_ARRAY:
        switch (type) {
        case MSGPACK_STRING:
            delete data.str;
            data.arr = new msgpack_array;
            break;
        case MSGPACK_ARRAY:
            break;
        case MSGPACK_MAP:
            delete data.map;
            data.arr = new msgpack_array;
            break;
        default:
            data.arr = new msgpack_array;
            break;
        }
        data.arr->assign(m.data.arr->begin(), m.data.arr->end());
        break;
    case MSGPACK_MAP:
        switch (type) {
        case MSGPACK_STRING:
            delete data.str;
            data.map = new msgpack_map;
            break;
        case MSGPACK_ARRAY:
            delete data.arr;
            data.map = new msgpack_map;
            break;
        case MSGPACK_MAP:
            break;
        default:
            data.map = new msgpack_map;
            break;
        }
        data.map->assign(m.data.map->begin(), m.data.map->end());
        break;
    default:
        data = {};
        break;
    }
    type = m.type;
    return *this;
}
