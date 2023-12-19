/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "header.hpp"

void f2_header_read(stream& s, f2_header* h) {
    memset(h, 0, sizeof(f2_header));

    if (s.check_null())
        return;

    s.read(h, 0x20);
    if (h->length == 0x40)
        s.read((uint8_t*)h + 0x20, 0x20);
}

void f2_header_write(stream& s, f2_header* h, bool extended) {
    if (s.check_null())
        return;

    h->length = extended ? 0x40 : 0x20;
    s.write(h, h->length);
}

void f2_header_write_end_of_container(stream& s, uint32_t depth) {
    if (s.check_null())
        return;

    f2_header h = {};
    h.signature = reverse_endianness_uint32_t('EOFC');
    h.length = 0x20;
    h.depth = depth;
    h.use_section_size = true;
    f2_header_write(s, &h, false);
}
