/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "str_utils.hpp"

bool str_utils_check_ends_with(const char* str, const char* mask) {
    if (!str || !mask)
        return false;

    size_t mask_len = utf8_length(mask);
    size_t len = utf8_length(str);
    const char* t = str;
    while (t) {
        t = strstr(t, mask);
        if (t) {
            t += mask_len;
            if (t == str + len)
                return true;
        }
    }
    return false;
}

bool str_utils_check_ends_with(const wchar_t* str, const wchar_t* mask) {
    if (!str || !mask)
        return false;

    size_t mask_len = utf16_length(mask);
    size_t len = utf16_length(str);
    const wchar_t* t = str;
    while (t) {
        t = wcsstr(t, mask);
        if (t) {
            t += mask_len;
            if (t == str + len)
                return true;
        }
    }
    return false;
}

const char* str_utils_get_next_int32_t(const char* str, int32_t& value, const char split) {
    std::string s;
    str = str_utils_get_next_string(str, s, split);
    sscanf_s(s.c_str(), "%d", &value);
    return str;
}

const wchar_t* str_utils_get_next_int32_t(const wchar_t* str, int32_t& value, const wchar_t split) {
    std::wstring s;
    str = str_utils_get_next_string(str, s, split);
    swscanf_s(s.c_str(), L"%d", &value);
    return str;
}

const char* str_utils_get_next_float_t(const char* str, float_t& value, const char split) {
    std::string s;
    str = str_utils_get_next_string(str, s, split);
    sscanf_s(s.c_str(), "%f", &value);
    return str;
}

const wchar_t* str_utils_get_next_float_t(const wchar_t* str, float_t& value, const wchar_t split) {
    std::wstring s;
    str = str_utils_get_next_string(str, s, split);
    swscanf_s(s.c_str(), L"%f", &value);
    return str;
}

const char* str_utils_get_next_string(const char* str, std::string& value, const char split) {
    value.clear();

    if (!str)
        return 0;

    const char* t = strchr(str, split);
    if (!t) {
        value.assign(str);
        return 0;
    }

    value.assign(str, t - str);
    t++;
    return *t ? t : 0;
}

const wchar_t* str_utils_get_next_string(const wchar_t* str, std::wstring& value, const wchar_t split) {
    value.clear();

    if (!str)
        return 0;

    const wchar_t* t = wcschr(str, split);
    if (!t) {
        value.assign(str);
        return 0;
    }

    value.assign(str, t - str);
    t++;
    return *t ? t : 0;
}

char* str_utils_split_get_right(const char* str, const char split) {
    if (!str)
        return 0;

    const char* t = strchr(str, split);
    if (!t)
        return str_utils_copy(str);
    t++;

    size_t len = utf8_length(t);
    char* p = force_malloc<char>(len + 1);
    memcpy(p, t, len);
    p[len] = 0;
    return p;
}

wchar_t* str_utils_split_get_right(const wchar_t* str, const wchar_t split) {
    if (!str)
        return 0;

    const wchar_t* t = wcschr(str, split);
    if (!t)
        return str_utils_copy(str);
    t++;

    size_t len = utf16_length(t);
    wchar_t* p = force_malloc<wchar_t>(len + 1);
    memcpy(p, t, sizeof(wchar_t) * len);
    p[len] = 0;
    return p;
}

char* str_utils_split_get_left(const char* str, const char split) {
    if (!str)
        return 0;

    const char* t = strchr(str, split);

    size_t len = t ? t - str : utf8_length(str);
    char* p = force_malloc<char>(len + 1);
    memcpy(p, str, len);
    p[len] = 0;
    return p;
}

wchar_t* str_utils_split_get_left(const wchar_t* str, const wchar_t split) {
    if (!str)
        return 0;

    const wchar_t* t = wcschr(str, split);

    size_t len = t ? t - str : utf16_length(str);
    wchar_t* p = force_malloc<wchar_t>(len + 1);
    memcpy(p, str, sizeof(wchar_t) * len);
    p[len] = 0;
    return p;
}

char* str_utils_split_get_right_include(const char* str, const char split) {
    if (!str)
        return 0;

    const char* t = strchr(str, split);
    if (!t)
        return str_utils_copy(str);

    size_t len = utf8_length(t);
    char* p = force_malloc<char>(len + 1);
    memcpy(p, t, len);
    p[len] = 0;
    return p;
}

