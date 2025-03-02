/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "frame_rate_control.hpp"
#include "types.hpp"

FrameRateControl::FrameRateControl() {
    frame_speed = 1.0f;
}

FrameRateControl::~FrameRateControl() {

}

void FrameRateControl::set_frame_speed(float_t value) {
    frame_speed = value;
}

FrameRateControl* get_sys_frame_rate() {
    return (FrameRateControl*)0x0000000140C9A6B0;
}
