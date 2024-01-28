/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "farc.hpp"
#include "io/path.hpp"
#include "io/file_stream.hpp"
#include "io/memory_stream.hpp"
#include "aes.hpp"
#include "deflate.hpp"
#include "hash.hpp"
#include "str_utils.hpp"

static const uint8_t key[] = {
    0x70, 0x72, 0x6F, 0x6A, 0x65, 0x63, 0x74, 0x5F,
    0x64, 0x69, 0x76, 0x61, 0x2E, 0x62, 0x69, 0x6E,
};

static const uint8_t key_ft[] = {
    0x13, 0x72, 0xD5, 0x7B, 0x6E, 0x9E, 0x31, 0xEB,
    0xA2, 0x39, 0xB8, 0x3C, 0x15, 0x57, 0xC6, 0xBB,
};

static errno_t farc_get_files(farc* f);
static void farc_pack_files(farc* f, stream& s, farc_signature signature, farc_flags flags, bool get_files = false);
static errno_t farc_read_header(farc* f, stream& s);
static void farc_unpack_files(farc* f, stream& s, bool save);
static void farc_unpack_file(farc* f, farc_file* ff);
static void farc_unpack_file(farc* f, stream& s, farc_file* ff,
    bool save = false, char* temp_path = 0, size_t dir_len = 0);
static void farc_write_padding(farc* f, stream& s, size_t size, bool x = false);

farc::farc() : flags(), ft() {
    signature = FARC_FArC;
    compression_level = 12;
    alignment = 0x10;
}

farc::~farc() {

}

farc_file* farc::add_file(const char* name) {
    if (!name)
        return 0;

    uint64_t name_hash = hash_utf8_fnv1a64m(name, true);
    for (farc_file& i : files)
        if (hash_string_fnv1a64m(i.name, true) == name_hash)
            return &i;

    size_t files_count = files.size();
    void** data_temp = new void* [files_count];
    void** data_comp_temp = new void* [files_count];
    if (!data_temp || !data_comp_temp) {
        if (data_temp)
            delete[] data_temp;
        if (data_comp_temp)
            delete[] data_comp_temp;
        return 0;
    }

    for (size_t i = 0; i < files_count; i++) {
        data_temp[i] = files[i].data;
        data_comp_temp[i] = files[i].data_compressed;
        files[i].data = 0;
        files[i].data_compressed = 0;
    }

    files.push_back({});
    files.back().name.assign(name);

    for (size_t i = 0; i < files_count; i++) {
        files[i].data = data_temp[i];
        files[i].data_compressed = data_comp_temp[i];
        data_temp[i] = 0;
        data_comp_temp[i] = 0;
    }

    delete[] data_temp;
    delete[] data_comp_temp;

    return &files.back();
}

farc_file* farc::add_file(const wchar_t* name) {
    if (!name)
        return 0;

    uint64_t name_hash = hash_utf16_fnv1a64m(name, true);
    for (farc_file& i : files)
        if (hash_string_fnv1a64m(i.name, true) == name_hash)
            return &i;

    size_t files_count = files.size();
    void** data_temp = new void* [files_count];
    void** data_comp_temp = new void* [files_count];
    if (!data_temp || !data_comp_temp) {
        if (data_temp)
            delete[] data_temp;
        if (data_comp_temp)
            delete[] data_comp_temp;
    }

    for (size_t i = 0; i < files_count; i++) {
        data_temp[i] = files[i].data;
        data_comp_temp[i] = files[i].data_compressed;
        files[i].data = 0;
        files[i].data_compressed = 0;
    }

    files.push_back({});
    if (name) {
        char* name_temp = utf16_to_utf8(name);
        files.back().name.assign(name_temp);
        free_def(name_temp);
    }

    for (size_t i = 0; i < files_count; i++) {
        files[i].data = data_temp[i];
        files[i].data_compressed = data_comp_temp[i];
        data_temp[i] = 0;
        data_comp_temp[i] = 0;
    }

    delete[] data_temp;
    delete[] data_comp_temp;

    return &files.back();
}

