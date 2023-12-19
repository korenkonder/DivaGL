/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "quat.hpp"
#include <pmmintrin.h>
#include <xmmintrin.h>

static const quat quat_identity = { 0.0f, 0.0f, 0.0f, 1.0f };
