/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "stream.hpp"

stream::stream() : buf(), length(), big_endian() {

}

stream::~stream() {
    close();
}

void stream::close() {
    if (!this)
        return;

    memset(buf, 0, sizeof(buf));
    length = 0;
    big_endian = false;
    position_stack.clear();
    position_stack.shrink_to_fit();
}

int32_t stream::position_push(int64_t pos, int32_t seek) {
    position_stack.push_back(get_position());
    return set_position(pos, seek);
}

void stream::position_pop() {
    if (position_stack.size() < 1)
        return;

    int64_t position = position_stack.back();
    position_stack.pop_back();
    set_position(position, SEEK_SET);
    flush();
}

int8_t stream::read_int8_t() {
    int32_t c = read_char();
    if (c != EOF)
        return (int8_t)c;
    return 0;
}

uint8_t stream::read_uint8_t() {
    int32_t c = read_char();
    if (c != EOF)
        return (uint8_t)c;
    return 0;
}

void stream::write_int8_t(int8_t val) {
    write_char((char)val);
}

void stream::write_uint8_t(uint8_t val) {
    write_char((char)val);
}

std::string stream::read_string(size_t length) {
    std::string str = std::string(length, 0);
    read(&str.front(), sizeof(char) * length);
    return str;
}

std::wstring stream::read_wstring(size_t length) {
    std::wstring str = std::wstring(length, 0);
    read(&str.front(), sizeof(wchar_t) * length);
    return str;
}

std::string stream::read_string_null_terminated() {
    int64_t offset = get_position();

    size_t length = read_utf8_string_null_terminated_offset_length(offset);
    if (length)
        return read_string(length);

    std::string str = {};
    return str;
}

std::wstring stream::read_wstring_null_terminated() {
    int64_t offset = get_position();

    size_t length = read_utf16_string_null_terminated_offset_length(offset);
    if (length)
        return read_wstring(length);

    std::wstring str = {};
    return str;
}

std::string stream::read_string_null_terminated_offset(int64_t offset) {
    if (offset) {
        size_t length = read_utf8_string_null_terminated_offset_length(offset);
        if (length) {
            position_push(offset, SEEK_SET);
            std::string str = read_string(length);
            position_pop();
            return str;
        }
    }

    std::string str = {};
    return str;
}

std::wstring stream::read_wstring_null_terminated_offset(int64_t offset) {
    if (offset) {
        size_t length = read_utf16_string_null_terminated_offset_length(offset);
        if (length) {
            position_push(offset, SEEK_SET);
            std::wstring str = read_wstring(length);
            position_pop();
            return str;
        }
    }

    std::wstring str = {};
    return str;
}

char* stream::read_utf8_string_null_terminated() {
    int64_t offset = get_position();
    return read_utf8_string_null_terminated_offset(offset);
}

wchar_t* stream::read_utf16_string_null_terminated() {
    int64_t offset = get_position();
    return read_utf16_string_null_terminated_offset(offset);
}

char* stream::read_utf8_string_null_terminated_offset(int64_t offset) {
    size_t len = read_utf8_string_null_terminated_offset_length(offset);
    if (!len) {
        return 0;
    }

    char* str = force_malloc<char>(len + 1);
    position_push(offset, SEEK_SET);
    read(str, len);
    str[len] = 0;
    position_pop();
    return str;
}

wchar_t* stream::read_utf16_string_null_terminated_offset(int64_t offset) {
    size_t len = read_utf16_string_null_terminated_offset_length(offset);
    if (!len) {
        position_pop();
        return 0;
    }

    wchar_t* str = force_malloc<wchar_t>(len + 1);
    position_push(offset, SEEK_SET);
    read(str, sizeof(wchar_t) * len);
    str[len] = 0;
    position_pop();
    return str;
}

size_t stream::read_utf8_string_null_terminated_length() {
    int64_t offset = get_position();
    return read_utf8_string_null_terminated_offset_length(offset);
}

size_t stream::read_utf16_string_null_terminated_length() {
    int64_t offset = get_position();
    return read_utf16_string_null_terminated_offset_length(offset);
}

size_t stream::read_utf8_string_null_terminated_offset_length(int64_t offset) {
    position_push(offset, SEEK_SET);

    size_t len = 0;
    int32_t c;
    while ((c = read_char()) != EOF && c != 0)
        len++;

    position_pop();
    return len;
}

