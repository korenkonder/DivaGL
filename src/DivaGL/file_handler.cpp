/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include <thread>
#include "file_handler.hpp"

void p_file_handler::call_free_callback() {
    static void(FASTCALL * p_file_handler__call_free_callback)(p_file_handler * pfhndl)
        = (void(FASTCALL*)(p_file_handler * pfhndl))0x0000000140181F30;
    p_file_handler__call_free_callback(this);
}

bool p_file_handler::check_not_ready() {
    static bool(FASTCALL * p_file_handler__check_not_ready)(p_file_handler * pfhndl)
        = (bool(FASTCALL*)(p_file_handler * pfhndl))0x0000000140183340;
    return p_file_handler__check_not_ready(this);
}

const void* p_file_handler::get_data() {
    static const void* (FASTCALL * p_file_handler__get_data)(p_file_handler * pfhndl)
        = (const void* (FASTCALL*)(p_file_handler * pfhndl))0x0000000140182EA0;
    return p_file_handler__get_data(this);
}

ssize_t p_file_handler::get_size() {
    static ssize_t(FASTCALL * p_file_handler__get_size)(p_file_handler * pfhndl)
        = (ssize_t(FASTCALL*)(p_file_handler * pfhndl))0x0000000140183250;
    return p_file_handler__get_size(this);
}

bool p_file_handler::read_file(const char* farc_path, const char* file_path, int32_t heap_malloc_type, bool cache) {
    static bool (FASTCALL * p_file_handler__read_file)(p_file_handler * pfhndl, const char* farc_path,
        const char* file_path, int32_t heap_malloc_type, bool cache)
        = (bool (FASTCALL*)(p_file_handler * pfhndl, const char* farc_path,
            const char* file_path, int32_t heap_malloc_type, bool cache))0x0000000140183430;
    return p_file_handler__read_file(this, farc_path, file_path, heap_malloc_type, cache);
}

void p_file_handler::read_now() {
    static void (FASTCALL * p_file_handler__read_now)(p_file_handler * pfhndl)
        = (void (FASTCALL*)(p_file_handler * pfhndl))0x00000001401841F0;
    p_file_handler__read_now(this);
}

void p_file_handler::reset() {
    static void (FASTCALL * p_file_handler__reset)(p_file_handler * pfhndl)
        = (void (FASTCALL*)(p_file_handler * pfhndl))0x0000000140182CA0;
    p_file_handler__reset(this);
}

void p_file_handler::set_callback_data(int32_t index, PFNFILEHANDLERCALLBACK* func, void* data) {
    static void (FASTCALL * p_file_handler__set_callback_data)(p_file_handler * pfhndl, int32_t index, PFNFILEHANDLERCALLBACK func, void* data)
        = (void (FASTCALL*)(p_file_handler * pfhndl, int32_t index, PFNFILEHANDLERCALLBACK func, void* data))0x0000000140183F40;
    p_file_handler__set_callback_data(this, index, func, data);
}
