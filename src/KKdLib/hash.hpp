/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <string>
#include "default.hpp"

// Empty string
extern const uint64_t hash_fnv1a64m_empty;
// Empty string
extern const uint32_t hash_murmurhash_empty;
// "NULL" string
extern const uint32_t hash_murmurhash_null;
// Empty string
extern const uint32_t hash_crc16_ccitt_empty;

extern uint64_t hash_fnv1a64m(const void* data, size_t size, bool make_upper = false);
extern uint32_t hash_murmurhash(const void* data, size_t size,
    uint32_t seed = 0, bool upper = false, bool big_endian = false);
extern uint16_t hash_crc16_ccitt(const void* data, size_t size, bool make_upper = false);

inline uint64_t hash_utf8_fnv1a64m(const char* data, bool make_upper = false) {
    return hash_fnv1a64m(data, utf8_length(data), make_upper);
}

inline uint64_t hash_utf16_fnv1a64m(const wchar_t* data, bool make_upper = false) {
    char* temp = utf16_to_utf8(data);
    uint64_t hash = hash_fnv1a64m(temp, utf8_length(temp), make_upper);
    free_def(temp);
    return hash;
}

inline uint64_t hash_string_fnv1a64m(const std::string& data, bool make_upper = false) {
    return hash_fnv1a64m(data.c_str(), data.size(), make_upper);
}

inline uint32_t hash_utf8_murmurhash(const char* data, uint32_t seed = 0, bool upper = false) {
    return hash_murmurhash(data, utf8_length(data), seed, upper);
}

inline uint32_t hash_utf16_murmurhash(const wchar_t* data, uint32_t seed = 0, bool upper = false) {
    char* temp = utf16_to_utf8(data);
    uint32_t hash = hash_murmurhash(temp, utf8_length(temp), seed, upper);
    free_def(temp);
    return hash;
}

inline uint32_t hash_string_murmurhash(const std::string& data, uint32_t seed = 0, bool upper = false) {
    return hash_murmurhash(data.c_str(), data.size(), seed, upper);
}

inline uint16_t hash_utf8_crc16_ccitt(const char* data, bool make_upper = false) {
    return hash_crc16_ccitt(data, utf8_length(data), make_upper);
}

inline uint16_t hash_utf16_crc16_ccitt(const wchar_t* data, bool make_upper = false) {
    char* temp = utf16_to_utf8(data);
    uint32_t hash = hash_crc16_ccitt(temp, utf8_length(temp), make_upper);
    free_def(temp);
    return hash;
}

inline uint16_t hash_string_crc16_ccitt(const std::string& data, bool make_upper = false) {
    return hash_crc16_ccitt(data.c_str(), data.size(), make_upper);
}

struct string_hash {
    std::string str;
    uint64_t hash_fnv1a64m;
    uint32_t hash_murmurhash;

    inline string_hash() {
        hash_fnv1a64m = hash_fnv1a64m_empty;
        hash_murmurhash = hash_murmurhash_empty;
    }

    inline string_hash(const char* str) {
        this->str.assign(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline string_hash(const char* str, size_t length) {
        this->str.assign(str, length);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline string_hash(const std::string& str) {
        this->str.assign(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline string_hash(const std::string&& str) {
        this->str.assign(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline ~string_hash() {

    }

    inline const char* c_str() const {
        return str.c_str();
    }
    
    inline size_t size() const {
        return str.size();
    }

    inline void append(const char* str) {
        this->str.append(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void append(const char* str, size_t length) {
        this->str.append(str, length);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void append(std::string& str) {
        this->str.append(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void append(std::string&& str) {
        this->str.append(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void append(string_hash& str) {
        this->str.append(str.str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void assign(const char* str) {
        this->str.assign(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void assign(const char* str, size_t length) {
        this->str.assign(str, length);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void assign(std::string& str) {
        this->str.assign(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void assign(std::string&& str) {
        this->str.assign(str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void assign(string_hash& str) {
        this->str.assign(str.str);
        this->hash_fnv1a64m = hash_string_fnv1a64m(this->str);
        this->hash_murmurhash = hash_string_murmurhash(this->str);
    }

    inline void clear() {
        str.clear();
        str.shrink_to_fit();
        hash_fnv1a64m = hash_fnv1a64m_empty;
        hash_murmurhash = hash_murmurhash_empty;
    }
};

inline bool operator >(const string_hash& left, const string_hash& right) {
    return left.str.compare(right.str) > 0;
}

inline bool operator <(const string_hash& left, const string_hash& right) {
    return left.str.compare(right.str) < 0;
}

inline bool operator >=(const string_hash& left, const string_hash& right) {
    return left.str.compare(right.str) >= 0;
}

inline bool operator <=(const string_hash& left, const string_hash& right) {
    return left.str.compare(right.str) <= 0;
}

inline bool operator ==(const string_hash& left, const string_hash& right) {
    return !left.str.compare(right.str);
}

inline bool operator !=(const string_hash& left, const string_hash& right) {
    return !!left.str.compare(right.str);
}