size_t stream::read_utf16_string_null_terminated_offset_length(int64_t offset) {
    position_push(offset, SEEK_SET);

    size_t len = 0;
    int32_t c0, c1;
    while ((c0 = read_char()) != EOF && (c1 = read_char()) != EOF
        && (((c0 & 0xFF) | ((c1 & 0xFF) << 8)) != 0))
        len++;

    position_pop();
    return len;
}

int16_t stream::read_int16_t() {
    read(buf, sizeof(int16_t));
    return *(int16_t*)buf;
}

int16_t stream::read_int16_t_reverse_endianness() {
    read(buf, sizeof(int16_t));
    int16_t val;
    if (big_endian)
        val = load_reverse_endianness_int16_t(buf);
    else
        val = *(int16_t*)buf;
    return val;
}

int16_t stream::read_int16_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(int16_t));
    int16_t val;
    if (big_endian)
        val = load_reverse_endianness_int16_t(buf);
    else
        val = *(int16_t*)buf;
    return val;
}

void stream::write_int16_t(int16_t val) {
    *(int16_t*)buf = val;
    write(buf, sizeof(int16_t));
}

void stream::write_int16_t_reverse_endianness(int16_t val) {
    if (big_endian)
        store_reverse_endianness_int16_t(buf, val);
    else
        *(int16_t*)buf = val;
    write(buf, sizeof(int16_t));
}

void stream::write_int16_t_reverse_endianness(int16_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_int16_t(buf, val);
    else
        *(int16_t*)buf = val;
    write(buf, sizeof(int16_t));
}

uint16_t stream::read_uint16_t() {
    read(buf, sizeof(uint16_t));
    return *(uint16_t*)buf;
}

uint16_t stream::read_uint16_t_reverse_endianness() {
    read(buf, sizeof(uint16_t));
    uint16_t val;
    if (big_endian)
        val = load_reverse_endianness_uint16_t(buf);
    else
        val = *(uint16_t*)buf;
    return val;
}

uint16_t stream::read_uint16_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(uint16_t));
    uint16_t val;
    if (big_endian)
        val = load_reverse_endianness_uint16_t(buf);
    else
        val = *(uint16_t*)buf;
    return val;
}

void stream::write_uint16_t(uint16_t val) {
    *(uint16_t*)buf = val;
    write(buf, sizeof(uint16_t));
}

void stream::write_uint16_t_reverse_endianness(uint16_t val) {
    if (big_endian)
        store_reverse_endianness_uint16_t(buf, val);
    else
        *(uint16_t*)buf = val;
    write(buf, sizeof(uint16_t));
}

void stream::write_uint16_t_reverse_endianness(uint16_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_uint16_t(buf, val);
    else
        *(uint16_t*)buf = val;
    write(buf, sizeof(uint16_t));
}

int32_t stream::read_int32_t() {
    read(buf, sizeof(int32_t));
    return *(int32_t*)buf;
}

int32_t stream::read_int32_t_reverse_endianness() {
    read(buf, sizeof(int32_t));
    int32_t val;
    if (big_endian)
        val = load_reverse_endianness_int32_t(buf);
    else
        val = *(int32_t*)buf;
    return val;
}

int32_t stream::read_int32_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(int32_t));
    int32_t val;
    if (big_endian)
        val = load_reverse_endianness_int32_t(buf);
    else
        val = *(int32_t*)buf;
    return val;
}

void stream::write_int32_t(int32_t val) {
    *(int32_t*)buf = val;
    write(buf, sizeof(int32_t));
}

void stream::write_int32_t_reverse_endianness(int32_t val) {
    if (big_endian)
        store_reverse_endianness_int32_t(buf, val);
    else
        *(int32_t*)buf = val;
    write(buf, sizeof(int32_t));
}

void stream::write_int32_t_reverse_endianness(int32_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_int32_t(buf, val);
    else
        *(int32_t*)buf = val;
    write(buf, sizeof(int32_t));
}

uint32_t stream::read_uint32_t() {
    read(buf, sizeof(uint32_t));
    return *(uint32_t*)buf;
}

uint32_t stream::read_uint32_t_reverse_endianness() {
    read(buf, sizeof(uint32_t));
    uint32_t val;
    if (big_endian)
        val = load_reverse_endianness_uint32_t(buf);
    else
        val = *(uint32_t*)buf;
    return val;
}

uint32_t stream::read_uint32_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(uint32_t));
    uint32_t val;
    if (big_endian)
        val = load_reverse_endianness_uint32_t(buf);
    else
        val = *(uint32_t*)buf;
    return val;
}

