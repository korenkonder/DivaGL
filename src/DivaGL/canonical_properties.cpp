/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "canonical_properties.hpp"

CanonicalProperties::CanonicalProperties() {

}

CanonicalProperties::~CanonicalProperties() {

}

void CanonicalProperties::Clear() {
    key_value_pair_storage.first.clear();
    key_value_pair_storage.first.shrink_to_fit();
    data.clear();
    data.shrink_to_fit();
}

prj::string_range* CanonicalProperties::GetValue(const prj::string_range& key) {
    prj::string_range* (FASTCALL * CanonicalProperties__GetValueStringRange)(CanonicalProperties * _this, const prj::string_range * key)
        = (prj::string_range * (FASTCALL*)(CanonicalProperties * _this, const prj::string_range * key))0x00000001400AE4F0;
    return CanonicalProperties__GetValueStringRange(this, &key);
}

int32_t CanonicalProperties::GetValueInt32(const prj::string_range& key) {
    prj::string_range* value = GetValue(key);
    if (!value)
        ThrowError(prj::string(key.begin, key.end - key.begin));

    prj::string temp(value->begin, value->end - value->begin);
    return atoi(temp.c_str());
}

int32_t CanonicalProperties::GetValueInt32(const prj::string_range& key, const int32_t& def) {
    prj::string_range* value = GetValue(key);
    if (!value)
        return def;

    prj::string temp(value->begin, value->end - value->begin);
    return atoi(temp.c_str());
}

float_t CanonicalProperties::GetValueFloat32(const prj::string_range& key) {
    prj::string_range* value = GetValue(key);
    if (!value)
        ThrowError(prj::string(key.begin, key.end - key.begin));

    prj::string temp(value->begin, value->end - value->begin);
    return (float_t)atof(temp.c_str());
}

prj::string CanonicalProperties::GetValueString(const prj::string_range& key) {
    prj::string_range* value = GetValue(key);
    if (!value)
        ThrowError(prj::string(key.begin, key.end - key.begin));

    return prj::string(value->begin, value->end - value->begin);
}

void CanonicalProperties::Parse(const char* str, size_t size) {
    void(FASTCALL * CanonicalProperties__ParseCharPtrLength)(
        CanonicalProperties * _this, const char* str, size_t size)
        = (void(FASTCALL*)(CanonicalProperties * _this, const char* str, size_t size))0x00000001400B52C0;
    CanonicalProperties__ParseCharPtrLength(this, str, size);
}

void CanonicalProperties::Parse(const prj::string_range& data) {
    void(FASTCALL * CanonicalProperties__ParseStringRange)(
        CanonicalProperties * _this, const prj::string_range * data)
        = (void(FASTCALL*)(CanonicalProperties * _this, const prj::string_range * data))0x000000014008E710;
    CanonicalProperties__ParseStringRange(this, &data);
}

void CanonicalProperties::ParseEscapeSeq(const char* str, size_t size) {
    void(FASTCALL * CanonicalProperties__ParseCharPtrLengthEscapeSeq)(
        CanonicalProperties * _this, const char* str, size_t size)
        = (void(FASTCALL*)(CanonicalProperties * _this, const char* str, size_t size))0x00000001400B5020;
    CanonicalProperties__ParseCharPtrLengthEscapeSeq(this, str, size);
}

void CanonicalProperties::ParseEscapeSeq(const prj::string_range& data) {
    void(FASTCALL * CanonicalProperties__ParseStringRangeEscapeSeq)(
        CanonicalProperties * _this, const prj::string_range * data)
        = (void(FASTCALL*)(CanonicalProperties * _this, const prj::string_range * data))0x000000014008E5A0;
    CanonicalProperties__ParseStringRangeEscapeSeq(this, &data);
}
