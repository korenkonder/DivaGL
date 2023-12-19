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
    template <class T, class U>
    class vector_pair : public std::vector<std::pair<T, U>> {
    public:
        using value_pair = std::pair<T, U>;

        inline void push_back(const T& first, const U& second) {
            push_back({ first, second });
        }

        inline void push_back(const T& first, U&& second) {
            push_back({ first, second });
        }

        inline void push_back(T&& first, const U& second) {
            push_back({ first, second });
        }

        inline void push_back(T&& first, U&& second) {
            push_back({ first, second });
        }

        inline void push_back(const value_pair& value) {
            std::vector<std::pair<T, U>>::push_back(value);
        }

        inline void push_back(value_pair&& value) {
            std::vector<std::pair<T, U>>::push_back(value);
        }

        inline void sort() {
            std::sort(this->begin(), this->end(),
                [](const std::pair<T, U>& a, const std::pair<T, U>& b) {
                    return a.first < b.first;
                });
        }

        inline void unique() {
            if (this->size() <= 1)
                return;

            auto begin = this->begin();
            auto end = this->end();
            for (auto i = begin, j = begin + 1; i != end && j != end; )
                if (i->first == j->first) {
                    std::move(j + 1, end, j);
                    end--;
                }
                else {
                    i++;
                    j++;
                }

            if (this->size() != end - begin)
                this->resize(end - begin);
        }

        inline void sort_unique() {
            sort();
            unique();
        }

        inline typename auto find(const T& key) {
            auto k = this->begin();
            size_t l = this->size();
            size_t temp;
            while (l > 0) {
                if (k[temp = l / 2].first >= key)
                    l /= 2;
                else {
                    k += temp + 1;
                    l -= temp + 1;
                }
            }
            if (k == this->end() || key < k->first)
                return this->end();
            return k;
        }

        inline typename auto find(const T& key) const {
            auto k = this->begin();
            size_t l = this->size();
            size_t temp;
            while (l > 0) {
                if (k[temp = l / 2].first >= key)
                    l /= 2;
                else {
                    k += temp + 1;
                    l -= temp + 1;
                }
            }
            if (k == this->end() || key < k->first)
                return this->end();
            return k;
        }

        inline typename auto find(T&& key) {
            auto k = this->begin();
            size_t l = this->size();
            size_t temp;
            while (l > 0) {
                if (k[temp = l / 2].first >= key)
                    l /= 2;
                else {
                    k += temp + 1;
                    l -= temp + 1;
                }
            }
            if (k == this->end() || key < k->first)
                return this->end();
            return k;
        }

        inline typename auto find(T&& key) const {
            auto k = this->begin();
            size_t l = this->size();
            size_t temp;
            while (l > 0) {
                if (k[temp = l / 2].first >= key)
                    l /= 2;
                else {
                    k += temp + 1;
                    l -= temp + 1;
                }
            }
            if (k == this->end() || key < k->first)
                return this->end();
            return k;
        }
    };
}
