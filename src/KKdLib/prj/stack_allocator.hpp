/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../default.hpp"

#define PRJ_STACK_ALLOCATOR_ORIGINAL_CODE 0

namespace prj {
    struct stack_allocator_node {
        stack_allocator_node * next;
#if !PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
        size_t size;
        size_t capacity;
#endif
#pragma warning(suppress: 4200)
        uint8_t data[];
    };

    struct stack_allocator {
        size_t size;
#if PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
        uint8_t* begin;
        uint8_t* end;
        uint8_t* capacity_end;
#else
        stack_allocator_node* next;
#endif

#if PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
        inline stack_allocator() : begin(), end(), capacity_end() {
            size = 4000;
        }
#else
        inline stack_allocator() : next() {
            size = 4000;
        }
#endif

        inline ~stack_allocator() {
            deallocate();
        }

        void* allocate(size_t size);
        void deallocate();

        template <typename T>
        inline T* allocate() {
            return new((T*)allocate(sizeof(T))) T;
        }

        template <typename T>
        inline T* allocate(size_t size) {
            if (!size)
                return 0;

            T* arr = (T*)allocate(sizeof(T) * size);
            for (size_t i = 0; i < size; i++)
                new(&arr[i]) T();
            return arr;
        }

        template <typename T>
        inline T* allocate(const T* src) {
            if (!src)
                return 0;

            return new((T*)allocate(sizeof(T))) T(*src);
        }

        template <typename T>
        inline T* allocate(const T* src, size_t size) {
            if (!src || !size)
                return 0;

            T* dst = (T*)allocate(sizeof(T) * size);
            for (size_t i = 0; i < size; i++)
                new(&dst[i]) T(src[i]);
            return dst;
        }
    };
}
