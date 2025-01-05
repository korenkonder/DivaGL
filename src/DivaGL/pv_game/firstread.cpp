/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "../../KKdLib/f2/header.hpp"
#include "../../KKdLib/aes.hpp"
#include "../../KKdLib/deflate.hpp"
#include "firstread.hpp"

enum {
    FIRSTREAD_STR_ARRAY = 0,
    FIRSTREAD_AUTH_3D,
    FIRSTREAD_OBJSET,
    FIRSTREAD_STAGE_DATA,
    FIRSTREAD_MAX,
};

const firstread_str_array* firstread_str_array::read(const void* data, size_t size) {
    if (!data || !size)
        return 0;

    size_t d = (size_t)data;
    firstread_str_array* str_array = (firstread_str_array*)data;
    if (str_array->signature != reverse_endianness_uint32_t('strx'))
        return 0;

    if (str_array->lang_array) {
        str_array->lang_array = (firstread_str_array_lang*)(d + (size_t)str_array->lang_array);

        const uint32_t num_lang = str_array->num_lang;
        for (uint32_t i = 0; i < num_lang; i++) {
            firstread_str_array_lang* lang = (firstread_str_array_lang*)&str_array->lang_array[i];

            if (lang->string_array) {
                lang->string_array = (firstread_str_array_string*)(d + (size_t)lang->string_array);

                const uint32_t num_string = lang->num_string;
                for (uint32_t j = 0; j < num_string; j++) {
                    firstread_str_array_string* string = (firstread_str_array_string*)&lang->string_array[j];

                    if (string->str)
                        string->str = (const char*)(d + (size_t)string->str);
                }
            }
        }
    }
    return str_array;
}

const firstread_auth_3d_array* firstread_auth_3d_array::read(const void* data, size_t size) {
    if (!data || !size)
        return 0;

    size_t d = (size_t)data;
    firstread_auth_3d_array* auth_3d_array = (firstread_auth_3d_array*)data;
    if (auth_3d_array->signature != reverse_endianness_uint32_t('a3dx'))
        return 0;

    if (auth_3d_array->auth_3d_array) {
        auth_3d_array->auth_3d_array = (firstread_auth_3d*)(d + (size_t)auth_3d_array->auth_3d_array);

        const uint32_t num_auth_3d = auth_3d_array->num_auth_3d;
        for (uint32_t i = 0; i < num_auth_3d; i++) {
            firstread_auth_3d* a3d = (firstread_auth_3d*)&auth_3d_array->auth_3d_array[i];

            if (a3d->object_array) {
                a3d->object_array = (firstread_auth_3d_object*)(d + (size_t)a3d->object_array);

                const uint32_t num_object = a3d->num_object;
                for (uint32_t j = 0; j < num_object; j++) {
                    firstread_auth_3d_object* obj = (firstread_auth_3d_object*)&a3d->object_array[j];

                    if (obj->name)
                        obj->name = (const char*)(d + (size_t)obj->name);

                    if (obj->uid_name)
                        obj->uid_name = (const char*)(d + (size_t)obj->uid_name);
                }
            }

            if (a3d->object_list_array) {
                a3d->object_list_array = (firstread_auth_3d_object_list*)(d + (size_t)a3d->object_list_array);

                const uint32_t num_object_list = a3d->num_object_list;
                for (uint32_t j = 0; j < num_object_list; j++) {
                    firstread_auth_3d_object_list* obj_list = (firstread_auth_3d_object_list*)&a3d->object_list_array[j];

                    if (obj_list->name)
                        obj_list->name = (const char*)(d + (size_t)obj_list->name);
                }
            }
        }
    }
    return auth_3d_array;
}

