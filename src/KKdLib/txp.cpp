/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "txp.hpp"
#include "f2/struct.hpp"
#include "io/memory_stream.hpp"

txp_mipmap::txp_mipmap() : width(), height(), format(), size() {

}

txp_mipmap::~txp_mipmap() {

}

uint32_t txp_mipmap::get_size() {
    return txp::get_size(format, width, height);
}

txp::txp() : has_cube_map(), array_size(), mipmaps_count() {

}

txp::~txp() {

}

uint32_t txp::get_size(txp_format format, uint32_t width, uint32_t height) {
    uint32_t size = width * height;
    switch (format) {
    case TXP_A8:
        return size;
    case TXP_RGB8:
        return size * 3;
    case TXP_RGBA8:
        return size * 4;
    case TXP_RGB5:
        return size * 2;
    case TXP_RGB5A1:
        return size * 2;
    case TXP_RGBA4:
        return size * 2;
    case TXP_L8:
        return size;
    case TXP_L8A8:
        return size * 2;
    case TXP_BC1:
    case TXP_BC1a:
    case TXP_BC2:
    case TXP_BC3:
    case TXP_BC4:
    case TXP_BC5:
        width = align_val(width, 4);
        height = align_val(height, 4);
        size = width * height;
        switch (format) {
        case TXP_BC1:
            return size / 2;
        case TXP_BC1a:
            return size / 2;
        case TXP_BC2:
            return size;
        case TXP_BC3:
            return size;
        case TXP_BC4:
            return size / 2;
        case TXP_BC5:
            return size;
        }
        break;
    }
    return 0;
}

txp_set::txp_set() {

}

txp_set::~txp_set() {

}

bool txp_set::pack_file(void** data, size_t* size, bool big_endian) {
    size_t l;
    txp* tex;
    txp_mipmap* tex_mipmap;

    if (!data || !size)
        return false;

    *data = 0;
    *size = 0;

    size_t count = textures.size();
    if (count < 1)
        return false;

    size_t* txp4_offset = force_malloc<size_t>(count);
    size_t** txp2_offset = force_malloc<size_t*>(count);
    tex = textures.data();
    for (size_t i = 0; i < count; i++, tex++)
        txp2_offset[i] = force_malloc<size_t>((size_t)tex->mipmaps_count * tex->array_size);

    l = 12 + count * 4;

    tex = textures.data();
    for (size_t i = 0; i < count; i++, tex++) {
        txp4_offset[i] = l;
        l += 12 + (size_t)tex->array_size * tex->mipmaps_count * 4;

        tex_mipmap = tex->mipmaps.data();
        for (size_t j = 0; j < tex->array_size; j++) {
            for (size_t k = 0; k < tex->mipmaps_count; k++, tex_mipmap++) {
                txp2_offset[i][j * tex->mipmaps_count + k] = l;
                l += 24;
                l += tex_mipmap->size;
            }
        }
    }

    memory_stream s;
    s.open(0, l);
    s.big_endian = big_endian;
    s.write_uint32_t_reverse_endianness(0x03505854);
    s.write_uint32_t_reverse_endianness((uint32_t)count);
    s.write_uint32_t_reverse_endianness((uint8_t)count | 0x01010100);
    for (size_t i = 0; i < count; i++)
        s.write_uint32_t_reverse_endianness((uint32_t)txp4_offset[i]);

    tex = textures.data();
    for (size_t i = 0; i < count; i++, tex++) {
        s.set_position(txp4_offset[i], SEEK_SET);
        s.write_uint32_t_reverse_endianness(tex->array_size > 1 ? 0x05505854 : 0x04505854);
        s.write_uint32_t_reverse_endianness(tex->mipmaps_count * tex->array_size);
        s.write_uint32_t_reverse_endianness((uint8_t)tex->mipmaps_count
            | ((uint8_t)tex->array_size << 8) | 0x01010000);
        for (size_t j = 0; j < tex->array_size; j++)
            for (size_t k = 0; k < tex->mipmaps_count; k++)
                s.write_uint32_t_reverse_endianness(
                    (uint32_t)(txp2_offset[i][j * tex->mipmaps_count + k] - txp4_offset[i]));

        tex_mipmap = tex->mipmaps.data();
        for (size_t j = 0; j < tex->array_size; j++)
            for (size_t k = 0; k < tex->mipmaps_count; k++, tex_mipmap++) {
                s.set_position(txp2_offset[i][j * tex->mipmaps_count + k], SEEK_SET);
                s.write_uint32_t_reverse_endianness(0x02505854);
                s.write_uint32_t_reverse_endianness(tex_mipmap->width);
                s.write_uint32_t_reverse_endianness(tex_mipmap->height);
                s.write_uint32_t_reverse_endianness(tex_mipmap->format);
                s.write_uint32_t_reverse_endianness((uint32_t)(j * tex->mipmaps_count + k));
                s.write_uint32_t_reverse_endianness(tex_mipmap->size);
                s.write(tex_mipmap->data.data(), tex_mipmap->size);
                s.align_write(0x04);
            }
    }
    s.set_position(0, SEEK_END);

    s.align_write(0x10);
    s.copy(data, size);

    for (size_t i = 0; i < count; i++)
        free_def(txp2_offset[i]);
    free_def(txp2_offset);
    free_def(txp4_offset);
    return true;
}

