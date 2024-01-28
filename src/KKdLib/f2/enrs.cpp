/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "enrs.hpp"

enum enrs_value_type {
    ENRS_VALUE_INT8    = 0x0,
    ENRS_VALUE_INT16   = 0x1,
    ENRS_VALUE_INT32   = 0x2,
    ENRS_VALUE_INVALID = 0x3,
};

inline static bool enrs_length_get_size_type(uint32_t* length, size_t val);
inline static bool enrs_length_get_size(uint32_t* length, size_t val);
static bool enrs_read_packed_value(stream& s, uint32_t* val);
static bool enrs_write_packed_value(stream& s, uint32_t val);
static bool enrs_read_packed_value_type(stream& s, uint32_t* val, enrs_type* type);
static bool enrs_write_packed_value_type(stream& s, uint32_t val, enrs_type type);

enrs_entry::enrs_entry(): offset(), count(), size(), repeat_count() {

}

enrs_entry::enrs_entry(uint32_t offset, uint32_t count, uint32_t size, uint32_t repeat_count) {
    this->offset = offset;
    this->count = count;
    this->size = size;
    this->repeat_count = repeat_count;
}

enrs_entry::~enrs_entry() {

}

void enrs_entry::append(uint32_t skip_bytes, uint32_t repeat_count, enrs_type type) {
    sub.push_back({ skip_bytes, repeat_count, type });
}

void enrs_entry::append(enrs_sub_entry&& data) {
    sub.push_back(data);
}

enrs_entry& enrs_entry::operator=(const enrs_entry& ee) {
    offset = ee.offset;
    count = ee.count;
    size = ee.size;
    repeat_count = ee.repeat_count;
    sub.assign(ee.sub.begin(), ee.sub.end());
    return *this;
}

enrs::enrs() {

}

enrs::~enrs() {

}

void enrs::apply(void* data) {
    if (!data)
        return;

    uint8_t* d = (uint8_t*)data;
    uint8_t* temp;
    for (enrs_entry& i : vec) {
        d += i.offset;
        for (size_t j = 0; j < i.repeat_count; j++) {
            temp = d + i.size * j;
            for (enrs_sub_entry& k : i.sub) {
                temp += k.skip_bytes;
                switch (k.type) {
                case ENRS_WORD:
                    for (size_t l = k.repeat_count; l; l--) {
                        *(uint16_t*)temp = reverse_endianness_uint16_t(*(uint16_t*)temp);
                        temp += 2;
                    }
                    break;
                case ENRS_DWORD:
                    for (size_t l = k.repeat_count; l; l--) {
                        *(uint32_t*)temp = reverse_endianness_uint32_t(*(uint32_t*)temp);
                        temp += 4;
                    }
                    break;
                case ENRS_QWORD:
                    for (size_t l = k.repeat_count; l; l--) {
                        *(uint64_t*)temp = reverse_endianness_uint64_t(*(uint64_t*)temp);
                        temp += 8;
                    }
                    break;
                }
            }
        }
    }
}

uint32_t enrs::length() {
    uint32_t l = 0x10;
    uint32_t o = 0;
    for (enrs_entry& i : vec) {
        uint32_t offset = i.offset;
        i.count = (uint32_t)i.sub.size();
        if (&i != vec.data() && (&i)[-1].count < 1) {
            o += (uint32_t)((size_t)(&i)[-1].size * (&i)[-1].repeat_count);
            if (i.count > 0) {
                offset += o;
                o = 0;
            }
        }

        if (i.count < 1)
            continue;

        if (enrs_length_get_size(&l, offset)
            || enrs_length_get_size(&l, i.count)
            || enrs_length_get_size(&l, i.size)
            || enrs_length_get_size(&l, i.repeat_count))
            goto End;

        if (i.repeat_count < 1 || i.count > 0x40000000)
            continue;

        for (enrs_sub_entry& j : i.sub) {
            if (enrs_length_get_size_type(&l, j.skip_bytes)
                || enrs_length_get_size(&l, j.repeat_count))
                goto End;
        }
    }
End:
    l = align_val(l, 0x10);
    return l;
}

void enrs::read(stream& s) {
    vec.clear();

    s.read_uint32_t();
    size_t l = s.read_uint32_t();
    s.read_uint32_t();
    s.read_uint32_t();

    vec.reserve(l);
    for (size_t i = 0; i < l; i++) {
        enrs_entry entry;
        if (enrs_read_packed_value(s, &entry.offset)
            || enrs_read_packed_value(s, &entry.count)
            || enrs_read_packed_value(s, &entry.size)
            || enrs_read_packed_value(s, &entry.repeat_count))
            return;

        if (!entry.count || !entry.repeat_count) {
            vec.push_back(entry);
            continue;
        }

        entry.sub.reserve(entry.count);
        for (size_t j = 0; j < entry.count; j++) {
            enrs_sub_entry sub_entry = {};
            if (enrs_read_packed_value_type(s, &sub_entry.skip_bytes, &sub_entry.type)
                || enrs_read_packed_value(s, &sub_entry.repeat_count))
                return;
            entry.sub.push_back(sub_entry);
        }
        vec.push_back(entry);
    }
}

