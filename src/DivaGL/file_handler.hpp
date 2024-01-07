/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <deque>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include "../KKdLib/default.hpp"
#include "../KKdLib/farc.hpp"
#include "types.hpp"

typedef void (PFNFILEHANDLERCALLBACK)(void*, const void*);

struct file_handler_callback_func {
    PFNFILEHANDLERCALLBACK* func;
    void* data;
    bool ready;
};

struct p_file_handler {
    struct file_handler* ptr;

    void call_free_callback();
    bool check_not_ready();
    const void* get_data();
    ssize_t get_size();
    bool read_file(const char* farc_path, const char* file_path, prj::HeapCMallocType heap_malloc_type, bool cache);
    void read_now();
    void reset();
    void set_callback_data(int32_t index, PFNFILEHANDLERCALLBACK* func, void* data);
};
