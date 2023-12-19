/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "stack_allocator.hpp"

namespace prj {
    void* stack_allocator::allocate(size_t size) {
#if PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
        stack_allocator_node* node = (stack_allocator_node*)begin;
#else
        stack_allocator_node* node = next;
#endif
        size = max_def(size, 1);
        size = align_val(size, 8);

#if PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
        bool allocate = !node || (capacity_end - end) < (ssize_t)size;
#else
        bool allocate = true;
        if (node) {
            stack_allocator_node* last_node = 0;
            while (node) {
                if ((size_t)(node->capacity - node->size) >= size)
                    last_node = node;
                node = node->next;
            }

            if (last_node) {
                node = last_node;
                allocate = false;
            }
            else
                node = next;
        }
#endif

        if (allocate) {
#if PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
            size_t data_size = this->size;
#else
            size_t data_size = node ? (node->capacity + sizeof(stack_allocator_node)) * 2 : this->size;
#endif
            size_t _size = size + sizeof(stack_allocator_node);
            size_t mults = 0;
            while (data_size < _size) {
                data_size *= 2;
                if (data_size >= _size)
                    break;

                mults++;
                data_size *= 2;

                if (mults >= 16) {
                    if (data_size < _size)
                        return 0;
                    break;
                }
            }

            stack_allocator_node* new_node = (stack_allocator_node*)malloc(data_size);
            if (!new_node)
                return 0;

#if PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
            new_node->next = node;
            begin = (uint8_t*)new_node;
            end = (uint8_t*)new_node + sizeof(stack_allocator_node);
            capacity_end = (uint8_t*)new_node + data_size;
#else
            new_node->next = node;
            new_node->size = 0;
            new_node->capacity = data_size - sizeof(stack_allocator_node);
            next = new_node;
#endif
            node = new_node;
        }

#if PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
        uint8_t* data = end;
        end += size;
#else
        uint8_t* data = node->data + node->size;
        node->size += size;
#endif
        memset(data, 0, size);
        return data;
    }

    void stack_allocator::deallocate() {
#if PRJ_STACK_ALLOCATOR_ORIGINAL_CODE
        stack_allocator_node* node = (stack_allocator_node*)begin;
#else
        stack_allocator_node* node = next;
#endif
        while (node) {
            stack_allocator_node* next_node = node->next;
            free(node);
            node = next_node;
        }
        next = 0;
    }
}