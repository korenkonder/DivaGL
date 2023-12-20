/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shader.hpp"
#include "../KKdLib/io/file_stream.hpp"
#include "../KKdLib/io/path.hpp"
#include "../KKdLib/hash.hpp"
#include "../KKdLib/str_utils.hpp"
#include "gl_state.hpp"
#include "print.hpp"
#include "uniform.hpp"
#include "wrap.hpp"
#include <shlobj_core.h>

struct program_binary {
    GLsizei length;
    GLenum binary_format;
    size_t binary;
};

struct program_spv {
    size_t size;
    size_t spv;
};

extern bool vulkan_render;

static GLuint shader_compile_shader(GLenum type, const char* data, const char* file);
static GLuint shader_compile(const char* vert, const char* frag, const char* vp, const char* fp);
static GLuint shader_compile_binary(const char* vert, const char* frag, const char* vp, const char* fp,
    program_binary* bin, GLsizei* buffer_size, void** binary);
static bool shader_load_binary_shader(program_binary* bin, GLuint* program);
static void shader_update_data(shader_set_data* set);

int32_t shader::bind(shader_set_data* set, uint32_t sub_index) {
    set->curr_shader = 0;

    if (num_sub < 1)
        return -1;

    int32_t sub_shader_index = 0;
    for (shader_sub* i = sub; i->sub_index != sub_index; i++)
        if (++sub_shader_index >= num_sub)
            return -1;

    shader_sub* sub_shader = &sub[sub_shader_index];

    int32_t unival_shad_curr = 1;
    int32_t unival_shad = 0;
    int32_t uniform_val[SHADER_MAX_UNIFORM_VALUES] = {};
    if (num_uniform > 0) {
        const int32_t* vp_unival_max = sub_shader->vp_unival_max;
        const int32_t* fp_unival_max = sub_shader->fp_unival_max;

        int32_t i = 0;
        for (i = 0; i < num_uniform && i < sizeof(uniform_val) / sizeof(int32_t); i++) {
            const int32_t unival = uniform->arr[use_uniform[i]];
            const int32_t unival_max = use_permut[i]
                ? max_def(vp_unival_max[i], fp_unival_max[i]) : 0;
            unival_shad += unival_shad_curr * min_def(unival, unival_max);
            unival_shad_curr *= unival_max + 1;

            int32_t unival_max_glsl = max_def(vp_unival_max[i], fp_unival_max[i]);
            uniform_val[i] = min_def(unival, unival_max_glsl);
        }
    }

    shader_sub_shader* shader = &sub_shader->shaders[unival_shad];
    set->curr_shader = shader;

    gl_state_use_program(shader->program);
    if (memcmp(shader->uniform_val, uniform_val, sizeof(uniform_val))) {
        memcpy(shader->uniform_val, uniform_val, sizeof(uniform_val));
        shader->uniform_val_update = true;
    }
    return 0;
}

bool shader::parse_define(const char* data, char** temp, size_t* temp_size) {
    if (!data)
        return false;

    const char* def = strstr(data, "//DEF\n");
    if (!def) {
        size_t len = utf8_length(data);
        if (len + 1 > *temp_size) {
            free_def(*temp);
            *temp_size = len + 1;
            *temp = force_malloc<char>(*temp_size);
        }

        memcpy(*temp, data, len);
        (*temp)[len] = 0;
        return true;
    }

    size_t len_a = def - data;
    def += 5;
    if (*def == '\n')
        def++;

    size_t len_b = utf8_length(def);
    size_t len = 0;
    len += len_a + len_b;

    if (len + 1 > *temp_size) {
        free_def(*temp);
        *temp_size = len + 1;
        *temp = force_malloc<char>(*temp_size);
    }

    size_t pos = 0;
    memcpy(*temp + pos, data, len_a);
    pos += len_a;
    memcpy(*temp + pos, def, len_b);
    pos += len_b;
    (*temp)[pos] = 0;
    return true;
}