bool txp_set::pack_file(std::vector<uint8_t>& data, bool big_endian) {
    size_t l;
    txp* tex;
    txp_mipmap* tex_mipmap;

    data.clear();
    data.shrink_to_fit();

    size_t count = textures.size();
    if (count < 1)
        return false;

    size_t* txp4_offset = force_malloc<size_t>(count);
    size_t** txp2_offset = force_malloc<size_t*>(count);
    tex = textures.data();
    for (size_t i = 0; i < count; i++, tex++)
        txp2_offset[i] = force_malloc<size_t>((size_t)tex->mipmaps_count * tex->array_size);

    l = 12 + count * 4;

    tex = textures.data();
    for (size_t i = 0; i < count; i++, tex++) {
        txp4_offset[i] = l;
        l += 12 + (size_t)tex->array_size * tex->mipmaps_count * 4;

        tex_mipmap = tex->mipmaps.data();
        for (size_t j = 0; j < tex->array_size; j++) {
            for (size_t k = 0; k < tex->mipmaps_count; k++, tex_mipmap++) {
                txp2_offset[i][j * tex->mipmaps_count + k] = l;
                l += 24;
                l += tex_mipmap->size;
            }
        }
    }

    memory_stream s;
    s.open(0, l);
    s.big_endian = big_endian;
    s.write_uint32_t_reverse_endianness(0x03505854);
    s.write_uint32_t_reverse_endianness((uint32_t)count);
    s.write_uint32_t_reverse_endianness((uint8_t)count | 0x01010100);
    for (size_t i = 0; i < count; i++)
        s.write_uint32_t_reverse_endianness((uint32_t)txp4_offset[i]);

    tex = textures.data();
    for (size_t i = 0; i < count; i++, tex++) {
        s.set_position(txp4_offset[i], SEEK_SET);
        s.write_uint32_t_reverse_endianness(tex->array_size > 1 ? 0x05505854 : 0x04505854);
        s.write_uint32_t_reverse_endianness(tex->mipmaps_count * tex->array_size);
        s.write_uint32_t_reverse_endianness((uint8_t)tex->mipmaps_count
            | ((uint8_t)tex->array_size << 8) | 0x01010000);
        for (size_t j = 0; j < tex->array_size; j++)
            for (size_t k = 0; k < tex->mipmaps_count; k++)
                s.write_uint32_t_reverse_endianness(
                    (uint32_t)(txp2_offset[i][j * tex->mipmaps_count + k] - txp4_offset[i]));

        tex_mipmap = tex->mipmaps.data();
        for (size_t j = 0; j < tex->array_size; j++)
            for (size_t k = 0; k < tex->mipmaps_count; k++, tex_mipmap++) {
                s.set_position(txp2_offset[i][j * tex->mipmaps_count + k], SEEK_SET);
                s.write_uint32_t_reverse_endianness(0x02505854);
                s.write_uint32_t_reverse_endianness(tex_mipmap->width);
                s.write_uint32_t_reverse_endianness(tex_mipmap->height);
                s.write_uint32_t_reverse_endianness(tex_mipmap->format);
                s.write_uint32_t_reverse_endianness((uint32_t)(j * tex->mipmaps_count + k));
                s.write_uint32_t_reverse_endianness(tex_mipmap->size);
                s.write(tex_mipmap->data.data(), tex_mipmap->size);
                s.align_write(0x04);
            }
    }
    s.set_position(0, SEEK_END);

    s.align_write(0x10);
    s.copy(data);

    for (size_t i = 0; i < count; i++)
        free_def(txp2_offset[i]);
    free_def(txp2_offset);
    free_def(txp4_offset);
    return true;
}

