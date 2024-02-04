/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    Counter* counter = (Counter*)0x0000000141199CB0;

    void Counter::Increment() {
        value++;
    }

    int32_t Counter::GetValue() {
        return value;
    }

    void Counter::Reset() {
        value = 0;
    }
}