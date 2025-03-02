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

struct file_handler {
    int32_t count;
    struct Mtx_t* mtx;
    bool not_ready;
    bool reading;
    prj::string farc_path;
    prj::string file_path;
    prj::HeapCMallocType heap_malloc_type;
    bool cache;
    file_handler_callback_func callback[2];
    size_t size;
    void* data;
};

struct p_file_handler {
    file_handler* ptr;

    p_file_handler();
    ~p_file_handler();

    void call_free_callback();
    bool check_not_ready();
    const void* get_data();
    size_t get_size();
    bool read_file(const char* farc_path, const char* file_path, prj::HeapCMallocType heap_malloc_type, bool cache);
    bool read_file_path(const char* file_path, prj::HeapCMallocType heap_malloc_type);
    void read_now();
    void reset();
    void set_callback_data(int32_t index, PFNFILEHANDLERCALLBACK* func, void* data);
};