bool shader::parse_define(const char* data, int32_t num_uniform,
    int32_t* uniform_value, char** temp, size_t* temp_size) {
    if (!data)
        return false;

    const char* def = strstr(data, "//DEF\n");
    if (!def) {
        size_t len = utf8_length(data);
        if (len + 1 > *temp_size) {
            free_def(*temp);
            *temp_size = len + 1;
            *temp = force_malloc<char>(*temp_size);
        }

        memcpy(*temp, data, len);
        (*temp)[len] = 0;
        return true;
    }

    const int32_t s = min_def(0x100, num_uniform);

    size_t t_len[0x100];
    char t[0x100] = {};

    for (int32_t i = 0; i < s; i++) {
        sprintf_s(t, sizeof(t), "#define _%d %d\n", i, uniform_value[i]);
        t_len[i] = utf8_length(t);
    }

    size_t len_a = def - data;
    def += 5;
    size_t len_b = utf8_length(def);
    size_t len = 0;
    for (int32_t i = 0; i < s; i++)
        len += t_len[i];
    if (!len)
        def++;
    len += len_a + len_b;

    if (len + 1 > *temp_size) {
        free_def(*temp);
        *temp_size = len + 1;
        *temp = force_malloc<char>(*temp_size);
    }

    size_t pos = 0;
    memcpy(*temp + pos, data, len_a);
    pos += len_a;
    for (int32_t i = 0; i < s; i++) {
        sprintf_s(t, sizeof(t), "#define _%d %d\n", i, uniform_value[i]);
        memcpy(*temp + pos, t, t_len[i]);
        pos += t_len[i];
    }
    memcpy(*temp + pos, def, len_b);
    pos += len_b;
    (*temp)[pos] = 0;
    return true;
}

char* shader::parse_include(char* data, farc* f) {
    if (!data || !f)
        return data;

    char* data_end = data + utf8_length(data);
    char* i0 = strstr(data, "#include \"");
    char* i1 = i0 ? strstr(i0, "\"\n") : 0;
    if (!i0 || !i1)
        return data;

    size_t count = 1;
    while (i1 && (i0 = strstr(i1, "#include \""))) {
        i0 += 10;
        i1 = strstr(i0, "\"\n");
        if (i1)
            i1 += 1;
        count++;
    }

    char** temp = force_malloc<char*>(count);
    size_t* temp_len = force_malloc<size_t>(count);
    char** temp_ptr0 = force_malloc<char*>(count);
    char** temp_ptr1 = force_malloc<char*>(count);
    if (!temp || !temp_len || !temp_ptr0 || !temp_ptr1) {
        free_def(temp);
        free_def(temp_len);
        free_def(temp_ptr0);
        free_def(temp_ptr1);
        return data;
    }

    i1 = data;
    for (size_t i = 0; i < count; i++) {
        temp[i] = 0;
        i0 = i1 ? strstr(i1, "#include \"") : 0;
        i1 = i0 ? strstr(i0, "\"\n") : 0;
        if (!i0 || !i1)
            continue;

        temp_ptr0[i] = i0;
        temp_ptr1[i] = i1 + 1;
        i0 += 10;
        size_t s = i1 - i0;
        i1 += 2;
        char* t = force_malloc<char>(s + 1);
        if (!t)
            continue;

        memcpy(t, i0, s);
        t[s] = 0;

        farc_file* ff = f->read_file(t);
        free_def(t);
        if (!ff)
            continue;

        t = force_malloc<char>(ff->size + 1);
        if (t) {
            memcpy(t, ff->data, ff->size);
            t[ff->size] = 0;
        }
        temp[i] = t;
        temp_len[i] = ff->size;
    }

    size_t len = data_end - data;
    i1 = data;
    for (size_t i = 0; i < count; i++) {
        i0 = temp_ptr0[i];
        i1 = temp_ptr1[i];
        if (!i0 || !i1)
            continue;

        len -= i1 - i0;
        len += temp_len[i];
    }

    char* temp_data = force_malloc<char>(len + 1);
    size_t pos = 0;
    memcpy(temp_data + pos, data, temp_ptr0[0] - data);
    pos += temp_ptr0[0] - data;
    for (int32_t i = 0; i < count; i++) {
        if (temp[i]) {
            size_t s = temp_len[i];
            memcpy(temp_data + pos, temp[i], s);
            pos += s;
        }

        if (i < count - 1 && temp_ptr1[i]) {
            size_t s = temp_ptr0[i + 1] - temp_ptr1[i];
            memcpy(temp_data + pos, temp_ptr1[i], s);
            pos += s;
        }
        else if (temp_ptr1[i]) {
            size_t s = data_end - temp_ptr1[i];
            memcpy(temp_data + pos, temp_ptr1[i], s);
            pos += s;
        }
    }
    temp_data[pos] = 0;

    free_def(data);
    for (size_t i = 0; i < count; i++)
        free_def(temp[i]);
    free_def(temp);
    free_def(temp_len);
    free_def(temp_ptr0);
    free_def(temp_ptr1);
    return temp_data;
}

void shader::unbind() {
    gl_state_use_program(0);
}

