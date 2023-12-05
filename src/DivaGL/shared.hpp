/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "wrap.hpp"

enum blur_filter_mode {
    BLUR_FILTER_4 = 0,
    BLUR_FILTER_9,
    BLUR_FILTER_16,
    BLUR_FILTER_32,
    BLUR_FILTER_MAX,
};