const char* farc::get_file_name(uint32_t hash) {
    if (!hash || hash == hash_murmurhash_empty)
        return 0;

    for (farc_file& i : files) {
        const char* l_str = i.name.c_str();
        const char* t = strrchr(l_str, '.');
        size_t l_len = i.name.size();
        if (t)
            l_len = t - l_str;

        if (hash_murmurhash(l_str, l_len) == hash)
            return l_str;
    }
    return 0;
}

size_t farc::get_file_size(const char* name) {
    if (!name)
        return 0;

    uint64_t name_hash = hash_utf8_fnv1a64m(name, true);
    for (farc_file& i : files)
        if (hash_string_fnv1a64m(i.name, true) == name_hash)
            return i.size;
    return 0;
}

size_t farc::get_file_size(const wchar_t* name) {
    if (!name)
        return 0;

    uint64_t name_hash = hash_utf16_fnv1a64m(name, true);
    for (farc_file& i : files)
        if (hash_string_fnv1a64m(i.name, true) == name_hash)
            return i.size;
    return 0;
}

size_t farc::get_file_size(uint32_t hash) {
    if (!hash || hash == hash_murmurhash_empty)
        return 0;

    for (farc_file& i : files) {
        const char* l_str = i.name.c_str();
        const char* t = strrchr(l_str, '.');
        size_t l_len = i.name.size();
        if (t)
            l_len = t - l_str;

        if (hash_murmurhash(l_str, l_len) == hash)
            return i.size;
    }
    return 0;
}

bool farc::has_file(const char* name) {
    if (!name)
        return false;

    uint64_t name_hash = hash_utf8_fnv1a64m(name, true);
    for (farc_file& i : files)
        if (hash_string_fnv1a64m(i.name, true) == name_hash)
            return true;
    return false;
}

bool farc::has_file(const wchar_t* name) {
    if (!name)
        return false;

    uint64_t name_hash = hash_utf16_fnv1a64m(name, true);
    for (farc_file& i : files)
        if (hash_string_fnv1a64m(i.name, true) == name_hash)
            return true;
    return false;
}

bool farc::has_file(uint32_t hash) {
    if (!hash || hash == hash_murmurhash_empty)
        return false;

    for (farc_file& i : files) {
        const char* l_str = i.name.c_str();
        const char* t = strrchr(l_str, '.');
        size_t l_len = i.name.size();
        if (t)
            l_len = t - l_str;

        if (hash_murmurhash(l_str, l_len) == hash)
            return true;
    }
    return false;
}

void farc::read(const char* path, bool unpack, bool save) {
    if (!path)
        return;

    wchar_t* path_buf = utf8_to_utf16(path);
    read(path_buf, unpack, save);
    free_def(path_buf);
}

void farc::read(const wchar_t* path, bool unpack, bool save) {
    if (!path)
        return;

    files.clear();

    wchar_t full_path_buf[MAX_PATH];
    wchar_t* full_path = _wfullpath(full_path_buf, path, MAX_PATH);

    if (!full_path)
        return;
    else if (!path_check_file_exists(full_path_buf))
        return;

    char* dir_temp = utf16_to_utf8(full_path_buf);
    size_t dir_temp_len = utf8_length(dir_temp);
    file_path.assign(dir_temp, dir_temp_len);
    directory_path.assign(dir_temp, dir_temp_len);
    free_def(dir_temp);

    const char* dot = strrchr(directory_path.c_str(), '.');
    if (dot)
        directory_path = directory_path.substr(0, dot - directory_path.c_str());

    file_stream s;
    s.open(file_path.c_str(), "rb");
    if (s.check_not_null() && !farc_read_header(this, s) && unpack)
        farc_unpack_files(this, s, save);
}

void farc::read(const void* data, size_t size, bool unpack) {
    if (!data || !size)
        return;

    files.clear();
    file_path.clear();

    directory_path.clear();

    memory_stream s;
    s.open(data, size);
    if (!farc_read_header(this, s) && unpack)
        farc_unpack_files(this, s, false);
}

farc_file* farc::read_file(const char* name) {
    if (!name)
        return 0;

    uint64_t name_hash = hash_utf8_fnv1a64m(name, true);
    for (farc_file& i : files)
        if (hash_string_fnv1a64m(i.name, true) == name_hash) {
            farc_unpack_file(this, &i);
            return &i;
        }
    return 0;
}