shader_set_data::shader_set_data() : size(), shaders(), curr_shader(),
primitive_restart(), primitive_restart_index(), get_index_by_name_func() {

}

void shader_set_data::disable_primitive_restart() {
    primitive_restart = false;
}

void shader_set_data::draw_arrays(GLenum mode, GLint first, GLsizei count) {
    shader_update_data(this);
    glDrawArraysDLL(mode, first, count);
}

void shader_set_data::draw_elements(GLenum mode,
    GLsizei count, GLenum type, const void* indices) {
    switch (mode) {
    case GL_TRIANGLE_STRIP:
        uint32_t index;
        switch (type) {
        case GL_UNSIGNED_BYTE:
            index = 0xFF;
            break;
        case GL_UNSIGNED_SHORT:
            index = 0xFFFF;
            break;
        case GL_UNSIGNED_INT:
        default:
            index = 0xFFFFFFFF;
            break;
        }

        enable_primitive_restart();

        if (primitive_restart_index != index)
            primitive_restart_index = index;
        break;
    }

    shader_update_data(this);
    glDrawElementsDLL(mode, count, type, indices);

    switch (mode) {
    case GL_TRIANGLE_STRIP:
        disable_primitive_restart();
        break;
    }
}

void shader_set_data::draw_range_elements(GLenum mode,
    GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices) {
    switch (mode) {
    case GL_TRIANGLE_STRIP:
        uint32_t index;
        switch (type) {
        case GL_UNSIGNED_BYTE:
            index = 0xFF;
            break;
        case GL_UNSIGNED_SHORT:
            index = 0xFFFF;
            break;
        case GL_UNSIGNED_INT:
        default:
            index = 0xFFFFFFFF;
            break;
        }

        enable_primitive_restart();

        if (primitive_restart_index != index)
            primitive_restart_index = index;
        break;
    }

    shader_update_data(this);
    glDrawRangeElements(mode, start, end, count, type, indices);

    switch (mode) {
    case GL_TRIANGLE_STRIP:
        disable_primitive_restart();
        break;
    }
}

void shader_set_data::enable_primitive_restart() {
    primitive_restart = true;
}

int32_t shader_set_data::get_index_by_name(const char* name) {
    if (get_index_by_name_func) {
        int32_t index = get_index_by_name_func(name);
        if (index != -1)
            return index;
    }

    for (size_t i = 0; i < size; i++)
        if (!str_utils_compare(shaders[i].name, name))
            return (int32_t)shaders[i].name_index;
    return -1;
}

