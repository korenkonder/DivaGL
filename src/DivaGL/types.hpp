/*
    by korenkonder
    GitHub/GitLab: korenkonder

    Original: Allocator.h, Types.h by blueskythlikesclouds from DivaModLoader
*/

#include "../KKdLib/default.hpp"
#include <list>
#include <map>
#include <vector>

#pragma once

namespace prj {
    typedef enum HeapCMallocType {
        HeapCMallocSystem = 0x00,
        HeapCMallocTemp = 0x01,
        HeapCMallocMode = 0x02,
        HeapCMallocEvent = 0x03,
        HeapCMallocDebug = 0x04,
        HeapCMallocMax = 0x05,
    } HeapCMallocType;

    static void* (*HeapCMallocAllocate)(HeapCMallocType type, size_t size, const char* name)
        = (void* (*)(HeapCMallocType, size_t, const char*))0x00000001403F2A00;
    static void(*HeapCMallocFree)(HeapCMallocType type, void* data)
        = (void(*)(HeapCMallocType, void*))0x00000001403F2960;
};

static void* (*_operator_new)(size_t) = (void* (*)(size_t))0x000000014084530C;
static void(*_operator_delete)(void*) = (void(*)(void*))0x0000000140845378;

template <class T>
class Allocator {
public:
    using value_type = T;

    Allocator() noexcept {}
    template <class U> Allocator(Allocator<U> const&) noexcept {}

    value_type* allocate(std::size_t n) {
        return reinterpret_cast<value_type*>(_operator_new(n * sizeof(value_type)));
    }

    void deallocate(value_type* p, std::size_t) noexcept {
        _operator_delete(reinterpret_cast<void*>(p));
    }
};

template <class T, class U>
bool operator==(Allocator<T> const&, Allocator<U> const&) noexcept {
    return true;
}

template <class T, class U>
bool operator!=(Allocator<T> const& x, Allocator<U> const& y) noexcept {
    return !(x == y);
}

namespace prj {
    using string = std::basic_string<char, std::char_traits<char>, Allocator<char>>;

    template<typename T>
    using vector = std::vector<T, Allocator<T>>;

    template<typename T>
    using list = std::list<T, Allocator<T>>;

    template<typename T, typename U>
    using map = std::map<T, U, Allocator<std::pair<T, U>>>;

    template<typename T, typename U>
    using pair = std::pair<T, U>;
}