farc_file* farc::read_file(const wchar_t* name) {
    if (!name)
        return 0;

    uint64_t name_hash = hash_utf16_fnv1a64m(name, true);
    for (farc_file& i : files)
        if (hash_string_fnv1a64m(i.name, true) == name_hash) {
            farc_unpack_file(this, &i);
            return &i;
        }
    return 0;
}

farc_file* farc::read_file(uint32_t hash) {
    if (!hash || hash == hash_murmurhash_empty)
        return 0;

    for (farc_file& i : files) {
        const char* l_str = i.name.c_str();
        const char* t = strrchr(l_str, '.');
        size_t l_len = i.name.size();
        if (t)
            l_len = t - l_str;

        if (hash_murmurhash(l_str, l_len) == hash) {
            farc_unpack_file(this, &i);
            return &i;
        }
    }
    return 0;
}

void farc::write(const char* path, farc_signature signature, farc_flags flags, bool get_files) {
    if (!path)
        return;

    wchar_t* path_buf = utf8_to_utf16(path);
    write(path_buf, signature, flags, get_files);
    free_def(path_buf);
}

void farc::write(const wchar_t* path, farc_signature signature, farc_flags flags, bool get_files) {
    if (!path)
        return;

    if (get_files)
        files.clear();

    wchar_t full_path_buf[MAX_PATH];
    wchar_t* full_path = _wfullpath(full_path_buf, path, MAX_PATH);

    if (!full_path)
        return;
    else if (get_files && !path_check_directory_exists(full_path_buf))
        return;

    char* dir_temp = utf16_to_utf8(full_path_buf);
    size_t dir_temp_len = utf8_length(dir_temp);
    directory_path.assign(dir_temp, dir_temp_len);
    file_path.assign(dir_temp, dir_temp_len);
    file_path.append(".farc", 5);
    free_def(dir_temp);

    if (!get_files || (get_files && !farc_get_files(this))) {
        file_stream s;
        s.open(file_path.c_str(), "wb");
        if (s.check_not_null())
            farc_pack_files(this, s, signature, flags, get_files);
    }
}

void farc::write(void** data, size_t* size, farc_signature signature, farc_flags flags) {
    if (!data || !size)
        return;

    directory_path.clear();
    file_path.clear();

    memory_stream s;
    s.open();
    farc_pack_files(this, s, signature, flags);
}

bool farc::load_file(void* data, const char* path, const char* file, uint32_t hash) {
    size_t file_len = utf8_length(file);
    if (file_len < 5 || memcmp(&file[file_len - 5], ".farc", 6))
        return false;

    size_t path_len = utf8_length(path);
    if (path_len + file_len + 2 > 0x1000)
        return false;

    char buf[0x1000];
    memcpy(buf, path, path_len);
    memcpy(buf + path_len, file, file_len + 1);
    if (!path_check_file_exists(buf))
        return false;

    farc* f = (farc*)data;
    f->read(buf, true, false);
    return !!f->files.size();
}

static errno_t farc_get_files(farc* f) {
    f->files.clear();
    f->files.shrink_to_fit();

    std::vector<std::string> files = path_get_files(f->directory_path.c_str());
    if (files.size() < 1)
        return -1;

    f->files = std::vector<farc_file>(files.size());
    for (farc_file& i : f->files)
        i.name = files[&i - f->files.data()];
    return 0;
}