void shader_set_data::load(farc* f, bool ignore_cache,
    const char* name, const shader_table* shaders_table, const size_t size,
    const shader_bind_func* bind_func_table, const size_t bind_func_table_size,
    PFNSHADERGLSLGETINDEXFUNCPROC get_index_by_name) {
    if (!this || !f || !shaders_table || !size)
        return;

    wchar_t temp_buf[MAX_PATH];
    if (FAILED(SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, 0, temp_buf)))
        return;

    wcscat_s(temp_buf, sizeof(temp_buf) / sizeof(wchar_t), L"\\PDAFT");
    CreateDirectoryW(temp_buf, 0);

    wchar_t buf[MAX_PATH];
    swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"\\%hs_shader_cache", name);
    wcscat_s(temp_buf, sizeof(temp_buf) / sizeof(wchar_t), buf);

    bool shader_cache_changed = false;
    farc shader_cache_farc;
    swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"%ls.farc", temp_buf);
    if (!ignore_cache && path_check_file_exists(buf))
        shader_cache_farc.read(buf, true, false);

    char vert_buf[MAX_PATH];
    char frag_buf[MAX_PATH];
    char vert_file_buf[MAX_PATH];
    char frag_file_buf[MAX_PATH];

    GLsizei buffer_size = 0x100000;
    void* binary = force_malloc(buffer_size);
    size_t temp_vert_size = 0x10000;
    char* temp_vert = force_malloc<char>(temp_vert_size);
    size_t temp_frag_size = 0x10000;
    char* temp_frag = force_malloc<char>(temp_frag_size);
    std::vector<int32_t> vec_vert;
    std::vector<int32_t> vec_frag;
    std::vector<program_binary> program_data_binary;
    shader* shaders = force_malloc<shader>(size);
    this->shaders = shaders;
    this->size = size;
    for (size_t i = 0; i < size; i++) {
        shader* shader = &shaders[i];
        shader->name = shaders_table[i].name;
        shader->name_index = shaders_table[i].name_index;
        shader->num_sub = shaders_table[i].num_sub;
        shader->sub = force_malloc<shader_sub>(shader->num_sub);
        shader->num_uniform = shaders_table[i].num_uniform;
        shader->use_uniform = shaders_table[i].use_uniform;
        shader->use_permut = shaders_table[i].use_permut;

        int32_t num_sub = shader->num_sub;
        const shader_sub_table* sub_table = shaders_table[i].sub;
        shader_sub* sub = shader->sub;
        vec_vert.resize(shader->num_uniform);
        vec_frag.resize(shader->num_uniform);
        int32_t* vec_vert_data = vec_vert.data();
        int32_t* vec_frag_data = vec_frag.data();
        for (size_t j = 0; j < num_sub; j++, sub++, sub_table++) {
            sub->sub_index = sub_table->sub_index;
            sub->vp_unival_max = sub_table->vp_unival_max;
            sub->fp_unival_max = sub_table->fp_unival_max;

            strcpy_s(vert_file_buf, sizeof(vert_file_buf), sub_table->vp);
            strcat_s(vert_file_buf, sizeof(vert_file_buf), ".vert");
            farc_file* vert_ff = f->read_file(vert_file_buf);

            char* vert_data = 0;
            if (vert_ff && vert_ff->data) {
                vert_data = force_malloc<char>(vert_ff->size + 1);
                if (vert_data) {
                    memcpy(vert_data, vert_ff->data, vert_ff->size);
                    vert_data[vert_ff->size] = 0;
                }
            }

            strcpy_s(frag_file_buf, sizeof(frag_file_buf), sub_table->fp);
            strcat_s(frag_file_buf, sizeof(frag_file_buf), ".frag");
            farc_file* frag_ff = f->read_file(frag_file_buf);

            char* frag_data = 0;
            if (frag_ff && frag_ff->data) {
                frag_data = force_malloc<char>(frag_ff->size + 1);
                if (frag_data) {
                    memcpy(frag_data, frag_ff->data, frag_ff->size);
                    frag_data[frag_ff->size] = 0;
                }
            }

            if (!vert_data || !frag_data) {
                free_def(vert_data);
                free_def(frag_data);
                continue;
            }

            uint64_t vert_file_name_cache = hash_utf8_fnv1a64m(vert_file_buf);
            uint64_t frag_file_name_cache = hash_utf8_fnv1a64m(frag_file_buf);
            for (int32_t i = 0; i < 64; i += 8)
                if (((vert_file_name_cache >> i) & 0xFF) == 0)
                    vert_file_name_cache |= 0xFFULL << i;

            for (int32_t i = 0; i < 64; i += 8)
                if (((frag_file_name_cache >> i) & 0xFF) == 0)
                    frag_file_name_cache |= 0xFFULL << i;

            char shader_cache_file_name[MAX_PATH];
            strcpy_s(shader_cache_file_name, sizeof(shader_cache_file_name), sub_table->vp);
            if (str_utils_compare(sub_table->vp, sub_table->fp)) {
                strcat_s(shader_cache_file_name, sizeof(shader_cache_file_name), ".");
                strcat_s(shader_cache_file_name, sizeof(shader_cache_file_name), sub_table->fp);
            }
            strcat_s(shader_cache_file_name, sizeof(shader_cache_file_name), ".bin");

            vert_data = shader::parse_include(vert_data, f);
            frag_data = shader::parse_include(frag_data, f);
            uint64_t vert_data_hash = hash_utf8_fnv1a64m(vert_data);
            uint64_t frag_data_hash = hash_utf8_fnv1a64m(frag_data);

            farc_file* shader_cache_file = shader_cache_farc.read_file(shader_cache_file_name);
            program_binary* bin = 0;
            if (!ignore_cache) {
                if (!shader_cache_file || !shader_cache_file->data)
                    printf_debug("Shader not compiled: %s %s\n", vert_file_buf, frag_file_buf);
                else if (vert_data_hash != ((uint64_t*)shader_cache_file->data)[0]
                    || frag_data_hash != ((uint64_t*)shader_cache_file->data)[1])
                    printf_debug("Shader hash not equal: %s %s\n", vert_file_buf, frag_file_buf);
                else
                    bin = (program_binary*)&((uint64_t*)shader_cache_file->data)[2];
            }

            if (shader->num_uniform > 0
                && (sub_table->vp_unival_max[0] != -1 || sub_table->fp_unival_max[0] != -1)) {
                int32_t num_uniform = shader->num_uniform;
                int32_t unival_shad_curr = 1;
                int32_t unival_shad_count = 1;
                const int32_t* vp_unival_max = sub_table->vp_unival_max;
                const int32_t* fp_unival_max = sub_table->fp_unival_max;
                for (size_t k = 0; k < num_uniform; k++) {
                    const int32_t unival_max = shader->use_permut[k]
                        ? max_def(vp_unival_max[k], fp_unival_max[k]) : 0;
                    unival_shad_count += unival_shad_curr * unival_max;
                    unival_shad_curr *= unival_max + 1;
                }

                if (!ignore_cache)
                    program_data_binary.reserve(unival_shad_count);
                shader_sub_shader* shaders = force_malloc<shader_sub_shader>(unival_shad_count);
                sub->shaders = shaders;
                if (shaders) {
                    strcpy_s(vert_buf, sizeof(vert_buf), sub_table->vp);
                    size_t vert_buf_pos = utf8_length(vert_buf);
                    vert_buf[vert_buf_pos++] = '.';
                    vert_buf[vert_buf_pos] = 0;
                    memset(&vert_buf[vert_buf_pos], '0', num_uniform);
                    vert_buf[vert_buf_pos + num_uniform] = 0;
                    strcat_s(vert_buf, sizeof(vert_buf), ".vert");

                    strcpy_s(frag_buf, sizeof(frag_buf), sub_table->fp);
                    size_t frag_buf_pos = utf8_length(frag_buf);
                    frag_buf[frag_buf_pos++] = '.';
                    frag_buf[frag_buf_pos] = 0;
                    memset(&frag_buf[frag_buf_pos], '0', num_uniform);
                    frag_buf[frag_buf_pos + num_uniform] = 0;
                    strcat_s(frag_buf, sizeof(frag_buf), ".frag");

                    for (size_t k = 0; k < unival_shad_count; k++) {
                        for (size_t l = 0, m = k; l < num_uniform; l++) {
                            size_t unival_max = (size_t)(shader->use_permut[l]
                                ? max_def(vp_unival_max[l], fp_unival_max[l]) : 0) + 1;
                            vec_vert_data[l] = (uint32_t)(min_def(m % unival_max, vp_unival_max[l]));
                            m /= unival_max;
                            vert_buf[vert_buf_pos + l] = (char)('0' + vec_vert_data[l]);
                        }

                        for (size_t l = 0, m = k; l < num_uniform; l++) {
                            size_t unival_max = (size_t)(shader->use_permut[l]
                                ? max_def(vp_unival_max[l], fp_unival_max[l]) : 0) + 1;
                            vec_frag_data[l] = (uint32_t)(min_def(m % unival_max, fp_unival_max[l]));
                            m /= unival_max;
                            frag_buf[frag_buf_pos + l] = (char)('0' + vec_frag_data[l]);
                        }

                        if (!bin || !bin->binary_format || !bin->length || !shader_load_binary_shader(bin, &shaders[k].program)) {
                            bool vert_succ = shader::parse_define(vert_data, num_uniform,
                                vec_vert_data, &temp_vert, &temp_vert_size);
                            bool frag_succ = shader::parse_define(frag_data, num_uniform,
                                vec_frag_data, &temp_frag, &temp_frag_size);

                            if (ignore_cache)
                                shaders[k].program = shader_compile(vert_succ ? temp_vert : 0,
                                    frag_succ ? temp_frag : 0, vert_buf, frag_buf);
                            else {
                                program_data_binary.push_back({});
                                shaders[k].program = shader_compile_binary(vert_succ ? temp_vert : 0,
                                    frag_succ ? temp_frag : 0, vert_buf, frag_buf,
                                    &program_data_binary.back(), &buffer_size, &binary);
                            }
                            shader_cache_changed |= shaders[k].program ? true : false;
                        }
                        else {
                            program_data_binary.push_back({});
                            program_binary* b = &program_data_binary.back();
                            b->length = bin->length;
                            b->binary_format = bin->binary_format;
                            b->binary = (size_t)force_malloc(bin->length);
                            memcpy((void*)b->binary, (void*)((size_t)bin + bin->binary), bin->length);
                        }

                        if (!ignore_cache && bin)
                            bin++;
                    }
                }
            }
            else {
                program_data_binary.reserve(1);
                shader_sub_shader* shaders = force_malloc<shader_sub_shader>();
                sub->shaders = shaders;
                if (shaders) {
                    strcpy_s(vert_buf, sizeof(vert_buf), sub_table->vp);
                    strcpy_s(frag_buf, sizeof(vert_buf), sub_table->fp);
                    strcat_s(vert_buf, sizeof(vert_buf), "..vert");
                    strcat_s(frag_buf, sizeof(vert_buf), "..frag");

                    if (!bin || !bin->binary_format || !bin->length || !shader_load_binary_shader(bin, &shaders[0].program)) {
                        bool vert_succ = shader::parse_define(vert_data, &temp_vert, &temp_vert_size);
                        bool frag_succ = shader::parse_define(frag_data, &temp_frag, &temp_frag_size);

                        if (ignore_cache)
                            shaders[0].program = shader_compile(vert_succ ? temp_vert : 0,
                                frag_succ ? temp_frag : 0, vert_buf, frag_buf);
                        else {
                            program_data_binary.push_back({});
                            shaders[0].program = shader_compile_binary(vert_succ ? temp_vert : 0,
                                frag_succ ? temp_frag : 0, vert_buf, frag_buf,
                                &program_data_binary.back(), &buffer_size, &binary);
                        }
                        shader_cache_changed |= shaders[0].program ? true : false;
                    }
                    else {
                        program_data_binary.push_back({});
                        program_binary* b = &program_data_binary.back();
                        b->length = bin->length;
                        b->binary_format = bin->binary_format;
                        b->binary = (size_t)force_malloc(bin->length);
                        memcpy((void*)b->binary, (void*)((size_t)bin + bin->binary), bin->length);
                    }

                    if (!ignore_cache && bin)
                        bin++;
                }
            }

            if (!ignore_cache) {
                if (!shader_cache_file)
                    shader_cache_file = shader_cache_farc.add_file(shader_cache_file_name);
                else
                    free_def(shader_cache_file->data);

                size_t bin_count = program_data_binary.size();
                size_t bin_size = sizeof(uint64_t) * 2 + bin_count * sizeof(program_binary);
                for (program_binary& k : program_data_binary)
                    bin_size += align_val(k.length, 0x04);
                shader_cache_file->data = force_malloc(bin_size);
                shader_cache_file->size = bin_size;
                shader_cache_file->compressed = true;
                shader_cache_file->data_changed = true;

                ((uint64_t*)shader_cache_file->data)[0] = vert_data_hash;
                ((uint64_t*)shader_cache_file->data)[1] = frag_data_hash;
                bin = (program_binary*)&((uint64_t*)shader_cache_file->data)[2];
                size_t bin_data_base = (size_t)shader_cache_file->data + sizeof(uint64_t) * 2;
                size_t bin_data = bin_data_base + bin_count * sizeof(program_binary);
                for (program_binary& k : program_data_binary) {
                    bin->length = k.length;
                    bin->binary_format = k.binary_format;
                    bin->binary = bin_data - bin_data_base;
                    memcpy((void*)bin_data, (void*)k.binary, k.length);
                    bin_data_base += sizeof(program_binary);
                    bin_data += align_val(k.length, 0x04);
                    void* binary = (void*)k.binary;
                    free_def(binary);
                    k.binary = 0;
                    bin++;
                }
            }

            free_def(vert_data);
            free_def(frag_data);
            program_data_binary.clear();
        }
        vec_vert.clear();
        vec_frag.clear();

        for (size_t j = 0; j < bind_func_table_size; j++)
            if (shader->name_index == bind_func_table[j].name_index) {
                shader->bind_func = bind_func_table[j].bind_func;
                break;
            }
    }
    free_def(binary);
    free_def(temp_vert);
    free_def(temp_frag);

    if (!ignore_cache && shader_cache_changed)
        shader_cache_farc.write(temp_buf, FARC_FArC, FARC_NONE, false);

    this->get_index_by_name_func = get_index_by_name;
}