void enrs::write(stream& s) {
    uint32_t o = 0;
    size_t length = enrs::length();
    s.write_uint32_t(0);
    s.write_uint32_t((uint32_t)vec.size());
    s.write_uint32_t(0);
    s.write_uint32_t(0);
    for (enrs_entry& i : vec) {
        uint32_t offset = i.offset;
        i.count = (uint32_t)i.sub.size();
        if (&i != vec.data() && (&i)[-1].count < 1) {
            o += (uint32_t)((size_t)(&i)[-1].size * (&i)[-1].repeat_count);
            if ((&i)->count > 0) {
                offset += o;
                o = 0;
            }
        }

        if (i.count < 1)
            continue;

        if (enrs_write_packed_value(s, offset)
            || enrs_write_packed_value(s, i.count)
            || enrs_write_packed_value(s, i.size)
            || enrs_write_packed_value(s, i.repeat_count))
            goto End;

        if (i.repeat_count < 1)
            continue;

        for (enrs_sub_entry& j : i.sub)
            if (enrs_write_packed_value_type(s, j.skip_bytes, j.type)
                || enrs_write_packed_value(s, j.repeat_count))
                goto End;
    }

End:
    s.align_write(0x10);
}

enrs& enrs::operator=(const enrs& e) {
    vec.assign(e.vec.begin(), e.vec.end());
    return *this;
}

inline static bool enrs_length_get_size_type(uint32_t* length, size_t val) {
    *length += val < 0x10 ? 1 : val < 0x1000 ? 2 : val < 0x10000000 ? 4 : 1;
    return val >= 0x10000000;
}

inline static bool enrs_length_get_size(uint32_t* length, size_t val) {
    if (!length)
        return true;

    *length += val < 0x40 ? 1 : val < 0x4000 ? 2 : val < 0x40000000 ? 4 : 1;
    return val >= 0x40000000;
}

static bool enrs_read_packed_value(stream& s, uint32_t* val) {
    *val = s.read_uint8_t();
    enrs_value_type value = (enrs_value_type)((*val >> 6) & 0x3);
    *val &= 0x3F;

    if (value == ENRS_VALUE_INT32)
        *val = (((((*val << 8) | s.read_uint8_t()) << 8) | s.read_uint8_t()) << 8) | s.read_uint8_t();
    else if (value == ENRS_VALUE_INT16)
        *val = (*val << 8) | s.read_uint8_t();
    else if (value == ENRS_VALUE_INVALID) {
        *val = 0;
        return true;
    }
    return false;
}

static bool enrs_write_packed_value(stream& s, uint32_t val) {
    if (val < 0x40)
        s.write_uint8_t((uint8_t)((ENRS_VALUE_INT8 << 6) | (val & 0x3F)));
    else if (val < 0x4000) {
        s.write_uint8_t((uint8_t)((ENRS_VALUE_INT16 << 6) | ((val >> 8) & 0x3F)));
        s.write_uint8_t((uint8_t)val);
    }
    else if (val < 0x40000000) {
        s.write_uint8_t((uint8_t)((ENRS_VALUE_INT32 << 6) | ((val >> 24) & 0x3F)));
        s.write_uint8_t((uint8_t)(val >> 16));
        s.write_uint8_t((uint8_t)(val >> 8));
        s.write_uint8_t((uint8_t)val);
    }
    else {
        s.write_uint8_t(ENRS_VALUE_INVALID << 6);
        return true;
    }
    return false;
}

static bool enrs_read_packed_value_type(stream& s, uint32_t* val, enrs_type* type) {
    *val = s.read_uint8_t();
    enrs_value_type value = (enrs_value_type)((*val >> 6) & 0x3);
    *type = (enrs_type)((*val >> 4) & 0x3);
    *val &= 0xF;

    if (value == ENRS_VALUE_INT32)
        *val = (((((*val << 8) | s.read_uint8_t()) << 8) | s.read_uint8_t()) << 8) | s.read_uint8_t();
    else if (value == ENRS_VALUE_INT16)
        *val = (*val << 8) | s.read_uint8_t();
    else if (value == ENRS_VALUE_INVALID) {
        *val = 0;
        return true;
    }
    return false;
}

static bool enrs_write_packed_value_type(stream& s, uint32_t val, enrs_type type) {
    uint8_t t = ((uint8_t)type & 0x3) << 4;
    if (val < 0x10)
        s.write_uint8_t((uint8_t)((ENRS_VALUE_INT8 << 6) | t | (val & 0xF)));
    else if (val < 0x1000) {
        s.write_uint8_t((uint8_t)((ENRS_VALUE_INT16 << 6) | t | ((val >> 8) & 0xF)));
        s.write_uint8_t((uint8_t)val);
    }
    else if (val < 0x10000000) {
        s.write_uint8_t((uint8_t)((ENRS_VALUE_INT32 << 6) | t | ((val >> 24) & 0xF)));
        s.write_uint8_t((uint8_t)(val >> 16));
        s.write_uint8_t((uint8_t)(val >> 8));
        s.write_uint8_t((uint8_t)val);
    }
    else {
        s.write_uint8_t(ENRS_VALUE_INVALID << 6);
        return true;
    }
    return false;
}