bool txp_set::pack_file_modern(void** data, size_t* size, bool big_endian, uint32_t signature) {
    f2_struct st;
    if (!pack_file(st.data, big_endian)) {
        *data = 0;
        *size = 0;
        return false;
    }

    produce_enrs(&st.enrs);

    st.header.signature = reverse_endianness_uint32_t(signature);
    st.header.length = 0x20;
    st.header.use_big_endian = big_endian;
    st.header.use_section_size = true;
    st.write(data, size, true, false);
    return true;
}

bool txp_set::produce_enrs(enrs* enrs) {
    size_t l;
    txp* tex;
    txp_mipmap* tex_mipmap;

    if (!enrs)
        return false;

    enrs->vec.clear();
    l = 0;

    size_t count = textures.size();
    if (count < 1)
        return false;

    uint32_t o;
    enrs_entry ee;

    ee = { 0, 1, 12, 1 };
    ee.append(0, 3, ENRS_DWORD);
    enrs->vec.push_back(ee);
    l += o = 12;

    ee = { o, 1, (uint32_t)(count * 4), 1 };
    ee.append(0, (uint32_t)count, ENRS_DWORD);
    enrs->vec.push_back(ee);
    l += (size_t)(o = (uint32_t)(count * 4ULL));

    tex = textures.data();
    for (size_t i = 0; i < count; i++, tex++) {
        ee = { o, 1, 12, 1 };
        ee.append(0, 3, ENRS_DWORD);
        enrs->vec.push_back(ee);
        l += o = 12;

        ee = { o, 1, tex->array_size * 4, tex->mipmaps_count };
        ee.append(0, tex->array_size, ENRS_DWORD);
        enrs->vec.push_back(ee);
        l += (size_t)(o = (uint32_t)((size_t)tex->array_size * tex->mipmaps_count * 4));

        tex_mipmap = tex->mipmaps.data();
        for (size_t j = 0; j < tex->array_size; j++) {
            for (size_t k = 0; k < tex->mipmaps_count; k++, tex_mipmap++) {
                ee = { o, 1, 24, 1 };
                ee.append(0, 6, ENRS_DWORD);
                enrs->vec.push_back(ee);
                l += (size_t)(o = (uint32_t)(24 + tex_mipmap->size));
            }
        }
    }
    return true;
}