void shader_set_data::set(uint32_t index) {
    if (this && index && index != -1) {
        shader* shader = &shaders[index];
        if (shader->bind_func)
            shader->bind_func(this, shader);
        else
            shader->bind(this, shader->sub[0].sub_index);
    }
    else
        shader::unbind();
}

void shader_set_data::unload() {
    size_t size = this->size;
    shader* shaders = this->shaders;
    for (size_t i = 0; i < size; i++) {
        shader* shader = &shaders[i];
        if (!shader->sub)
            continue;

        int32_t num_sub = shader->num_sub;
        shader_sub* sub = shader->sub;
        for (size_t j = 0; j < num_sub; j++, sub++) {
            int32_t unival_shad_count = 1;
            if (shader->num_uniform > 0) {
                int32_t num_uniform = shader->num_uniform;
                int32_t unival_shad_curr = 1;
                const int32_t* vp_unival_max = sub->vp_unival_max;
                const int32_t* fp_unival_max = sub->fp_unival_max;
                for (size_t k = 0; k < num_uniform; k++) {
                    const int32_t unival_max = shader->use_permut[k]
                        ? max_def(vp_unival_max[k], fp_unival_max[k]) : 0;
                    unival_shad_count += unival_shad_curr * unival_max;
                    unival_shad_curr *= unival_max + 1;
                }
            }

            if (sub->shaders) {
                shader_sub_shader* shaders = sub->shaders;
                for (size_t k = 0; k < unival_shad_count; k++)
                    glDeleteProgram(shaders[k].program);
                free(shaders);
                sub->shaders = 0;
            }
        }
        free(shader->sub);
        shader->sub = 0;
    }
    free_def(shaders);
    this->shaders = 0;

    get_index_by_name_func = 0;
}