void stream::write_uint32_t(uint32_t val) {
    *(uint32_t*)buf = val;
    write(buf, sizeof(uint32_t));
}

void stream::write_uint32_t_reverse_endianness(uint32_t val) {
    if (big_endian)
        store_reverse_endianness_uint32_t(buf, val);
    else
        *(uint32_t*)buf = val;
    write(buf, sizeof(uint32_t));
}

void stream::write_uint32_t_reverse_endianness(uint32_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_uint32_t(buf, val);
    else
        *(uint32_t*)buf = val;
    write(buf, sizeof(uint32_t));
}

int64_t stream::read_int64_t() {
    read(buf, sizeof(int64_t));
    return *(int64_t*)buf;
}

int64_t stream::read_int64_t_reverse_endianness() {
    read(buf, sizeof(int64_t));
    int64_t val;
    if (big_endian)
        val = load_reverse_endianness_int64_t(buf);
    else
        val = *(int64_t*)buf;
    return val;
}

int64_t stream::read_int64_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(int64_t));
    int64_t val;
    if (big_endian)
        val = load_reverse_endianness_int64_t(buf);
    else
        val = *(int64_t*)buf;
    return val;
}

void stream::write_int64_t(int64_t val) {
    *(int64_t*)buf = val;
    write(buf, sizeof(int64_t));
}

void stream::write_int64_t_reverse_endianness(int64_t val) {
    if (big_endian)
        store_reverse_endianness_int64_t(buf, val);
    else
        *(int64_t*)buf = val;
    write(buf, sizeof(int64_t));
}

void stream::write_int64_t_reverse_endianness(int64_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_int64_t(buf, val);
    else
        *(int64_t*)buf = val;
    write(buf, sizeof(int64_t));
}

uint64_t stream::read_uint64_t() {
    read(buf, sizeof(uint64_t));
    return *(uint64_t*)buf;
}

uint64_t stream::read_uint64_t_reverse_endianness() {
    read(buf, sizeof(uint64_t));
    uint64_t val;
    if (big_endian)
        val = load_reverse_endianness_uint64_t(buf);
    else
        val = *(uint64_t*)buf;
    return val;
}

uint64_t stream::read_uint64_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(uint64_t));
    uint64_t val;
    if (big_endian)
        val = load_reverse_endianness_uint64_t(buf);
    else
        val = *(uint64_t*)buf;
    return val;
}

void stream::write_uint64_t(uint64_t val) {
    *(uint64_t*)buf = val;
    write(buf, sizeof(uint64_t));
}

void stream::write_uint64_t_reverse_endianness(uint64_t val) {
    if (big_endian)
        store_reverse_endianness_uint64_t(buf, val);
    else
        *(uint64_t*)buf = val;
    write(buf, sizeof(uint64_t));
}

void stream::write_uint64_t_reverse_endianness(uint64_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_uint64_t(buf, val);
    else
        *(uint64_t*)buf = val;
    write(buf, sizeof(uint64_t));
}

half_t stream::read_half_t() {
    read(buf, sizeof(half_t));
    return *(half_t*)buf;
}

half_t stream::read_half_t_reverse_endianness() {
    read(buf, sizeof(half_t));
    half_t val;
    if (big_endian)
        val = load_reverse_endianness_half_t(buf);
    else
        val = *(half_t*)buf;
    return val;
}

half_t stream::read_half_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(half_t));
    half_t val;
    if (big_endian)
        val = load_reverse_endianness_half_t(buf);
    else
        val = *(half_t*)buf;
    return val;
}

void stream::write_half_t(half_t val) {
    *(half_t*)buf = val;
    write(buf, sizeof(half_t));
}

void stream::write_half_t_reverse_endianness(half_t val) {
    if (big_endian)
        store_reverse_endianness_half_t(buf, val);
    else
        *(half_t*)buf = val;
    write(buf, sizeof(half_t));
}

void stream::write_half_t_reverse_endianness(half_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_half_t(buf, val);
    else
        *(half_t*)buf = val;
    write(buf, sizeof(half_t));
}

float_t stream::read_float_t() {
    read(buf, sizeof(float_t));
    return *(float_t*)buf;
}

float_t stream::read_float_t_reverse_endianness() {
    read(buf, sizeof(float_t));
    float_t val;
    if (big_endian)
        val = load_reverse_endianness_float_t(buf);
    else
        val = *(float_t*)buf;
    return val;
}

float_t stream::read_float_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(float_t));
    float_t val;
    if (big_endian)
        val = load_reverse_endianness_float_t(buf);
    else
        val = *(float_t*)buf;
    return val;
}

