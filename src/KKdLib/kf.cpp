/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "kf.hpp"

void kft_check(void* src_key, kf_type src_type, void* dst_key, kf_type* dst_type) {
    switch (src_type) {
    case KEY_FRAME_TYPE_0: {
        kft0* sk = (kft0*)src_key;
        kft0* dk = (kft0*)dst_key;
        dk->frame = sk->frame;
        *dst_type = KEY_FRAME_TYPE_1;
    } break;
    case KEY_FRAME_TYPE_1: {
        kft1* sk = (kft1*)src_key;
        if (*(uint32_t*)&sk->value != 0) {
            kft1* dk = (kft1*)dst_key;
            dk->frame = sk->frame;
            dk->value = sk->value;
            *dst_type = KEY_FRAME_TYPE_1;
        }
        else {
            kft0* dk = (kft0*)dst_key;
            dk->frame = sk->frame;
            *dst_type = KEY_FRAME_TYPE_0;
        }
    } break;
    case KEY_FRAME_TYPE_2: {
        kft2* sk = (kft2*)src_key;
        if (*(uint32_t*)&sk->tangent != 0) {
            kft2* dk = (kft2*)dst_key;
            dk->frame = sk->frame;
            dk->value = sk->value;
            dk->tangent = sk->tangent;
            *dst_type = KEY_FRAME_TYPE_2;
        }
        else if (*(uint32_t*)&sk->value != 0) {
            kft1* dk = (kft1*)dst_key;
            dk->frame = sk->frame;
            dk->value = sk->value;
            *dst_type = KEY_FRAME_TYPE_1;
        }
        else {
            kft0* dk = (kft0*)dst_key;
            dk->frame = sk->frame;
            *dst_type = KEY_FRAME_TYPE_0;
        }
    } break;
    case KEY_FRAME_TYPE_3: {
        kft3* sk = (kft3*)src_key;
        if (*(uint32_t*)&sk->tangent1 != *(uint32_t*)&sk->tangent2) {
            kft3* dk = (kft3*)dst_key;
            dk->frame = sk->frame;
            dk->value = sk->value;
            dk->tangent1 = sk->tangent1;
            dk->tangent2 = sk->tangent2;
            *dst_type = KEY_FRAME_TYPE_3;
        }
        else if (*(uint32_t*)&sk->tangent1 != 0) {
            kft2* dk = (kft2*)dst_key;
            dk->frame = sk->frame;
            dk->value = sk->value;
            dk->tangent = sk->tangent1;
            *dst_type = KEY_FRAME_TYPE_2;
        }
        else if (*(uint32_t*)&sk->value != 0) {
            kft1* dk = (kft1*)dst_key;
            dk->frame = sk->frame;
            dk->value = sk->value;
            *dst_type = KEY_FRAME_TYPE_1;
        }
        else {
            kft0* dk = (kft0*)dst_key;
            dk->frame = sk->frame;
            *dst_type = KEY_FRAME_TYPE_0;
        }
    } break;
    }
}
