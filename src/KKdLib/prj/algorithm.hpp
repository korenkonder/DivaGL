/*
    by korenkonder
    GitHub/GitLab: korenkonder

    Taken from MSVC's VC/include/memory
*/

#pragma once

#include "../default.hpp"
#include <algorithm>
#include <vector>

namespace prj {
    template <class T>
    bool find(std::vector<T>& vec, T& value) {
        auto begin = vec.begin();
        auto end = vec.end();
        for (auto i = begin; i != end; i++)
            if (*i == value)
                return true;
        return false;
    }

    template <class T>
    void sort(std::vector<T>& vec) {
        std::sort(vec.begin(), vec.end());
    }

    template <class T>
    void unique(std::vector<T>& vec) {
        if (vec.size() <= 1)
            return;

        auto begin = vec.begin();
        auto end = vec.end();
        for (auto i = begin, j = begin + 1; i != end && j != end; )
            if (*i == *j) {
                std::move(j + 1, end, j);
                end--;
            }
            else {
                i++;
                j++;
            }

        if (vec.size() != end - begin)
            vec.resize(end - begin);
    }

    template <class T>
    void sort_unique(std::vector<T>& vec) {
        sort(vec);
        unique(vec);
    }
}
