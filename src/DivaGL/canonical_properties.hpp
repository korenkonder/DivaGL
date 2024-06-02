/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "types.hpp"
#include <stdexcept>

struct CanonicalProperties {
    prj::string data;
    prj::vector_pair_alloc<prj::string_range, prj::string_range> key_value_pair_storage;

    CanonicalProperties();
    ~CanonicalProperties();

    void Clear();
    prj::string_range* GetValue(const prj::string_range& key);
    int32_t GetValueInt32(const prj::string_range& key);
    int32_t GetValueInt32(const prj::string_range& key, const int32_t& def);
    float_t GetValueFloat32(const prj::string_range& key);
    prj::string GetValueString(const prj::string_range& key);
    void Parse(const char* str, size_t size);
    void Parse(const prj::string_range& data);
    void ParseEscapeSeq(const char* str, size_t size);
    void ParseEscapeSeq(const prj::string_range& data);

    [[noreturn]] inline void ThrowError(const prj::string& str) {
        throw std::out_of_range(("CanonicalProperties::get(\"" + str + "\"").c_str());
    }
};

static_assert(sizeof(CanonicalProperties) == 0x38, "\"CanonicalProperties\" struct should have a size of 0x38");