static GLuint shader_compile_shader(GLenum type, const char* data, const char* file) {
    if (!data)
        return 0;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &data, 0);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar* info_log = force_malloc<GLchar>(0x10000);
        glGetShaderInfoLog(shader, 0x10000, 0, info_log);
        const char* type_str = "Unknown";
        switch (type) {
        case GL_FRAGMENT_SHADER:
            type_str = "Fragment";
            break;
        case GL_VERTEX_SHADER:
            type_str = "Vertex";
            break;
        }
        printf_debug("%s shader compile error:\nfile: %s\n%s\n", type_str, file, info_log);
        free_def(info_log);
        glDeleteShader(shader);

#if defined(CRE_DEV)
        wchar_t temp_buf[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, 0, temp_buf))) {
            wcscat_s(temp_buf, sizeof(temp_buf) / sizeof(wchar_t), L"\\ReDIVA");
            temp_buf[sizeof(temp_buf) / sizeof(wchar_t) - 1] = 0;
            CreateDirectoryW(temp_buf, 0);

            wchar_t buf[MAX_PATH];
            swprintf_s(buf, sizeof(buf) / sizeof(wchar_t),
                L"%ls\\shader_error", temp_buf);
            buf[sizeof(buf) / sizeof(wchar_t) - 1] = 0;
            CreateDirectoryW(buf, 0);

            swprintf_s(buf, sizeof(buf) / sizeof(wchar_t),
                L"%ls\\shader_error\\%hs", temp_buf, file);
            buf[sizeof(buf) / sizeof(wchar_t) - 1] = 0;

            file_stream s;
            s.open(buf, L"wb");
            s.write(data, utf8_length(data));
        }
