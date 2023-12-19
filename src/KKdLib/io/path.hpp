/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <string>
#include <vector>
#include "../default.hpp"

extern bool path_check_path_exists(const char* path);
extern bool path_check_path_exists(const wchar_t* path);
extern bool path_check_file_exists(const char* path);
extern bool path_check_file_exists(const wchar_t* path);
extern bool path_check_directory_exists(const char* path);
extern bool path_check_directory_exists(const wchar_t* path);
extern std::vector<std::string> path_get_files(const char* path);
extern std::vector<std::wstring> path_get_files(const wchar_t* path);
extern std::vector<std::string> path_get_directories(
    const char* path, const char** exclude_list = 0, size_t exclude_count = 0);
extern std::vector<std::wstring> path_get_directories(
    const wchar_t* path, const wchar_t** exclude_list = 0, size_t exclude_count = 0);
extern std::vector<std::string> path_get_directories_recursive(
    const char* path, const char** exclude_list = 0, size_t exclude_count = 0);
extern std::vector<std::wstring> path_get_directories_recursive(
    const wchar_t* path, const wchar_t** exclude_list = 0, size_t exclude_count = 0);