static void farc_pack_files(farc* f, stream& s, farc_signature signature, farc_flags flags, bool get_files) {
    bool plain = false;
    for (farc_file& i : f->files) {
        bool is_a3da = i.name.find(".a3da") == i.name.size() - 5;
        bool is_diva = i.name.find(".diva") == i.name.size() - 5;
        bool is_vag  = i.name.find(".vag" ) == i.name.size() - 4;

        if (is_a3da || is_diva || is_vag) {
            plain = true;
            break;
        }
    }

    if (plain)
        signature = FARC_FArc;

    bool compressed = false;
    bool encrypted = false;
    switch (signature) {
    case FARC_FARC:
        f->flags = flags;
        compressed = !!(flags & FARC_GZIP);
        encrypted = !!(flags & FARC_AES);
        break;
    case FARC_FArC:
        f->flags = (farc_flags)0;
        compressed = true;
        break;
    default:
        f->flags = (farc_flags)0;
        break;
    }

    size_t header_length = 0;
    switch (signature) {
    case FARC_FARC:
        header_length += sizeof(int32_t) * 5;
        break;
    default:
        header_length += sizeof(int32_t);
        break;
    }

    if (signature == FARC_FArc)
        for (farc_file& i : f->files) {
            header_length += i.name.size() + 1;
            header_length += sizeof(int32_t) * 2;
        }
    else
        for (farc_file& i : f->files) {
            header_length += i.name.size() + 1;
            header_length += sizeof(int32_t) * 3;
        }

    size_t align = header_length + 8;
    s.set_position(align_val(align, f->alignment), SEEK_SET);
    size_t dir_len = f->directory_path.size();

    aes128_ctx ctx;
    if (signature == FARC_FARC)
        aes128_init_ctx(&ctx, key);

    f->compression_level = clamp_def(f->compression_level, 0, 12);

    if (get_files) {
        char* temp = force_malloc<char>(dir_len + 2 + MAX_PATH);
        memcpy(temp, f->directory_path.c_str(), sizeof(char) * dir_len);
        temp[dir_len] = '\\';
        for (farc_file& i : f->files) {
            if (i.name.size()) {
                memcpy(temp + dir_len + 1, i.name.c_str(), sizeof(char) * i.name.size());
                temp[dir_len + 1 + i.name.size()] = '\0';
            }

            i.offset = s.get_position();
            i.size = 0;
            i.size_compressed = 0;
            free_def(i.data);
            free_def(i.data_compressed);
            i.compressed = false;
            i.encrypted = false;

            file_stream s_t;
            s_t.open(temp, "rb");
            if (s_t.check_null())
                continue;

            size_t file_len = s_t.get_length();

            i.size = file_len;
            i.data = force_malloc(file_len);
            s_t.read(i.data, file_len);
            i.compressed = compressed;
            i.encrypted = encrypted;
            i.data_changed = false;
        }
        free_def(temp);
    }

    for (farc_file& i : f->files) {
        i.offset = s.get_position();
        size_t file_len = i.size;

        if (i.encrypted && encrypted) {
            void* t1;
            size_t t1_len;
            if (i.compressed && compressed) {
                if (!i.data_compressed || i.data_changed) {
                    free_def(i.data_compressed);
                    deflate::compress_gzip(i.data, file_len, &i.data_compressed,
                        &i.size_compressed, f->compression_level, i.name.c_str());
                }
                t1 = i.data_compressed;
                t1_len = i.size_compressed;
            }
            else {
                i.compressed = false;
                free_def(i.data_compressed);
                t1 = i.data;
                t1_len = i.size;
            }

            size_t t2_len = align_val(t1_len, f->alignment);
            uint8_t* t2 = (uint8_t*)force_malloc(t2_len);
            memcpy(t2, t1, t1_len);
            memset(t2 + t1_len, 0x78, t2_len - t1_len);

            aes128_ecb_encrypt_buffer(&ctx, t2, t2_len);

            s.write(t2, t2_len);
            free_def(t2);
        }
        else if (i.compressed && compressed) {
            i.encrypted = false;
            if (!i.data_compressed || i.data_changed) {
                free_def(i.data_compressed);
                deflate::compress_gzip(i.data, file_len, &i.data_compressed,
                    &i.size_compressed, f->compression_level, i.name.c_str());
            }
            s.write(i.data_compressed, i.size_compressed);
            farc_write_padding(f, s, i.size_compressed, signature != FARC_FArc);
        }
        else {
            i.compressed = false;
            i.encrypted = false;
            free_def(i.data_compressed);
            s.write(i.data, file_len);
            farc_write_padding(f, s, i.size);
        }
        i.data_changed = false;
    }

    s.set_position(0, SEEK_SET);
    switch (signature) {
    case FARC_FArc:
    default:
        s.write_uint32_t_reverse_endianness(FARC_FArc, true);
        s.write_uint32_t_reverse_endianness((int32_t)header_length, true);
        s.write_uint32_t_reverse_endianness(f->alignment, true);
        break;
    case FARC_FArC:
        s.write_uint32_t_reverse_endianness(FARC_FArC, true);
        s.write_uint32_t_reverse_endianness((int32_t)header_length, true);
        s.write_uint32_t_reverse_endianness(f->alignment, true);
        break;
    case FARC_FARC:
        s.write_uint32_t_reverse_endianness(FARC_FARC, true);
        s.write_uint32_t_reverse_endianness((int32_t)header_length, true);
        s.write_uint32_t_reverse_endianness(f->flags, true);
        s.write_uint32_t_reverse_endianness(0x00, true);
        s.write_uint32_t_reverse_endianness(f->alignment, true);
        s.write_uint32_t_reverse_endianness(0x00, true);
        s.write_uint32_t_reverse_endianness(0x00, true);
        break;
    }

    switch (signature) {
    case FARC_FArc:
        for (farc_file& i : f->files) {
            s.write_string_null_terminated(i.name);
            s.write_int32_t_reverse_endianness((int32_t)i.offset, true);
            s.write_int32_t_reverse_endianness((int32_t)i.size, true);
        }
        break;
    case FARC_FArC:
        for (farc_file& i : f->files) {
            s.write_string_null_terminated(i.name);
            s.write_int32_t_reverse_endianness((int32_t)i.offset, true);
            if (i.compressed) {
                s.write_int32_t_reverse_endianness((int32_t)i.size_compressed, true);
                s.write_int32_t_reverse_endianness((int32_t)i.size, true);
            }
            else {
                s.write_int32_t_reverse_endianness((int32_t)i.size, true);
                s.write_int32_t_reverse_endianness(0x00, true);
            }
        }
        break;
    case FARC_FARC:
        for (farc_file& i : f->files) {
            s.write_string_null_terminated(i.name);
            s.write_int32_t_reverse_endianness((int32_t)i.offset, true);
            if (i.compressed) {
                s.write_int32_t_reverse_endianness((int32_t)i.size_compressed, true);
                s.write_int32_t_reverse_endianness((int32_t)i.size, true);
            }
            else {
                s.write_int32_t_reverse_endianness((int32_t)i.size, true);
                s.write_int32_t_reverse_endianness(0x00, true);
            }
        }
        break;
    }

    farc_write_padding(f, s, header_length + 0x08, signature != FARC_FArc);
}