#endif
        return 0;
    }
    return shader;
}

static GLuint shader_compile(const char* vert, const char* frag, const char* vp, const char* fp) {
    GLuint vert_shad = shader_compile_shader(GL_VERTEX_SHADER, vert, vp);
    GLuint frag_shad = shader_compile_shader(GL_FRAGMENT_SHADER, frag, fp);

    GLuint program = glCreateProgram();
    if (vert_shad)
        glAttachShader(program, vert_shad);
    if (frag_shad)
        glAttachShader(program, frag_shad);
    glLinkProgram(program);

    if (vert_shad)
        glDeleteShader(vert_shad);
    if (frag_shad)
        glDeleteShader(frag_shad);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar* info_log = force_malloc<GLchar>(0x10000);
        glGetProgramInfoLog(program, 0x10000, 0, info_log);
        printf_debug("Program Shader Permut linking error:\nvp: %s; fp: %s\n%s\n", vp, fp, info_log);
        free_def(info_log);
        glDeleteProgram(program);

#if defined(CRE_DEV)
        wchar_t temp_buf[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, 0, temp_buf))) {
            wcscat_s(temp_buf, sizeof(temp_buf) / sizeof(wchar_t), L"\\ReDIVA");
            temp_buf[sizeof(temp_buf) / sizeof(wchar_t) - 1] = 0;
            CreateDirectoryW(temp_buf, 0);

            wchar_t buf[MAX_PATH];
            swprintf_s(buf, sizeof(buf) / sizeof(wchar_t),
                L"%ls\\shader_error", temp_buf);
            buf[sizeof(buf) / sizeof(wchar_t) - 1] = 0;
            CreateDirectoryW(buf, 0);

            swprintf_s(buf, sizeof(buf) / sizeof(wchar_t),
                L"%ls\\shader_error\\%hs", temp_buf, vp);
            buf[sizeof(buf) / sizeof(wchar_t) - 1] = 0;

            file_stream s;
            s.open(buf, L"wb");
            s.write(vert, utf8_length(vert));
            s.close();

            swprintf_s(buf, sizeof(buf) / sizeof(wchar_t),
                L"%ls\\shader_error\\%hs", temp_buf, fp);
            buf[sizeof(buf) / sizeof(wchar_t) - 1] = 0;

            s.open(buf, L"wb");
            s.write(frag, utf8_length(frag));
            s.close();
        }
