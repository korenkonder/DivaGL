/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../default.hpp"

namespace prj {
    struct time {
        time_t value;

        inline time() {
            value = -1;
        }

        inline time(time_t value) {
            this->value = value;
        }

        static prj::time get_default();
    };

    prj::time strptime(std::string& str);
    prj::time strptime(std::string&& str);
}