static errno_t farc_read_header(farc* f, stream& s) {
    if (!f || s.check_null())
        return -1;

    s.set_position(0, SEEK_SET);
    f->signature = (farc_signature)s.read_uint32_t_reverse_endianness(true);
    switch (f->signature) {
    case FARC_FArc:
    case FARC_FArC:
    case FARC_FARC:
        break;
    default:
        return -2;
    }

    f->ft = false;

    uint32_t header_length = s.read_uint32_t_reverse_endianness(true);
    if (f->signature == FARC_FARC) {
        f->flags = (farc_flags)s.read_uint32_t_reverse_endianness(true);
        s.read_uint32_t();
        uint32_t alignment = s.read_uint32_t_reverse_endianness(true);
        bool modern = !!s.read_uint32_t_reverse_endianness(true);

        // If alignment is way too big, then it might be part of IV
        f->ft = (f->flags & FARC_AES) && modern && alignment > 0x1000;
        s.set_position(0x10, SEEK_SET);
        header_length -= 0x08;
    }

    f->files.clear();

    uint8_t* d_t;
    uint8_t* dt;
    int32_t length = 0;

    dt = d_t = force_malloc<uint8_t>(header_length);
    s.read(d_t, header_length);
    if (f->ft) {
        header_length -= 0x10;

        aes128_ctx ctx;
        aes128_init_ctx_iv(&ctx, key_ft, dt);
        dt += 0x10;
        aes128_cbc_decrypt_buffer(&ctx, dt, header_length);

        header_length -= ((uint8_t*)dt)[header_length - 1]; // PKCS7 Padding
    }

    if (f->ft) {
        f->alignment = load_reverse_endianness_uint32_t((void*)dt);
        uint32_t files_count = load_reverse_endianness_uint32_t((void*)(dt + 8));
        uint32_t entry_size = load_reverse_endianness_uint32_t((void*)(dt + 12));
        dt += sizeof(uint32_t) * 4;

        f->files.clear();
        f->files.reserve(files_count);
        while ((dt - d_t < header_length + 0x08LL) && files_count) {
            size_t length = 0;
            while (dt[length])
                length++;

            farc_file ff;
            ff.name.assign((const char*)dt, length);
            dt += length + 1;
            ff.offset = (size_t)load_reverse_endianness_uint32_t((void*)dt);
            ff.size_compressed = (size_t)load_reverse_endianness_uint32_t((void*)(dt + 4));
            ff.size = (size_t)load_reverse_endianness_uint32_t((void*)(dt + 8));
            farc_flags flags = (farc_flags)load_reverse_endianness_uint32_t((void*)(dt + 12));

            if (ff.size)
                ff.compressed = true;
            else {
                ff.size = ff.size_compressed;
                ff.size_compressed = 0;
                ff.compressed = false;
            }

            ff.encrypted = ((f->flags | flags) & FARC_AES) != 0;
            f->files.push_back(ff);
            dt += entry_size;
            files_count--;
        }

        free_def(d_t);
        return 0;
    }

    size_t entry_size;
    if (f->signature == FARC_FARC) {
        f->alignment = load_reverse_endianness_uint32_t((void*)dt);
        uint32_t entry_offset = load_reverse_endianness_uint32_t((void*)(dt + 4));
        uint32_t header_offset = load_reverse_endianness_uint32_t((void*)(dt + 8));
        dt += sizeof(uint32_t) * 3;

        entry_size = sizeof(uint32_t) * 3 + entry_offset;
    }
    else if (f->signature != FARC_FArc) {
        f->alignment = load_reverse_endianness_uint32_t((void*)dt);
        dt += sizeof(uint32_t);

        entry_size = sizeof(uint32_t) * 3;
    }
    else {
        f->alignment = load_reverse_endianness_uint32_t((void*)dt);
        dt += sizeof(uint32_t);

        entry_size = sizeof(uint32_t) * 2;
    }

    size_t count = 0;
    uint8_t* position = dt;

    while (dt - d_t < header_length) {
        while (*dt++);
        dt += entry_size;
        count++;
    }
    dt = position;

    bool encrypted = !!(f->flags & FARC_AES);

    f->files.clear();
    f->files.resize(count);
    if (f->signature != FARC_FArc)
        for (farc_file& i : f->files) {
            size_t length = 0;
            while (dt[length])
                length++;
            i.name.assign((const char*)dt, length);
            dt += length + 1;
            i.offset = (size_t)load_reverse_endianness_uint32_t((void*)dt);
            i.size_compressed = (size_t)load_reverse_endianness_uint32_t((void*)(dt + 4));
            i.size = (size_t)load_reverse_endianness_uint32_t((void*)(dt + 8));

            if (i.size)
                i.compressed = true;
            else {
                i.size = i.size_compressed;
                i.size_compressed = 0;
                i.compressed = false;
            }

            i.encrypted = encrypted;
            dt += entry_size;
        }
    else
        for (farc_file& i : f->files) {
            size_t length = 0;
            while (dt[length])
                length++;
            i.name.assign((const char*)dt, length);
            dt += length + 1;
            i.offset = (size_t)load_reverse_endianness_uint32_t((void*)dt);
            i.size = (size_t)load_reverse_endianness_uint32_t((void*)(dt + 4));

            i.size_compressed = 0;
            i.compressed = false;
            i.encrypted = false;
            dt += sizeof(int32_t) * 2;
        }

    free_def(d_t);
    return 0;
}

