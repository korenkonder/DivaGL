/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <string>
#include <vector>
#include "default.hpp"

inline int32_t str_utils_compare(const char* str0, const char* str1) {
    return strcmp(str0, str1);
}

inline int32_t str_utils_compare(const wchar_t* str0, const wchar_t* str1) {
    return wcscmp(str0, str1);
}

extern bool str_utils_check_ends_with(const char* str, const char* mask);
extern bool str_utils_check_ends_with(const wchar_t* str, const wchar_t* mask);
extern const char* str_utils_get_next_int32_t(const char* str, int32_t& value, const char split);
extern const wchar_t* str_utils_get_next_int32_t(const wchar_t* str, int32_t& value, const wchar_t split);
extern const char* str_utils_get_next_float_t(const char* str, float_t& value, const char split);
extern const wchar_t* str_utils_get_next_float_t(const wchar_t* str, float_t& value, const wchar_t split);
extern const char* str_utils_get_next_string(const char* str, std::string& value, const char split);
extern const wchar_t* str_utils_get_next_string(const wchar_t* str, std::wstring& value, const wchar_t split);
extern char* str_utils_split_get_right(const char* str, const char split);
extern wchar_t* str_utils_split_get_right(const wchar_t* str, const wchar_t split);
extern char* str_utils_split_get_left(const char* str, const char split);
extern wchar_t* str_utils_split_get_left(const wchar_t* str, const wchar_t split);
extern char* str_utils_split_get_right_include(const char* str, const char split);
extern wchar_t* str_utils_split_get_right_include(const wchar_t* str, const wchar_t split);
extern char* str_utils_split_get_left_include(const char* str, const char split);
extern wchar_t* str_utils_split_get_left_include(const wchar_t* str, const wchar_t split);
extern char* str_utils_split_right_get_right(const char* str, const char split);
extern wchar_t* str_utils_split_right_get_right(const wchar_t* str, const wchar_t split);
extern char* str_utils_split_right_get_left(const char* str, const char split);
extern wchar_t* str_utils_split_right_get_left(const wchar_t* str, const wchar_t split);
extern char* str_utils_split_right_get_right_include(const char* str, const char split);
extern wchar_t* str_utils_split_right_get_right_include(const wchar_t* str, const wchar_t split);
extern char* str_utils_split_right_get_left_include(const char* str, const char split);
extern wchar_t* str_utils_split_right_get_left_include(const wchar_t* str, const wchar_t split);
extern char* str_utils_get_extension(const char* str);
extern wchar_t* str_utils_get_extension(const wchar_t* str);
extern char* str_utils_get_without_extension(const char* str);
extern wchar_t* str_utils_get_without_extension(const wchar_t* str);
extern char* str_utils_add(const char* str0, const char* str1);
extern wchar_t* str_utils_add(const wchar_t* str0, const wchar_t* str1);
extern char* str_utils_copy(const char* str);
extern wchar_t* str_utils_copy(const wchar_t* str);
extern int32_t str_utils_compare_length(const char* str0, size_t str0_len, const char* str1, size_t str1_len);
extern int32_t str_utils_compare_length(const wchar_t* str0, size_t str0_len, const wchar_t* str1, size_t str1_len);
extern size_t str_utils_get_substring_offset(const char* str0, size_t str0_len,
    size_t str0_off, const char* str1, size_t str1_len);
extern size_t str_utils_get_substring_offset(const wchar_t* str0, size_t str0_len,
    size_t str0_off, const wchar_t* str1, size_t str1_len);
extern bool str_utils_text_file_parse(const void* data, size_t size,
    char*& buf, char**& lines, size_t& count);
