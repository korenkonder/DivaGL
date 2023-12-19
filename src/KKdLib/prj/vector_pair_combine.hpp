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
    class vector_pair_combine {
    public:
        using value_pair = std::pair<T, U>;
        using iterator = typename std::vector<value_pair>::iterator;
        using const_iterator = typename std::vector<value_pair>::const_iterator;
        std::vector<value_pair> data;
        std::vector<value_pair> new_data;

        inline auto find(const T& key) {
            auto k = data.begin();
            size_t l = data.size();
            size_t temp;
            while (l > 0) {
                if (k[temp = l / 2].first >= key)
                    l /= 2;
                else {
                    k += temp + 1;
                    l -= temp + 1;
                }
            }
            if (k == data.end() || key < k->first)
                return data.end();
            return k;
        }

        inline auto find(const T& key) const {
            auto k = data.begin();
            size_t l = data.size();
            size_t temp;
            while (l > 0) {
                if (k[temp = l / 2].first >= key)
                    l /= 2;
                else {
                    k += temp + 1;
                    l -= temp + 1;
                }
            }
            if (k == data.end() || key < k->first)
                return data.end();
            return k;
        }

        inline auto find(T&& key) {
            auto k = data.begin();
            size_t l = data.size();
            size_t temp;
            while (l > 0) {
                if (k[temp = l / 2].first >= key)
                    l /= 2;
                else {
                    k += temp + 1;
                    l -= temp + 1;
                }
            }
            if (k == data.end() || key < k->first)
                return data.end();
            return k;
        }

        inline auto find(T&& key) const {
            auto k = data.begin();
            size_t l = data.size();
            size_t temp;
            while (l > 0) {
                if (k[temp = l / 2].first >= key)
                    l /= 2;
                else {
                    k += temp + 1;
                    l -= temp + 1;
                }
            }
            if (k == data.end() || key < k->first)
                return data.end();
            return k;
        }

        inline void combine() {
            if (data.size() > 1)
                std::sort(data.begin(), data.end(),
                    [](const value_pair& a, const value_pair& b) {
                        return a.first < b.first;
                    });

            for (auto& i : new_data) {
                auto elem = find(i.first);
                if (elem != data.end())
                    elem->second = i.second;
                else
                    data.push_back(i);
            }

            new_data.clear();

            if (data.size() > 1) {
                std::sort(data.begin(), data.end(),
                    [](const value_pair& a, const value_pair& b) {
                        return a.first < b.first;
                    });

                auto begin = data.begin();
                auto end = data.end();
                for (auto i = begin, j = begin + 1; i != end && j != end; )
                    if (i->first == j->first) {
                        std::move(j + 1, end, j);
                        end--;
                    }
                    else {
                        i++;
                        j++;
                    }

                if (data.size() != end - begin)
                    data.resize(end - begin);
            }
        }

        inline auto begin() {
            return data.begin();
        }

        inline auto begin() const {
            return data.begin();
        }

        inline auto cbegin() const {
            return data.cbegin();
        }

        inline auto end() {
            return data.end();
        }

        inline auto end() const {
            return data.end();
        }

        inline auto cend() const {
            return data.cend();
        }

        inline auto rbegin() {
            return data.rbegin();
        }

        inline auto rbegin() const  {
            return data.rbegin();
        }

        inline auto crbegin() const {
            return data.crbegin();
        }

        inline auto rend() {
            return data.rend();
        }

        inline auto rend() const {
            return data.rend();
        }

        inline auto crend() const {
            return data.crend();
        }

        inline void push_back(const T& first, const U& second) {
            new_data.push_back({ first, second });
        }

        inline void push_back(const T& first, U&& second) {
            new_data.push_back({ first, second });
        }

        inline void push_back(T&& first, const U& second) {
            new_data.push_back({ first, second });
        }

        inline void push_back(T&& first, U&& second) {
            new_data.push_back({ first, second });
        }

        inline void push_back(const value_pair& value) {
            new_data.push_back(value);
        }

        inline void push_back(value_pair&& value) {
            new_data.push_back(value);
        }

        inline auto erase(const_iterator where) noexcept  {
            return data.erase(where);
        }

        inline auto erase(const_iterator first, const_iterator last) noexcept {
            return data.erase(first, last);
        }

        inline void clear() noexcept {
            data.clear();
            new_data.clear();
        }

        inline void shrink_to_fit() noexcept {
            data.shrink_to_fit();
            new_data.shrink_to_fit();
        }

        inline void reserve(size_t new_capacity) {
            new_data.reserve(new_capacity);
        }

        inline size_t size() const {
            return data.size();
        }

        inline value_pair& operator[](const size_t pos) noexcept {
            return data[pos];
        }

        inline const value_pair& operator[](const size_t pos) const noexcept {
            return data[pos];
        }

        inline value_pair& at(const size_t pos) {
            return data.at(pos);
        }

        inline const value_pair& at(const size_t pos) const {
            return data.at(pos);
        }

        inline value_pair& front() noexcept {
            return data.front();
        }

        inline const value_pair& front() const noexcept  {
            return data.front();
        }

        inline value_pair& back() noexcept  {
            return data.back();
        }

        inline const value_pair& back() const noexcept {
            return data.back();
        }
    };
}