wchar_t* str_utils_split_get_right_include(const wchar_t* str, const wchar_t split) {
    if (!str)
        return 0;

    const wchar_t* t = wcschr(str, split);
    if (!t)
        return str_utils_copy(str);

    size_t len = utf16_length(t);
    wchar_t* p = force_malloc<wchar_t>(len + 1);
    memcpy(p, t, sizeof(wchar_t) * len);
    p[len] = 0;
    return p;
}

char* str_utils_split_get_left_include(const char* str, const char split) {
    if (!str)
        return 0;

    const char* t = strchr(str, split);
    t++;

    size_t len = t ? t - str : utf8_length(str);
    char* p = force_malloc<char>(len + 1);
    memcpy(p, str, len);
    p[len] = 0;
    return p;
}

wchar_t* str_utils_split_get_left_include(const wchar_t* str, const wchar_t split) {
    if (!str)
        return 0;

    const wchar_t* t = wcschr(str, split);
    t++;

    size_t len = t ? t - str : utf16_length(str);
    wchar_t* p = force_malloc<wchar_t>(len + 1);
    memcpy(p, str, sizeof(wchar_t) * len);
    p[len] = 0;
    return p;
}

char* str_utils_split_right_get_right(const char* str, const char split) {
    if (!str)
        return 0;

    const char* t = strrchr(str, split);
    if (!t)
        return str_utils_copy(str);
    t++;

    size_t len = t - str;
    char* p = force_malloc<char>(len + 1);
    memcpy(p, t, len);
    p[len] = 0;
    return p;
}

wchar_t* str_utils_split_right_get_right(const wchar_t* str, const wchar_t split) {
    if (!str)
        return 0;

    const wchar_t* t = wcsrchr(str, split);
    if (!t)
        return str_utils_copy(str);
    t++;

    size_t len = t - str;
    wchar_t* p = force_malloc<wchar_t>(len + 1);
    memcpy(p, t, sizeof(wchar_t) * len);
    p[len] = 0;
    return p;
}

char* str_utils_split_right_get_left(const char* str, const char split) {
    if (!str)
        return 0;

    const char* t = strrchr(str, split);

    size_t len = t ? t - str : utf8_length(str);
    char* p = force_malloc<char>(len + 1);
    memcpy(p, str, len);
    p[len] = 0;
    return p;
}

wchar_t* str_utils_split_right_get_left(const wchar_t* str, const wchar_t split) {
    if (!str)
        return 0;

    const wchar_t* t = wcsrchr(str, split);

    size_t len = t ? t - str : utf16_length(str);
    wchar_t* p = force_malloc<wchar_t>(len + 1);
    memcpy(p, str, sizeof(wchar_t) * len);
    p[len] = 0;
    return p;
}

char* str_utils_split_right_get_right_include(const char* str, const char split) {
    if (!str)
        return 0;

    const char* t = strrchr(str, split);
    if (!t)
        return str_utils_copy(str);

    size_t len = utf8_length(t);
    char* p = force_malloc<char>(len + 1);
    memcpy(p, t, len);
    p[len] = 0;
    return p;
}

wchar_t* str_utils_split_right_get_right_include(const wchar_t* str, const wchar_t split) {
    if (!str)
        return 0;

    const wchar_t* t = wcsrchr(str, split);
    if (!t)
        return str_utils_copy(str);

    size_t len = utf16_length(t);
    wchar_t* p = force_malloc<wchar_t>(len + 1);
    memcpy(p, t, sizeof(wchar_t) * len);
    p[len] = 0;
    return p;
}

char* str_utils_split_right_get_left_include(const char* str, const char split) {
    if (!str)
        return 0;

    const char* t = strrchr(str, split);
    if (t)
        t++;

    size_t len = t ? t - str : utf8_length(str);
    char* p = force_malloc<char>(len + 1);
    memcpy(p, str, len);
    p[len] = 0;
    return p;
}

wchar_t* str_utils_split_right_get_left_include(const wchar_t* str, const wchar_t split) {
    if (!str)
        return 0;

    const wchar_t* t = wcsrchr(str, split);
    if (t)
        t++;

    size_t len = t ? t - str : utf16_length(str);
    wchar_t* p = force_malloc<wchar_t>(len + 1);
    memcpy(p, str, sizeof(wchar_t) * len);
    p[len] = 0;
    return p;
}

char* str_utils_get_extension(const char* str) {
    if (!str)
        return 0;

    const char* t = strrchr(str, '\\');
    return str_utils_split_right_get_right_include(t ? t + 1 : str, '.');
}

wchar_t* str_utils_get_extension(const wchar_t* str) {
    if (!str)
        return 0;

    const wchar_t* t = wcsrchr(str, L'\\');
    return str_utils_split_right_get_right_include(t ? t + 1 : str, L'.');
}