#endif
        return 0;
    }
    else {
        gl_state_get_all_gl_errors();
        return program;
    }
}

static GLuint shader_compile_binary(const char* vert, const char* frag, const char* vp, const char* fp,
    program_binary* bin, GLsizei* buffer_size, void** binary) {
    memset(bin, 0, sizeof(*bin));

    GLuint vert_shad = shader_compile_shader(GL_VERTEX_SHADER, vert, vp);
    GLuint frag_shad = shader_compile_shader(GL_FRAGMENT_SHADER, frag, fp);

    GLuint program = glCreateProgram();
    if (vert_shad)
        glAttachShader(program, vert_shad);
    if (frag_shad)
        glAttachShader(program, frag_shad);
    glLinkProgram(program);

    if (vert_shad)
        glDeleteShader(vert_shad);
    if (frag_shad)
        glDeleteShader(frag_shad);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar* info_log = force_malloc<GLchar>(0x10000);
        glGetProgramInfoLog(program, 0x10000, 0, info_log);
        printf_debug("Program Shader Permut linking error:\nvp: %s; fp: %s\n%s\n", vp, fp, info_log);
        free_def(info_log);
        glDeleteProgram(program);

#if defined(CRE_DEV)
        wchar_t temp_buf[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, 0, temp_buf))) {
            wcscat_s(temp_buf, sizeof(temp_buf) / sizeof(wchar_t), L"\\ReDIVA");
            temp_buf[sizeof(temp_buf) / sizeof(wchar_t) - 1] = 0;
            CreateDirectoryW(temp_buf, 0);

            wchar_t buf[MAX_PATH];
            swprintf_s(buf, sizeof(buf) / sizeof(wchar_t),
                L"%ls\\shader_error", temp_buf);
            buf[sizeof(buf) / sizeof(wchar_t) - 1] = 0;
            CreateDirectoryW(buf, 0);

            swprintf_s(buf, sizeof(buf) / sizeof(wchar_t),
                L"%ls\\shader_error\\%hs", temp_buf, vp);
            buf[sizeof(buf) / sizeof(wchar_t) - 1] = 0;

            file_stream s;
            s.open(buf, L"wb");
            s.write(vert, utf8_length(vert));
            s.close();

            swprintf_s(buf, sizeof(buf) / sizeof(wchar_t),
                L"%ls\\shader_error\\%hs", temp_buf, fp);
            buf[sizeof(buf) / sizeof(wchar_t) - 1] = 0;

            s.open(buf, L"wb");
            s.write(frag, utf8_length(frag));
            s.close();
        }
#endif
        return 0;
    }
    else {
        gl_state_get_all_gl_errors();

        GLenum binary_format = 0x0;
        GLsizei length = 0;
        while (*buffer_size < 0x7FFFFFF) {
            glGetProgramBinary(program, *buffer_size, &length, &binary_format, *binary);
            if (!gl_state_get_error())
                break;

            free_def(*binary);
            *buffer_size <<= 1;
            *binary = force_malloc(*buffer_size);
        }

        bin->length = length;
        bin->binary_format = binary_format;
        bin->binary = (size_t)force_malloc(length);
        if (*binary)
            memcpy((void*)bin->binary, *binary, length);
        return program;
    }
}

static bool shader_load_binary_shader(program_binary* bin, GLuint* program) {
    *program = glCreateProgram();
    glProgramBinary(*program, bin->binary_format, (void*)((size_t)bin + bin->binary), bin->length);
    GLint success = 0;
    glGetProgramiv(*program, GL_LINK_STATUS, &success);
    if (!success) {
        glDeleteProgram(*program);
        *program = 0;
        return false;
    }
    return true;
}

static void shader_update_data(shader_set_data* set) {
    if (!set)
        return;

    if (set->curr_shader) {
        shader_sub_shader* shader = set->curr_shader;
        if (shader->uniform_val_update) {
            glUniform1iv(0, SHADER_MAX_UNIFORM_VALUES, (GLint*)shader->uniform_val);
            shader->uniform_val_update = false;
        }
    }

    if (set->primitive_restart) {
        gl_state_enable_primitive_restart();
        gl_state_set_primitive_restart_index(set->primitive_restart_index);
    }
    else
        gl_state_disable_primitive_restart();
}
