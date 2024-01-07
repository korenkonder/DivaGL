/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"

class FrameRateControl {
public:
    float_t frame_speed;

    FrameRateControl();
    virtual ~FrameRateControl();

    virtual float_t get_delta_frame() = 0;
    virtual void set_frame_speed(float_t value);
};

extern FrameRateControl* get_sys_frame_rate();