char* str_utils_get_without_extension(const char* str) {
    if (!str)
        return 0;

    const char* t = strrchr(str, '\\');
    return str_utils_split_right_get_left(t ? t + 1 : str, '.');
}

wchar_t* str_utils_get_without_extension(const wchar_t* str) {
    if (!str)
        return 0;

    const wchar_t* t = wcsrchr(str, L'\\');
    return str_utils_split_right_get_left(t ? t + 1 : str, L'.');
}

char* str_utils_add(const char* str0, const char* str1) {
    if (str0 && str1) {
        size_t str0_len = utf8_length(str0);
        size_t str1_len = utf8_length(str1);
        char* p = force_malloc<char>(str0_len + str1_len + 1);
        memcpy(p, str0, str0_len + 1);
        memcpy(p + str0_len, str1, str1_len + 1);
        return p;
    }
    else if (str0)
        return str_utils_copy(str0);
    else if (str1)
        return str_utils_copy(str1);
    else
        return 0;
}

wchar_t* str_utils_add(const wchar_t* str0, const wchar_t* str1) {
    if (str0 && str1) {
        size_t str0_len = utf16_length(str0);
        size_t str1_len = utf16_length(str1);
        wchar_t* p = force_malloc<wchar_t>(str0_len + str1_len + 1);
        memcpy(p, str0, sizeof(wchar_t) * (str0_len + 1));
        memcpy(p + str0_len, str1, sizeof(wchar_t) * (str1_len + 1));
        return p;
    }
    else if (str0)
        return str_utils_copy(str0);
    else if (str1)
        return str_utils_copy(str1);
    else
        return 0;
}

char* str_utils_copy(const char* str) {
    if (!str)
        return 0;

    size_t len = utf8_length(str) + 1;
    char* p = force_malloc<char>(len);
    memcpy(p, str, len);
    return p;
}

wchar_t* str_utils_copy(const wchar_t* str) {
    if (!str)
        return 0;

    size_t len = utf16_length(str) + 1;
    wchar_t* p = force_malloc<wchar_t>(len);
    memcpy(p, str, sizeof(wchar_t) * len);
    return p;
}

inline int32_t str_utils_compare_length(const char* str0, size_t str0_len, const char* str1, size_t str1_len) {
    if (!str0_len)
        return -*str1;
    else if (!str1_len)
        return *str0;

    size_t str0_len_act = str0_len;
    const char* i0 = str0;
    for (size_t i = str0_len; i; i--)
        if (!*i0++) {
            str0_len_act = i + 1;
            break;
        }

    size_t str1_len_act = str1_len;
    const char* i1 = str1;
    for (size_t i = str1_len; i; i--)
        if (!*i1++) {
            str1_len_act = i + 1;
            break;
        }

    str0_len = str0_len_act;
    str1_len = str1_len_act;

    int32_t diff = 0;
    char c0;
    char c1;
    do {
        c0 = *str0++;
        c1 = *str1++;
        if (!c0 || !c1)
            return c0 - c1;
    } while (c0 == c1 && --str0_len && --str1_len);
    return c0 - c1;
}

inline int32_t str_utils_compare_length(const wchar_t* str0, size_t str0_len, const wchar_t* str1, size_t str1_len) {
    if (!str0_len)
        return -*str1;
    else if (!str1_len)
        return *str0;

    size_t str0_len_act = str0_len;
    const wchar_t* i0 = str0;
    for (size_t i = str0_len; i; i--)
        if (!*i0++) {
            str0_len_act = i + 1;
            break;
        }

    size_t str1_len_act = str1_len;
    const wchar_t* i1 = str1;
    for (size_t i = str1_len; i; i--)
        if (!*i1++) {
            str1_len_act = i + 1;
            break;
        }

    str0_len = str0_len_act;
    str1_len = str1_len_act;

    int32_t diff = 0;
    wchar_t c0;
    wchar_t c1;
    do {
        c0 = *str0++;
        c1 = *str1++;
        if (!c0 || !c1)
            return c0 - c1;
    } while (c0 == c1 && --str0_len && --str1_len);
    return c0 - c1;
}

size_t str_utils_get_substring_offset(const char* str0, size_t str0_len,
    size_t str0_off, const char* str1, size_t str1_len) {
    if (!str1_len && str0_off <= str0_len)
        return str0_off;

    if (str0_off < str0_len && str1_len <= str0_len - str0_off) {
        size_t len = str0_len - str1_len - str0_off + 1;
        const char* str = &str0[str0_off];
        for (; len; ) {
            const char* s = (const char*)memchr(str, *str1, len);
            if (!s)
                break;

            if (!str1_len || !memcmp(s, str1, str1_len))
                return s - str0;

            len += str - (s + 1);
            str = s + 1;
        }
    }
    return -1;
}

