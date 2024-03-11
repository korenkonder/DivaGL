/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "path.hpp"
#include "../str_utils.hpp"

bool path_check_path_exists(const char* path) {
    wchar_t* path_temp = utf8_to_utf16(path);
    DWORD ftyp = GetFileAttributesW(path_temp);
    free_def(path_temp);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return true;
}

bool path_check_path_exists(const wchar_t* path) {
    DWORD ftyp = GetFileAttributesW(path);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return true;
}

bool path_check_file_exists(const char* path) {
    wchar_t* path_temp = utf8_to_utf16(path);
    DWORD ftyp = GetFileAttributesW(path_temp);
    free_def(path_temp);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return ftyp & FILE_ATTRIBUTE_DIRECTORY ? false : true;
}

bool path_check_file_exists(const wchar_t* path) {
    DWORD ftyp = GetFileAttributesW(path);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return ftyp & FILE_ATTRIBUTE_DIRECTORY ? false : true;
}

bool path_check_directory_exists(const char* path) {
    wchar_t* path_temp = utf8_to_utf16(path);
    DWORD ftyp = GetFileAttributesW(path_temp);
    free_def(path_temp);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return ftyp & FILE_ATTRIBUTE_DIRECTORY ? true : false;
}

bool path_check_directory_exists(const wchar_t* path) {
    DWORD ftyp = GetFileAttributesW(path);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return ftyp & FILE_ATTRIBUTE_DIRECTORY ? true : false;
}

std::vector<std::string> path_get_files(const char* path) {
    wchar_t* dir_temp = utf8_to_utf16(path);
    size_t dir_len = utf16_length(dir_temp);
    if (!dir_temp)
        return {};

    std::wstring dir;
    dir.assign(dir_temp, dir_len);
    if (dir.size() && dir.back() != L'\\')
        dir.push_back(L'\\');
    dir.push_back(L'*');
    free_def(dir_temp);

    WIN32_FIND_DATAW fdata = {};
    HANDLE h = FindFirstFileW(dir.c_str(), &fdata);
    if (h == INVALID_HANDLE_VALUE)
        return {};

    std::vector<std::string> files;
    do {
        if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        char* file_temp = utf16_to_utf8(fdata.cFileName);
        if (file_temp) {
            files.push_back(file_temp);
            free(file_temp);
        }
    } while (FindNextFileW(h, &fdata));
    FindClose(h);
    return files;
}

std::vector<std::wstring> path_get_files(const wchar_t* path) {
    size_t dir_len = utf16_length(path);

    std::wstring dir;
    dir.assign(path, dir_len);
    if (dir.size() && dir.back() != L'\\')
        dir.push_back(L'\\');
    dir.push_back(L'*');

    WIN32_FIND_DATAW fdata = {};
    HANDLE h = FindFirstFileW(dir.c_str(), &fdata);
    if (h == INVALID_HANDLE_VALUE)
        return {};

    std::vector<std::wstring> files;
    do {
        if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        files.push_back(fdata.cFileName);
    } while (FindNextFileW(h, &fdata));
    FindClose(h);
    return files;
}

