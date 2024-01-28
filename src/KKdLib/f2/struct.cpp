/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "struct.hpp"
#include "../io/file_stream.hpp"
#include "../io/memory_stream.hpp"
#include "../io/path.hpp"
#include "../divafile.hpp"

static void f2_struct_read_data(stream& s, f2_struct* st, f2_header* h);
static void f2_struct_write_inner(stream& s, f2_struct* st, uint32_t depth, bool use_depth, bool shift_x);
static void f2_struct_get_length(f2_struct* s, bool shift_x);
static void f2_struct_write_pof(stream& s, pof* pof, uint32_t depth, bool shift_x);
static void f2_struct_write_enrs(stream& s, enrs* enrs, uint32_t depth);

f2_struct::f2_struct() : header() {

}

f2_struct::~f2_struct() {

}

void f2_struct::read(const char* path) {
    if (!path)
        return;

    file_stream s;
    s.open(path, "rb");
    if (s.check_not_null()) {
        memory_stream ms;
        divafile::decrypt(s, ms);

        f2_header h;
        f2_header_read(ms, &h);
        f2_struct_read_data(ms, this, &h);
    }
}

void f2_struct::read(const wchar_t* path) {
    if (!path)
        return;

    file_stream s;
    s.open(path, L"rb");
    if (s.check_not_null()) {
        memory_stream ms;
        divafile::decrypt(s, ms);

        f2_header h;
        f2_header_read(ms, &h);
        f2_struct_read_data(ms, this, &h);
    }
}

void f2_struct::read(const void* data, size_t size) {
    if (!data || !size)
        return;

    memory_stream s;
    s.open(data, size);
    if (s.check_not_null()) {
        memory_stream ms;
        divafile::decrypt(s, ms);

        f2_header h;
        f2_header_read(ms, &h);
        f2_struct_read_data(ms, this, &h);
    }
}

void f2_struct::read(stream& s) {
    if (s.check_null())
        return;

    memory_stream ms;
    divafile::decrypt(s, ms);

    f2_header h;
    f2_header_read(ms, &h);
    f2_struct_read_data(ms, this, &h);
}

void f2_struct::write(const char* path, bool use_depth, bool shift_x) {
    if (!path)
        return;

    file_stream s;
    s.open(path, "wb");
    if (s.check_not_null()) {
        f2_struct_get_length(this, shift_x);
        f2_struct_write_inner(s, this, 0, use_depth, shift_x);
    }
}

void f2_struct::write(const wchar_t* path, bool use_depth, bool shift_x) {
    if (!path)
        return;

    file_stream s;
    s.open(path, L"wb");
    if (s.check_not_null()) {
        f2_struct_get_length(this, shift_x);
        f2_struct_write_inner(s, this, 0, use_depth, shift_x);
    }
}

void f2_struct::write(void** data, size_t* size, bool use_depth, bool shift_x) {
    if (!data || !size)
        return;

    f2_struct_get_length(this, shift_x);
    memory_stream s;
    s.open(0, header.data_size + 0x40ULL);
    f2_struct_write_inner(s, this, 0, use_depth, shift_x);

    s.copy(data, size);
}

void f2_struct::write(stream& s, bool use_depth, bool shift_x) {
    if (s.check_null())
        return;

    f2_struct_get_length(this, shift_x);
    f2_struct_write_inner(s, this, 0, use_depth, shift_x);

}

static void f2_struct_get_length(f2_struct* s, bool shift_x) {
    bool has_pof = s->pof.vec.size() > 0 ? true : false;
    bool has_enrs = s->enrs.vec.size() > 0 ? true : false;
    bool has_sub_structs = s->sub_structs.size() > 0 ? true : false;

    s->header.section_size = (uint32_t)s->data.size();

    uint32_t l = s->header.section_size;
    if (has_enrs) {
        uint32_t len = s->enrs.length();
        l += 0x20 + align_val(len, 0x10);
    }

    if (has_pof) {
        s->pof.shift_x = shift_x;
        uint32_t len = s->pof.length();
        l += 0x20 + align_val(len, 0x10);
    }

    if (has_sub_structs) {
        for (f2_struct& i : s->sub_structs) {
            f2_struct_get_length(&i, shift_x);
            l += i.header.data_size;
            l += i.header.length;
        }
        l += 0x20;
    }

    s->header.data_size = l;
}

static void f2_struct_read_data(stream& s, f2_struct* st, f2_header* h) {
    uint32_t l = h->use_section_size ? h->section_size : h->data_size;
    uint32_t depth = h->depth;
    st->header = *h;
    if (l) {
        st->data.resize(l);
        s.read(st->data.data(), l);
    }

    uint32_t sig;
    size_t length = (size_t)h->data_size - l;
    size_t position = 0;
    while (length > position) {
        f2_header_read(s, h);
        sig = reverse_endianness_uint32_t(h->signature);
        l = h->use_section_size ? h->section_size : h->data_size;
        position += (size_t)h->length + l;
        if (sig == 'EOFC')
            break;
        else if (sig == 'ENRS') {
            size_t pos = s.get_position();
            st->enrs.read(s);
            s.set_position(pos + l, SEEK_SET);
        }
        else if ((sig & 0xFFFFFFF0) == 'POF0') {
            size_t pos = s.get_position();
            st->pof.shift_x = sig == 'POF1';
            st->pof.read(s);
            s.set_position(pos + l, SEEK_SET);
        }
        else {
            st->sub_structs.push_back({});
            f2_struct_read_data(s, &st->sub_structs.back(), h);
        }
    }
}

static void f2_struct_write_inner(stream& s, f2_struct* st, uint32_t depth, bool use_depth, bool shift_x) {
    bool has_pof = st->pof.vec.size() > 0 ? true : false;
    bool has_enrs = st->enrs.vec.size() > 0 ? true : false;
    bool has_sub_structs = st->sub_structs.size() > 0 ? true : false;

    st->header.depth = use_depth ? depth : 0;
    f2_header_write(s, &st->header, st->header.length == 0x40);
    if (st->data.size())
        s.write(st->data.data(), st->data.size());
    if (has_enrs)
        f2_struct_write_enrs(s, &st->enrs, use_depth ? depth + 1 : 0);
    if (has_pof)
        f2_struct_write_pof(s, &st->pof, use_depth ? depth + 1 : 0, shift_x);
    if (has_sub_structs) {
        for (f2_struct& i : st->sub_structs)
            f2_struct_write_inner(s, &i, depth + 1, use_depth, shift_x);
        f2_header_write_end_of_container(s, use_depth ? depth + 1 : 0);
    }
    if (!depth)
        f2_header_write_end_of_container(s, 0);
}

static void f2_struct_write_pof(stream& s, pof* pof, uint32_t depth, bool shift_x) {
    pof->shift_x = shift_x;
    size_t len = pof->length();
    f2_header h = {};
    h.signature = shift_x ? reverse_endianness_uint32_t('POF1') : reverse_endianness_uint32_t('POF0');
    h.length = 0x20;
    h.depth = depth;
    h.use_section_size = true;
    h.data_size = h.section_size = (uint32_t)align_val(len, 0x10);
    f2_header_write(s, &h, false);
    pof->write(s);
}

static void f2_struct_write_enrs(stream& s, enrs* enrs, uint32_t depth) {
    size_t len = enrs->length();
    f2_header h = {};
    h.signature = reverse_endianness_uint32_t('ENRS');
    h.length = 0x20;
    h.depth = depth;
    h.use_section_size = true;
    h.data_size = h.section_size = (uint32_t)align_val(len, 0x10);
    f2_header_write(s, &h, false);
    enrs->write(s);
}
