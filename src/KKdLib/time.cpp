/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "time.hpp"

static double_t time_struct_get_freq();

time_struct::time_struct() : timestamp() {
    get_timestamp();
    inv_freq = time_struct_get_freq();
}

double_t time_struct::calc_time() {
    LARGE_INTEGER timestamp;
    if (QueryPerformanceCounter(&timestamp))
        return (double_t)(timestamp.QuadPart - this->timestamp.QuadPart) * inv_freq;
    return 0.0;
}

int64_t time_struct::calc_time_int() {
    LARGE_INTEGER timestamp;
    if (QueryPerformanceCounter(&timestamp))
        return (int64_t)((double_t)(timestamp.QuadPart - this->timestamp.QuadPart) * inv_freq * 1000.0);
    return 0;
}

void time_struct::get_timestamp() {
    if (!QueryPerformanceCounter(&timestamp))
        timestamp.QuadPart = 0;
}

static double_t time_struct_get_freq() {
    LARGE_INTEGER freq;
    if (QueryPerformanceFrequency(&freq))
        return 1000.0 / (double_t)freq.LowPart;
    return 0.0;
}