std::vector<std::string> path_get_directories(
    const char* path, const char** exclude_list, size_t exclude_count) {
    wchar_t* dir_temp = utf8_to_utf16(path);
    size_t dir_len = utf16_length(dir_temp);
    if (!dir_temp)
        return {};

    std::wstring dir;
    std::wstring temp;
    dir.assign(dir_temp, dir_len);
    if (dir.size() && dir.back() != L'\\')
        dir.push_back(L'\\');
    temp.assign(dir);
    dir.push_back(L'*');
    free_def(dir_temp);

    WIN32_FIND_DATAW fdata = {};
    HANDLE h = FindFirstFileW(dir.c_str(), &fdata);
    if (h == INVALID_HANDLE_VALUE)
        return {};

    std::vector<std::string> directories;
    do {
        if (!(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            || !str_utils_compare(fdata.cFileName, L".")
            || !str_utils_compare(fdata.cFileName, L".."))
            continue;

        if (exclude_list && exclude_count) {
            size_t len = utf16_length(fdata.cFileName);
            temp.append(fdata.cFileName, len);
            std::string temp_utf8 = utf16_to_utf8(temp);

            bool exclude = false;
            for (size_t i = 0; i < exclude_count; i++) {
                size_t exclude_path_len = utf8_length(exclude_list[i]);
                size_t pos = temp_utf8.rfind(exclude_list[i], -1, exclude_path_len);
                if (pos != -1 && pos == temp_utf8.size() - exclude_path_len) {
                    exclude = true;
                    break;
                }
            }
            temp.resize(temp.size() - len);

            if (exclude)
                continue;
        }

        char* directory_temp = utf16_to_utf8(fdata.cFileName);
        if (directory_temp)
            directories.push_back(directory_temp);
        free_def(directory_temp);
    } while (FindNextFileW(h, &fdata));
    FindClose(h);
    return directories;
}

std::vector<std::wstring> path_get_directories(
    const wchar_t* path, wchar_t** exclude_list, size_t exclude_count) {
    size_t dir_len = utf16_length(path);

    std::wstring dir;
    std::wstring temp;
    dir.assign(path, dir_len);
    if (dir.size() && dir.back() != L'\\')
        dir.push_back(L'\\');
    temp.assign(dir);
    dir.push_back(L'*');

    WIN32_FIND_DATAW fdata = {};
    HANDLE h = FindFirstFileW(dir.c_str(), &fdata);
    if (h == INVALID_HANDLE_VALUE)
        return {};

    std::vector<std::wstring> directories;
    do {
        if (!(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            || !str_utils_compare(fdata.cFileName, L".")
            || !str_utils_compare(fdata.cFileName, L".."))
            continue;

        if (exclude_list && exclude_count) {
            size_t len = utf16_length(fdata.cFileName);
            temp.append(fdata.cFileName, len);

            bool exclude = false;
            for (size_t i = 0; i < exclude_count; i++) {
                size_t exclude_path_len = utf16_length(exclude_list[i]);
                size_t pos = temp.rfind(exclude_list[i], -1, exclude_path_len);
                if (pos != -1 && pos == temp.size() - exclude_path_len) {
                    exclude = true;
                    break;
                }
            }
            temp.resize(temp.size() - len);

            if (exclude)
                continue;
        }

        directories.push_back(fdata.cFileName);
    } while (FindNextFileW(h, &fdata));
    FindClose(h);
    return directories;
}

std::vector<std::string> path_get_directories_recursive(
    const char* path, const char** exclude_list, size_t exclude_count) {
    wchar_t* dir_temp = utf8_to_utf16(path);
    size_t dir_len = utf16_length(dir_temp);
    if (!dir_temp)
        return {};

    std::wstring dir;
    std::wstring temp;
    dir.assign(dir_temp, dir_len);
    if (dir.size() && dir.back() != L'\\')
        dir.push_back(L'\\');
    temp.assign(dir);
    dir.push_back(L'*');
    free_def(dir_temp);

    WIN32_FIND_DATAW fdata = {};
    HANDLE h = FindFirstFileW(dir.c_str(), &fdata);
    if (h == INVALID_HANDLE_VALUE)
        return {};

    std::vector<std::string> temp_vec;
    do {
        if (!(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            || !str_utils_compare(fdata.cFileName, L".")
            || !str_utils_compare(fdata.cFileName, L".."))
            continue;

        if (exclude_list && exclude_count) {
            size_t len = utf16_length(fdata.cFileName);
            temp.append(fdata.cFileName, len);
            std::string temp_utf8 = utf16_to_utf8(temp);

            bool exclude = false;
            for (size_t i = 0; i < exclude_count; i++) {
                size_t exclude_path_len = utf8_length(exclude_list[i]);
                size_t pos = temp_utf8.rfind(exclude_list[i], -1, exclude_path_len);
                if (pos != -1 && pos == temp_utf8.size() - exclude_path_len) {
                    exclude = true;
                    break;
                }
            }
            temp.resize(temp.size() - len);

            if (exclude)
                continue;
        }

        char* directory_temp = utf16_to_utf8(fdata.cFileName);
        temp_vec.push_back(directory_temp);
        free_def(directory_temp);
    } while (FindNextFileW(h, &fdata));
    FindClose(h);

    size_t max_len = 0;
    for (std::string& i : temp_vec) {
        size_t len = i.size();
        if (max_len < len)
            max_len = len;
    }

    std::vector<std::string> directories;
    std::string path_temp;
    path_temp.assign(path);
    path_temp += '\\';
    for (std::string& i : temp_vec) {
        path_temp.append(i);
        std::vector<std::string> temp = path_get_directories_recursive(
            path_temp.c_str(), exclude_list, exclude_count);
        path_temp.resize(path_temp.size() - i.size());

        directories.push_back(i);

        if (temp.size() < 1)
            continue;

        max_len = 0;
        for (std::string& j : temp) {
            size_t len = j.size();
            if (max_len < len)
                max_len = len;
        }

        if (i.size()) {
            std::string sub_path_temp;
            sub_path_temp.assign(i);
            sub_path_temp += '\\';
            for (std::string& j : temp)
                directories.push_back(sub_path_temp + j);
        }
    }
    return directories;
}

std::vector<std::wstring> path_get_directories_recursive(
    const wchar_t* path, const wchar_t** exclude_list, size_t exclude_count) {
    size_t dir_len = utf16_length(path);

    std::wstring dir;
    std::wstring temp;
    dir.assign(path, dir_len);
    if (dir.size() && dir.back() != L'\\')
        dir.push_back(L'\\');
    temp.assign(dir);
    dir.push_back(L'*');

    WIN32_FIND_DATAW fdata = {};
    HANDLE h = FindFirstFileW(dir.c_str(), &fdata);
    if (h == INVALID_HANDLE_VALUE)
        return {};

    std::vector<std::wstring> temp_vec;
    do {
        if (!(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            || !str_utils_compare(fdata.cFileName, L".")
            || !str_utils_compare(fdata.cFileName, L".."))
            continue;

        if (exclude_list && exclude_count) {
            size_t len = utf16_length(fdata.cFileName);
            temp.append(fdata.cFileName, len);

            bool exclude = false;
            for (size_t i = 0; i < exclude_count; i++) {
                size_t exclude_path_len = utf16_length(exclude_list[i]);
                size_t pos = temp.rfind(exclude_list[i], -1, exclude_path_len);
                if (pos != -1 && pos == temp.size() - exclude_path_len) {
                    exclude = true;
                    break;
                }
            }
            temp.resize(temp.size() - len);

            if (exclude)
                continue;
        }

        std::wstring directory = std::wstring(fdata.cFileName);
        temp_vec.push_back(directory);
    } while (FindNextFileW(h, &fdata));
    FindClose(h);

    size_t max_len = 0;
    for (std::wstring& i : temp_vec) {
        size_t len = i.size();
        if (max_len < len)
            max_len = len;
    }

    std::vector<std::wstring> directories;
    std::wstring path_temp;
    path_temp.assign(path);
    path_temp.push_back(L'\\');
    for (std::wstring& i : temp_vec) {
        path_temp.append(i);
        std::vector<std::wstring> temp = path_get_directories_recursive(
            path_temp.c_str(), exclude_list, exclude_count);
        path_temp.resize(path_temp.size() - i.size());

        directories.push_back(i);

        if (temp.size() < 1)
            continue;

        max_len = 0;
        for (std::wstring& j : temp) {
            size_t len = j.size();
            if (max_len < len)
                max_len = len;
        }

        if (i.size()) {
            std::wstring sub_path_temp;
            sub_path_temp.assign(i);
            sub_path_temp.push_back(L'\\');
            for (std::wstring& j : temp)
                directories.push_back(sub_path_temp + j);
        }
    }
    return directories;
}

void path_get_full_path(std::string& str) {
    wchar_t buf[MAX_PATH * 2];
    buf[0] = 0;
    wchar_t* utf16_temp = utf8_to_utf16(str.c_str());
    if (utf16_temp)
        GetFullPathNameW(utf16_temp, MAX_PATH * 2, buf, 0);
    free_def(utf16_temp);

    char* utf0_temp = utf16_to_utf8(buf);
    if (utf0_temp)
        str.assign(utf0_temp);
    free_def(utf0_temp);
}

void path_get_full_path(std::wstring& str) {
    wchar_t buf[MAX_PATH * 2];
    buf[0] = 0;
    GetFullPathNameW(str.c_str(), MAX_PATH * 2, buf, 0);
    str.assign(buf);
}