void stream::write_float_t(float_t val) {
    *(float_t*)buf = val;
    write(buf, sizeof(float_t));
}

void stream::write_float_t_reverse_endianness(float_t val) {
    if (big_endian)
        store_reverse_endianness_float_t(buf, val);
    else
        *(float_t*)buf = val;
    write(buf, sizeof(float_t));
}

void stream::write_float_t_reverse_endianness(float_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_float_t(buf, val);
    else
        *(float_t*)buf = val;
    write(buf, sizeof(float_t));
}

double_t stream::read_double_t() {
    read(buf, sizeof(double_t));
    return *(double_t*)buf;
}

double_t stream::read_double_t_reverse_endianness() {
    read(buf, sizeof(double_t));
    double_t val;
    if (big_endian)
        val = load_reverse_endianness_double_t(buf);
    else
        val = *(double_t*)buf;
    return val;
}

double_t stream::read_double_t_reverse_endianness(bool big_endian) {
    read(buf, sizeof(double_t));
    double_t val;
    if (big_endian)
        val = load_reverse_endianness_double_t(buf);
    else
        val = *(double_t*)buf;
    return val;
}

void stream::write_double_t(double_t val) {
    *(double_t*)buf = val;
    write(buf, sizeof(double_t));
}

void stream::write_double_t_reverse_endianness(double_t val) {
    if (big_endian)
        store_reverse_endianness_double_t(buf, val);
    else
        *(double_t*)buf = val;
    write(buf, sizeof(double_t));
}

void stream::write_double_t_reverse_endianness(double_t val, bool big_endian) {
    if (big_endian)
        store_reverse_endianness_double_t(buf, val);
    else
        *(double_t*)buf = val;
    write(buf, sizeof(double_t));
}

void stream::write_string(std::string& str) {
    write(str.c_str(), str.size());
}

void stream::write_string(std::string&& str) {
    write(str.c_str(), str.size());
}

void stream::write_wstring(std::wstring& str) {
    write(str.c_str(), sizeof(wchar_t) * str.size());
}

void stream::write_wstring(std::wstring&& str) {
    write(str.c_str(), sizeof(wchar_t) * str.size());
}

void stream::write_string_null_terminated(std::string& str) {
    write(str.c_str(), str.size());
    write_uint8_t(0);
}

void stream::write_string_null_terminated(std::string&& str) {
    write_string_null_terminated(str);
}

void stream::write_wstring_null_terminated(std::wstring& str) {
    write(str.c_str(), sizeof(wchar_t) * str.size());
    write_uint16_t(0);
}

void stream::write_wstring_null_terminated(std::wstring&& str) {
    write_wstring_null_terminated(*(std::wstring*)&str);
}

void stream::write_utf8_string(const char* str) {
    write(str, utf8_length(str));
}

void stream::write_utf16_string(const wchar_t* str) {
    write(str, sizeof(wchar_t) * utf16_length(str));
}

void stream::write_utf8_string_null_terminated(const char* str) {
    write(str, utf8_length(str));
    write_uint8_t(0);
}

void stream::write_utf16_string_null_terminated(const wchar_t* str) {
    write(str, sizeof(wchar_t) * utf16_length(str));
    write_uint16_t(0);
}

int64_t stream::read_offset(int64_t offset, bool is_x) {
    int64_t val;
    if (!is_x) {
        val = read_uint32_t_reverse_endianness();
        if (val)
            val -= offset;
    }
    else {
        align_read(0x08);
        val = read_int64_t_reverse_endianness();
    }
    return val;
}

int64_t stream::read_offset_f2(int64_t offset) {
    int64_t val = read_uint32_t_reverse_endianness();
    if (val)
        val -= offset;
    return val;
}

int64_t stream::read_offset_x() {
    align_read(0x08);
    int64_t val = read_int64_t_reverse_endianness();
    return val;
}

void stream::write_offset(int64_t val, int64_t offset, bool is_x) {
    if (!is_x) {
        if (val)
            val += offset;
        write_uint32_t_reverse_endianness((uint32_t)val);
    }
    else {
        align_write(0x08);
        write_int64_t_reverse_endianness(val);
    }
}

void stream::write_offset_f2(int64_t val, int64_t offset) {
    if (val)
        val += offset;
    write_uint32_t_reverse_endianness((uint32_t)val);
}

void stream::write_offset_x(int64_t val) {
    align_write(0x08);
    write_int64_t_reverse_endianness(val);
}