const firstread_objset* firstread_objset::read(
    prj::shared_ptr<prj::stack_allocator>& alloc, const void* data, size_t size) {
    if (!data || !size)
        return 0;

    f2_header* head = (f2_header*)alloc->allocate(size);
    if (!head)
        return 0;

    memcpy(head, data, size);

    bool reset_flags = head->attrib.get_gzip() || head->attrib.get_xor_data();

    if (head->attrib.get_crc()) {
        if (!head->validate_crc())
            return 0;

        if (reset_flags)
            head->reset_crc();
    }

    if (head->attrib.get_xor_data())
        head->remove_xor();

    const deflate::allocator allocator = {
        (void*)&alloc,
        [](void* data, size_t size) {
            return (*(prj::shared_ptr<prj::stack_allocator>*)data)->allocate(size);
        },
        [](void* data, void* ptr) {

        }
    };

    if (head->attrib.get_aes()) {
        uint8_t* section_data = head->get_section_data();
        uint32_t section_size = head->get_section_size();
        if (section_data) {
            static const uint32_t key_iv_size = 0x30;
            if (section_size < key_iv_size)
                return 0;

            section_size -= key_iv_size;

            if (section_size == align_val(section_size, 0x20)) {
                const uint32_t data_size = head->get_data_size();
                uint8_t* key = section_data + data_size - key_iv_size;
                uint8_t* iv = section_data + data_size + 0x20 - key_iv_size;

                aes256_ctx aes;
                aes256_init_ctx_iv(&aes, key, iv);
                aes256_cbc_decrypt_buffer(&aes, section_data, section_size);
                head->attrib.set_aes(false);
            }
        }
    }

    void* dec = 0;
    if (head->attrib.get_gzip()) {
        uint8_t* section_data = head->get_section_data();
        uint32_t section_size = head->get_section_size();
        if (!head->custom || !section_data || !section_size)
            return 0;

        dec = 0;
        size_t dec_len = head->custom;
        if (deflate::decompress(section_data, section_size, dec, dec_len, deflate::MODE_GZIP, allocator) < 0
            || !dec_len || dec_len < head->custom)
            return 0;

        if (reset_flags)
            head->attrib.set_gzip(false);
    }
    else {
        dec = alloc->allocate(head->custom);
        if (dec) {
            memset(dec, 0, head->custom);
            memcpy(dec, head->get_section_data(), min_def(head->custom, head->get_section_size()));
        }
    }

    if (!dec)
        return 0;

    size_t d = (size_t)dec;
    firstread_objset* objset = (firstread_objset*)dec;
    if (objset->signature != reverse_endianness_uint32_t('objx'))
        return 0;

    if (objset->obj_data) {
        objset->obj_data = (firstread_obj*)(d + (size_t)objset->obj_data);

        const uint32_t obj_num = objset->obj_num;
        for (uint32_t i = 0; i < obj_num; i++) {
            firstread_obj* obj = (firstread_obj*)&objset->obj_data[i];

            if (obj->mesh_array) {
                obj->mesh_array = (firstread_obj_mesh*)(d + (size_t)obj->mesh_array);

                const uint32_t num_mesh = obj->num_mesh;
                for (uint32_t j = 0; j < num_mesh; j++) {
                    firstread_obj_mesh* mesh = (firstread_obj_mesh*)&obj->mesh_array[j];

                    if (mesh->submesh_array) {
                        mesh->submesh_array = (firstread_obj_sub_mesh*)(d + (size_t)mesh->submesh_array);

                        const uint32_t num_submesh = mesh->num_submesh;
                        for (uint32_t j = 0; j < num_submesh; j++) {
                            firstread_obj_sub_mesh* sub_mesh = (firstread_obj_sub_mesh*)&mesh->submesh_array[j];

                            if (sub_mesh->index_array)
                                sub_mesh->index_array = (void*)(d + (size_t)sub_mesh->index_array);

                            if (sub_mesh->split) {
                                sub_mesh->split = (firstread_obj_sub_mesh_split*)(d + (size_t)sub_mesh->split);

                                firstread_obj_sub_mesh_split* split = sub_mesh->split;
                                if (split->index_array[0])
                                    split->index_array[0] = (void*)(d + (size_t)split->index_array[0]);
                                if (split->index_array[1])
                                    split->index_array[1] = (void*)(d + (size_t)split->index_array[1]);
                            }
                        }
                    }

                    for (void*& k : mesh->vertex_array_ptr)
                        if (k)
                            k = (void*)(d + (size_t)k);
                }
            }

            if (obj->material_array)
                obj->material_array = (obj_material_data*)(d + (size_t)obj->material_array);
        }
    }
    return objset;
}

const firstread_objset_array* firstread_objset_array::read(const void* data, size_t size) {
    if (!data || !size)
        return 0;

    size_t d = (size_t)data;
    firstread_objset_array* objset_array = (firstread_objset_array*)data;
    if (objset_array->signature != reverse_endianness_uint32_t('osdx'))
        return 0;

    if (objset_array->objset_id_array)
        objset_array->objset_id_array = (int32_t*)(d + (size_t)objset_array->objset_id_array);

    if (objset_array->objset_array) {
        objset_array->objset_array = (firstread_objset_data*)(d + (size_t)objset_array->objset_array);

        const uint32_t num_objset = objset_array->num_objset;
        for (uint32_t i = 0; i < num_objset; i++) {
            firstread_objset_data* objset = (firstread_objset_data*)&objset_array->objset_array[i];

            if (objset->data)
                objset->data = (void*)(d + (size_t)objset->data);
        }
    }
    return objset_array;
}

const firstread_stage_data_array* firstread_stage_data_array::read(const void* data, size_t size) {
    if (!data || !size)
        return 0;

    size_t d = (size_t)data;
    firstread_stage_data_array* stage_data_array = (firstread_stage_data_array*)data;
    if (stage_data_array->signature != reverse_endianness_uint32_t('stgx'))
        return 0;

    if (stage_data_array->stage_data_array) {
        stage_data_array->stage_data_array = (firstread_stage_data*)(d + (size_t)stage_data_array->stage_data_array);

        const uint32_t num_stage_data = stage_data_array->num_stage_data;
        for (uint32_t i = 0; i < num_stage_data; i++) {
            firstread_stage_data* stage_data = (firstread_stage_data*)&stage_data_array->stage_data_array[i];

            if (stage_data->name)
                stage_data->name = (const char*)(d + (size_t)stage_data->name);

            if (stage_data->reflect)
                stage_data->reflect = (stage_data_reflect*)(d + (size_t)stage_data->reflect);
        }
    }
    return stage_data_array;
}

const firstread* firstread::read(const void* data, size_t size) {
    if (!data || !size)
        return 0;

    size_t d = (size_t)data;
    firstread* fr = (firstread*)data;
    if (fr->signature != reverse_endianness_uint32_t('frbx'))
        return 0;

    if (fr->str_array)
        fr->str_array = firstread_str_array::read(
            (void*)(d + (size_t)fr->str_array), fr->str_array_size);

    if (fr->auth_3d_array)
        fr->auth_3d_array = firstread_auth_3d_array::read(
            (void*)(d + (size_t)fr->auth_3d_array), fr->auth_3d_array_size);

    if (fr->objset_array)
        fr->objset_array = firstread_objset_array::read(
            (void*)(d + (size_t)fr->objset_array), fr->objset_array_size);

    if (fr->stage_data_array)
        fr->stage_data_array = firstread_stage_data_array::read(
            (void*)(d + (size_t)fr->stage_data_array), fr->stage_data_array_size);
    return fr;
}
