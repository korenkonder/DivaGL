/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"

struct time_struct {
    LARGE_INTEGER timestamp;
    double_t inv_freq;

    time_struct();

    double_t calc_time();
    int64_t calc_time_int();
    void get_timestamp();
};