bool txp_set::unpack_file(const void* data, bool big_endian) {
    uint32_t signature;
    uint32_t tex_count;
    txp* tex;
    txp_mipmap* tex_mipmap;
    size_t set_d;
    size_t d;
    size_t mipmap_d;
    uint32_t sub_tex_count;
    uint32_t info;

    if (!data)
        return false;

    if (big_endian)
        signature = load_reverse_endianness_uint32_t((void*)data);
    else
        signature = *(uint32_t*)data;

    if (signature != 0x03505854)
        return false;

    set_d = (size_t)data;
    if (big_endian)
        tex_count = load_reverse_endianness_uint32_t((void*)(set_d + 4));
    else
        tex_count = *(uint32_t*)(set_d + 4);

    textures.resize(tex_count);
    for (size_t i = 0; i < tex_count; i++) {
        if (big_endian) {
            d = set_d + (size_t)load_reverse_endianness_uint32_t((uint32_t*)(set_d + 12) + i);
            signature = load_reverse_endianness_uint32_t((void*)d);
        }
        else {
            d = set_d + (size_t)((uint32_t*)(set_d + 12))[i];
            signature = *(uint32_t*)d;
        }

        if (signature != 0x04505854 && signature != 0x05505854) {
            textures.pop_back();
            continue;
        }

        if (big_endian) {
            sub_tex_count = load_reverse_endianness_uint32_t((void*)(d + 4));
            info = load_reverse_endianness_uint32_t((void*)(d + 8));
        }
        else {
            sub_tex_count = *(uint32_t*)(d + 4);
            info = *(uint32_t*)(d + 8);
        }

        tex = &textures[i - (tex_count - textures.size())];
        tex->has_cube_map = signature == 0x05505854;
        tex->mipmaps_count = info & 0xFF;
        tex->array_size = (info >> 8) & 0xFF;

        if (tex->array_size == 1 && tex->mipmaps_count != sub_tex_count)
            tex->mipmaps_count = sub_tex_count & 0xFF;

        uint32_t mipmaps_count = tex->mipmaps_count;
        tex->mipmaps.resize((size_t)tex->array_size * tex->mipmaps_count);
        tex_mipmap = tex->mipmaps.data();
        for (size_t j = 0; j < tex->array_size; j++)
            for (size_t k = 0; k < tex->mipmaps_count; k++, tex_mipmap++) {
                if (big_endian) {
                    mipmap_d = d + (size_t)load_reverse_endianness_uint32_t((uint32_t*)(d + 12) + j * mipmaps_count + k);
                    signature = load_reverse_endianness_uint32_t((void*)mipmap_d);
                }
                else {
                    mipmap_d = d + (size_t)((uint32_t*)(d + 12))[j * mipmaps_count + k];
                    signature = *(uint32_t*)mipmap_d;
                }

                if (big_endian) {
                    tex_mipmap->width = load_reverse_endianness_uint32_t((void*)(mipmap_d + 4));
                    tex_mipmap->height = load_reverse_endianness_uint32_t((void*)(mipmap_d + 8));
                    tex_mipmap->format = (txp_format)load_reverse_endianness_uint32_t((void*)(mipmap_d + 12));
                    tex_mipmap->size = load_reverse_endianness_uint32_t((void*)(mipmap_d + 20));
                }
                else {
                    tex_mipmap->width = *(uint32_t*)(mipmap_d + 4);
                    tex_mipmap->height = *(uint32_t*)(mipmap_d + 8);
                    tex_mipmap->format = (txp_format)*(uint32_t*)(mipmap_d + 12);
                    tex_mipmap->size = *(uint32_t*)(mipmap_d + 20);
                }

                ssize_t size = tex_mipmap->get_size();
                tex_mipmap->data.resize(max_def(size, tex_mipmap->size));
                memcpy(tex_mipmap->data.data(), (void*)(mipmap_d + 24), tex_mipmap->size);
                size -= tex_mipmap->size;
                if (size > 0)
                    memset((void*)((size_t)tex_mipmap->data.data() + tex_mipmap->size), 0, size);
            }
    }
    return true;
}

bool txp_set::unpack_file_modern(const void* data, size_t size, uint32_t signature) {
    bool ret = false;
    f2_struct st;
    st.read(data, size);
    if (st.header.signature == reverse_endianness_uint32_t(signature))
        ret = unpack_file(st.data.data(), st.header.use_big_endian);
    return ret;
}

txp_set& txp_set::operator=(const txp_set& set) {
    textures.assign(set.textures.begin(), set.textures.end());
}