static void farc_unpack_files(farc* f, stream& s, bool save) {
    if (!f || s.check_null() || !f->files.size())
        return;

    size_t max_path_len = 0;
    size_t dir_len = f->directory_path.size();
    for (farc_file& i : f->files) {
        size_t path_len = dir_len + 1 + utf8_to_utf16_length(i.name.c_str());
        if (max_path_len < path_len)
            max_path_len = path_len;
    }

    if (save) {
        wchar_t* dir_temp = utf8_to_utf16(f->directory_path.c_str());
        CreateDirectoryW(dir_temp, 0);
        free_def(dir_temp);
    }

    char* temp_path = force_malloc<char>(max_path_len + 1);
    memcpy(temp_path, f->directory_path.c_str(), sizeof(char) * dir_len);
    temp_path[dir_len] = '\\';

    for (farc_file& i : f->files)
        farc_unpack_file(f, s, &i, save, temp_path, dir_len);

    free_def(temp_path);
}

static void farc_unpack_file(farc* f, farc_file* ff) {
    if (ff->data)
        return;
    else if (ff->data_compressed) {
        deflate::decompress(ff->data_compressed, ff->size_compressed,
            &ff->data, &ff->size, deflate::MODE_GZIP);
        return;
    }

    file_stream s;
    s.open(f->file_path.c_str(), "rb");
    if (s.check_not_null())
        farc_unpack_file(f, s, ff);
}