size_t str_utils_get_substring_offset(const wchar_t* str0, size_t str0_len,
    size_t str0_off, const wchar_t* str1, size_t str1_len) {
    if (!str1_len && str0_off <= str0_len)
        return str0_off;

    if (str0_off < str0_len && str1_len <= str0_len - str0_off) {
        size_t len = str0_len - str1_len - str0_off + 1;
        const wchar_t* str = &str0[str0_off];
        for (; len; ) {
            const wchar_t* s = wmemchr(str, *str1, len);
            if (!s)
                break;

            if (!str1_len || !memcmp(s, str1, sizeof(wchar_t) * str1_len))
                return s - str0;

            len += str - (s + 1);
            str = s + 1;
        }
    }
    return -1;
}

bool str_utils_text_file_parse(const void* data, size_t size,
    char*& buf, char**& lines, size_t& count) {
    if (!data || !size)
        return false;

    const char* d = (const char*)data;
    bool del = false;
    size_t c;
    buf = 0;
    lines = 0;
    count = 0;
    if ((uint8_t)d[0] == 0x00)
        return false;
    else if (d[0] == 0xFF) {
        if (size == 1 || (uint8_t)d[1] != 0xFE || size == 2)
            return false;

        wchar_t* w_d = (wchar_t*)data + 1;
        d = utf16_to_utf8(w_d);
        size = utf8_length(d);
        del = true;
        goto decode_utf8_ansi;
    }
    else if ((uint8_t)d[0] == 0xFE) {
        if (size == 1 || (uint8_t)d[1] != 0xFF || size == 2)
            return false;

        size /= 2;
        wchar_t* w_d = (wchar_t*)data + 1;
        w_d = str_utils_copy(w_d);
        for (size_t i = 0; i < size; i++)
            w_d[i] = (wchar_t)reverse_endianness_uint16_t((uint16_t)w_d[i]);
        d = utf16_to_utf8(w_d);
        size = utf8_length(d);
        del = true;
        free_def(w_d);
        goto decode_utf8_ansi;
    }
    else if ((uint8_t)d[0] == 0xEF) {
        if (size == 1 || (uint8_t)d[1] != 0xBB || size == 2 || (uint8_t)d[2] != 0xBF || size == 3)
            return false;

        d += 3;
        size -= 3;
        goto decode_utf8_ansi;
    }
    else {
    decode_utf8_ansi:
        c = 1;
        bool lf;
        char ch;
        const char* t;
        lf = false;
        t = d;
        ch = 0;

        size_t buf_len = size;
        for (size_t i = 0, l = 0, m = 0; i < size; i++) {
            ch = *t++;
            if (ch == '\r') {
                if (i + 1 < size && *t == '\n') {
                    i++;
                    t++;
                    l++;
                }
                lf = true;
            }
            else if (ch == '\n')
                lf = true;

            if (lf) {
                if (!l && c > 1)
                    buf_len--;
                c++;
                l = 0;
                m = 0;
                lf = false;
            }
            else {
                l++;
                m++;
            }
        }

        if (ch != '\r' && ch != '\n')
            buf_len++;
        else
            c--;

        lf = false;
        t = d;
        char* temp_buf = force_malloc<char>(buf_len);
        char** temp_lines = force_malloc<char*>(c);

        char* b = temp_buf;
        for (size_t i = 0, j = 0, l = 0, m = 0; j < c; i++) {
            ch = *t++;
            if (ch == '\r') {
                if (i + 1 < size && *t == '\n') {
                    i++;
                    t++;
                    l++;
                }
                lf = true;
            }
            else if (ch == '\n')
                lf = true;

            if (i >= size || lf) {
                temp_lines[j] = b;
                if (l) {
                    memcpy(b, d + i - l, m);
                    b[l] = 0;
                    b += l + 1;
                }
                else if (j)
                    temp_lines[j]--;
                else
                    *b++ = 0;
                j++;

                if (!lf)
                    break;

                l = 0;
                m = 0;
                lf = false;
            }
            else {
                l++;
                m++;
            }
        }

        buf = temp_buf;
        lines = temp_lines;
        count = c;
    }

    if (del) {
        void* data = (void*)d;
        free_def(data);
    }
    return true;
}
