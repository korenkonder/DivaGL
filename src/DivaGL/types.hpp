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
    static void* (*HeapCMallocAllocateByType)(HeapCMallocType type, size_t size, const char* name)
        = (void* (*)(HeapCMallocType, size_t, const char*))0x0000000140181C30;
    static void(*HeapCMallocFree)(HeapCMallocType type, void* data)
        = (void(*)(HeapCMallocType, void*))0x00000001403F2960;
    static void(*HeapCMallocFreeByType)(HeapCMallocType type, void* data)
        = (void(*)(HeapCMallocType, void*))0x0000000140182DB0;
};

static void* (*_operator_new)(size_t) = (void* (*)(size_t))0x000000014084530C;
static void(*_operator_delete)(void*) = (void(*)(void*))0x0000000140845378;

static float_t(*get_anim_frame_speed)() = (float_t(*)())0x0000000140192CC0;
static float_t(*get_delta_frame)() = (float_t(*)())0x0000000140192D50;

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
    template<typename T, typename U>
    using pair = std::pair<T, U>;

    using string = std::basic_string<char, std::char_traits<char>, Allocator<char>>;

    template<typename T>
    using vector = std::vector<T, Allocator<T>>;

    template<typename T>
    using list = std::list<T, Allocator<T>>;

    template<typename T, typename U>
    using map = std::map<T, U, std::less<T>, Allocator<pair<T, U>>>;

    template<typename T>
    class ref_count {
    private:
        int32_t uses;
        int32_t weaks;
        T* ptr;
        void(*delete_this_func)(ref_count* ref);
        void(*destroy_func)(ref_count* ref, T* ptr);
    };

    template<class T>
    class shared_ptr;

    template<class T>
    class ptr_base {
    private:
        T* ptr;
        ref_count<T>* ref;

        template<class U>
        friend class ptr_base;
    };

    template<class T>
    class shared_ptr : public ptr_base<T> {
    public:
    };

    template<typename T, typename U>
    struct sorted_vector {
        vector<pair<T, U>> field_0;
        size_t field_18;
        vector<pair<T, U>> field_20;
        bool field_38;
    };
}