static void farc_unpack_file(farc* f, stream& s, farc_file* ff, bool save, char* temp_path, size_t dir_len) {
    if (!f || s.check_null())
        return;

    if (ff->data)
        free_def(ff->data);

    s.set_position(ff->offset, SEEK_SET);

    if (f->signature == FARC_FArc) {
        ff->data_compressed = 0;
        ff->data = force_malloc(ff->size);
        s.read(ff->data, ff->size);
    }
    else if (f->signature == FARC_FArC) {
        if (ff->compressed) {
            ff->data_compressed = force_malloc(ff->size_compressed);
            s.read(ff->data_compressed, ff->size_compressed);
            deflate::decompress(ff->data_compressed, ff->size_compressed,
                &ff->data, &ff->size, deflate::MODE_GZIP);
        }
        else {
            ff->data = force_malloc(ff->size);
            s.read(ff->data, ff->size);
        }
    }
    else if (ff->compressed || ff->encrypted) {
        size_t temp_s = ff->compressed ? ff->size_compressed : ff->size;
        temp_s = ff->encrypted ? align_val(temp_s, f->alignment) : temp_s;
        void* temp = force_malloc(temp_s);
        s.read(temp, temp_s);

        size_t t = (size_t)temp;
        if (ff->encrypted)
            if (f->ft) {
                temp_s -= 0x10;
                t += 0x10;

                aes128_ctx ctx;
                aes128_init_ctx_iv(&ctx, key_ft, (uint8_t*)temp);
                aes128_cbc_decrypt_buffer(&ctx, (uint8_t*)t, temp_s);

                ff->size_compressed = temp_s - ((uint8_t*)t)[temp_s - 1]; // PKCS7 Padding
            }
            else {
                aes128_ctx ctx;
                aes128_init_ctx(&ctx, key);
                aes128_ecb_decrypt_buffer(&ctx, (uint8_t*)t, temp_s);
            }

        if (ff->compressed) {
            ff->data_compressed = force_malloc(ff->size_compressed);
            memcpy(ff->data_compressed, (void*)t, ff->size_compressed);
            deflate::decompress(ff->data_compressed, ff->size_compressed,
                &ff->data, &ff->size, deflate::MODE_GZIP);
        }
        else {
            ff->data_compressed = 0;
            ff->data = force_malloc(ff->size);
            memcpy(ff->data, (void*)t, ff->size);
        }
        free_def(temp);
    }
    else {
        ff->data_compressed = 0;
        ff->data = force_malloc(ff->size);
        s.read(ff->data, ff->size);
    }
    ff->data_changed = false;

    if (!save)
        return;

    if (ff->data) {
        if (ff->name.size()) {
            memcpy(temp_path + dir_len + 1, ff->name.c_str(), ff->name.size());
            temp_path[dir_len + 1 + ff->name.size()] = '\0';

            file_stream temp_s;
            temp_s.open(temp_path, "wb");
            if (temp_s.check_not_null())
                temp_s.write(ff->data, ff->size);
        }

        free(ff->data);
        ff->data = 0;
    }

    if (ff->data_compressed) {
        free(ff->data_compressed);
        ff->data_compressed = 0;
    }
}

static void farc_write_padding(farc* f, stream& s, size_t size, bool x) {
    size_t align = align_val(size, f->alignment) - size;
    if (!x) {
        uint8_t padding[] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        };
        s.write(padding, align);
    }
    else {
        uint8_t padding[] = {
            0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
            0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
        };
        s.write(padding, align);
    }
}
